#include "test_implementation.h"




void test_implementation(const int& nb_disc, const int& nb_paths,
	const int& nb_bins, const bool& uniform_noise, std::string path_suffixe,
	const bool& display_matrices, const bool& add_noise) {
	

	// We give ourselves a Heston Model

	double risk_free_rate = 0.02;
	double init_spot = 105.;
	double init_variance = 0.01;

	pair<double, double> init_spot_variance = make_pair(init_spot, init_variance);

	double mean_reversion_speed = 2.;
	double mean_reversion_level = 0.01;
	double vol_of_vol = 0.1;
	double correl = 0.;

	HestonModel model(mean_reversion_speed, mean_reversion_level, 
		vol_of_vol, risk_free_rate, correl, init_spot_variance);

	// We give ourseleves strikes and maturities 

	vector<double> market_maturities = {1., 2., 3., 4., 5.};
	vector<double> market_strikes = {95., 100., 105., 110., 115.};

	// Compute initial Heston prices:

	int nb_maturities = market_maturities.size();
	int nb_strikes = market_strikes.size();

	/// Compute Heston prices for a given Heston model

	vector<vector<double>> vanilla_heston_prices;

	for (int i = 0; i < nb_maturities; i++) {
		vector<double> hest_prices_const_mat;
		for (int j = 0; j < nb_strikes; j++) {

			double maturity = market_maturities[i];
			double strike = market_strikes[j];


			vector<double> time_points = dicretize_time(0., maturity, nb_disc);

			PathSimulatorEuler2F path_simulator(time_points, model, nb_paths);
			EuropeanOptionPayoff payoff(CALL, strike);
			MCPricer2F pricer(payoff, path_simulator, risk_free_rate);

			double price = pricer.price();

			hest_prices_const_mat.push_back(price);
		}
		vanilla_heston_prices.push_back(hest_prices_const_mat);
	}

	if (display_matrices == true) {
		display_matrix(vanilla_heston_prices);
		cout << endl;
	}
	
	/// Retrieve implied volatilities for the prices 
	// given by the vanilla Heston simulation
	vector<vector<double>> vheston_imp_vol = compute_implied_vol_matrix(market_maturities,
		market_strikes, vanilla_heston_prices, init_spot, risk_free_rate);

	std::string vheston_path = "vheston_imp_vol_" + path_suffixe + ".csv";
	export_market_vol_matrix(vheston_path, vheston_imp_vol, 
		market_maturities, market_strikes);

	if (display_matrices == true) {
		display_matrix(vheston_imp_vol);
		cout << endl;
	}

	// We add noise to the volatility matrix, and the new matrix is 
	// considered to be the matrix of market implied volatility

	vector<vector<double>> market_vol;
	if (add_noise == true) {
		market_vol  = add_random_noise(vheston_imp_vol, uniform_noise);
	}
	else {
		market_vol = vheston_imp_vol;
	}
	

	std::string market_vol_path = "market_vol_" + path_suffixe + ".csv";
	export_market_vol_matrix(market_vol_path,
		market_vol, market_maturities, market_strikes);

	if (display_matrices == true) {
		display_matrix(market_vol);
		cout << endl;
	}
	
	// Compute Implied volatility surface and dupire local volatility as 
	// it is later needed in the SLV paths simulation
	ImpliedVolatilitySurface implied_volatility_surface(market_maturities,
		market_strikes, market_vol, risk_free_rate);

	double eps_mat = 0.0001;
	double eps_strike = 0.0001;
	
	DupireLocalVolatilitySurface dupire_vol_surface(implied_volatility_surface,
		eps_mat, eps_strike, init_spot);

	/// Compute prices with Stochastic Local Volatility

	vector<vector<double>> slv_prices;

	for (int i = 0; i < nb_maturities; i++) {
		vector<double> slv_prices_const_mat;
		for (int j = 0; j < nb_strikes; j++) {

			double maturity = market_maturities[i];
			double strike = market_strikes[j];

			vector<double> time_points = dicretize_time(0.001, maturity, nb_disc);

			PathSimulatorSLV path_simulator(time_points, model, nb_paths, 
				dupire_vol_surface, nb_bins);
			EuropeanOptionPayoff payoff(CALL, strike);
			MCPricer2F pricer(payoff, path_simulator, risk_free_rate);

			double price = pricer.price();

			slv_prices_const_mat.push_back(price);
		}
		slv_prices.push_back(slv_prices_const_mat);
	}

	if (display_matrices == true) {
		display_matrix(slv_prices);
		cout << endl;
	}
	

	// Compute implied volatility of SLV prices:

	vector<vector<double>> slv_imp_vol = compute_implied_vol_matrix(market_maturities,
		market_strikes, slv_prices, init_spot, risk_free_rate);

	if (display_matrices == true) {
		display_matrix(slv_imp_vol);
		cout << endl;
	}
	
	std::string slv_imp_vol_path = "slv_imp_vol_" + path_suffixe + ".csv";
	export_market_vol_matrix(slv_imp_vol_path,
		slv_imp_vol, market_maturities, market_strikes);
}
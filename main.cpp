// StochasticLocalVolatility.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <random>
#include <math.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <cmath>
#include <cstdlib>

#include "ImpliedVolatilitySurface.h"
#include "HestonModel.h"
#include "PathSimulator2F.h"
#include "Payoff.h"
#include "MCPricer.h"
#include "utils.h"
#include "ImpliedVolComputation.h"

using namespace std;

int main()

{	
	srand(time(NULL));
	double risk_free_rate = 6.5e-4;
	double init_spot = 100.;
	double init_variance = 1.;

	/* Implied Volatility object */
	// Vector market_maturities = { 0.25, 0.5, 0.75, 1., 2., 3., 4., 5. };
	// Vector market_strikes = { 20., 40., 60., 80., 100., 120., 140., 160., 180. };
	Vector market_maturities = { 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425};
	Vector market_strikes = { 99.6, 99.7, 99.8, 99.9, 100., 100.1, 100.2, 100.3, 100.4 };


	Matrix market_implied_vols = {
		{0.39, 0.31, 0.24, 0.22, 0.16, 0.19, 0.23, 0.29, 0.38},
		{0.44, 0.36, 0.27, 0.21, 0.17, 0.21, 0.27, 0.35, 0.40},
		{0.45, 0.30, 0.25, 0.21, 0.18, 0.22, 0.29, 0.37, 0.45},
		{0.48, 0.42, 0.34, 0.28, 0.20, 0.26, 0.31, 0.42, 0.50},
		{0.52, 0.43, 0.34, 0.26, 0.21, 0.27, 0.38, 0.45, 0.55},
		{0.54, 0.46, 0.34, 0.27, 0.23, 0.28, 0.36, 0.49, 0.58},
		{0.57, 0.50, 0.46, 0.35, 0.25, 0.32, 0.45, 0.54, 0.60},
		{0.60, 0.52, 0.41, 0.31, 0.26, 0.34, 0.40, 0.55, 0.62}
	};

	ImpliedVolatilitySurface implied_volatility_surface(market_maturities, 
		market_strikes, market_implied_vols, risk_free_rate);

	double eps_mat = 0.000001;
	double eps_strike = 0.000001;

	DupireLocalVolatilitySurface dupire_vol_surface(implied_volatility_surface,
		eps_mat, eps_strike, init_spot);


	pair <double, double> init_spot_variance;
	init_spot_variance = make_pair(init_spot, init_variance);

	HestonModel model(2., 0, 0.01, risk_free_rate, -0.8, init_spot_variance);

	vector<vector<pair<double, double> > > all_steps;
	vector<double> time_points; 
	for (int i = 0; i < 2; i++) {
		time_points.push_back(i);
	}

	int nb_paths(9);
	int nb_bins(3);


	PathSimulatorEuler2F path_simulator(time_points, model, nb_paths);

	vector<pair<double, double>> last_step = path_simulator.paths();

	EuropeanOptionPayoff payoff(CALL, 10);

	int nb_sim = 10;

	MCPricer2F pricer(payoff, path_simulator, risk_free_rate);

	double price = pricer.price();

	PathSimulatorSLV path_simulator_slv(time_points, model, nb_paths,
		dupire_vol_surface, nb_bins);

	cout << price << endl;

	
	// Parameters:

	int nb_disc = 10;
	nb_paths = 10;


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

	/// Retrieve implied volatilities fothe prices given by the vanilla Heston simulation
	vector<vector<double>> vheston_imp_vol = compute_implied_vol_matrix(market_maturities,
		market_strikes, vanilla_heston_prices, init_spot, risk_free_rate);


	/// Move these volatilites a little

	vector<vector<double>> vheston_imp_vol_noised_mat;
	for (int i = 0; i < nb_maturities; i++) {
		vector<double> vheston_imp_vol_noised_vect;
		for (int j = 0; j < nb_strikes; j++) {
			unsigned seed = chrono::system_clock::now().time_since_epoch().count();
			default_random_engine generator(seed);
			normal_distribution<double> distribution(0.0, 0.001);
			vheston_imp_vol_noised_vect.push_back(distribution(generator) + vheston_imp_vol[i][j]);
		}
		vheston_imp_vol_noised_mat.push_back(vheston_imp_vol_noised_vect);
	}

	/// The new volatility matrix is considered to be the 
	/// market implied volatility of option prices


	/// Compute prices with SLV Monte Carlo by keeping the same Heston model and 
	/// taking as an input the matrix of market impied volatilities


	/// Compute the implied volatilities of prices obtained by SLV Monte Carlo simulation

	return 0;
}
#include <iostream>
#include "DupireLocalVolatilitySurface.h"

int main()
{

	FILE* stream;
	freopen_s(&stream, "implied_vols.txt", "w", stdout);

	/* Risk Free Rate and init spot*/

	double risk_free_rate = 0.05;
	double init_spot = 100.;

	/* Implied Volatility object */
	Vector market_maturities = { 0.25, 0.5, 0.75, 1., 2., 3., 4., 5. };
	Vector market_strikes = { 20., 40., 60., 80., 100., 120., 140., 160., 180. };
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

	ImpliedVolatilitySurface implied_volatility_surface(market_maturities, market_strikes, market_implied_vols, risk_free_rate);
	Vector new_mats;
	for (int i = 1; i <= 120; ++i)
		new_mats.push_back((double)i * 0.05);

	Vector new_strikes;
	for (int j = 1; j <= 20; ++j)
		new_strikes.push_back((double)j * 10.);

	//Matrix new_implied_vols;
	for (int i = 0; i < new_mats.size(); ++i)
	{
		for (int j = 0; j < new_strikes.size(); ++j)
		{
			double sigma = implied_volatility_surface.implied_volatility(new_mats[i], new_strikes[j]);
		
			std::cout << sigma << ',';
		}
		std::cout << '\n';
	}

	return 0;
}
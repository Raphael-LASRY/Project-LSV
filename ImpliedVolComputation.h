#pragma once
#include <vector>

using namespace std;

// This function implements Newton Raphson method to inverse Bacl Scholes call price 
// to get market implied volatility

double NewtonRaphson(const double& call_price, const double& T,
    const double& K, const double& S, const double& r);

// Compute market implied volatilities for a matrix of market prices
vector<vector<double>> compute_implied_vol_matrix(vector<double> market_maturities,
    vector<double> market_strikes, vector<vector<double>>& market_prices, 
    const double& spot, const double& r);
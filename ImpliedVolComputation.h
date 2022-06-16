#pragma once
#include <vector>

using namespace std;

double NewtonRaphson(const double& call_price, const double& T,
    const double& K, const double& S, const double& r);


vector<vector<double>> compute_implied_vol_matrix(vector<double> market_maturities,
    vector<double> market_strikes, vector<vector<double>>& market_prices, 
    const double& spot, const double& r);
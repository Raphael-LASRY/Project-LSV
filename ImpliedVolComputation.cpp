#define _USE_MATH_DEFINES

#include "ImpliedVolComputation.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

double normalPDF(const double& x) {
    return exp(-0.5 * x * x) / sqrt(2 * M_PI);
}

double normalCDF(const double& x)
{
    return 0.5 * erfc(-x * M_SQRT1_2);
}

double NewtonRaphson(const double& call_price, const double& T,
    const double& K, const double& S, const double& r)
{
    double v_new = (double)rand() / RAND_MAX;
    for (int k=0; k<100; k++)
    {
        double v_old = v_new;
        double d_1 = (log(S / K) + (r + 0.5 * v_old * v_old) * T) / (v_old * sqrt(T));
        double d_2 = d_1 - v_old * sqrt(T);

        double current_call_price = S * normalCDF(d_1) - K * normalCDF(d_2) * exp(-r * T);
        double vega = S * sqrt(T) * normalPDF(d_1);
        if (abs(vega) < 1e-9) {
            v_old = (double)rand() / RAND_MAX;
            k = 0;
            vega = 1;
        }
        v_new = v_old + (call_price - current_call_price) / vega;

        if (abs(v_new - v_old) < 1e-2)
        {
            return v_new;
        }
    }
    return v_new;
}


vector<vector<double>> compute_implied_vol_matrix(vector<double> market_maturities,
    vector<double> market_strikes, vector<vector<double>>& market_prices,
    const double& spot, const double& rfree_rate) {

    vector<vector<double>> implied_vol_matrix;

    int nb_rows = market_maturities.size();
    int nb_cols = market_strikes.size();

    for (int i = 0; i < nb_rows; i++) {

        vector<double> implied_vol_row;
        double maturity = market_maturities[i];

        for (int j = 0; j < nb_cols; j++) {
            
            double strike = market_strikes[j];
            double price = market_prices[i][j];
            double implied_vol = NewtonRaphson(price, maturity, strike, 
                                               spot, rfree_rate);

            implied_vol_row.push_back(implied_vol);
            cout << price << " " << maturity << " " << strike << " " << implied_vol << endl;
        }

        implied_vol_matrix.push_back(implied_vol_row);
    }
    return implied_vol_matrix;
}
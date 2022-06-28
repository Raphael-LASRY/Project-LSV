#pragma once
#include <random>
#include <math.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <string>

#include "ImpliedVolatilitySurface.h"
#include "HestonModel.h"
#include "PathSimulator2F.h"
#include "Payoff.h"
#include "MCPricer.h"
#include "utils.h"
#include "ImpliedVolComputation.h"

using namespace std;


// Function with all the framework to test our implementation of the stochastic
// local volatility model

// This test comprises : 

// Take a Heston model, vectors of strikes and maturities
// Compute the prices given by this Heston Model
// Compute the implied volatilities
// Add a random noise to the impled volatility matrix:
// the next matrix is considered to be the matrix of market implied volatility.
// This simulates the fact that the initial Heston model was not well calibrated
// Compute new prices with SLV Monte Carlo simulation with matrix of 
// market implied volatility as an input 
// Compute the implied volatility of the new prices and compare them to
// the implied volatility of the initial Heston Model. 

void test_implementation(const int& nb_disc, const int& nb_paths,
	const int& nb_bins, const bool& uniform_noise, std::string path_suffixe,
	const bool& display_matrices, const bool& add_noise);
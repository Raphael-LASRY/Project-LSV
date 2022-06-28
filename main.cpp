// StochasticLocalVolatility.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
#include "test_implementation.h"

using namespace std;


int main()

{	
	// Monte Carlo parameters
	// Our results are obtained with 10000 paths but it takes
	// quite some time to compute, we advise to reduce this number for a quick test

	int nb_disc = 100;
	int nb_paths = 1000;
	int nb_bins = 20;
	
	// display parameter
	bool display_matrices = true;

	// First test
	std::string path_suffixe = "test_1";
	bool uniform_noise = true;
	bool add_noise = true;
	test_implementation(nb_disc, nb_paths, nb_bins, uniform_noise, path_suffixe,
		display_matrices, add_noise);

	// Second test 
	path_suffixe = "test_2";
	uniform_noise = false;
	add_noise = true;
	test_implementation(nb_disc, nb_paths, nb_bins, uniform_noise, path_suffixe,
		display_matrices, add_noise);

	// Third test 
	// no modification on the vanilla heston implied volatilities
	path_suffixe = "test_3";
	uniform_noise = false;
	add_noise = false;
	test_implementation(nb_disc, nb_paths, nb_bins, uniform_noise, path_suffixe,
		display_matrices, add_noise);

	return 0;

}
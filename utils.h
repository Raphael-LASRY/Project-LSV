#pragma once
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>



#include <vector>
using namespace std;

// Function to display a matrix
void display_matrix(const vector<vector<double>>& matrix);


// Function returning a vector of uniformly discetized time points 
// between start_time and end_time
vector<double> dicretize_time(double start_time, double end_time, int nb_disc);


// Function to add the same random noise to all coefficients of a matrix
// If uniform_noise = true, the same value is added to all coefficients
vector<vector<double>> add_random_noise(const vector<vector<double>>& matrix,
	bool uniform_noise);


// Function to export volatility matrix
void export_market_vol_matrix(const std::string& filename,
	const vector<vector<double>> vol,
	const vector<double> maturities,
	const vector<double> strikes);

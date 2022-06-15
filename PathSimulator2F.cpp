#include "PathSimulator2F.h"
#include "Model2F.h" 
#include "HestonModel.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>
using namespace std;


PathSimulator2F::PathSimulator2F(const vector<double>& time_points,
	const Model2F& model, int nb_paths)
	: _time_points(time_points), _model(model.clone()), nb_paths(nb_paths)
{
}

PathSimulator2F::PathSimulator2F(const PathSimulator2F& path_simulator)
	: _time_points(path_simulator._time_points), _model((path_simulator._model)->clone()),
	nb_paths(path_simulator.nb_paths)
{
}

PathSimulatorEuler2F* PathSimulatorEuler2F::clone() const
{
	return new PathSimulatorEuler2F(*this);
}


PathSimulatorEuler2F::PathSimulatorEuler2F(const vector<double>& time_points,
	const Model2F& model, int nb_paths)
	: PathSimulator2F(time_points, model, nb_paths)
{
}

PathSimulatorEuler2F::PathSimulatorEuler2F(const PathSimulatorEuler2F& path_simulator)
	: PathSimulator2F(path_simulator)
{
}

PathSimulatorEuler2F::~PathSimulatorEuler2F()
{
	delete _model;
}


vector<vector<pair<double, double>>> PathSimulatorEuler2F::paths() const

{
	// Initialization
	int nb_paths = this->nb_paths;

	
	vector<pair<double, double>> current_step;
	vector<vector<pair<double, double>>> all_paths;
	

	pair<double, double> init_spot_variance = _model->init_spot_variance();
	for (int i = 0; i < nb_paths; i++) {
		current_step.push_back(init_spot_variance);
	}
	all_paths.push_back(current_step);

	int size = _time_points.size();

	for (int time_idx = 0; time_idx < size - 1; time_idx++) {

		double delta_t = _time_points[time_idx + 1] - _time_points[time_idx];
		double cur_time = _time_points[time_idx];
		double correlation = _model->correlation();
		double sqrtDt = sqrt(delta_t);

		vector<pair<double, double>> next_step;

		for (int path_idx = 0; path_idx < nb_paths; path_idx++) {
			pair<double, double> current_spot_variance = current_step[path_idx];

			std::mt19937 generator = std::mt19937(std::chrono::system_clock::now()
				.time_since_epoch().count());
			std::normal_distribution<double> distribution(0., 1.);
			double z_1 = distribution(generator);
			double z_2 = distribution(generator);

			double delta_brownian_spot = sqrtDt * z_1;
			double delta_brownian_variance = sqrtDt 
											 * (correlation * z_1
											    + sqrt(1 - correlation * correlation) * z_2);
			pair <double, double> drift_pair = _model->drift_pair(cur_time, 
																  current_spot_variance);

			pair <double, double> diffusion_pair = _model->diffusion_pair(cur_time,
				                                                          current_spot_variance);

			double current_spot = current_spot_variance.first;

			double next_spot = current_spot
				+ drift_pair.first * delta_t
				+ diffusion_pair.first * delta_brownian_spot;

			double current_variance = current_spot_variance.second;

			double next_variance = current_variance
				+ drift_pair.second * delta_t
				+ diffusion_pair.second * delta_t * delta_brownian_variance;

			// To not take square root of a negative value:
			next_variance = max(0., next_variance);


			pair <double, double> next_pair(next_spot, next_variance);

			next_step.push_back(next_pair);

		}

		all_paths.push_back(next_step);
		current_step = next_step;
		
	}
	return all_paths; 
}


vector<vector<pair<double, double> >> SLVPathSimulation(const HestonModel& model, 
	vector<double> time_points, int nb_paths, int nb_bins){

	vector<vector<pair<double, double> > > all_steps;

	// current_step a vector of simulated (price, variance) for all monte-carlo simulation
	vector< pair<double, double> > current_step ;

	// Initialization
	for (int i = 0; i < nb_paths; i++) {
		current_step.push_back(model.init_spot_variance());
	}

	all_steps.push_back(current_step);

	int nb_time_points = time_points.size();
	
	for (int i = 0; i < (nb_time_points-1); i++) {

		vector< pair < pair<double, double>, int > > sorted_vector;
		for (int i = 0; i < nb_paths; i++) {  
			sorted_vector.push_back(make_pair(current_step[i], i));
		}

		// Add comparison function in sort  pair<<pair <double, double>, int>>

		std::sort(sorted_vector.begin(), sorted_vector.end());

		// Compute the conditional expectation in each bin and create a map
		// between indexes and their bins to retrieve conditional
		// expectation later.
		int bin_size = nb_paths / nb_bins;
		int lower_bin_index = 0;
		int upper_bin_index = bin_size;

		map <int, double> conditional_expect_dict;


		while (upper_bin_index <= nb_paths) {

			int upper_idx = min(upper_bin_index, nb_paths);

			double conditional_expectation = 0;

			int nb_observations = 0;

			for (int j = lower_bin_index; j < upper_idx; j++) {

				// This is where we put psi(sorted_vector[j].first.second)
				conditional_expectation += sorted_vector[j].first.second;

				nb_observations += 1;
			}
			conditional_expectation = ((conditional_expectation * nb_observations)
				/ nb_paths);

			// Save conditional expectation for each index in the unsorted vector
			for (int j = lower_bin_index; j < upper_idx; j++) {
				int idx = sorted_vector[j].second;
				conditional_expect_dict[idx] = conditional_expectation;
			}
			lower_bin_index += bin_size;
			upper_bin_index += bin_size;
		}

		// Compute next step

		vector< pair<double, double> > next_step = current_step;
		double t = time_points[i];
		double delta_t = time_points[i + 1] - time_points[i];

		for (int j = 0; j < nb_paths; j++) {

			std::mt19937 generator = std::mt19937(std::chrono::system_clock::now()
												  .time_since_epoch().count());
			std::normal_distribution<double> distribution(0., 1.);
			double normal_1 = distribution(generator);
			double normal_2 = distribution(generator);
			double correlation = model.correlation();
			double normal_corr = correlation * normal_1
				                + sqrt(1 - correlation * correlation) * normal_2;
			

			double previous_vol = current_step[j].second; 
			double next_vol = previous_vol
							  + model.variance_drift(t, previous_vol)*delta_t 
						      + (model.variance_diffusion(t, previous_vol) 
								  * sqrt(delta_t) * normal_corr);
			double previous_spot = current_step[j].first;
			double r = model.risk_free_rate();

			double dupire_vol = 0.1; // TO REPLACE WITH DUPIRE VOLATILTIY

			double next_spot = previous_spot
							+ r * previous_spot * delta_t
							+ (sqrt(dupire_vol / conditional_expect_dict[j])
								* previous_spot
								* model.psi_function(previous_vol)
								* sqrt(delta_t)
								* normal_1);

			next_step[j].first = next_spot;
			next_step[j].second = next_vol;
		}
		all_steps.push_back(next_step);
	}
	return all_steps;
}
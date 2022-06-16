#include "PathSimulator2F.h"
#include "Model2F.h" 
#include "HestonModel.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>
using namespace std;

//////////////////////////////
// PathSimulator2F Methods
//////////////////////////////

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

double PathSimulator2F::expiry() const
{
	return _time_points[_time_points.size() - 1];
}


//////////////////////////////
// PathSimulatorEuler2F Methods
//////////////////////////////


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


vector<pair<double, double>> PathSimulatorEuler2F::paths() const
{
	// Initialization
	int nb_paths = this->nb_paths;

	
	vector<pair<double, double>> current_step;
	
	pair<double, double> init_spot_variance = _model->init_spot_variance();
	for (int i = 0; i < nb_paths; i++) {
		current_step.push_back(init_spot_variance);
	}

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

		current_step = next_step;
		
	}
	return current_step; 
}


///////////////////////////////
// SLV Methods
///////////////////////////////


PathSimulatorSLV::PathSimulatorSLV(const vector<double>& time_points, 
	const Model2F& model, int nb_paths, 
	const DupireLocalVolatilitySurface& dupire_volatility,
	int nb_bins)
	: PathSimulator2F(time_points, model, nb_paths),
	_dupire_volatility(dupire_volatility),
	nb_bins(nb_bins)
{
}


PathSimulatorSLV::PathSimulatorSLV(const PathSimulatorSLV& path_simulator)
	: PathSimulator2F(path_simulator), 
	_dupire_volatility(path_simulator._dupire_volatility),
	nb_bins(path_simulator.nb_bins)
{
}


PathSimulatorSLV* PathSimulatorSLV::clone() const
{
	return new PathSimulatorSLV(*this);
}


PathSimulatorSLV::~PathSimulatorSLV()
{
	delete _model;
}

vector<pair<double, double>> PathSimulatorSLV::paths() const
{	

// Initialization
	int nb_paths = this->nb_paths;

	// current_step a vector of simulated (price, variance) for all monte-carlo simulation
	vector< pair<double, double> > current_step;

	for (int i = 0; i < nb_paths; i++) {
		current_step.push_back(this->_model->init_spot_variance());
	}

	int nb_time_points = this->_time_points.size();

// Monte Carlo Simulations

	
	return current_step;
}






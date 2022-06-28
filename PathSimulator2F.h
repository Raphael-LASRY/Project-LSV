#pragma once

#include "Model2F.h"
#include "HestonModel.h"
#include <random>
#include <chrono>
#include <vector>
#include "DupireLocalVolatilitySurface.h"

using namespace std;



class PathSimulator2F {


public:

	virtual PathSimulator2F* clone() const = 0;

	PathSimulator2F(const vector<double>& time_points, const Model2F& model,
		int nb_paths);
	
	PathSimulator2F(const PathSimulator2F& path_simulator);
	
	PathSimulator2F& operator =(const PathSimulator2F& path_simulator);

	~PathSimulator2F() = default;

	virtual vector<pair<double, double>> paths() const = 0;
	double expiry() const;

protected:
	vector<double> _time_points;
	const Model2F* _model;
	int nb_paths;
};


class PathSimulatorEuler2F : public PathSimulator2F
{

public:

	PathSimulatorEuler2F(const vector<double>& time_points,
		const Model2F& model, int nb_paths);

	PathSimulatorEuler2F(const PathSimulatorEuler2F& path_simulator);

	PathSimulatorEuler2F* clone() const;

	PathSimulatorEuler2F& operator=(const PathSimulatorEuler2F& path_simulator);

	~PathSimulatorEuler2F();

	// This function implements the Euler 
	// discretization scheme for a 2 factor model
	vector<pair<double, double>> paths() const;

};


class PathSimulatorSLV : public PathSimulator2F
{ 

public:

	PathSimulatorSLV(const vector<double>& time_points, const Model2F& model,
		int nb_paths, const DupireLocalVolatilitySurface& dupire_volatility,
		int nb_bins);

	PathSimulatorSLV(const PathSimulatorSLV& path_simulator);

	PathSimulatorSLV* clone() const;

	PathSimulatorSLV& operator=(const PathSimulatorSLV path_simulator);

	~PathSimulatorSLV();

	// This function implements the SLV simulation as explained 
	// in the article "The Heston Stochastic-Local Volatility Model: 
	// Efficient Monte Carlo simulation"
	vector<pair<double, double>> paths() const;

private:

	const DupireLocalVolatilitySurface _dupire_volatility;
	int nb_bins;
};







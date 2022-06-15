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

	vector<vector<pair<double, double>>> paths() const;
	double expiry() const;

protected:
	vector<double> _time_points;
	const Model2F* _model;
	int nb_paths;
};


class PathSimulatorEuler2F : public PathSimulator2F
{

public:

	PathSimulatorEuler2F* clone() const;

	PathSimulatorEuler2F(const vector<double>& time_points,
		const Model2F& model, int nb_paths);

	PathSimulatorEuler2F(const PathSimulatorEuler2F& path_simulator);

	PathSimulatorEuler2F& operator=(const PathSimulatorEuler2F& path_simulator);

	~PathSimulatorEuler2F();

	vector<vector<pair<double, double>>> paths() const;

};


class PathSimulatorSLV{

public:

	PathSimulatorSLV(const vector<double>& time_points, const Model2F& model,
		const DupireLocalVolatilitySurface& dupire_volatility, int nb_paths,
		int nb_bins);

	PathSimulatorSLV(const PathSimulatorSLV& path_simulator);

	PathSimulatorSLV& operator=(const PathSimulatorSLV path_simulator);

	~PathSimulatorSLV();

	vector<vector<pair<double, double>>> paths();


private:
	vector<double> _time_points;
	const Model2F* _model;
	int nb_paths;

	const DupireLocalVolatilitySurface* _dupire_volatility;
	int nb_bins;
};


vector<vector<pair<double, double>>> SLVPathSimulation(const HestonModel& model,
	vector<double> time_points, int nb_paths, int nb_bins);







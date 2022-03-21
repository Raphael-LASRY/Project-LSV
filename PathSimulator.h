#ifndef PATHSIMULATOR_H
#define PATHSIMULATOR_H

#include "HestonModel.h"
#include <random>
#include <chrono>
#include <Vector>

using Vector = std::vector<double>;

class PathSimulator
{
public:
	virtual PathSimulator* clone() const = 0;
	PathSimulator(const std::vector<double>& time_points, const HestonModel& model);
	PathSimulator(const PathSimulator& path_simulator);

	PathSimulator& operator = (const PathSimulator & path_simulator);
	~PathSimulator();
	std::vector<Pair> path() const;
	double psi_function(const double& time, const double& variance) const override;// à déclarer
	double a_function(const double& time, const double& variance) const override; // à déclarer
	double b_function(const double& time, const double& variance) const override; // à déclarer
	double variance_function(const double& Monte_carlo_variance, const double& Dupire_local_vol); 

	std::vector<std::vector<Pair>> Monte_carlo_variance() const;


private:
	
    std::vector<double> _time_points;
	const HestonModel* _model;
	int _nbr_simulation;
	int _nbr_bins;
	int _alpha_coeficient;
	std::vector<std::vector<double>> _bins_vector;
	int risk_free_rate;
	Pair init_sport_variance;
};


#endif



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

	PathSimulator& oprator = (const PathSimulator & path_simulator);
	~PathSimulator();
	std::vector<Pair> path() const;
	double Phi_function(const double& time, const double& variance) const override;// à déclarer
	double a_function(const double& time, const double& variance) const override; // à déclarer
	double b_function(const double& time, const double& variance) const override; // à déclarer

protected:
	
	std::vector<double> _time_points;
	const HestonModel* _model;

};


class Monte_carlo_variance : public PathSimulator
{
public: 
	Monte_carlo_variance(const double& nbr_simulations)
	PathSimulator* clone() const override;
	

private:
	int _nbr_simulations;
	int _nbr_bins;
	const double alpha_coeficient; 
	std::vector<std::vector<Pair>> _Simulations;

};

#endif



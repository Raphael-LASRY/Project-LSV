#include "MCPricer.h"
#include <cmath>
#include <iostream>
using namespace std;


MCPricer2F::MCPricer2F(const Payoff& payoff,
	const PathSimulator2F& path_simulator, const double& risk_free_rate)
	:  _payoff(payoff.clone()), _path_simulator(path_simulator.clone()),
	_risk_free_rate(risk_free_rate)
{
}

double MCPricer2F::price() const
{	
	double price = 0.;
	vector <pair <double, double>> last_step;

	last_step = _path_simulator->paths();
	

	int nb_sim = last_step.size();

	// Compute average over MC simulations

	for (int i = 0; i < nb_sim; i++) {
		double realised_spot = last_step[i].first;
		double realised_payoff = _payoff->payoff(realised_spot);
		price += realised_payoff;
	}

	price /= (double)nb_sim;
	double maturity = _path_simulator->expiry();
	price *= std::exp(-_risk_free_rate * maturity);

	return price;
}


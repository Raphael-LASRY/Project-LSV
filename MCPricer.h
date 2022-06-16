
#pragma once

#include "Payoff.h"
#include "PathSimulator2F.h" 


class MCPricer2F
{
public:

	MCPricer2F(const Payoff& payoff,
		const PathSimulator2F& path_simulator,
		const double& risk_free_rate);

	double price() const;


private:

	Payoff* _payoff;
	PathSimulator2F* _path_simulator;
	double _risk_free_rate;
};


#include "Payoff.h"

EuropeanOptionPayoff* EuropeanOptionPayoff::clone() const
{
	return new EuropeanOptionPayoff(*this);
}

EuropeanOptionPayoff::EuropeanOptionPayoff(const Call_Put& call_put, 
	const double& strike)
	: _call_put(call_put), _strike(strike)
{
}

double EuropeanOptionPayoff::payoff(const double& spot) const
{
	// CALL : max(0, S_T - K)
	// PUT :  max(0, K - S_T)
	// if CALL then 1 * (S_T- K) else (-1) * (S_T- K)
	double mult = (_call_put == Call_Put::CALL) ? 1. : -1.;
	// return_type variable = (bool condition) ? val_if_true : val_if_false;
	return std::max(mult * (spot - _strike), 0.);
}
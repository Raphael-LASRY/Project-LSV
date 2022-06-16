#pragma once



#pragma once
#include <vector>
#include <algorithm>

using Vector = std::vector<double>;

enum Call_Put
{
	CALL,
	PUT
};


class Payoff
{
public:
	virtual Payoff* clone() const = 0;
	virtual double payoff(const double& spot) const = 0;
};



class EuropeanOptionPayoff : public Payoff
{
public:
	EuropeanOptionPayoff* clone() const override;

	EuropeanOptionPayoff(const Call_Put& call_put, const double& strike);
	double payoff(const double& spot) const override;

private:
	Call_Put _call_put;
	double _strike;
};
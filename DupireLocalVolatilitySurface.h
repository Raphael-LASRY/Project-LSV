#pragma once
#ifndef DUPIRELOCALVOLATILITYSURFACE
#define DUPIRELOCALVOLATILITYSURFACE

#include "ImpliedVolatilitySurface.h"

class DupireLocalVolatilitySurface
{
public:

	DupireLocalVolatilitySurface(const ImpliedVolatilitySurface& implied_vol_surface,
		const double& eps_mat, const double& eps_strike, const double& init_spot);


	DupireLocalVolatilitySurface(const DupireLocalVolatilitySurface& dupire_vol_surface);

	DupireLocalVolatilitySurface* clone() const;

	double local_volatility(const double& maturity, const double& strike) const;

	inline double risk_free_rate() const
	{
		return _implied_volatility_surface.risk_free_rate();
	}

private:
	double first_order_derivative_impliedvol_maturity(const double& maturity, 
		const double& strike) const;
	double first_order_derivative_impliedvol_strike(const double& maturity, 
		const double& strike) const;
	double second_order_derivative_impliedvol_strike(const double& maturity, 
		const double& strike) const;


	ImpliedVolatilitySurface _implied_volatility_surface;
	double _epsilon_maturity;
	double _epsilon_strike;
	double _initial_spot;
};

#endif // !DUPIRELOCALVOLATILITYSURFACE

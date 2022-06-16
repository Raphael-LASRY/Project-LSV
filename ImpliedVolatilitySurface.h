#pragma once

#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

class ImpliedVolatilitySurface
{
public:
	// Constructor
	ImpliedVolatilitySurface(const Vector& maturities, const Vector& strikes,
		const Matrix& implied_vols, const double& risk_free_rate);

	ImpliedVolatilitySurface(const ImpliedVolatilitySurface& implied_vol_surface);

	// return sigma*(T,K) for all T, K >= 0
	double implied_volatility(const double& maturity, const double& strike) const;

	// getter for the risk free rate
	inline double risk_free_rate() const
	{
		return _risk_free_rate;
	}

private:
	// Check methods called during object construction
	bool check_ordered_vectors() const;
	bool check_dimensions() const;
	bool check_vols_positivity() const;

	// size of strikes vector
	inline size_t strike_size() const
	{
		return _strikes.size();
	}

	// size of maturites vector
	inline size_t maturity_size() const
	{
		return _maturities.size();
	}

	// Evaluate the coefficients
	void evaluate_cubic_spline_coefficients();

	// For a given index {1, ... , M}, compute sigma*(T_i, K) for all K >= 0
	double compute_smile_implied_vol(const size_t& maturity_index, const double& strike) const;

	Matrix _alpha_coefficients;
	// alpha_1, ..., alpha_{N-1} [T_1]
	// ...............................
	// alpha_1, ...., alpha_{N-1} [T_M]

	Matrix _beta_coefficients;
	Matrix _gamma_coefficients;

	Vector _delta_K;

	Vector _maturities; // [T_1, ..., T_M]
	Vector _strikes; // [K_1, ..., K_N]
	Matrix _market_implied_vols; // Matrix M*N 
	// [sigma*_1,1, ... sigma*_1,N]
	// ..........................
	// ...........................
	// [sigma*_M,1, ... sigma*_M,N]

	double _risk_free_rate; // we could have used any time-dependent function.

};

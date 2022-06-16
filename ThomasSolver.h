

#pragma once

#include<vector>
using Vector = std::vector<double>;

class ThomasSolver
{
public:
	ThomasSolver(const Vector& lower_diag, const Vector& center_diag,
		const Vector& upper_diag, const Vector& rhs);

	// returns [X_1, X_2, ...]
	Vector solve() const;

private:
	Vector _lower_diagonal;   // a_j
	Vector _center_diagonal;  // b_j
	Vector _upper_diagonal;   // c_j
	Vector _right_hand_side;  // R_j
};
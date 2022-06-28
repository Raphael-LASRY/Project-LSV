#include "utils.h"

using namespace std;


vector<double> dicretize_time(double start_time, double end_time, int nb_disc) {
	vector<double> time_points; 

	double time = start_time;
	double time_incr = (end_time - start_time) / (double)nb_disc;

	for (int k = 0; k < nb_disc; k++) {
		time_points.push_back(time);
		time += time_incr;
	}
	time_points.push_back(end_time);

	return time_points;
}


void display_matrix(const vector<vector<double>>& matrix) {

	int nb_rows = matrix.size();
	int nb_cols = matrix[0].size();

	for (int i = 0; i < nb_rows; i++) {
		for (int j = 0; j < nb_cols; j++) {
			cout << matrix[i][j];
			cout << "  ";
		}
		cout << endl;
	}
}


vector<vector<double>> add_random_noise(const vector<vector<double>>& matrix,
	bool uniform_noise){

	int nb_rows = matrix.size();
	int nb_cols = matrix[0].size();

	vector<vector<double>> noised_matrix = matrix;

	unsigned seed = 1;
	default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0., 0.01);

	double noise = distribution(generator);

	for (int i = 0; i < nb_rows; i++) {
		for (int j = 0; j < nb_cols; j++) {
			if (uniform_noise == true) {
				noised_matrix[i][j] += noise;
			}
			else{
				noise = distribution(generator);
				noised_matrix[i][j] += noise;
			}
				
		}
	}
	return noised_matrix;
}


void export_market_vol_matrix(const std::string& filename, 
	const vector<vector<double>> vol,
	const vector<double> maturities,
	const vector<double> strikes)
{
	ofstream file;
	file.open(filename);
	int n_rows = maturities.size();
	int n_cols = strikes.size();
	file << ",";
	for (int col_idx = 0; col_idx < n_cols - 1; col_idx++)
		file << strikes[col_idx] << ",";
	file << strikes[n_cols - 1];
	file << "\n";

	for (int row_idx = 0; row_idx < n_rows; row_idx++)
	{	
		file << maturities[row_idx] << ",";
		for (int col_idx = 0; col_idx < n_cols - 1; col_idx++)
		{
			file << vol[col_idx][row_idx] << ",";
		}
		file << vol[n_cols - 1][row_idx];
		file << "\n";
	}
	file.close();
}

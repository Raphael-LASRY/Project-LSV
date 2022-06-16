#include "utils.h"


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
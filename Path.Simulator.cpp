#include "~vs2DDE.h"

PathSimulator* PathSimulator::clone() const
{
    return new PathSimulatorEuler(*this);
}

PathSimulator::PathSimulator(const std::vector<double>& time_points, const HestonModel& model)
    :_time_points(time_points),_model(model.clone())
{
}

PathSimulator::~PathSimulator()
{
    delete _model;
}

std::vector<Pair> PathSimulator::path() const
{
    std::vector<pair> path;
    pair init_sport_variance = _model->init_spot_variance();
    path.push_back(init_sport_variance);
    size_t.size = _time_points.size();
    for (size_t time_idx = 0; time_idx < size_t.size - 1; time_idx++)
    {
        pair current_spot_variance = path[time_idx];
        double delta_t = _time_points[time_idx + 1] - _time_points[idx];
        double current_time = _time_points[time_idx];
        double correlation = _model->correlation();
        double z_1 = std::normal_distribution<double> distribution(0., 1.);
        double z_2 = std::normal_distribution<double> distribution(0., 1.);
        double sqrt_Dt = std::sqrt(delta_t);
        double delta_brownian_spot = sqrt_DT* z_1;
        double delta_brownian_variance = sqrt_Dt* (correlation* z_1 + std::sqrt(1 - std::pow(correlation, 2))* z_2);
        pair drift_pair = _model->drift_pair(current_time, current_spot_variance);
        pair diffusion_pair = _model->diffusion_pair(current_time, current_spot_variance);
        double current_spot = current_spot_variance.first;
        double current_variance = current_spot_variance.second;
        double next_spot = current_spot + risk_free_rate* delta_t + LAFORMULE * current_spot* PHI_FUNCTION * sqrt_Dt* z_1;//3.3 page7
        double next_variance = current_variance + a_function(current_time,current_variance)* delta_t + b_function(current_time,current_variance)* sqrt_Dt* z_2;// 3.3 page 7 
        pair next_pair(next_spot, next_variance);
        path.push_back(next_pair);
    }
    return path;
}

#include "~vs2DDE.h"
#include "PathSimulator.h"

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
    pair init_sport_variance = _model-> init_spot_variance();
    path.push_back(init_spot_variance);
    size_t.size = _time_points.size();
    for (size_t time_idx = 0; time_idx < size_t.size - 1; time_idx++)
    {
        Pair current_spot_variance = path[time_idx];// ca m'à l'air faux
        double delta_t = _time_points[time_idx + 1] - _time_points[time_idx];
        double current_time = _time_points[time_idx];
        double correlation = _model-> correlation();
        double z_1 = std::normal_distribution<double> distribution(0., 1.);
        double z_2 = std::normal_distribution<double> distribution(0., 1.);
        double sqrt_Dt = std::sqrt(delta_t);
        double delta_brownian_spot = sqrt_DT* z_1;
        double delta_brownian_variance = sqrt_Dt* (correlation* z_1 + std::sqrt(1 - std::pow(correlation, 2))* z_2);
        Pair drift_pair = _model->drift_pair(current_time, current_spot_variance);
        Pair diffusion_pair = _model->diffusion_pair(current_time, current_spot_variance);
        double current_spot = current_spot_variance.first;
        double current_variance = current_spot_variance.second;
        double next_spot = current_spot + risk_free_rate* delta_t + variance_function* current_spot* psi_function(current_time,current_variance) * sqrt_Dt * z_1;//3.3 page7
        double next_variance = current_variance + a_function(current_time,current_variance)* delta_t + b_function(current_time,current_variance)* sqrt_Dt* z_2;// 3.3 page 7 
        Pair next_pair(next_spot, next_variance);
        path.push_back(next_pair);
    }
    return path;
}

double PathSimulator::variance_function(const double& Monte_carlo_variance, const double& Dupire_local_vol)
{
    return std::sqrt(std::pow(Dupire_local_vol,2)/Monte_carlo_variance);
}

std::vector<std::vector<Pair>> PathSimulator::Monte_carlo_variance() const
{
    size_t.size = _time_points.size();
    Pair current_spot_variance = path[time_idx];
    double current_spot = current_spot_variance.first;
    double current_variance = current_spot_variance.second;
    double delta_t = _time_points[time_idx + 1] - _time_points[time_idx];

    for (size_t time_idx = 0; time_idx < size_t.size  1; time_idx++)
    {
        std::vector<Pair> _Pair_simulation;
        std::vector<double> _sous_bins_vector;
        for (int sim_idx = 0; sim_idx < _nbr_simulations; sim_idx++)
        {
            double z_1 = std::normal_distribution<double> distribution(0., 1.);
            double z_2 = std::normal_distribution<double> distribution(0., 1.);
            _pair_simulation.push_back(current_spot + risk_free_rate * delta_t + variance_function * current_spot * psi_function(current_time, current_variance) * sqrt_Dt * z_1, current_variance + a_function(current_time, current_variance) * delta_t + b_function(current_time, current_variance) * sqrt_Dt * z_2);
            _pair_simulation.first = std::sort(_pair_simulation.first.begin(), _pair_simulator.first.end());
            _sous_bins_vector[0] = _pair_simulator.first[0];
            _sous_bins_vector[_nbr_bins + 1] = _pair_simulation.first[_nbr_simulation];
            for (int i = 1; i < _nbr_bins; i++)
            {
                _sous_bins_vector[i] = _pair_simulation.first[(i - 1) * (_nbr_simulation / _nbr_bins)];
            }

            // Il faut maintenant envoyer les S_t dans leurs bins respectifs : besoin d'aide 



        }

        Monte_carlo_variance.push_back(_pair_simulation);
        _bins_vector.push_back(_sous_bins_vector);
    }

    return Monte_carlo_variance;
}
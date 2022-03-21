#include "HestonModel.h"

HestonModel::HestonModel(const double& mean_reversion_speed, const double& mean_reversion_level, const double& vol_of_vol,
    const double& risk_free_rate, const double& correlation, const Pair& init_spot_variance)
    : Model2F(risk_free_rate, correlation, init_spot_variance),
    _mean_reversion_speed(mean_reversion_speed), _mean_reversion_level(mean_reversion_level), _vol_of_vol(vol_of_vol)
{
}

HestonModel::HestonModel(const HestonModel& model)
    : Model2F(model._risk_free_rate, model._correlation, model._init_spot_variance),
    _mean_reversion_speed(model._mean_reversion_speed), _mean_reversion_level(model._mean_reversion_level), _vol_of_vol(model._vol_of_vol)
{
}

HestonModel& HestonModel::operator=(const HestonModel& model)
{
    if (this != &model)
    {
        _mean_reversion_speed = model._mean_reversion_speed;
        _mean_reversion_level = model._mean_reversion_level;
        _vol_of_vol = model._vol_of_vol;
        _risk_free_rate = model._risk_free_rate;
        _correlation = model._correlation;
        _init_spot_variance = model._init_spot_variance;
    }
    return *this;
}

double HestonModel::mean_reversion_speed() const
{
    return _mean_reversion_speed;
}

double HestonModel::mean_reversion_level() const
{
    return _mean_reversion_level;
}

double HestonModel::vol_of_vol() const
{
    return _vol_of_vol;
}

HestonModel* HestonModel::clone() const
{
    return new HestonModel(*this);
}

double HestonModel::psi_function(const double& variance) const
{
    return sqrt(variance);
}

double HestonModel::sigma_function(const double& time, const double& variance) const
{
    return 1;
}

double HestonModel::variance_drift(const double& time, const double& variance) const
{
    return _mean_reversion_speed * (_mean_reversion_level - variance);
}

double HestonModel::variance_diffusion(const double& time, const double& variance) const
{
    return _vol_of_vol * sqrt(variance);
}

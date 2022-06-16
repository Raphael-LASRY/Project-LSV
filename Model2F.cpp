#include <iostream>
#include <cmath>
#include "Model2F.h"

using namespace std;

Model2F::Model2F(const double& risk_free_rate, const double& correlation, 
                 const Pair& init_spot_variance)
    : _risk_free_rate(risk_free_rate), _correlation(correlation),
    _init_spot_variance(init_spot_variance)
{
}

Model2F::Model2F(const Model2F& model)
    : _risk_free_rate(model._risk_free_rate), 
    _correlation(model._correlation), 
    _init_spot_variance(model._init_spot_variance)
{
}

Model2F& Model2F::operator=(const Model2F& model)
{
    if (this != &model)
    {
        _risk_free_rate = model._risk_free_rate;
        _correlation = model._correlation;
        _init_spot_variance = model._init_spot_variance;
    }
    return *this;
}

double Model2F::risk_free_rate() const
{
    return _risk_free_rate;
}

double Model2F::correlation() const
{
    return _correlation;
}

Pair Model2F::init_spot_variance() const
{
    return _init_spot_variance;
}

Pair Model2F::drift_pair(const double& time, const Pair& spot_variance) const
{
    Pair pair(_risk_free_rate * spot_variance.first, 
              variance_drift(time, spot_variance.second));
    return pair;
}

Pair Model2F::diffusion_pair(const double& time, const Pair& spot_variance) const
{
    Pair pair(sigma_function(time, spot_variance.second) 
              * psi_function(spot_variance.second) 
              * spot_variance.first
        , variance_diffusion(time, spot_variance.second));
    return pair;
}
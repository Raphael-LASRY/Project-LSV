#pragma once

#ifndef MODEL2F_H
#define MODEL2F_H

#include <utility>

using Pair = std::pair<double, double>;

class Model2F
{
public:

    Model2F(const double& risk_free_rate, const double& correlation, 
            const Pair& init_spot_variance);
    Model2F(const Model2F& model);
    Model2F& operator=(const Model2F& model);

    double risk_free_rate() const;
    double correlation() const;
    Pair init_spot_variance() const;

    virtual Model2F* clone() const = 0;
    virtual ~Model2F() = default;

    Pair drift_pair(const double& time, const Pair& spot_variance) const;
    Pair diffusion_pair(const double& time, const Pair& spot_variance) const;

protected:

    virtual double psi_function(const double& variance) const = 0;
    virtual double sigma_function(const double& time, const double& variance) const = 0;
    virtual double variance_drift(const double& time, const double& variance) const = 0;
    virtual double variance_diffusion(const double& time, const double& variance) const = 0;

    double _risk_free_rate;
    double _correlation;
    Pair _init_spot_variance;
};

#endif
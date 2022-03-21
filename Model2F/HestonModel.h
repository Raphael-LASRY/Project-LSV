#ifndef HESTONMODEL
#define HESTONMODEL

#include "Model2F.h"

class HestonModel : public Model2F
{
public:
    HestonModel(const double& mean_reversion_speed, const double& mean_reversion_level, const double& vol_of_vol,
        const double& risk_free_rate, const double& correlation, const Pair& init_spot_variance);
    HestonModel(const HestonModel& model);
    HestonModel& operator=(const HestonModel& model);

    double mean_reversion_speed() const;
    double mean_reversion_level() const;
    double vol_of_vol() const;

    HestonModel* clone() const override;
    ~HestonModel() = default;

    double psi_function(const double& variance) const override;
    double sigma_function(const double& time, const double& variance) const override;
    double variance_diffusion(const double& time, const double& variance) const override;
    double variance_drift(const double& time, const double& variance) const override;

private:
    double _mean_reversion_speed;
    double _mean_reversion_level;
    double _vol_of_vol;
};

#endif
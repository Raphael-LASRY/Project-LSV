#ifndef HESTONMODEL_H
#define HESTONMODEL_H

class HestonModel
{
public:

    HestonModel(const double& initial_spot, const double& initial_variance, const double& drift, const double& mean_reversion_speed,
    const double& mean_reversion_level, const double& vol_of_vol, const double& correlation); //constructor with parameters
    HestonModel(const HestonModel& model); //constructor by recopy

    // getter methods
    double initial_spot() const;
    double initial_variance() const;
    double drift() const;
    double mean_reversion_speed() const;
    double mean_reversion_level() const;
    double vol_of_vol() const;
    double correlation() const;


private:

    double _initial_spot;         // S_O
    double _initial_variance;     // V_O
    double _drift;                // rate r
    double _mean_reversion_speed; // kappa
    double _mean_reversion_level; // theta
    double _vol_of_vol;           // sigma_V
    double _correlation;          // rho
};



#endif
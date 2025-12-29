#pragma once
#include <cmath>
#include "../grid/FdGrid.hpp"
#include "../model/BlackScholesModel.hpp"
#include "../products/InterfaceProducts.hpp"

struct GridSpec {
    double Smin = 0.0;
    double Smax = 0.0;
    int Nt = 0;
    int Ns = 0;
};

class GridParameters {
public:
    // quality: 0=fast, 1=balanced, 2=accurate
    static FdGrid makeGrid(const InterfaceProducts& opt,
                           const BlackScholesModel& model,
                           double S0,
                           int quality = 1)
    {
        const double T = opt.maturity();
        const double r = model.r();
        const double q = model.q();
        const double sigma = model.sigma();

        // 1) choose Smax via lognormal quantile
        const double z = (quality == 0 ? 3.0 : (quality == 1 ? 4.0 : 5.0));
        const double mu = (r - q - 0.5 * sigma * sigma) * T;
        const double s  = sigma * std::sqrt(T);

        const double Smax = S0 * std::exp(mu + z * s);
        const double Smin = 0.0;

        // 2) choose Ns
        int Ns = (quality == 0 ? 250 : (quality == 1 ? 400 : 800));

        // 3) choose Nt for explicit stability
        const double dS = (Smax - Smin) / Ns;
        const double sig2S2 = (sigma * sigma) * (Smax * Smax);
        const double denom = sig2S2 / (dS * dS) + r;

        // safety factor
        const double dt_max = 1.0 / denom;
        const double safety = 0.8;
        const double dt = safety * dt_max;

        int Nt = static_cast<int>(std::ceil(T / dt));
        if (Nt < 1) Nt = 1;

        return FdGrid(T, Smax, Nt, Ns, Smin);
    }
};

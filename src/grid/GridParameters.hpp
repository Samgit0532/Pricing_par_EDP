#pragma once
#include <cmath>
#include "FdGrid.hpp"
#include "../model/BlackScholesModel.hpp"
#include "../products/InterfaceProducts.hpp"

class GridParameters {
public:
    // Explicit scheme grid builder
    // rel_dS: spatial step as a fraction of S0 (e.g. 0.002 = 0.2%)
    static FdGrid makeGrid(const InterfaceProducts& product,
                           const BlackScholesModel& model,
                           double S0,
                           double rel_dS,
                           double Smin = 0.0)
    {
        const double T   = product.maturity();
        const double r   = model.r();
        const double q   = model.q();
        const double sig = model.sigma();

        // 1) Spatial domain: high lognormal quantile
        const double z = 5.0;
        const double drift   = (r - q - 0.5 * sig * sig) * T;
        const double volTerm = z * sig * std::sqrt(T);

        const double Smax = S0 * std::exp(drift + volTerm);

        // 2) Spatial resolution: relative to S0
        const double dS = rel_dS * S0;

        const int Ns = static_cast<int>(std::ceil((Smax - Smin) / dS));

        // 3) Time step from explicit stability (worst case at Smax)
        const double denom =
            (sig * sig * Smax * Smax) / (dS * dS) + r;

        const double dt = 0.45 / denom;
        const int Nt = static_cast<int>(std::ceil(T / dt));

        return FdGrid(T, Smax, Nt, Ns, Smin);
    }
};

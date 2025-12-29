#include "ExplicitFdSolver.hpp" 
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

ExplicitFdSolver::Result ExplicitFdSolver::price(const InterfaceProducts& option,
                                                const BlackScholesModel& model,
                                                const FdGrid& grid,
                                                double S0) const
{
    const int Ns = grid.Ns();
    const int Nt = grid.Nt();
    const double dS = grid.dS();
    const double dt = grid.dt();

    if (Ns < 2 || Nt < 1) throw std::invalid_argument("Grid too small (Ns<2 or Nt<1).");
    if (dS <= 0.0 || dt <= 0.0) throw std::invalid_argument("Invalid grid steps (dS<=0 or dt<=0).");

    const auto& S = grid.priceGrid();
    const auto& t = grid.timeGrid();
    if ((int)S.size() != Ns + 1 || (int)t.size() != Nt + 1)
        throw std::runtime_error("Grid vectors have inconsistent sizes.");

    // Clamp S0 inside the grid to avoid boundary issues for Greeks
    if (S0 <= S.front()) S0 = S.front() + 1e-12;
    if (S0 >= S.back())  S0 = S.back()  - 1e-12;

    // Cache model params (cleaner + faster)
    const double r = model.r();
    const double q = model.q();
    const double sigma = model.sigma();
    const double sigma2 = sigma * sigma;

    // Terminal condition: V(T,S)=payoff(S)
    std::vector<double> V(Ns + 1);
    std::vector<double> Vnew(Ns + 1);

    for (int i = 0; i <= Ns; ++i) {
        V[i] = option.payoff(S[i]);
    }

    // Backward time stepping
    for (int n = Nt - 1; n >= 0; --n) {
        const double tn = t[n];
        
        // Boundaries
        Vnew[0]  = option.leftBoundary(tn);
        Vnew[Ns] = option.rightBoundary(tn, S.back());

        // Interior points
        for (int i = 1; i < Ns; ++i) {
            const double Si = S[i];

            const double sig2S2 = sigma2 * Si * Si;
            const double muS    = (r - q) * Si;

            const double A = 0.5 * dt * ( sig2S2 / (dS * dS) - muS / dS );
            const double B = 1.0 - dt * ( sig2S2 / (dS * dS) + r );
            const double C = 0.5 * dt * ( sig2S2 / (dS * dS) + muS / dS );

            double val = A * V[i - 1] + B * V[i] + C * V[i + 1];

            if (option.isAmerican()) {
                val = std::max(val, option.earlyExerciseValue(Si));
            }

            Vnew[i] = val;
        }

        V.swap(Vnew);
    }

    Result res;
    res.V0 = V;
    res.price = grid.interpolate(V, S0);

    // -------- Greeks: Delta & Gamma --------
    // Find i such that S[i] <= S0 < S[i+1]
    auto it = std::upper_bound(S.begin(), S.end(), S0);
    int i = static_cast<int>(std::distance(S.begin(), it)) - 1;

    // Ensure i-1 and i+1 exist (avoid boundaries)
    if (i < 1) i = 1;
    if (i > Ns - 1) i = Ns - 1;

    res.delta = (V[i + 1] - V[i - 1]) / (2.0 * dS);
    res.gamma = (V[i + 1] - 2.0 * V[i] + V[i - 1]) / (dS * dS);

    return res;
}

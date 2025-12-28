#include "ExplicitFdSolver.cpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

ExplicitFdSolver::Result ExplicitFdSolver::price(const InterfaceProducts& option,
                                                const BlackScholesModel& model,
                                                const FdGrid& grid,
                                                double S0) const {
    const int Ns = grid.Ns();
    const int Nt = grid.Nt();
    const double dS = grid.dS();
    const double dt = grid.dt();

    //  Terminal condition: V(T, S) = payoff(S)
    std::vector<double> V(Ns + 1);
    const auto& S = grid.priceGrid();
    const auto& t = grid.timeGrid();

    for (int i = 0; i <= Ns; ++i)
        V[i] = option.payoff(S[i]);

    // Backward time stepping: n = Nt-1, ..., 0
    for (int n = Nt - 1; n >= 0; --n) {
        const double tn = t[n];
        std::vector<double> Vnew(Ns + 1);

        // Boundaries for this time layer
        Vnew[0]  = option.leftBoundary(tn);
        Vnew[Ns] = option.rightBoundary(tn, S.back());

        // Interior points
        for (int i = 1; i < Ns; ++i) {
            const double Si = S[i];

            // Coefficients using central differences in S, explicit in time
            const double sig2S2 = model.sigma() * model.sigma() * Si * Si;
            const double muS    = (model.r() - model.q()) * Si;

            const double A = 0.5 * dt * ( sig2S2 / (dS * dS) - muS / dS );
            const double B = 1.0 - dt * ( sig2S2 / (dS * dS) + model.r() );
            const double C = 0.5 * dt * ( sig2S2 / (dS * dS) + muS / dS );

            Vnew[i] = A * V[i - 1] + B * V[i] + C * V[i + 1];

            // For later (American): projection
            if (option.isAmerican()) {
                Vnew[i] = std::max(Vnew[i], option.earlyExerciseValue(Si));
            }
        }

        V.swap(Vnew);
    }

    Result res;
    res.V0 = V;
    res.price = grid.interpolate(V, S0);


// Greeks computation
const auto& S = grid.priceGrid();
const double dS = grid.dS();
const int Ns = grid.Ns();

// Find i0 such that S[i0] = S0
int i0 = 0;
while (i0 < Ns - 1 && S[i0 + 1] < S0)
    ++i0;
  
if (i0 == 0)  i0 = 1;  // In order to avoid boundaries
if (i0 >= Ns) i0 = Ns - 1;

// Delta
res.delta = (V[i0+1] - V[i0-1)] / (2.0*dS);

//Gamma
res.gamma= ( V[i0+1] - 2.0*V[i0] + V[i0-1]) / (dS*dS);


return res;
}  

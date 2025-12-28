#pragma once
#include <vector>
#include "../grid/FdGrid.cpp"
#include "../model/BlackScholesModel.cpp"
#include "../products/InterfaceProducts.cpp"

/**
 * Explicit finite-difference solver for the Black–Scholes PDE
 */
class ExplicitFdSolver {
public:
    struct Result {
        std::vector<double> V0; // values at t=0 on the grid
        double price;           // interpolated price at S0
        double delta;           // delta
        double gamma;           // gamma
    };

    Result price(const InterfaceProducts& option,
                 const BlackScholesModel& model,
                 const FdGrid& grid,
                 double S0) const;
};

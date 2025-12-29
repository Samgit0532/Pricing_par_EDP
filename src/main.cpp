#include <iostream>
#include <cmath>
#include <iomanip>

#include "model/BlackScholesModel.hpp"
#include "grid/FdGrid.hpp"
#include "grid/GridParameters.hpp"

#include "solvers/ExplicitFdSolver.hpp"

#include "products/EuropeanCall.hpp"
#include "products/EuropeanPut.hpp"
#include "products/AmericanCall.hpp"
#include "products/AmericanPut.hpp"
#include "products/Future.hpp"

static void print_line() {
    std::cout << "---------------------------------------------\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(6);

    // --- Market & contract ---
    const double S0 = 100.0;
    const double K  = 100.0;
    const double T  = 1.0;

    const double r     = 0.05;
    const double sigma = 0.20;
    const double q     = 0.02;   // mets 0.0 pour tester "American Call = European Call"

    BlackScholesModel model(r, sigma, q);

    // --- Products ---
    EuropeanCall  euroCall(K, T, model);
    EuropeanPut   euroPut(K, T, model);
    AmericanCall  amerCall(K, T, model);
    AmericanPut   amerPut(K, T, model);
    Future        future(K, T, model);

    // --- Grid (auto) ---
    // Une grille unique suffit car tous ont même T.
    FdGrid grid = GridParameters::makeGrid(euroCall, model, S0, /*quality=*/1);

    std::cout << "Grid: Nt=" << grid.Nt() << " Ns=" << grid.Ns()
              << " dt=" << grid.dt() << " dS=" << grid.dS() << "\n";
    print_line();

    // --- Solver ---
    ExplicitFdSolver solver;

    const auto resEuroCall = solver.price(euroCall, model, grid, S0);
    const auto resEuroPut  = solver.price(euroPut,  model, grid, S0);
    const auto resAmerCall = solver.price(amerCall, model, grid, S0);
    const auto resAmerPut  = solver.price(amerPut,  model, grid, S0);
    const auto resFuture   = solver.price(future,   model, grid, S0);

    // --- Output prices & greeks ---
    std::cout << "[European Call]  Price=" << resEuroCall.price
              << "  Delta=" << resEuroCall.delta << "  Gamma=" << resEuroCall.gamma << "\n";
    std::cout << "[European Put ]  Price=" << resEuroPut.price
              << "  Delta=" << resEuroPut.delta  << "  Gamma=" << resEuroPut.gamma << "\n";
    std::cout << "[American Call]  Price=" << resAmerCall.price
              << "  Delta=" << resAmerCall.delta << "  Gamma=" << resAmerCall.gamma << "\n";
    std::cout << "[American Put ]  Price=" << resAmerPut.price
              << "  Delta=" << resAmerPut.delta  << "  Gamma=" << resAmerPut.gamma << "\n";
    std::cout << "[Future       ]  Price=" << resFuture.price
              << "  Delta=" << resFuture.delta   << "  Gamma=" << resFuture.gamma << "\n";

    print_line();

    // --- Test 1: Future analytic value ---
    // V0 = S0*exp(-qT) - K*exp(-rT)
    const double future_ref = S0 * std::exp(-q * T) - K * std::exp(-r * T);
    const double future_err = resFuture.price - future_ref;

    std::cout << "Future check:\n";
    std::cout << "  FD price     = " << resFuture.price << "\n";
    std::cout << "  Analytic     = " << future_ref << "\n";
    std::cout << "  Error (FD-Ref)= " << future_err << "\n";
    print_line();

    // --- Test 2: American Put >= European Put ---
    std::cout << "American Put >= European Put check:\n";
    std::cout << "  AmerPut - EuroPut = " << (resAmerPut.price - resEuroPut.price) << "\n";
    if (resAmerPut.price + 1e-10 >= resEuroPut.price) {
        std::cout << "  OK\n";
    } else {
        std::cout << "  WARNING (should not happen; check boundaries / projection)\n";
    }
    print_line();

    // --- Test 3: American Call >= European Call (strictly if q>0 can happen) ---
    std::cout << "American Call >= European Call check:\n";
    std::cout << "  AmerCall - EuroCall = " << (resAmerCall.price - resEuroCall.price) << "\n";
    if (resAmerCall.price + 1e-10 >= resEuroCall.price) {
        std::cout << "  OK\n";
    } else {
        std::cout << "  WARNING (should not happen)\n";
    }
    print_line();

    // --- Bonus: Put-Call parity for European options ---
    const double parity_rhs = S0 * std::exp(-q * T) - K * std::exp(-r * T);
    const double parity_lhs = resEuroCall.price - resEuroPut.price;
    const double parity_err = parity_lhs - parity_rhs;

    std::cout << "European Put-Call parity check:\n";
    std::cout << "  C - P (FD)          = " << parity_lhs << "\n";
    std::cout << "  S0*exp(-qT)-K*e(-rT)= " << parity_rhs << "\n";
    std::cout << "  Error               = " << parity_err << "\n";

    return 0;
}

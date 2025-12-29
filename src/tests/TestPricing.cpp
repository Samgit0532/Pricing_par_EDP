#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include "model/BlackScholesModel.hpp"
#include "grid/FdGrid.hpp"
#include "grid/GridParameters.hpp"
#include "solvers/ExplicitFdSolver.hpp"
#include "products/EuropeanCall.hpp"
#include "products/EuropeanPut.hpp"
#include "products/AmericanCall.hpp"
#include "products/AmericanPut.hpp"
#include "products/Future.hpp"
#include "products/BullCallSpread.hpp"
#include "products/BearPutSpread.hpp"
#include "products/Straddle.hpp"

static bool approx(double a, double b, double tol) {
    return std::fabs(a - b) <= tol;
}

static void check(bool cond, const std::string& name) {
    if (cond) std::cout << "[OK] " << name << "\n";
    else      std::cout << "[FAIL] " << name << "\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(6);

    const double S0 = 100.0, K = 100.0, T = 1.0;
    const double r = 0.05, sigma = 0.20, q = 0.02;
    BlackScholesModel model(r, sigma, q);

    ExplicitFdSolver solver;

    // base products
    EuropeanCall euroCall(K, T, model);
    EuropeanPut  euroPut(K, T, model);
    AmericanCall amerCall(K, T, model);
    AmericanPut  amerPut(K, T, model);
    Future       future(K, T, model);

    // composite products
    const double K1 = 90.0, K2 = 110.0;
    BullCallSpread bull(K1, K2, T, model);
    BearPutSpread  bear(K1, K2, T, model);
    Straddle       straddle(K, T, model);

    // one grid for same maturity
    FdGrid grid = GridParameters::makeGrid(euroCall, model, S0, 0.002);

    auto C = solver.price(euroCall, model, grid, S0);
    auto P = solver.price(euroPut,  model, grid, S0);
    auto AC = solver.price(amerCall, model, grid, S0);
    auto AP = solver.price(amerPut,  model, grid, S0);
    auto F  = solver.price(future,   model, grid, S0);

    auto Bull = solver.price(bull, model, grid, S0);
    auto Bear = solver.price(bear, model, grid, S0);
    auto Str  = solver.price(straddle, model, grid, S0);

    // --- Checks ---
    const double tol_price = 1e-2;   // tol assez “large” (FD explicite)
    const double tol_tight = 1e-4;   // tol stricte pour identités très fortes

    // 1) Put-call parity (European)
    const double rhs = S0 * std::exp(-q*T) - K * std::exp(-r*T);
    check(approx(C.price - P.price, rhs, tol_price), "European put-call parity");

    // 2) Future price analytic
    check(approx(F.price, rhs, tol_price), "Future analytic value");

    // 3) Future greeks: delta ~ exp(-qT), gamma ~ 0
    check(approx(F.delta, std::exp(-q*T), 5e-3), "Future delta ~ exp(-qT)");
    check(std::fabs(F.gamma) < 1e-3, "Future gamma ~ 0");

    // 4) American dominance
    check(AP.price + 1e-10 >= P.price, "American Put >= European Put");
    check(AC.price + 1e-10 >= C.price, "American Call >= European Call");

    // 5) Spreads are bounded: 0 <= spread <= PV(K2-K1)
    const double pvWidth = (K2 - K1) * std::exp(-r*T);
    check(Bull.price >= -1e-6 && Bull.price <= pvWidth + 1e-2, "Bull call spread bounded");
    check(Bear.price >= -1e-6 && Bear.price <= pvWidth + 1e-2, "Bear put spread bounded");

    // 6) Straddle identity: Call + Put = Straddle (same K,T) (European)
    check(approx(Str.price, C.price + P.price, tol_price), "Straddle ~ Call + Put (European)");

    // 7) Put/Call gamma equality (European)
    check(approx(C.gamma, P.gamma, 5e-3), "European Call gamma == Put gamma");

    std::cout << "\n--- Values (for info) ---\n";
    std::cout << "C=" << C.price << "  P=" << P.price << "  F=" << F.price << "\n";
    std::cout << "AP=" << AP.price << "  AC=" << AC.price << "\n";
    std::cout << "Bull=" << Bull.price << "  Bear=" << Bear.price << "  Str=" << Str.price << "\n";

    return 0;
}

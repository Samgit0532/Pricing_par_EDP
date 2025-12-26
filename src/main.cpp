#include <iostream>
#include "model/BlackScholesModel.cpp"
#include "grid/FdGrid.cpp"
#include "products/EuropeanCall.cpp"
#include "products/EuropeanPut.cpp"
#include "solvers/ExplicitFdSolver.cpp"

int main() {
    double S0 = 100.0, K = 100.0, T = 1.0; /* observable parameters*/
    double r = 0.05, sigma = 0.2;

    BlackScholesModel model(r, sigma);

    FdGrid grid(T, 400.0, /*Nt*/ 20000, /*Ns*/ 400);  /* algorithm precision*/

    EuropeanCall call(K, T, model);
    EuropeanPut  put(K, T, model);

    ExplicitFdSolver solver;

    auto callRes = solver.price(call, model, grid, S0);
    auto putRes  = solver.price(put,  model, grid, S0);

    std::cout << "Call (FD explicit): " << callRes.price << "\n";
    std::cout << "Put  (FD explicit): " << putRes.price  << "\n";

    return 0;
}

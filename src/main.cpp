#include <iostream>
#include "model/BlackScholesModel.cpp"
#include "grid/FdGrid.cpp"

int main() {
    BlackScholesModel model(0.05, 0.2);

    std::cout << "Black–Scholes model:" << std::endl;
    std::cout << "r = " << model.r() << std::endl;
    std::cout << "sigma = " << model.sigma() << std::endl;
    std::cout << "discount(1) = " << model.discount(1.0) << std::endl;
    FdGrid grid(
        1.0,    // T
        200.0,  // Smax
        10,     // Nt
        5       // Ns
    );

    std::cout << "dt = " << grid.dt() << std::endl;
    std::cout << "dS = " << grid.dS() << std::endl;

    std::cout << "Price grid:" << std::endl;
    for (double s : grid.priceGrid())
        std::cout << s << " ";
    std::cout << std::endl;
    
    return 0;
}

#include <iostream>
#include <memory>
#include <limits>
#include "model/BlackScholesModel.hpp"
#include "grid/FdGrid.hpp"
#include "grid/GridParameters.hpp"
#include "solvers/ExplicitFdSolver.hpp"
#include "products/InterfaceProducts.hpp"
#include "products/EuropeanCall.hpp"
#include "products/EuropeanPut.hpp"
#include "products/AmericanCall.hpp"
#include "products/AmericanPut.hpp"
#include "products/Future.hpp"
#include "products/BullCallSpread.hpp"
#include "products/BearPutSpread.hpp"
#include "products/Straddle.hpp"

static void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int read_int(const std::string& prompt, int minv, int maxv) {
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x && x >= minv && x <= maxv) return x;
        std::cout << "Invalid input. Please enter an integer in [" << minv << ", " << maxv << "].\n";
        clear_input();
    }
}

static double read_double(const std::string& prompt, double minv = -1e300, double maxv = 1e300) {
    while (true) {
        std::cout << prompt;
        double x;
        if (std::cin >> x && x >= minv && x <= maxv) return x;
        std::cout << "Invalid input. Please enter a number in [" << minv << ", " << maxv << "].\n";
        clear_input();
    }
}

static void print_menu() {
    std::cout << "\nChoose a product:\n";
    std::cout << " 1) European Call\n";
    std::cout << " 2) European Put\n";
    std::cout << " 3) American Call\n";
    std::cout << " 4) American Put\n";
    std::cout << " 5) Future (payoff S-K)\n";
    std::cout << " 6) Bull Call Spread (Call(K1)-Call(K2), K1<K2)\n";
    std::cout << " 7) Bear Put Spread  (Put(K2)-Put(K1), K1<K2)\n";
    std::cout << " 8) Straddle (Call+Put)\n";
}

int main() {
    std::cout << "=== Black-Scholes PDE Pricer (FD Explicit) ===\n";

    // --- Market parameters ---
    const double S0    = read_double("Spot S0 (e.g 100): ", 0.0);
    const double r     = read_double("Risk-free rate r (e.g 0.5): ");
    const double sigma = read_double("Volatility sigma (>=0) (e.g 0.2): ", 0.0);
    const double q     = read_double("Dividend yield q (e.g 0.02): ");

    BlackScholesModel model(r, sigma, q);

    // --- Common option params ---
    const double T = read_double("Maturity T (>0) (e.g 1): ", 1e-12);

    print_menu();
    const int choice = read_int("Your choice (1-8): ", 1, 8);

    // --- Grid resolution as percentage of S0 ---
    std::cout << "\nSpatial resolution (relative step):\n";
    std::cout << "Enter rel_dS such that dS = rel_dS * S0.\n";
    std::cout << "Typical values: 0.004 (fast), 0.002 (balanced), 0.001 (accurate)\n";
    const double rel_dS = read_double("rel_dS (e.g 0.002): ", 1e-15);

    // We must keep product objects alive after creation.
    std::unique_ptr<InterfaceProducts> product;

    if (choice == 1) {
        const double K = read_double("Strike K (e.g 100): ", 0.0);
        product = std::make_unique<EuropeanCall>(K, T, model);
    } else if (choice == 2) {
        const double K = read_double("Strike K (e.g 100): ", 0.0);
        product = std::make_unique<EuropeanPut>(K, T, model);
    } else if (choice == 3) {
        const double K = read_double("Strike K (e.g 100): ", 0.0);
        product = std::make_unique<AmericanCall>(K, T, model);
    } else if (choice == 4) {
        const double K = read_double("Strike K (e.g 100): ", 0.0);
        product = std::make_unique<AmericanPut>(K, T, model);
    } else if (choice == 5) {
        const double K = read_double("Forward strike K (e.g 100): ");
        product = std::make_unique<Future>(K, T, model);
    } else if (choice == 6) {
        const double K1 = read_double("K1 (e.g 100): ");
        const double K2 = read_double("K2 (>K1) (e.g 120): ");
        if (K2 <= K1) {
            std::cerr << "Error: require K2 > K1.\n";
            return 1;
        }
        product = std::make_unique<BullCallSpread>(K1, K2, T, model);
    } else if (choice == 7) {
        const double K1 = read_double("K1 (e.g 100): ");
        const double K2 = read_double("K2 (>K1) (e.g 120): ");
        if (K2 <= K1) {
            std::cerr << "Error: require K2 > K1.\n";
            return 1;
        }
        product = std::make_unique<BearPutSpread>(K1, K2, T, model);
    } else if (choice == 8) {
        const double K = read_double("Strike K (e.g 100): ", 0.0);
        product = std::make_unique<Straddle>(K, T, model);
    }

    // --- Grid auto (rel_dS controls Ns via dS = rel_dS*S0) ---
    FdGrid grid = GridParameters::makeGrid(*product, model, S0, rel_dS);

    std::cout << "\nGrid: Nt=" << grid.Nt() << " Ns=" << grid.Ns()
              << " dt=" << grid.dt() << " dS=" << grid.dS() << "\n";

    // --- Price ---
    ExplicitFdSolver solver;
    const auto res = solver.price(*product, model, grid, S0);

    std::cout << "\n=== Results ===\n";
    std::cout << "Price : " << res.price << "\n";
    std::cout << "Delta : " << res.delta << "\n";
    std::cout << "Gamma : " << res.gamma << "\n";

    std::cout << "\nDone.\n";
    return 0;
}

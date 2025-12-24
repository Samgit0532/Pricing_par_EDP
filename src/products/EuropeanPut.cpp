#pragma once
#include "InterfaceProducts.cpp"
#include "../model/BlackScholesModel.cpp"
#include <algorithm>
#include <cmath>

/**
 * European Put option under Black–Scholes
 */
class EuropeanPut : public InterfaceProducts {
public:
    EuropeanPut(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(K_ - S, 0.0);
    }

    // At S = 0, put is worth discounted strike
    double leftBoundary(double t) const override {
        const double tau = T_ - t;
        return K_ * std::exp(-model_.r() * tau);
    }

    // For large S, put tends to 0
    double rightBoundary(double /*t*/, double /*Smax*/) const override {
        return 0.0;
    }

private:
    double K_;
    double T_;
    const BlackScholesModel& model_;
};

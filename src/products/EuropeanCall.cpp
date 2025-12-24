#pragma once
#include "InterfaceProducts.cpp"
#include "../model/BlackScholesModel.cpp"
#include <algorithm>
#include <cmath>

/**
 * European Call option under Black–Scholes
 */
class EuropeanCall : public InterfaceProducts {
public:
    EuropeanCall(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(S - K_, 0.0);
    }

    // At S = 0, call is worthless
    double leftBoundary(double /*t*/) const override {
        return 0.0;
    }

    // For large S, call behaves like S - K e^{-r(T-t)}
    double rightBoundary(double t, double Smax) const override {
        const double tau = T_ - t;
        return Smax - K_ * std::exp(-model_.r() * tau);
    }

private:
    double K_;
    double T_;
    const BlackScholesModel& model_;
};

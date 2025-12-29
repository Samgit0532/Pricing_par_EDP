#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>

class Straddle : public InterfaceProducts {
public:
    Straddle(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::fabs(S - K_);
    }

    // At S=0: value behaves like K*e^{-r(T-t)}
    double leftBoundary(double t) const override {
        const double tau = T_ - t;
        if (tau <= 0.0) return payoff(0.0);
        return K_ * std::exp(-model_.r() * tau);
    }

    // At large S: behaves like S*e^{-q(T-t)} - K*e^{-r(T-t)} (dominant call side)
    double rightBoundary(double t, double Smax) const override {
        const double tau = T_ - t;
        if (tau <= 0.0) return payoff(Smax);
        return Smax * std::exp(-model_.q() * tau) - K_ * std::exp(-model_.r() * tau);
    }

private:
    double K_;
    double T_;
    const BlackScholesModel& model_;
};

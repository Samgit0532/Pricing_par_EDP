#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>

class EuropeanCall : public InterfaceProducts {
public:
    EuropeanCall(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(S - K_, 0.0);
    }

    double leftBoundary(double /*t*/) const override {
        return 0.0;
    }

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

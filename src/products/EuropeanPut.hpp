#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>

class EuropeanPut : public InterfaceProducts {
public:
    EuropeanPut(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(K_ - S, 0.0);
    }

    double leftBoundary(double t) const override {
        const double tau = T_ - t;
        if (tau < 0.0) return payoff(0.0);
        return K_ * std::exp(-model_.r() * tau);
    }

    double rightBoundary(double /*t*/, double /*Smax*/) const override {
        return 0.0;
    }

private:
    double K_;
    double T_;
    const BlackScholesModel& model_;
};

#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>

/**
 * American Call option under Black–Scholes.
 * Early exercise can be optimal when q > 0.
 */
class AmericanCall : public InterfaceProducts {
public:
    AmericanCall(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(S - K_, 0.0);
    }

    bool isAmerican() const override { return true; }

    double earlyExerciseValue(double S) const override {
        return payoff(S);
    }

    // At S = 0: worthless
    double leftBoundary(double /*t*/) const override {
        return 0.0;
    }

    // At large S: behaves like S*e^{-q(T-t)} - K*e^{-r(T-t)}
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

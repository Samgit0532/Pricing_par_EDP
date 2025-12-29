#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>

/**
 * American Put option under Black–Scholes.
 */
class AmericanPut : public InterfaceProducts {
public:
    AmericanPut(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return std::max(K_ - S, 0.0);
    }

    bool isAmerican() const override { return true; }

    // Immediate exercise value
    double earlyExerciseValue(double S) const override {
        return payoff(S);
    }

    // At S = 0: for an American put, immediate exercise is optimal -> value = K
    double leftBoundary(double /*t*/) const override {
        return K_;
    }

    // At large S: put ~ 0
    double rightBoundary(double /*t*/, double /*Smax*/) const override {
        return 0.0;
    }

private:
    double K_;
    double T_;
    const BlackScholesModel& model_;
};

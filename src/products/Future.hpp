#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <cmath>

/**
 * Future/Forward-like contract: payoff = (S - K) at maturity.
 * Under dividends q, the fair forward price uses exp(-qT) on S and exp(-rT) on K.
 */
class Future : public InterfaceProducts {
public:
    Future(double strike, double maturity, const BlackScholesModel& model)
        : K_(strike), T_(maturity), model_(model) {}

    double maturity() const override { return T_; }
    double strike() const override { return K_; }

    double payoff(double S) const override {
        return S - K_;
    }

    // At S = 0: value ~ -K*e^{-r(T-t)}
    double leftBoundary(double t) const override {
        const double tau = T_ - t;
        if (tau <= 0.0) return payoff(0.0);
        return -K_ * std::exp(-model_.r() * tau);
    }

    // At large S: value ~ S*e^{-q(T-t)} - K*e^{-r(T-t)}
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

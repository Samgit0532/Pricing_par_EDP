#pragma once
#include <cmath>

/**
 * Black–Scholes model parameters
 * r     : risk-free rate
 * sigma : volatility
 * q     : continuous dividend yield
 */


class BlackScholesModel {
public:
    BlackScholesModel(double r, double sigma, double q = 0.0)
        : r_(r), sigma_(sigma), q_(q) {}

    double r() const { return r_; }
    double sigma() const { return sigma_; }
    double q() const { return q_; }

    // Discount factor exp(-r * taux)
    double discount(double taux) const {
        return std::exp(-r_ * taux);
    }

private:
    double r_;
    double sigma_;
    double q_;
};

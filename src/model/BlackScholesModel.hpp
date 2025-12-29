#pragma once
#include <cmath>
#include <stdexcept>

/**
 * Black–Scholes model parameters
 * r     : risk-free rate
 * sigma : volatility
 * q     : continuous dividend yield
 */

class BlackScholesModel {
public:
    BlackScholesModel(double r, double sigma, double q = 0.0)
        : r_(r), sigma_(sigma), q_(q)
    {
        if (sigma_ < 0.0)
            throw std::invalid_argument("Volatility sigma must be non-negative.");
    }

    double r() const     { return r_; }
    double sigma() const { return sigma_; }
    double q() const     { return q_; }

    // Discount factor exp(-r * t)
    double discount(double t) const {
        return std::exp(-r_ * t);
    }

private:
    double r_;
    double sigma_;
    double q_;
};

#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

class BearPutSpread : public InterfaceProducts {
public:
    BearPutSpread(double K1, double K2, double maturity, const BlackScholesModel& model)
        : K1_(K1), K2_(K2), T_(maturity), model_(model)
    {
        if (K1_ >= K2_) throw std::invalid_argument("BearPutSpread requires K1 < K2");
    }

    double maturity() const override { return T_; }
    double strike() const override { return K2_; } // arbitrary; return upper strike

    double payoff(double S) const override {
        return std::max(K2_ - S, 0.0) - std::max(K1_ - S, 0.0);
    }

    // At S=0: payoff saturates at (K2-K1)
    double leftBoundary(double t) const override {
        const double tau = T_ - t;
        if (tau <= 0.0) return (K2_ - K1_);
        return (K2_ - K1_) * std::exp(-model_.r() * tau);
    }

    // At large S: 0
    double rightBoundary(double /*t*/, double /*Smax*/) const override {
        return 0.0;
    }

private:
    double K1_;
    double K2_;
    double T_;
    const BlackScholesModel& model_;
};

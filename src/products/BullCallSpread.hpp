#pragma once
#include "InterfaceProducts.hpp"
#include "../model/BlackScholesModel.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

class BullCallSpread : public InterfaceProducts {
public:
    BullCallSpread(double K1, double K2, double maturity, const BlackScholesModel& model)
        : K1_(K1), K2_(K2), T_(maturity), model_(model)
    {
        if (K1_ >= K2_) throw std::invalid_argument("BullCallSpread requires K1 < K2");
    }

    double maturity() const override { return T_; }
    double strike() const override { return K1_; } 

    double payoff(double S) const override {
        return std::max(S - K1_, 0.0) - std::max(S - K2_, 0.0);
    }

    double leftBoundary(double /*t*/) const override {
        return 0.0;
    }

    // For large S: value tends to (K2-K1) discounted by r (since payoff saturates at K2-K1)
    double rightBoundary(double t, double /*Smax*/) const override {
        const double tau = T_ - t;
        if (tau <= 0.0) return (K2_ - K1_);
        return (K2_ - K1_) * std::exp(-model_.r() * tau);
    }

private:
    double K1_;
    double K2_;
    double T_;
    const BlackScholesModel& model_;
};

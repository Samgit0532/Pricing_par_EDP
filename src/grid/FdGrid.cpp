#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * Finite-difference grid in time and asset price
 */
class FdGrid {
public:
    FdGrid(double T, double Smax, int NbTimeSteps, int NbPriceSteps, double Smin = 0.0)
        : T_(T), Smin_(Smin), Smax_(Smax), Nt_(NbTimeSteps), Ns_(NbPriceSteps) {

        if (Nt_ <= 0 || Ns_ <= 1)
            throw std::invalid_argument("Nt > 0 and Ns > 1 required");

        dt_ = T_ / Nt_;
        dS_ = (Smax_ - Smin_) / Ns_;

        // Time grid
        t_.resize(Nt_ + 1);
        for (int n = 0; n <= Nt_; ++n)
            t_[n] = n * dt_;

        // Price grid
        S_.resize(Ns_ + 1);
        for (int i = 0; i <= Ns_; ++i)
            S_[i] = Smin_ + i * dS_;
    }

    // --- getters ---
    double T() const { return T_; }
    double dt() const { return dt_; }
    double dS() const { return dS_; }

    int Nt() const { return Nt_; }
    int Ns() const { return Ns_; }

    const std::vector<double>& timeGrid() const { return t_; }
    const std::vector<double>& priceGrid() const { return S_; }

    // Linear interpolation of a value defined on the S-grid
    double interpolate(const std::vector<double>& V, double S0) const {
        if (S0 <= S_.front()) return V.front();
        if (S0 >= S_.back())  return V.back();

        auto it = std::upper_bound(S_.begin(), S_.end(), S0);
        int i = int(it - S_.begin()) - 1;

        double w = (S0 - S_[i]) / (S_[i+1] - S_[i]);
        return (1.0 - w) * V[i] + w * V[i+1];
    }

private:
    double T_;
    double Smin_;
    double Smax_;
    int Nt_;
    int Ns_;

    double dt_;
    double dS_;

    std::vector<double> t_;
    std::vector<double> S_;
};

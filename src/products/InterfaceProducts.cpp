#pragma once

/**
 * Interface for option products
 */
class InterfaceProducts {
public:
    virtual ~InterfaceProducts() = default;

    // Contract parameters
    virtual double maturity() const = 0;
    virtual double strike() const = 0;

    // Terminal payoff V(T, S)
    virtual double payoff(double S) const = 0;

    // Boundary conditions
    virtual double leftBoundary(double t) const = 0;                 // S = Smin
    virtual double rightBoundary(double t, double Smax) const = 0;   // S = Smax

    // Early exercise (for American options)
    virtual bool isAmerican() const { return false; }
    virtual double earlyExerciseValue(double S) const { return payoff(S); }
};

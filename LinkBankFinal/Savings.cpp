#include "Savings.h"
#include <cmath>

double Savings::computeInterest(double initialBalance, double annualRate, int years) const {
    auto calculateInterest = [initialBalance, annualRate, years]() -> double {
        int n = 12;  // Compounded monthly
        return initialBalance * pow(1 + annualRate / n, n * years);
    };
    return calculateInterest();
}

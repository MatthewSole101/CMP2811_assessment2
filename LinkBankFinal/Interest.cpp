#include "Interest.h"

Interest::Interest() {}

double Interest::computeInterest(double initialBalance, double annualRate, int years) const {
    // Compound interest formula: A = P(1 + r/n)^(nt)
    int n = 12;  // Compounded monthly
    return initialBalance * pow(1 + annualRate / n, n * years);
}

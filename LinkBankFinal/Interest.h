#ifndef INTEREST_H
#define INTEREST_H

#include <iostream>
#include <cmath>

class Interest {
public:
    Interest();
    double computeInterest(double initialBalance, double annualRate, int years) const;
};

#endif

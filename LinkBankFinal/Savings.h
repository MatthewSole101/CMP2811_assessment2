#ifndef SAVINGS_H
#define SAVINGS_H

#include "Account.h"
#include "InterestEarning.h"

class Savings : public Account, public InterestEarning {
public:
    double computeInterest(double initialBalance, double annualRate, int years) const override;
};

#endif

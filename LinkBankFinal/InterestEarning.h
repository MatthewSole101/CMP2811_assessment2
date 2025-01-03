#ifndef INTEREST_EARNING_H
#define INTEREST_EARNING_H

class InterestEarning {
public:
    virtual double computeInterest(double initialBalance, double annualRate, int years) const = 0;
    virtual ~InterestEarning() {}
};

#endif

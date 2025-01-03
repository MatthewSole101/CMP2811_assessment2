#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class Account {
protected:
    map<int, double> SavingsBalance;
    map<int, double> CurrentBalance;
    map<int, double> IsaBalance;
    int nextAccountNumber = 1;
    map<int, string> AccountCreationTime;
    map<int, vector<pair<string, string>>> TransferHistory;
    map<int, int> Balance;
    map<int, string> InitialSavingDeposit;
    map<int, string> InitialCurrentBalanceDeposit;
    map<int, string> InitialIsaDeposit;
    map<int, vector<pair<double, string>>> DepositHistory;

public:
    Account();
    string getCurrentTimestamp();
    virtual void makeAccount(int type, double depositAmount);
    virtual void getAccountDetails(int accountNumber) const;
    void getTimeStamp(int type, int accountnum);
    int getAccountType(int accountnum);
    virtual double getBalance(int accountNumber);
    int getIsaBalance();
    virtual void deposit(int accountNumber, double amount);
    void withdraw(int accountNumber, double amount);
    void transfer(int fromAccount, int toAccount, double amount);
    virtual ~Account();
};

#endif

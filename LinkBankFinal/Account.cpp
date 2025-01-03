#include "Account.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

Account::Account() {}

string Account::getCurrentTimestamp() {
    time_t timestamp;
    time(&timestamp);
    char timeString[26];
    ctime_s(timeString, sizeof(timeString), &timestamp);
    timeString[24] = '\0';  // Remove the newline character
    return string(timeString);
}

void Account::makeAccount(int type, double depositAmount) {
    int accountNumber = nextAccountNumber++;
    string timestampStr = getCurrentTimestamp();

    if (type == 1) { // Savings Account
        CurrentBalance[accountNumber] = depositAmount;
        AccountCreationTime[accountNumber] = timestampStr;
        InitialCurrentBalanceDeposit[accountNumber] = timestampStr;
        DepositHistory[accountNumber].emplace_back(depositAmount, timestampStr);
        cout << "Current account created with Account Number: " << accountNumber << endl;
    } else if (type == 2) { // Current Account
        SavingsBalance[accountNumber] = depositAmount;
        AccountCreationTime[accountNumber] = timestampStr;
        InitialSavingDeposit[accountNumber] = timestampStr;
        DepositHistory[accountNumber].emplace_back(depositAmount, timestampStr);
        cout << "Savings account created with Account Number: " << accountNumber << endl;
    } else if (type == 3) { // ISA Account
        if (depositAmount < 1000) {
            cout << "ISA initial balance must be >= £1000" << endl;
            nextAccountNumber--;
            return;
        } else if (IsaBalance.empty()) {
            IsaBalance[accountNumber] = depositAmount;
            AccountCreationTime[accountNumber] = timestampStr;
            InitialIsaDeposit[accountNumber] = timestampStr;
            DepositHistory[accountNumber].emplace_back(depositAmount, timestampStr);
            cout << "ISA account created with Account Number: " << accountNumber << endl;
        } else {
            cout << "Only one ISA account allowed." << endl;
            nextAccountNumber--;
        }
    } else {
        cout << "Invalid account type." << endl;
        nextAccountNumber--;
    }
}

void Account::getAccountDetails(int accountNumber) const {
    auto it = AccountCreationTime.find(accountNumber);
    if (it != AccountCreationTime.end()) {
        cout << "Account Number: " << accountNumber
             << ", Created on: " << it->second << endl;

        if (SavingsBalance.count(accountNumber)) {
            cout << "Savings Balance: £" << SavingsBalance.at(accountNumber) << endl;
        } else if (CurrentBalance.count(accountNumber)) {
            cout << "Current Balance: £" << CurrentBalance.at(accountNumber) << endl;
        } else if (IsaBalance.count(accountNumber)) {
            cout << "ISA Balance: £" << IsaBalance.at(accountNumber) << endl;
        }
    } else {
        cout << "Account not found." << endl;
    }
}

void Account::getTimeStamp(int type, int accountnum) {
    cout << "Transaction history for Account " << accountnum << ":" << endl;

    if (DepositHistory.count(accountnum) && !DepositHistory[accountnum].empty()) {
        const auto& initialDeposit = DepositHistory[accountnum][0];
        cout << "-- Initial deposit: £" << fixed << setprecision(2) << initialDeposit.first
             << " on " << initialDeposit.second << endl;

        for (size_t i = 1; i < DepositHistory[accountnum].size(); ++i) {
            const auto& record = DepositHistory[accountnum][i];
            cout << "-- Deposit: £" << fixed << setprecision(2) << record.first
                 << " on " << record.second << endl;
        }
    } else {
        cout << "No deposits recorded for this account." << endl;
    }

    if (TransferHistory.count(accountnum)) {
        for (const auto& transfer : TransferHistory[accountnum]) {
            cout << "-- " << transfer.first << " on " << transfer.second << endl;
        }
    } else {
        cout << "No transfers recorded for this account." << endl;
    }
}

int Account::getAccountType(int accountnum) {
    if (SavingsBalance.count(accountnum)) {
        cout << "Account number " << accountnum << " is a Savings account." << endl;
        return 1;
    } else if (CurrentBalance.count(accountnum)) {
        cout << "Account number " << accountnum << " is a Current account." << endl;
        return 2;
    } else if (IsaBalance.count(accountnum)) {
        cout << "Account number " << accountnum << " is an ISA account." << endl;
        return 3;
    } else {
        cout << "Account number " << accountnum << " not found." << endl;
        return 0;
    }
}

double Account::getBalance(int accountNumber) {
    if (SavingsBalance.count(accountNumber)) {
        return SavingsBalance[accountNumber];
    } else if (CurrentBalance.count(accountNumber)) {
        return CurrentBalance[accountNumber];
    } else if (IsaBalance.count(accountNumber)) {
        return IsaBalance[accountNumber];
    } else {
        cout << "Account not found." << endl;
        return -1;
    }
}

int Account::getIsaBalance() {
    auto it = IsaBalance.begin();
    return it->first;
}

void Account::deposit(int accountNumber, double amount) {
    string timestampStr = getCurrentTimestamp();

    if (SavingsBalance.count(accountNumber)) {
        SavingsBalance[accountNumber] += amount;
        DepositHistory[accountNumber].emplace_back(amount, timestampStr);
        cout << "Deposit successful. New Savings balance: £" << fixed << setprecision(2)
             << SavingsBalance[accountNumber] << endl;
    } else if (CurrentBalance.count(accountNumber)) {
        CurrentBalance[accountNumber] += amount;
        DepositHistory[accountNumber].emplace_back(amount, timestampStr);
        cout << "Deposit successful. New Current balance: £" << fixed << setprecision(2)
             << CurrentBalance[accountNumber] << endl;
    } else if (IsaBalance.count(accountNumber)) {
        IsaBalance[accountNumber] += amount;
        DepositHistory[accountNumber].emplace_back(amount, timestampStr);
        cout << "Deposit successful. New ISA balance: £" << fixed << setprecision(2)
             << IsaBalance[accountNumber] << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void Account::withdraw(int accountNumber, double amount) {
    if (SavingsBalance.count(accountNumber)) {
        if (SavingsBalance[accountNumber] >= amount) {
            SavingsBalance[accountNumber] -= amount;
            cout << "Withdrawal successful. New balance: £" << SavingsBalance[accountNumber] << endl;
        } else {
            cout << "Insufficient funds." << endl;
        }
    } else if (CurrentBalance.count(accountNumber)) {
        if (CurrentBalance[accountNumber] - amount >= -500) {
            CurrentBalance[accountNumber] -= amount;
            cout << "Withdrawal successful. New balance: £" << CurrentBalance[accountNumber] << endl;
        } else {
            cout << "Insufficient funds." << endl;
        }
    } else {
        cout << "Account not found." << endl;
    }
}

void Account::transfer(int fromAccount, int toAccount, double amount) {
    string timestamp = getCurrentTimestamp();
    if (fromAccount == toAccount) {
        cout << "Cannot transfer to the same account." << endl;
        return;
    }

    auto updateBalance = [&](map<int, double>& fromBalance, map<int, double>& toBalance) {
        if (fromBalance.count(fromAccount) && (fromBalance[fromAccount] >= amount)) {
            fromBalance[fromAccount] -= amount;
            toBalance[toAccount] += amount;

            stringstream transferFrom, transferTo;
            transferFrom << std::fixed << std::setprecision(2) << amount;
            transferTo << std::fixed << std::setprecision(2) << amount;

            TransferHistory[fromAccount].emplace_back(
                "Transfer to account " + to_string(toAccount) + ": -£" + transferFrom.str(), timestamp);
            TransferHistory[toAccount].emplace_back(
                "Transfer from account " + to_string(fromAccount) + ": +£" + transferTo.str(), timestamp);

            cout << "Transfer successful!" << endl;
            return true;
        }
        return false;
    };

    if (SavingsBalance.count(fromAccount)) {
        if (updateBalance(SavingsBalance, SavingsBalance.count(toAccount) ? SavingsBalance : CurrentBalance)) return;
    } else if (CurrentBalance.count(fromAccount)) {
        if (CurrentBalance[fromAccount] - amount >= -500) {
            if (updateBalance(CurrentBalance, CurrentBalance.count(toAccount) ? CurrentBalance : SavingsBalance)) return;
        }
    }

    cout << "Transfer failed: Insufficient funds or invalid accounts." << endl;
}

Account::~Account() {}

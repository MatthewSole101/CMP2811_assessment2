#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <iomanip>


using namespace std;

class Account {
protected:
    map<int, double> SavingsBalance;
    map<int, double> CurrentBalance;
    map<int, double> IsaBalance;

    int nextAccountNumber = 1;
    map<int, string> AccountCreationTime; // Maps account number to creation timestamp

    map<int, vector<pair<string, string>>> TransferHistory;  // keeps track of transactions in tranfer cfunction





    //amount type - 1 - saving 2 - current 3 - ISA
    map<int, int> Balance;

    map <int, string>InitialSavingDeposit;
    map <int, string>InitialCurrentBalanceDeposit;
    map <int, string>InitialIsaDeposit;
    map<int, vector<pair<double, string>>> DepositHistory;





public:
    Account() {
    }
    string getCurrentTimestamp() {
        time_t timestamp;
        time(&timestamp);
        char timeString[26];
        ctime_s(timeString, sizeof(timeString), &timestamp);
        timeString[24] = '\0';  // Remove the newline character
        return string(timeString);
    }
    virtual void makeAccount(int type, double depositAmount) {
        int accountNumber = nextAccountNumber++;
        string timestampStr = getCurrentTimestamp();

        if (type == 1) { // Savings Account
            SavingsBalance[accountNumber] = depositAmount;
            AccountCreationTime[accountNumber] = timestampStr;
            InitialSavingDeposit[accountNumber] = timestampStr;
            DepositHistory[accountNumber].emplace_back(depositAmount, timestampStr);
            cout << "Savings account created with Account Number: " << accountNumber << endl;
        } else if (type == 2) { // Current Account
            CurrentBalance[accountNumber] = depositAmount;
            AccountCreationTime[accountNumber] = timestampStr;
            InitialCurrentBalanceDeposit[accountNumber] = timestampStr;
            DepositHistory[accountNumber].emplace_back(depositAmount, timestampStr);
            cout << "Current account created with Account Number: " << accountNumber << endl;
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


    virtual void getAccountDetails(int accountNumber) const {
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
    void getTimeStamp(int type, int accountnum) {
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




    int getAccountType(int accountnum) {
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



    virtual double getBalance(int accountNumber) {
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

    //used for project
    int getIsaBalance() {
        auto it = IsaBalance.begin();
        return it->first;
    }








    virtual void deposit(int accountNumber, double amount) {
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




    void withdraw(int accountNumber, double amount) {
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



    void transfer(int fromAccount, int toAccount, double amount) {
        string timestamp = getCurrentTimestamp();
        if (fromAccount == toAccount) {
            cout << "Cannot transfer to the same account." << endl;
            return;
        }

        auto updateBalance = [&](map<int, double>& fromBalance, map<int, double>& toBalance) {
            if (fromBalance.count(fromAccount) && (fromBalance[fromAccount] >= amount)) {
                fromBalance[fromAccount] -= amount;
                toBalance[toAccount] += amount;

                TransferHistory[fromAccount].emplace_back(
                    "Transfer to account " + to_string(toAccount) + ": -£" + to_string(amount), timestamp);
                TransferHistory[toAccount].emplace_back(
                    "Transfer from account " + to_string(fromAccount) + ": +£" + to_string(amount), timestamp);

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





    virtual ~Account() {}
};

class InterestEarning {
public:
    virtual double computeInterest(double initialBalance, double annualRate, int years) const = 0;
    virtual ~InterestEarning() {}
};

class Savings : public Account, public InterestEarning {
public:
    double computeInterest(double initialBalance, double annualRate, int years) const override {
        auto calculateInterest = [initialBalance, annualRate, years]() -> double {
            int n = 12;
            return initialBalance * pow(1 + annualRate / n, n * years);
        };
        return calculateInterest();
    }
};




class Interest: public Account {
    public:


};

int main() {
    vector<string> parameters;
    string userCommand;
    Account* account = new Savings();

    cout << "~~~ Welcome to LincBank! ~~~" << endl;
    cout << "open type initial_deposit: open a current (1), savings (2) or ISA (3) account" << endl;
    cout << "view [index]: view balance and recent transactions" << endl;
    cout << "withdraw sum: withdraw funds from most recently viewed account" << endl;
    cout << "deposit sum: deposit funds into most recently viewed account" << endl;
    cout << "transfer src dest sum: transfer funds between accounts" << endl;
    cout << "project years: project balance forward in time" << endl;
    cout << "exit: close this application" << endl;
    cout << "options: view these options again" << endl;

    while (true) {
        parameters.clear();
        cout << endl << ">>> ";
        getline(cin, userCommand);

        if (userCommand == "exit") {
            cout << "Goodbye!" << endl;
            break;
        }

        stringstream ss(userCommand);
        string token;
        while (ss >> token) {
            parameters.push_back(token);
        }

        string command = parameters[0];
        if (command == "options") {
            // Reprint the options
            cout << "open type initial_deposit: open a current (1), savings (2) or ISA (3) account" << endl;
            cout << "view [index]: view balance and recent transactions" << endl;
            cout << "withdraw sum: withdraw funds from most recently viewed account" << endl;
            cout << "deposit sum: deposit funds into most recently viewed account" << endl;
            cout << "transfer src dest sum: transfer funds between accounts" << endl;
            cout << "project years: project balance forward in time" << endl;
            cout << "exit: close this application" << endl;
            cout << "options: view these options again" << endl;
        }
        else if (command == "open") {
            int type = stoi(parameters[1]);
            double depositAmount = stod(parameters[2]);
            account->makeAccount(type, depositAmount);
            cin.ignore();
        }
        else if (command == "view") {
            if (parameters.size() < 2) {
                cout << "Invalid command. Usage: view <account_number>" << endl;
                continue;
            }
            int accountNumber = stoi(parameters[1]);
            double balance = account->getBalance(accountNumber);
            int accounttype = account->getAccountType(accountNumber);
            if (balance != -1) {
                cout << "Account: " << accountNumber << ", Balance: £" << fixed << setprecision(2) << balance << endl;
                account->getTimeStamp(accounttype, accountNumber);
            } else {
                cout << "Account not found." << endl;
            }
            cin.ignore();
        }

        else if (command == "deposit") {
            int accountNumber = stoi(parameters[1]);
            double amount = stod(parameters[2]);
            account->deposit(accountNumber, amount);
        }
        else if (command == "withdraw") {
            // int type = stoi(parameters[1]);
            int accountNumber = stoi(parameters[1]);
            double amount = stod(parameters[2]);
            account->withdraw(accountNumber, amount);
        }
        else if (command == "transfer") {
            int fromaccount = stoi(parameters[1]);
            int toaccount = stoi(parameters[2]);
            double amount = stod(parameters[3]);

            account->transfer(fromaccount, toaccount, amount);
        }
        else if (command == "project") {
            int years = stoi(parameters[1]);
            int isabalance = account->getIsaBalance();
            cout << isabalance << endl;
            double initialBalance = account->getBalance(isabalance);
            if (initialBalance != -1) {
                Savings* savingsAccount = dynamic_cast<Savings*>(account);
                double annualRate = 1.15; // Example 0.85% annual interest
                double projectedBalance = savingsAccount->computeInterest(initialBalance, annualRate, years);
                cout << "Projected balance: £" << fixed << setprecision(2) << projectedBalance << endl;
            }
        }
        else {
            cout << "Invalid command. Input 'options' to see all commands." << endl;
        }
    }

    // Move delete outside the loop and after the break
    delete account;
    return 0;
}

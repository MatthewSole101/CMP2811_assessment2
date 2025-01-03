#include "Account.h"
#include "Savings.h"
#include "InterestEarning.h"
#include "Interest.h"

int main() {
    vector<string> parameters;
    string userCommand;
    Account* account = new Savings(); // Use Savings or Interest as needed

    // Your main logic for account management here
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

#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountHolderName;
    int accountNumber;
    double balance;
public:
    BankAccount(string name, int number, double initialBalance) {
        accountHolderName = name;
        accountNumber = number;
        balance = initialBalance;
    }

    // Add this getter method
    int getAccountNumber() const {
        return accountNumber;
    }
    void deposit(double amount) {
        balance += amount;
        cout << "Deposited $" << amount << ". New balance: $" << balance << endl;
    }
    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds. Withdrawal failed." << endl;
        }
        else {
            balance -= amount;
            cout << "Withdrew $" << amount << ". New balance: $" << balance << endl;
        }
    }
    void displayBalance() {
    cout << "Current balance: $" << balance << endl;
    }
};

class BankSystem {
private:
    BankAccount* accounts[100];
    int accountCount;

public:
    BankSystem() : accountCount(0) {}

    void createAccount(string name, double initialBalance) {
        int accNum = rand() % 9000 + 1000;
        accounts[accountCount++] = new BankAccount(name, accNum, initialBalance);
        cout << "Account created successfully! Account Number: " << accNum << endl;
    }

    BankAccount* findAccount(int accNum) {
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i]->getAccountNumber() == accNum)
                return accounts[i];
        }
        return nullptr;
    }

    // Add deposit, withdraw, displayBalance wrappers here
};


int main() {
    srand(time(0));
    BankSystem chase;
    int choice;

    do {
        cout << "\n***************Welcome to Chase***************\n";
        cout << "1- Create a new bank account\n";
        cout << "2- Deposit money\n";
        cout << "3- Withdraw money\n";
        cout << "4- Display balance\n";
        cout << "5- Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            double balance;
            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter initial balance: ";
            cin >> balance;
            chase.createAccount(name, balance);
        }
        else if (choice >= 2 && choice <= 4) {
            int accNum;
            cout << "Enter account number: ";
            cin >> accNum;
            BankAccount* acc = chase.findAccount(accNum);
            if (acc) {
                if (choice == 2) {
                    double amount;
                    cout << "Enter deposit amount: ";
                    cin >> amount;
                    acc->deposit(amount);
                }
                else if (choice == 3) {
                    double amount;
                    cout << "Enter withdrawal amount: ";
                    cin >> amount;
                    acc->withdraw(amount);
                }
                else {
                    acc->displayBalance();
                }
            }
            else {
                cout << "No account number exists!" << endl;
            }
        }
    } while (choice != 5);

    cout << "Thank you for banking with Chase!" << endl;
    return 0;
}

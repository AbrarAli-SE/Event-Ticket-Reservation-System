#include <iostream>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <sstream>
#include <conio.h>

using namespace std;

bool phoneValidation(string input)
{
    if (input.empty())
    {
        return false;
    }
    if (input[0] != '0')
    {
        return false;
    }

    if (input.size() > 1 && input[1] != '3')
    {
        return false;
    }

    for (int i = 0; i < input.size(); ++i)
    {
        if (!isdigit(input[i]) && input[i] == ' ')
        {
            return false;
        }
    }
    if (input.size() == 11)
    {
        return true;
    }
    return false;
}

bool menuChoice(string choice)
{
    for (int i = 0; i < choice.size(); ++i)
    {
        if (!isdigit(choice[i]))
        {
            return false;
        }
    }
    if (choice.empty())
    {
        return false;
    }
    for (int i = 0; i < choice.size(); ++i)
    {
        if (choice[i] == ' ')
        {
            return false;
        }
    }
    return true;
}

bool amountValid(string &amount)
{
    if (amount.empty())
    {
        return false;
    }

    for (char c : amount)
    {
        if (!isdigit(c)) // Only digits are allowed
        {
            return false;
        }
    }

    return true;
}

void passLogic(string &password, string promptText)
{
    char pass[32] = {0};
    char ch;
    bool enter = false;
    int i = 0;
    bool show = false;

    cout << promptText;

    while (!enter)
    {
        ch = _getch();

        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
        {
            pass[i] = ch;
            if (show)
            {
                cout << ch;
            }
            else
            {
                cout << "*";
            }
            i++;
        }

        if (ch == '\b' && i >= 1)
        {
            cout << "\b \b";
            i--;
        }

        if (ch == '\r')
        {
            enter = true;
        }

        if (ch == '\t')
        {
            show = !show;
            cout << "\r" << promptText;
            for (int j = 0; j < i; j++)
            {
                cout << (show ? pass[j] : '*');
            }
        }
    }

    pass[i] = '\0';
    password = pass;
}

bool pinValid(string token)
{
    if (token.empty())
    {
        return false;
    }
    for (int i = 0; i < token.size(); ++i)
    {
        if (!isdigit(token[i]) && token[i] == ' ')
        {
            return false;
        }
    }
    if (token.size() == 4)
    {
        return true;
    }
    return false;
}

bool cnicValid(string token)
{
    if (token.empty())
    {
        return false;
    }
    for (int i = 0; i < token.size(); ++i)
    {
        if (!isdigit(token[i]) && token[i] == ' ')
        {
            return false;
        }
    }
    if (token.size() == 13)
    {
        return true;
    }
    return false;
}

bool accValid(string token)
{
    if (token.empty())
    {
        return false;
    }
    for (int i = 0; i < token.size(); ++i)
    {
        if (!isdigit(token[i]) && token[i] == ' ')
        {
            return false;
        }
    }
    if (token.size() == 7)
    {
        return true;
    }
    return false;
}

bool nameValid(string name)
{
    if (name.empty())
    {
        return false;
    }
    return true;
}

class Account
{

public:
    string name;
    string accountNo;
    string pin;
    string cnic;
    double balance;
    string cellNo;
    vector<string> transactionHistory;
    bool loanRequested;
    bool loanApproved;
    double loanAmountRequested; // New variable to store requested loan amount

    // Default constructor
    Account()
        : name(""), accountNo(""), pin(""), cnic(""), balance(0.0), cellNo(""), loanRequested(false), loanApproved(false), loanAmountRequested(0.0) {}

    Account(string name, string accNo, string accPin, string accCnic, string accCellNo, double initialBalance)
        : accountNo(accNo), pin(accPin), cnic(accCnic), cellNo(accCellNo), balance(initialBalance), loanRequested(false), loanApproved(false), loanAmountRequested(0.0) {}

    void addTransaction(const string &transaction)
    {
        stringstream ss;
        time_t now = time(0);
        tm *localtm = localtime(&now);
        ss << asctime(localtm) << ": " << transaction;
        transactionHistory.push_back(ss.str());
    }

    void requestLoan(double amount) // Modify the loan request function to include the amount
    {
        if (amount > 0)
        {
            loanRequested = true;
            loanAmountRequested = amount;
            cout << "Loan request for Rs. " << amount << " has been made.\n";
        }
        else
        {
            cout << "Invalid loan amount.\n";
        }
    }
};

class Bank
{
private:
    unordered_map<string, Account> accounts;     // Account number to Account object
    unordered_map<string, string> cnicToAccount; // CNIC to Account number

public:
    // Customer operations
    void createAccount(const string &name, const string &accPin, const string &accCnic, const string &accCellNo, double initialBalance);
    string generateAccountNumber();
    string forgotPin(const string &cnic);
    void showAccountDetails(const string &accNo);
    void showTransactionHistory(const string &accNo);
    void requestLoan(const string &accNo);

    // Admin operations
    void deleteAccount(const string &accNo);
    void searchAccount(const string &accNo);
    void approveLoan(const string &accNo, bool approve);
    void showAllAccounts();

    // ATM operations
    bool authenticate(const string &accNo, const string &pin);
    void withdraw(const string &accNo, double amount);
    void deposit(const string &accNo, double amount);
    void atmInterface();

    // Panels
    void userPanel();
    void adminPanel();

    // Utility functions
    bool accountExists(const string &accNo);
    bool verifyPin(const string &accNo, const string &pin);
};

string Bank::forgotPin(const string &cnic)
{
    if (cnicToAccount.find(cnic) != cnicToAccount.end())
    {
        string accNo = cnicToAccount[cnic];
        return accounts[accNo].pin;
    }
    return "CNIC not found.";
}

bool Bank::authenticate(const string &accNo, const string &pin)
{
    return verifyPin(accNo, pin);
}

// Utility functions implementation
bool Bank::accountExists(const string &accNo)
{
    return accounts.find(accNo) != accounts.end();
}

bool Bank::verifyPin(const string &accNo, const string &pin)
{
    if (accounts.find(accNo) != accounts.end())
    {
        return accounts[accNo].pin == pin;
    }
    return false;
}
string Bank::generateAccountNumber()
{
    srand(time(0)); // Seed the random number generator
    stringstream ss;
    for (int i = 0; i < 7; ++i)
    {
        ss << rand() % 10; // Generate a random digit
    }
    return ss.str();
}

void Bank::createAccount(const string &name, const string &accPin, const string &accCnic, const string &accCellNo, double initialBalance)
{
    string accNo = generateAccountNumber();
    // string accNo = "1";
    if (accounts.find(accNo) == accounts.end())
    {
        accounts[accNo] = Account(name, accNo, accPin, accCnic, accCellNo, initialBalance);
        cnicToAccount[accCnic] = accNo;
        cout << "Account created successfully.\n";
        cout << "Account Number: " << accNo << "\n";
    }
    else
    {
        cout << "Account number already exists. Please try again.\n";
    }
}

void Bank::showAccountDetails(const string &accNo)
{
    if (accounts.find(accNo) != accounts.end())
    {
        Account acc = accounts[accNo];
        cout << "Account Number: " << acc.accountNo << "\nBalance: " << acc.balance << "\nLoan Requested: " << (acc.loanRequested ? "Yes" : "No") << "\nLoan Approved: " << (acc.loanApproved ? "Yes" : "No") << "\n";
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::showTransactionHistory(const string &accNo)
{
    if (accounts.find(accNo) != accounts.end())
    {
        Account acc = accounts[accNo];
        cout << "Transaction History for Account Number: " << acc.accountNo << "\n";
        for (const string &transaction : acc.transactionHistory)
        {
            cout << transaction << "\n";
        }
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::withdraw(const string &accNo, double amount)
{
    if (accounts.find(accNo) != accounts.end())
    {
        if (accounts[accNo].balance >= amount)
        {
            accounts[accNo].balance -= amount;
            accounts[accNo].addTransaction("Withdraw: Rs. " + to_string(amount));
            cout << "Withdrawal successful. New Balance: " << accounts[accNo].balance << "\n";
        }
        else
        {
            cout << "Insufficient balance.\n";
        }
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::deposit(const string &accNo, double amount)
{
    if (accounts.find(accNo) != accounts.end())
    {
        accounts[accNo].balance += amount;
        accounts[accNo].addTransaction("Deposit: Rs. " + to_string(amount));
        cout << "Deposit successful. New Balance: " << accounts[accNo].balance << "\n";
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::showAllAccounts()
{
    for (const auto &pair : accounts)
    {
        const Account &acc = pair.second;
        cout << "Account Number: " << acc.accountNo << "\n"
             << "CNIC: " << acc.name << "\n"
             << "CNIC: " << acc.cellNo << "\n"
             << "CNIC: " << acc.cnic << "\n"
             << "Balance: " << acc.balance << "\n"
             << "Loan Requested: " << (acc.loanRequested ? "Yes, Amount: Rs. " + to_string(acc.loanAmountRequested) : "No") << "\n"
             << "Loan Approved: " << (acc.loanApproved ? "Yes" : "No") << "\n\n";
    }
}

void Bank::deleteAccount(const string &accNo)
{
    if (accounts.find(accNo) != accounts.end())
    {
        accounts.erase(accNo);
        cout << "Account deleted successfully.\n";
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::searchAccount(const string &accNo)
{
    if (accounts.find(accNo) != accounts.end())
    {
        showAccountDetails(accNo);
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::requestLoan(const string &accNo)
{
    if (accounts.find(accNo) != accounts.end())
    {
        if (accounts[accNo].loanRequested)
        {
            cout << "Loan has already been requested for this account.\n";
        }
        else
        {
            string amountstr;
            double loanAmount;
            cout << "Enter loan amount: Rs. ";
            while(true)
            {
                cout << "Enter amount to withdraw: ";
                getline(cin, amountstr);
                if (amountValid(amountstr))
                {
                    loanAmount = stod(amountstr);
                    break;
                }
                cout << "\n\tInvalid amount.\n";
            }

            if (loanAmount <= 0)
            {
                cout << "Invalid loan amount. Please enter a positive value.\n";
            }
            else
            {
                accounts[accNo].requestLoan(loanAmount); // Request loan with amount
            }
        }
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::approveLoan(const string &accNo, bool approve)
{
    if (accounts.find(accNo) != accounts.end())
    {
        Account &acc = accounts[accNo];
        if (acc.loanRequested)
        {
            cout << "Loan Request Details for Account Number: " << accNo << "\n";
            cout << "Requested Loan Amount: Rs. " << acc.loanAmountRequested << "\n";

            // Show loan amount first and then give the option to approve or deny
            cout << "Approve this loan? (1 for Yes, 0 for No): ";
            string approveInput;
            cin >> approveInput;
            approve = approveInput == "1";

            acc.loanApproved = approve;
            if (approve)
            {
                acc.balance += acc.loanAmountRequested; // Add the loan amount to the balance
                cout << "Loan of Rs. " << acc.loanAmountRequested << " approved. Added to balance.\n";
            }
            else
            {
                cout << "Loan of Rs. " << acc.loanAmountRequested << " denied.\n";
            }
        }
        else
        {
            cout << "No loan request found for this account.\n";
        }
    }
    else
    {
        cout << "Account not found.\n";
    }
}

void Bank::atmInterface()
{
    string accNo, pin, cnic;

    while (true)
    {
        cout << "Enter your account number: ";
        getline(cin, accNo);
        if (accValid(accNo))
        {
            break;
        }
        cout << "\n\tInvalid Account No.\n";
    }

    while (true)
    {
        cout << "Enter your PIN: ";
        getline(cin, pin);
        if (pinValid(pin))
        {
            break;
        }
        cout << "\n\tInvalid PIN.\n";
    }

    if (authenticate(accNo, pin))
    {
        string choice;
        double amount;
        string amountStr;
        while (true)
        {
            cout << "\nATM Menu:\n";
            cout << "1. Withdraw Money\n";
            cout << "2. Deposit Money\n";
            cout << "3. Exit\n";
            while (true)
            {
                cout << "\nEnter your choice: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\tInvalid input.\n";
            }

            if (choice == "1")
            {

                while (true)
                {
                    cout << "Enter amount to withdraw: ";
                    getline(cin, amountStr);
                    if (amountValid(amountStr))
                    {
                        amount = stod(amountStr);
                        break;
                    }
                    cout << "\n\tInvalid amount.\n";
                }
                withdraw(accNo, amount);
            }
            else if (choice == "2")
            {

                while (true)
                {
                    cout << "Enter amount to deposit: ";
                    getline(cin, amountStr);
                    if (amountValid(amountStr))
                    {
                        amount = stod(amountStr);
                        break;
                    }
                    cout << "\n\tInvalid amount.\n";
                }
                deposit(accNo, amount);
            }
            else if (choice == "3")
            {
                cout << "Exiting ATM.\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    else
    {
        cout << "Authentication failed.\n\t Please check your account number and PIN.\n";
    }
}

void Bank::userPanel()
{
    string accNo, pin;

    while (true)
    {
        cout << "Enter your account number: ";
        getline(cin, accNo);
        if (accValid(accNo))
        {
            break;
        }
        cout << "\n\tInvalid Account No.\n";
    }

    while (true)
    {
        cout << "Enter your PIN: ";
        getline(cin, pin);
        if (pinValid(pin))
        {
            break;
        }
        cout << "\n\tInvalid PIN.\n";
    }

    if (authenticate(accNo, pin))
    {
        string choice;
        while (true)
        {
            cout << "\nUser Menu:\n";
            cout << "1. View Account Details\n";
            cout << "2. View Transaction History\n";
            cout << "3. Request Loan\n";
            cout << "5. Exit\n";
            while (true)
            {
                cout << "\nEnter your choice: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\tInvalid input.\n";
            }

            if (choice == "1")
            {
                showAccountDetails(accNo);
            }
            else if (choice == "2")
            {

                showTransactionHistory(accNo);
            }
            else if (choice == "3")
            {
                requestLoan(accNo);
            }
            else if (choice == "5")
            {
                cout << "Exiting User Panel.\n";
                break;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    else
    {
        cout << "Authentication failed. \n\nPlease check your account number and PIN.\n";
    }
}

void Bank::adminPanel()
{
    string choice;
    string accNo;
    while (true)
    {
        cout << "\nAdmin Menu:\n";
        cout << "1. Delete Account\n";
        cout << "2. Search Account\n";
        cout << "3. Approve/Deny Loan\n";
        cout << "4. Show All Accounts\n";
        cout << "5. Exit\n";
        while (true)
        {
            cout << "\nEnter your choice: ";
            getline(cin, choice);
            if (menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid input.\n";
        }

        if (choice == "1")
        {

            while (true)
            {
                cout << "Enter account number to delete: ";
                getline(cin, accNo);
                if (accValid(accNo))
                {
                    break;
                }
                cout << "\n\tInvalid Account No.\n";
            }
            deleteAccount(accNo);
        }
        else if (choice == "2")
        {

            while (true)
            {
                cout << "Enter account number to search: ";
                getline(cin, accNo);
                if (accValid(accNo))
                {
                    break;
                }
                cout << "\n\tInvalid Account No.\n";
            }
            searchAccount(accNo);
        }
        else if (choice == "3")
        {

            while (true)
            {
                cout << "Enter account number for loan approval/denial: ";
                getline(cin, accNo);
                if (accValid(accNo))
                {
                    break;
                }
                cout << "\n\tInvalid Account No.\n";
            }
            approveLoan(accNo, false); // false is just a placeholder
        }
        else if (choice == "4")
        {

            showAllAccounts();
        }
        else if (choice == "5")
        {

            cout << "Exiting Admin Panel.\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main()
{
    Bank bank;

    string mainChoice;
    while (true)
    {
        cout << "\nMain Menu:\n";
        cout << "1. Create Account\n";
        cout << "2. User Panel\n";
        cout << "3. Admin Panel\n";
        cout << "4. ATM Panel\n";
        cout << "5. Forget  account Pin\n";
        cout << "6. Exit\n";
        while (true)
        {
            cout << "\nEnter your choice: ";
            getline(cin, mainChoice);
            if (menuChoice(mainChoice))
            {
                break;
            }
            cout << "\n\t\tInvalid input.\n";
        }

        if (mainChoice == "1")
        {

            string name, pin, cnic, cellNo;
            string balance;
            double initialBalance;
            while (true)
            {
                cout << "Enter Your good name: ";
                getline(cin, name);
                if (nameValid(name))
                {
                    break;
                }
                cout << "\n\tInvalid name.\n";
            }
            while (true)
            {
                cout << "Enter Cell Number in PK: ";
                getline(cin, cellNo);
                if (phoneValidation(cellNo))
                {
                    break;
                }
                cout << "\n\tInvalid Phone no.\n";
            }
            while (true)
            {
                cout << "Enter Valid CNIC No: ";
                getline(cin, cnic);
                if (cnicValid(cnic))
                {
                    break;
                }
                cout << "\n\tInvalid CNIC No.\n";
            }

            while (true)
            {
                cout << "Please Set four digit PIN: ";
                getline(cin, pin);
                if (pinValid(pin))
                {
                    break;
                }
                cout << "\n\tInvalid PIN.\n";
            }

            while (true)
            {
                cout << "Enter initial balance: ";
                getline(cin, balance);
                if (amountValid(balance))
                {
                    initialBalance = stod(balance); // Convert to double
                    break;
                }
                cout << "\n\tInvalid PIN.\n";
            }
            bank.createAccount(name, pin, cnic, cellNo, initialBalance);
        }
        else if (mainChoice == "2")
        {
            bank.userPanel();
        }
        else if (mainChoice == "3")
        {
            bank.adminPanel();
        }
        else if (mainChoice == "4")
        {
            bank.atmInterface();
        }
        else if (mainChoice == "5")
        {
            string cnic;
            while (true)
            {
                cout << "Enter Valid CNIC No: ";
                getline(cin, cnic);
                if (cnicValid(cnic))
                {
                    break;
                }
                cout << "\n\tInvalid CNIC No.\n";
            }
            bank.forgotPin(cnic);

            string pin = bank.forgotPin(cnic); // Store the returned PIN or error message
            cout << "PIN: " << pin << endl;    // Display the PIN or error message
        }
        else if (mainChoice == "6")
        {
            cout << "Exiting System.\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

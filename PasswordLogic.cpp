#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

bool menuchoice(string choice)
{
    for (int i = 0; i < choice.size(); i++)
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
    for (int i = 0; i < choice.size(); i++)
    {
        if (choice[i] == ' ')
        {
            return false;
        }
    }
    return true;
}

void passLogic(string &password,  string promptText) {
    char pass[32] = {0};
    char ch;
    bool enter = false;
    int i = 0;
    bool show = false;

    cout << promptText;
    
    while (!enter) {
        ch = _getch();
        
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
            pass[i] = ch;
            if (show) {
                cout << ch;
            } else {
                cout << "*";
            }
            i++;
        }
        
        if (ch == '\b' && i >= 1) {
            cout << "\b \b";
            i--;
        }
        
        if (ch == '\r') {
            enter = true;
        }
        
        if (ch == '\t') {
            show = !show;
            cout << "\r" << promptText;
            for (int j = 0; j < i; j++) {
                cout << (show ? pass[j] : '*');
            }
        }
    }
    
    pass[i] = '\0';
    password = pass;
}

void checkPass(const string &defaultPass) {
    string pass;
    passLogic(pass, "Enter password to check: ");
    if (pass == defaultPass) {
        cout << "\nPassword is correct\n";
    } else {
        cout << "\nPassword is incorrect\n";
    }
}

void login(const string &defaultPass) {
    string pass;
    passLogic(pass, "Enter Password: ");
    if (pass == defaultPass) {
        cout << "\nAccess Granted!!!\n";
    } else {
        cout << "\nAccess Denied\n";
    }
}

void changePassword(string &defaultPass) {
    string oldPass, newPass;
    int attempts = 0;

    while (attempts < 3) {
        passLogic(oldPass, "Enter old password: ");

        if (oldPass == defaultPass) {
            do {
                cout<<endl;
                passLogic(newPass, "Enter new password: ");

                if (newPass == defaultPass) {
                    cout << "\nNew password cannot be the same as the current password. Please enter a unique password.";
                }
            } while (newPass == defaultPass);

            defaultPass = newPass;
            cout << "\nPassword changed successfully.\n";
            return;
        } else {
            ++attempts;
            if (attempts < 3) {
                cout << "\nIncorrect current password. You have " << (3 - attempts) << " attempt(s) left.";
            }
        }
    }

    cout << "\nAll attempts used. Password change failed.\n";
}


int main()
{
    string password = "Saad";
    string choice;

    do
    {
        cout << "\nMenu:\n";
        cout << "1. Login\n";
        cout << "2. Check Password\n";
        cout << "3. Change Password\n";
        cout << "4. Exit\n";
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (menuchoice(choice))
            {
                break;
            }
            cout << "\n\t\tInvalid input!\n";
        }

        if (choice == "1")
        {
            login(password);
        }
        else if (choice == "2")
        {
            checkPass(password);
        }
        else if (choice == "3")
        {
            changePassword(password);
        }
        else if (choice == "4")
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

    } while (choice != "4");

    return 0;
}
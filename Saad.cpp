#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <conio.h>

using namespace std;

class ErrorHandling
{
public:
    // Email validation function
    bool emailValidation(const string &email)
    {
        const string gmailSuffix = "@gmail.com";

        // Check if email is empty
        if (email.empty())
        {
            // cout << "Debug: Email is empty.\n";
            return false;
        }

        // Ensure there's no whitespace
        for (char ch : email)
        {
            if (isspace(ch))
            {
                // cout << "Debug: Email contains whitespace.\n";
                return false;
            }
        }

        // Check minimum length for "@gmail.com" emails
        if (email.size() <= gmailSuffix.size() + 4)
        {
            // cout << "Debug: Email is too short for a valid Gmail address.\n";
            return false;
        }

        // Separate the prefix and suffix
        string prefix = email.substr(0, email.size() - gmailSuffix.size());
        string suffix = email.substr(email.size() - gmailSuffix.size());

        // Check if the suffix matches "@gmail.com"
        if (suffix != gmailSuffix)
        {
            // cout << "Debug: Email does not end with '@gmail.com'.\n";
            return false;
        }

        // Validate prefix characters
        for (char ch : prefix)
        {
            if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
            {
                // cout << "Debug: Invalid character in email prefix.\n";
                return false;
            }
        }

        // If all checks pass, the email is valid
        // cout << "Debug: Email passed all checks.\n";
        return true;
    }

    // Name validation function
    bool nameValidation(string str)
    {
        if (str.empty())
        {
            return false;
        }
        for (int i = 0; i < str.size(); ++i)
        {
            if (str[i] == ' ')
            {
                return false;
                break;
            }
        }
        return true;
    }

    // Menu choice validation (to ensure input is numeric)
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

    // Password logic to handle input and validation
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

    // Password validation function
    bool passwordValidation(const string &str)
    {
        bool isLengthValid = str.length() >= 6; // Check if password length is at least 6 characters.

        if (!isLengthValid)
        {
            cout << "\n\n\tPassword must be at least 6 characters long.\n\n";
        }

        return isLengthValid;
    }
};

// Global ErrorHandling object for universal access
ErrorHandling errorHandler;

// Define structures and global variables
struct User
{
    string username;
    string password;
    string email;
    unordered_map<int, int> tickets; // event_id -> number of tickets
};

struct Event
{
    int eventId;
    string eventName;
    int totalSeats;
    int vipSeats;
    int regularSeats;
    priority_queue<int> vipSeatsQueue;                                // VIP seats with higher priority
    priority_queue<int, vector<int>, greater<int>> regularSeatsQueue; // Regular seats with lower priority
};

unordered_map<string, User> users;
unordered_map<int, Event> events;
unordered_map<string, bool> admins; // Track admin status


//              Alll Pre - Defined Functions

void setupDefaultAdmin();
void updateUserProfile(const string &username);
void signup();
void login();
bool adminLogin(string &loggedInAdmin);
void searchEvents();
bool adminLogin();
void buyTickets(const string &username);
void viewTickets(const string &username);
void cancelTicket(const string &username);
void addEvent();
void updateEvent();
void manageTickets();
void viewAllUsers();
void displayUserMenu(const string &username);
void addNewAdmin();
void removeAdmin();
void updateadminPanel(const string &adminUsername);
void updateAdminPassword(const string &adminUsername);
void displayAdminMenu();


//          All FUNCTION LOGIC IMPLEMENTS 


void updateUserProfile(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "\n\tUser not found.";
        return;
    }

    string choice;
    string newValue;
    while (true)
    {
        cout << "\n";
        cout << "Update Profile Menu:\n";
        cout << "1. Update Username\n";
        cout << "2. Update Password\n";
        cout << "3. Update Email\n";
        cout << "4. Exit\n";
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }
        // cin.ignore(); // To clear the newline character

        if (choice == "1")
        {
            while (true)
            {
                cout << "Enter new username: ";
                getline(cin, newValue);
                if (errorHandler.nameValidation(newValue))
                {
                    break;
                }
                cout << "\n\tInvalid username\n\n";
            }
            users[username].username = newValue;
            cout << "\n\tUsername updated successfully!\n";
        }

        else if (choice == "2")
        {
            while (true)
            {
                errorHandler.passLogic(newValue, "Enter new Password: "); // Handle password input
                if (errorHandler.passwordValidation(newValue))            // Validate password
                {
                    break; // Exit loop if password is valid
                }
            }
            users[username].password = newValue;
            cout << "\n\tPassword updated successfully!\n";
        }
        else if (choice == "3")
        {
            while (true)
            {
                cout << "Enter new email: ";
                getline(cin, newValue);
                if (errorHandler.emailValidation(newValue))
                {
                    break;
                }
                else
                {
                    cout << "\n\tInvalid Email. Please try again.\n\n";
                }
            }
            users[username].email = newValue;
            cout << "\n\tEmail updated successfully!\n";
        }
        else if (choice == "4")
        {
            cout << "\nExiting User Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}

// Function to set up the default admin user
void setupDefaultAdmin()
{
    string defaultAdminUsername = "admin1";
    string defaultAdminPassword = "adminpass";
    string defaultAdminEmail = "admin1@example.com";

    if (users.find(defaultAdminUsername) == users.end())
    {
        User adminUser = {defaultAdminUsername, defaultAdminPassword, defaultAdminEmail, {}};
        users[defaultAdminUsername] = adminUser;
        admins[defaultAdminUsername] = true; // Mark as admin
        cout << "Default admin account created successfully!\n";
    }
    else
    {
        cout << "Default admin already exists.\n";
    }
}

// Function to handle signup
void signup()
{
    string username, password, email;

    while (true)
    {
        cout << "\nEnter Username: ";
        getline(cin, username);
        if (errorHandler.nameValidation(username)) // Validate the username
        {
            break; // Exit loop if username is valid
        }
        else
        {
            cout << "\n\tInvalid Username. Please try again.\n";
        }
    }

    while (true)
    {
        errorHandler.passLogic(password, "Enter Password: "); // Handle password input
        if (errorHandler.passwordValidation(password))        // Validate password
        {
            break; // Exit loop if password is valid
        }
    }

    while (true)
    {
        cout << "\nEnter Valid Email: ";
        getline(cin, email);
        // cout << "Debug: Entered email is '" << email << "'\n"; // Debug email input

        if (errorHandler.emailValidation(email))
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid Email. Please try again.\n";
        }
    }

    // Create a new user and add them to the users map
    User newUser = {username, email, password, {}};
    users[username] = newUser;

    cout << "\n\tSignup Successfully!";
}


void displayUserMenu(const string &username)
{
    while (true)
    {
        cout << "\nUser Panel\n";
        cout << "1. Buy Tickets\n";
        cout << "2. View Tickets\n";
        cout << "3. Cancel Tickets\n";
        cout << "4. Update Profile\n";
        cout << "5. Exit\n";
        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }
        if (option == "1")
        {
            // buyTickets(username);
        }
        else if (option == "2")
        {
            // viewTickets(username);
        }
        else if (option == "3")
        {
            // cancelTicket(username);
        }
        else if (option == "4")
        {

            // updateUserProfile(username);
        }
        else if (option == "5")
        {
            cout << "Exit User Panel\n";
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Function to handle login
void login()
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Username: ";
        getline(cin, username);
        if (errorHandler.nameValidation(username)) // Validate the username
        {
            break; // Exit loop if username is valid
        }
        else
        {
            cout << "\n\n\tInvalid Username. Please try again.\n";
        }
    }

    errorHandler.passLogic(password, "Enter Password: "); // Handle password input

    if (users.find(username) != users.end() && users[username].password == password)
    {
        // cout << "\n\tLogin Successfully!\n";
        cout << "Welcome, " << username << ".\n";
        displayUserMenu(username);
    }
    else
    {
        cout << "\n\n\tInvalid username or password!\n";
    }
}

// Function for admin login
bool adminLogin(string &loggedInAdmin)
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Admin Username: ";
        getline(cin, username);
        if (errorHandler.nameValidation(username)) // Validate the username
        {
            break; // Exit loop if username is valid
        }
        else
        {
            cout << "\n\n\tInvalid Username. Please try again.\n";
        }
    }
    errorHandler.passLogic(password, "Enter Admin Password: "); // Handle password input

    // Check if admin exists and the password is correct
    if (admins.find(username) == admins.end())
    {
        cout << "\n\n\tAdmin not found.\n";
        return false;
    }
    if (users[username].password != password)
    {
        cout << "Incorrect password.\n";
        return false;
    }

    // If login is successful, store the username for further use
    loggedInAdmin = username;
    return true;
}

// Function to update user profile
void updateAdminPassword(const string &adminUsername)
{
    if (admins.find(adminUsername) == admins.end())
    {
        cout << "You must be an admin to access this feature.\n";
        return;
    }

    string newPassword;
    while (true)
    {
        errorHandler.passLogic(newPassword, "Enter new Password: ");
        if (errorHandler.passwordValidation(newPassword))
            break;
    }
    users[adminUsername].password = newPassword;
    cout << "\n\n\tPassword updated successfully!\n";
}

void addNewAdmin()
{
    string newAdminUsername, password, email;
    while (true)
    {
        cout << "\nEnter new admin's Username: ";
        getline(cin, newAdminUsername);
        if (errorHandler.nameValidation(newAdminUsername) && admins.find(newAdminUsername) == admins.end())
            break;
        cout << "\n\tInvalid Username. Please try again.\n";
    }

    while (true)
    {
        errorHandler.passLogic(password, "Enter new admin's Password: ");
        if (errorHandler.passwordValidation(password))
            break;
    }

    while (true)
    {
        cout << "\nEnter new admin's Valid Email: ";
        getline(cin, email);
        if (errorHandler.emailValidation(email))
            break;
        cout << "\n\tInvalid Email. Please try again.\n";
    }

    User newAdmin = {newAdminUsername, password, email, {}};
    users[newAdminUsername] = newAdmin;
    admins[newAdminUsername] = true;
    cout << "\n\tNew admin added successfully!\n";
}

void removeAdmin()
{
    string targetAdminUsername;
    cout << "\nEnter the username of the admin to remove: ";
    getline(cin, targetAdminUsername);

    if (admins.find(targetAdminUsername) != admins.end())
    {
        admins.erase(targetAdminUsername);
        users.erase(targetAdminUsername);
        cout << "\n\tAdmin removed successfully!\n";
    }
    else
    {
        cout << "\n\tAdmin not found or cannot be removed.\n";
    }
}

void updateadminPanel(const string &adminUsername)
{
    if (admins.find(adminUsername) == admins.end())
    {
        cout << "\n\tAccess Denied. Only admins can access this panel.\n";
        return;
    }

    while (true)
    {
        cout << "\nAdmin Panel:\n";
        cout << "1. Update own Password\n";
        cout << "2. Add New Admin\n";
        cout << "3. Remove an Admin\n";
        cout << "4. Exit Admin Panel\n";
        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }
        // cin.ignore();

        if (option == "1")
        {
            updateAdminPassword(adminUsername);
        }
        else if (option == "2")
        {
            addNewAdmin();
        }
        else if (option == "3")
        {
            removeAdmin();
        }
        else if (option == "4")
        {
            cout << "\nExiting Update Admin Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}

// Admin functions
void displayAdminMenu()
{
    string loggedInAdmin;
    if (!adminLogin(loggedInAdmin))
    {
        return; // Exit if login fails
    }
    while (true)
    {   
        cout<<"\n";
        cout << "1. Add Event\n";
        cout << "2. Update Event\n";
        cout << "3. View All Users\n";
        cout << "4. Manage Tickets\n";
        cout << "5. Manage Admin\n";
        cout << "6. Exit\n";
        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (option == "1")
        {
            // addEvent();
        }
        else if (option == "2")
        {
            // updateEvent();
            // Simplified version, you can expand based on requirements
        }
        else if (option == "3")
        {
            // viewAllUsers();
        }
        else if (option == "4")
        {
            // manageTickets();
        }
        else if (option == "5")
        {

            updateadminPanel(loggedInAdmin);
        }
        else if (option == "6")
        {
            cout << "Exit User Panel\n";
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
    ErrorHandling errorHandler;

    // Set up the default admin user
    setupDefaultAdmin();

    // Main menu
    while (true)
    {
        cout << "\n\n\t**** Event Ticket Reservation System ****\n";
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Admin Panel\n";
        cout << "4. Search Events\n";
        cout << "5. Exit\n";
        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1")
        {
            signup();
        }
        else if (choice == "2")
        {
            login();
        }
        else if (choice == "4")
        {
        }
        else if (choice == "3")
        {
            displayAdminMenu();
        }
        else if (choice == "5")

        {
            cout << "\n\tExiting... Have a great day!\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid option! Please try again.\n";
        }
    }

    return 0;
}

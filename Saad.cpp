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

// Function to set up the default admin user
void setupDefaultAdmin()
{
    string defaultAdminUsername = "admin1";
    string defaultAdminPassword = "adminpass";
    string defaultAdminEmail = "admin1@example.com";

    if (users.find(defaultAdminUsername) == users.end())
    {
        User adminUser = {defaultAdminUsername, defaultAdminEmail, defaultAdminPassword, {}};
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

void buyTickets(const string &username)
{
    int eventID, numberOfTickets;
    cout << "Enter event ID: ";
    cin >> eventID;

    // if (!isPositiveInteger(eventID) || events.find(eventID) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    cout << "Enter number of tickets: ";
    cin >> numberOfTickets;

    // if (!isPositiveInteger(numberOfTickets))
    {
        cout << "Number of tickets must be a positive integer.\n";
        return;
    }

    Event &event = events[eventID];
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    // First allocate VIP seats if available
    // while (vipTicketsBooked < numberOfTickets && !event.vipSeats.empty())
    {
        // event.vipSeats.pop();
        vipTicketsBooked++;
    }

    // Allocate regular seats if VIP seats are less than required
    // while (regularTicketsBooked < numberOfTickets - vipTicketsBooked && !event.regularSeats.empty())
    {
        event.regularSeatsQueue.pop();
        regularTicketsBooked++;
    }

    if (vipTicketsBooked + regularTicketsBooked == numberOfTickets)
    {
        // Successfully booked tickets
        users[username].tickets[eventID] += numberOfTickets;
        cout << "Tickets booked successfully! VIP: " << vipTicketsBooked << ", Regular: " << regularTicketsBooked << endl;
    }
    else
    {
        cout << "Not enough seats available.\n";
    }
}

void viewTickets(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }

    const User &user = users[username];
    cout << "Your tickets:\n";
    for (const auto &ticket : user.tickets)
    {
        cout << "Event ID: " << ticket.first << ", Number of Tickets: " << ticket.second << '\n';
    }
}

void cancelTicket(const string &username)
{
    int eventID, numberOfTickets;
    cout << "Enter event ID: ";
    cin >> eventID;

    cout << "Enter number of tickets to cancel: ";
    cin >> numberOfTickets;

    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    if (users[username].tickets[eventID] < numberOfTickets)
    {
        cout << "You don't have that many tickets.\n";
        return;
    }

    Event &event = events[eventID];

    // Release VIP seats
    int vipSeatsToCancel = min(numberOfTickets, users[username].tickets[eventID]);
    for (int i = 0; i < vipSeatsToCancel; ++i)
    {
        event.vipSeatsQueue.push(1); // Simulate releasing a VIP seat
    }

    // Release Regular seats
    int regularSeatsToCancel = numberOfTickets - vipSeatsToCancel;
    for (int i = 0; i < regularSeatsToCancel; ++i)
    {
        event.regularSeatsQueue.push(1); // Simulate releasing a regular seat
    }

    users[username].tickets[eventID] -= numberOfTickets;
    if (users[username].tickets[eventID] == 0)
    {
        users[username].tickets.erase(eventID);
    }

    cout << "Tickets cancelled successfully!\n";
}

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

void addEvent()
{
    int eventID;
    string eventName;
    int totalSeats, vipSeats, regularSeats;

    cout << "Enter event ID: ";
    cin >> eventID;

    if (events.find(eventID) != events.end())
    {
        cout << "Event ID already exists.\n";
        return;
    }

    cout << "Enter event name: ";
    getline(cin, eventName);

    cout << "Enter total seats: ";
    cin >> totalSeats;

    cout << "Enter number of VIP seats: ";
    cin >> vipSeats;

    regularSeats = totalSeats - vipSeats;
    Event newEvent = {eventID, eventName, totalSeats, vipSeats, regularSeats};
    for (int i = 0; i < vipSeats; ++i)
    {
        // newEvent.vipSeats.push(1); // Simulate VIP seats
    }
    for (int i = 0; i < regularSeats; ++i)
    {
        // newEvent.vipSeats.push(1); // Simulate regular seats
    }

    events[eventID] = newEvent;
    cout << "Event added successfully!\n";
}

void updateEvent()
{
    // Simplified version, you can expand based on requirements
}

void manageTickets()
{
    // Simplified ticket management for now
}

void viewAllUsers()
{
    cout << "All registered users:\n";
    for (const auto &user : users)
    {
        cout << "Username: " << user.first << ", Email: " << user.second.email << '\n';
    }
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
            buyTickets(username);
        }
        else if (option == "2")
        {
            viewTickets(username);
        }
        else if (option == "3")
        {
            cancelTicket(username);
        }
        else if (option == "4")
        {

            updateUserProfile(username);
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
            cout << "\nExiting Admin Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}

void searchEvents()
{
    string query;
    while (true)
    {
        cout << "\nEnter event name to search: ";
        getline(cin, query);
        if (errorHandler.nameValidation(query)) // Validate the username
        {
            break; // Exit loop if username is valid
        }
        else
        {
            cout << "\n\n\tInvalid Event Name. Please try again.\n";
        }
    }

    cout << "Search results:\n";
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        if (e.eventName.find(query) != string::npos)
        {
            cout << "Event ID: " << eventID << ", Event Name: " << e.eventName << ", VIP Seats: " << e.vipSeats << ", Regular Seats: " << e.regularSeats << '\n';
        }
    }
}
// Admin functions
void displayAdminMenu()
{

    while (true)
    {
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
            addEvent();
        }
        else if (option == "2")
        {
            updateEvent();
            // Simplified version, you can expand based on requirements
        }
        else if (option == "3")
        {
            viewAllUsers();
        }
        else if (option == "4")
        {
            manageTickets();
        }
        else if (option == "5")
        {

            string adminUsername;
            while (true)
            {
                cout << "\nEnter Admin Username to update user profile: ";
                getline(cin, adminUsername);
                if (errorHandler.nameValidation(adminUsername))
                {

                    updateadminPanel(adminUsername);
                    break;
                }
                else
                {
                    cout << "\nInvalid Username. Please try again.\n";
                }
            }
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
            // string adminUsername;
            // while (true)
            // {
            //     cout << "\nEnter Admin Username to update user profile: ";
            //     getline(cin, adminUsername);
            //     if (errorHandler.nameValidation(adminUsername))
            //     {

            //         // updateadminPanel(adminUsername);
            //         break;
            //     }
            //     else
            //     {
            //         cout << "\nInvalid Username. Please try again.\n";
            //     }
            // }
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

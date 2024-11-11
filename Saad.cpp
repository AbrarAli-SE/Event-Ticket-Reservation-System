#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <conio.h>
#include <algorithm>
#include <vector>

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

    bool eventIDValid(const string &eventID)
    {
        // Check if the eventID is empty
        if (eventID.empty())
        {
            return false;
        }

        // Remove spaces from the input
        string cleanedEventID;
        for (size_t i = 0; i < eventID.size(); ++i)
        {
            if (eventID[i] != ' ') // Skip spaces
            {
                cleanedEventID += eventID[i]; // Add non-space characters to cleanedEventID
            }
        }

        // Ensure the cleanedEventID is numeric
        for (size_t i = 0; i < cleanedEventID.size(); ++i)
        {
            if (!isdigit(cleanedEventID[i])) // Check if all characters are digits
            {
                return false;
            }
        }

        return true;
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

    // Default constructor to properly initialize the priority queues 
    Event() : eventId(0), eventName(""), totalSeats(0), vipSeats(0), regularSeats(0) {
        // Priority queues are already default-constructed, so no need to initialize them here
    }

    // Parameterized constructor (already present in your code)
    Event(int id, const string& name, int total, int vip, int regular) 
        : eventId(id), eventName(name), totalSeats(total), vipSeats(vip), regularSeats(regular) {}
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

// Helper function to convert string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
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

void buyTickets(const string &username)
{
    string eventIDStr, numberOfTicketsStr;
    int eventID, numberOfTickets;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.eventIDValid(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    cout << "Enter number of tickets: ";
    getline(cin, numberOfTicketsStr);

    if (!errorHandler.eventIDValid(numberOfTicketsStr))
    {
        cout << "Number of tickets must be a positive integer.\n";
        return;
    }

    numberOfTickets = stoi(numberOfTicketsStr);

    Event &event = events[eventID];
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    // First allocate VIP seats if available
    while (vipTicketsBooked < numberOfTickets && !event.vipSeatsQueue.empty())
    {
        event.vipSeatsQueue.pop();
        vipTicketsBooked++;
    }

    // Allocate regular seats if VIP seats are less than required
    while (regularTicketsBooked < numberOfTickets - vipTicketsBooked && !event.regularSeatsQueue.empty())
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

        // Revert the seats popped from the queues
        for (int i = 0; i < vipTicketsBooked; ++i)
        {
            event.vipSeatsQueue.push(0); // Simulating re-adding the seat
        }
        for (int i = 0; i < regularTicketsBooked; ++i)
        {
            event.regularSeatsQueue.push(0); // Simulating re-adding the seat
        }
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

        // Convert username to lowercase for case-insensitive check
        string lowerUsername = toLowerCase(username);

        // Check for duplicate username
        bool usernameExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.first) == lowerUsername)
            {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            cout << "\n\tUsername already exists. Please try again.\n";
            continue;
        }

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

        // Convert email to lowercase for case-insensitive check
        string lowerEmail = toLowerCase(email);

        // Check for duplicate email
        bool emailExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.second.email) == lowerEmail)
            {
                emailExists = true;
                break;
            }
        }

        if (emailExists)
        {
            cout << "\n\tEmail already exists. Please try again.\n";
            continue;
        }

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
    User newUser = {username, password, email, {}};
    users[username] = newUser;

    cout << "\n\tSignup Successfully!";
}

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

    // Convert the entered username to lowercase for case-insensitive comparison
    string lowerUsername = toLowerCase(username);

    // Check if the user exists and the password matches
    bool userFound = false;
    for (const auto &user : users)
    {
        if (toLowerCase(user.first) == lowerUsername && user.second.password == password)
        {
            userFound = true;
            cout << "\n\n\tWelcome, " << user.first << ".\n";
            displayUserMenu(user.first);
            break;
        }
    }

    if (!userFound)
    {
        cout << "\n\n\tInvalid username or password!\n";
    }
}

void viewAllUsers()
{
    if (users.empty())
    {
        cout << "\nNo users are registered yet.\n";
    }
    else
    {
        cout << "\nRegistered Users:\n";
        for (const auto &user : users)
        {
            cout << "Username: " << user.second.username << ", Email: " << user.second.email << "\n";
        }
    }
}

void cancelTicket(const string &username)
{
    string eventIDStr, numberOfTicketsStr;
    int eventID, numberOfTickets;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.eventIDValid(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    cout << "Enter number of tickets to cancel: ";
    getline(cin, numberOfTicketsStr);

    if (!errorHandler.eventIDValid(numberOfTicketsStr))
    {
        cout << "Number of tickets must be a positive integer.\n";
        return;
    }

    numberOfTickets = stoi(numberOfTicketsStr);

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

    int vipSeatsToCancel = 0;
    int regularSeatsToCancel = 0;

    // Calculate VIP and Regular seats to cancel
    int totalTickets = users[username].tickets[eventID];
    if (totalTickets <= event.vipSeats)
    {
        vipSeatsToCancel = min(numberOfTickets, totalTickets);
    }
    else
    {
        vipSeatsToCancel = min(numberOfTickets, event.vipSeats);
        regularSeatsToCancel = numberOfTickets - vipSeatsToCancel;
    }

    // Release VIP seats
    for (int i = 0; i < vipSeatsToCancel; ++i)
    {
        event.vipSeatsQueue.push(event.vipSeats - i); // Simulate releasing a VIP seat
    }

    // Release Regular seats
    for (int i = 0; i < regularSeatsToCancel; ++i)
    {
        event.regularSeatsQueue.push(event.regularSeats - i); // Simulate releasing a regular seat
    }

    // Update user's tickets
    users[username].tickets[eventID] -= numberOfTickets;
    if (users[username].tickets[eventID] == 0)
    {
        users[username].tickets.erase(eventID);
    }

    cout << "Tickets cancelled successfully!\n";
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
        int eventID = ticket.first;
        int totalTickets = ticket.second;

        int vipTickets = 0;
        int regularTickets = 0;

        const Event &event = events[eventID];

        // Calculate VIP and Regular tickets booked
        if (totalTickets <= event.vipSeats)
        {
            vipTickets = totalTickets;
        }
        else
        {
            vipTickets = event.vipSeats;
            regularTickets = totalTickets - vipTickets;
        }

        cout << "Event ID: " << eventID 
             << ", Event Name: " << event.eventName 
             << ", VIP Tickets: " << vipTickets 
             << ", Regular Tickets: " << regularTickets << '\n';
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
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
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

void addEvent()
{
    string eventIDStr, totalSeatsStr, vipSeatsStr;
    int eventID;
    string eventName;
    int totalSeats, vipSeats, regularSeats;

    while (true)
    {
        cout << "Enter event ID: ";
        getline(cin, eventIDStr);
        if (errorHandler.eventIDValid(eventIDStr))
        {
            eventID = stoi(eventIDStr);
            break;
        }
        cout << "Invalid event ID. Please enter a numeric event ID with no spaces.\n";
    }

    if (events.find(eventID) != events.end())
    {
        cout << "Event ID already exists. Please choose a different event ID.\n";
        return;
    }

    cout << "Enter event name: ";
    getline(cin, eventName);

        // Validate Total Seats
    while (true)
    {
        cout << "Enter total seats: ";
        getline(cin, totalSeatsStr);
        
        if (errorHandler.eventIDValid(totalSeatsStr))  // Check for valid total seats input
        {
            totalSeats = stoi(totalSeatsStr);  // Convert the valid total seats string to an integer
            
            if (totalSeats > 0)  // Ensure total seats is a positive number
            {
                break;  // Break the loop if valid
            }
            else
            {
                cout << "Total seats must be a positive number. Please enter again.\n";
                continue;
            }
        }
        else
        {
            cout << "Invalid number of total seats. Please enter a numeric value with no spaces.\n";
            continue;
        }
    }

    // Validate VIP Seats
    while (true)
    {
        cout << "Enter number of VIP seats: ";
        getline(cin, vipSeatsStr);
        
        if (errorHandler.eventIDValid(vipSeatsStr))  // Check for valid VIP seats input
        {
            vipSeats = stoi(vipSeatsStr);  // Convert the valid VIP seats string to an integer
            
            if (vipSeats >= 0 && vipSeats <= totalSeats)  // Ensure VIP seats is between 0 and total seats
            {
                regularSeats = totalSeats - vipSeats;
                break;  // Break the loop if valid
            }
            else
            {
                cout << "VIP seats must be between 0 and the total number of seats. Please enter again.\n";
                continue;
            }
        }
        else
        {
            cout << "Invalid number of VIP seats. Please enter a numeric value with no spaces.\n";
            continue;
        }
    }

    regularSeats = totalSeats - vipSeats;

    Event newEvent = {eventID, eventName, totalSeats, vipSeats, regularSeats};

    // Fill VIP seats queue
    for (int i = 1; i <= vipSeats; ++i)
    {
        newEvent.vipSeatsQueue.push(i); // Simulate VIP seats
    }

    // Fill regular seats queue
    for (int i = 1; i <= regularSeats; ++i)
    {
        newEvent.regularSeatsQueue.push(i); // Simulate regular seats
    }

    events[eventID] = newEvent;

    cout << "Event added successfully!\n";
}

void updateEvent()
{
    int eventID;
    cout << "Enter event ID to update: ";
    cin >> eventID;
    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }
    string newEventName;
    cout << "Enter new event name: ";
    getline(cin, newEventName);
    events[eventID].eventName = newEventName;
    cout << "Event updated successfully!\n";
}

void manageTickets()
{
    string eventIDStr;
    int eventID;

    while (true)
    {
        cout << "Enter event ID to manage tickets: ";
        getline(cin, eventIDStr);

        if (errorHandler.eventIDValid(eventIDStr))
        {
            eventID = stoi(eventIDStr); // Convert the valid event ID string to an integer
            break;                      // Break the loop if the event ID is valid
        }
        else
        {
            cout << "Invalid event ID. Please enter a numeric event ID with no spaces.\n";
            continue; // Continue the loop to ask for the event ID again
        }
    }

    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    const Event &event = events[eventID];
    cout << "Managing tickets for Event ID: " << event.eventId << ", Event Name: " << event.eventName << '\n';
    cout << "Total seats: " << event.totalSeats << ", VIP seats: " << event.vipSeats << ", Regular seats: " << event.regularSeats << '\n';
    cout << "Available VIP seats: " << event.vipSeatsQueue.size() << ", Available Regular seats: " << event.regularSeatsQueue.size() << '\n';

    int choice;
    cout << "1. Add seats" << endl;
    cout << "1. Add Vip Seats" << endl;
    cout << "2. Add Regular Seats" << endl;
    cout << "2. Remove seats" << endl;
    cout << " Choose an option: ";
    cin >> choice;

    int seats;
    if (choice == 1)
    {
        cout << "1. Add Vip Seats" << endl;
        cout << "2. Add Regular Seats" << endl;
        cout << " Choose an option: ";
        if(choice == 1)
        {
        cout << "Enter number of VIP seats to add: ";
        cin >> seats;
        for (int i = 0; i < seats; ++i)
        {
            events[eventID].vipSeatsQueue.push(event.vipSeats + 1 + i);
        }
        events[eventID].vipSeats += seats;
        events[eventID].totalSeats += seats;
        }
        else if(choice == 2)
        {
        cout << "Enter number of Regular seats to add: ";
        cin >> seats;
        for (int i = 0; i < seats; ++i)
        {
            events[eventID].regularSeatsQueue.push(event.regularSeats + 1 + i);
        }
        events[eventID].regularSeats += seats;
        events[eventID].totalSeats += seats;
        }
        else{
            cout<<"Invalid options"<<endl;
        }

        cout << "Seats added successfully.\n";
    }
    else if (choice == 2)
    {
        cout << "1. Remove Vip seats" << endl;
        cout << "2. Remove Regular Seats" << endl;
        cout << " Choose an option: ";
        if (choice == 1)
        {
            cout << "Enter number of VIP seats to remove: ";
            cin >> seats;
            if (seats <= event.vipSeatsQueue.size())
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].vipSeatsQueue.pop();
                }
                events[eventID].vipSeats -= seats;
                events[eventID].totalSeats -= seats;
                cout << "VIP seats removed successfully.\n";
            }
            else
            {
                cout << "Not enough VIP seats available to remove.\n";
            }
        }
        else if (choice == 2)
        {
            cout << "Enter number of Regular seats to remove: ";
            cin >> seats;
            if (seats <= event.regularSeatsQueue.size())
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].regularSeatsQueue.pop();
                }
                events[eventID].regularSeats -= seats;
                events[eventID].totalSeats -= seats;
                cout << "Regular seats removed successfully.\n";
            }
            else
            {
                cout << "Not enough Regular seats available to remove.\n";
            }
        }
        else
        {
            cout << "Invalid option selected.\n";
        }
    }
    else
    {
        cout << "Invalid option selected.\n";
    }
}


void searchEvents()
{
    if (events.empty())
    {
        cout << "No events available to search.\n";
        return;
    }

    string query;
    cout << "Enter event name to search: ";
    getline(cin, query);

    bool found = false;
    cout << "Search results:\n";
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        if (e.eventName.find(query) != string::npos)
        {
            cout << "Event ID: " << eventID << ", Event Name: " << e.eventName << '\n';
            cout << "VIP Seats Available: " << e.vipSeatsQueue.size() << ", Regular Seats Available: " << e.regularSeatsQueue.size() << '\n';
            found = true;
        }
    }

    if (!found)
    {
        cout << "No events found matching the query.\n";
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
        cout << "\n";
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
            searchEvents();
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

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <conio.h>
#include <iomanip>

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

    bool idValidation(string idString)
    {
        for (int i = 0; i < idString.size(); ++i)
        {
            if (!isdigit(idString[i]))
            {
                return false;
            }
        }
        if (idString.empty())
        {
            return false;
        }
        for (int i = 0; i < idString.size(); ++i)
        {
            if (idString[i] == ' ')
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
    unordered_map<int, int> tickets;                // event_id -> number of tickets
    unordered_map<int, int> canceledVipTickets;     // event_id -> number of canceled VIP tickets
    unordered_map<int, int> canceledRegularTickets; // event_id -> number of canceled regular tickets
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
    Event() : eventId(0), eventName(""), totalSeats(0), vipSeats(0), regularSeats(0)
    {
        // Priority queues are already default-constructed, so no need to initialize them here
    }

    // Parameterized constructor (already present in your code)
    Event(int id, const string &name, int total, int vip, int regular)
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

void displayAllEvents();

void displayAllUserData();
//          All FUNCTION LOGIC IMPLEMENTS

// Helper function to convert string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str; // Create a copy to modify
    for (int i = 0; i < lowerStr.length(); i++)
    {
        lowerStr[i] = tolower(lowerStr[i]);
    }
    return lowerStr;
}

void displayAllUserData()
{
    if (users.empty())
    {
        cout << "No users available.\n";
        return;
    }

    cout << "User Data:\n";
    for (const auto &userPair : users)
    {
        const string &username = userPair.first;
        const User &user = userPair.second;

        // Skip admins
        if (admins.find(username) != admins.end() && admins.at(username))
        {
            continue;
        }

        cout << "Username: " << user.username << "\nEmail: " << user.email << "\nTickets:\n";

        if (user.tickets.empty())
        {
            cout << "  No tickets bought.\n";
        }
        else
        {
            for (const auto &ticketPair : user.tickets)
            {
                int eventID = ticketPair.first;
                int totalTickets = ticketPair.second;

                int vipTickets = 0;
                int regularTickets = 0;

                const Event &event = events.at(eventID); // Use 'at' for better error handling

                // Calculate VIP and Regular tickets booked
                if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
                {
                    vipTickets = user.canceledVipTickets.at(eventID);
                }

                if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
                {
                    regularTickets = user.canceledRegularTickets.at(eventID);
                }

                cout << "  Event ID: " << eventID
                     << ", Event Name: " << event.eventName
                     << ", VIP Tickets: " << vipTickets
                     << ", Regular Tickets: " << regularTickets << '\n';
            }
        }

        cout << "-------------------------------\n";
    }
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
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update Username" << setw(20) << "|\n";
        cout << "\t\t| [2]   |     Update Password" << setw(20) << "|\n";
        cout << "\t\t| [3]   |     Update Email" << setw(23) << "|\n";
        cout << "\t\t| [4]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

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
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    Event &event = events[eventID];
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Buy VIP tickets" << setw(21) << "|\n";
        cout << "\t\t| [2]   |     Buy Regular tickets" << setw(16) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        // Read user choice
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

        if (choice == "1" || choice == "2")
        {
            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets: ";
            getline(cin, numberOfTicketsStr);

            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }

            numberOfTickets = stoi(numberOfTicketsStr);

            if (choice == "1")
            {
                // VIP Tickets
                if (event.vipSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough VIP seats available. Total VIP seats left: " << event.vipSeatsQueue.size() << ".\n";
                    return;
                }

                // Allocate VIP seats
                while (vipTicketsBooked < numberOfTickets && !event.vipSeatsQueue.empty())
                {
                    event.vipSeatsQueue.pop();
                    vipTicketsBooked++;
                }

                // Store the VIP tickets booked by the user
                users[username].canceledVipTickets[eventID] += vipTicketsBooked;
            }
            else if (choice == "2")
            {
                // Regular Tickets
                if (event.regularSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough Regular seats available. Total Regular seats left: " << event.regularSeatsQueue.size() << ".\n";
                    return;
                }

                // Allocate Regular seats
                while (regularTicketsBooked < numberOfTickets && !event.regularSeatsQueue.empty())
                {
                    event.regularSeatsQueue.pop();
                    regularTicketsBooked++;
                }

                // Store the Regular tickets booked by the user
                users[username].canceledRegularTickets[eventID] += regularTicketsBooked;
            }

            if ((choice == "1" && vipTicketsBooked == numberOfTickets) || (choice == "2" && regularTicketsBooked == numberOfTickets))
            {
                // Successfully booked tickets
                users[username].tickets[eventID] += numberOfTickets;
                cout << "Tickets booked successfully! ";

                if (choice == "1")
                    cout << "VIP: " << vipTicketsBooked << "\n";
                else if (choice == "2")
                    cout << "Regular: " << regularTicketsBooked << "\n";

                return;
            }
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
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
    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }

    if (users[username].tickets.find(eventID) == users[username].tickets.end())
    {
        cout << "You have not bought any tickets for this event.\n";
        return;
    }

    Event &event = events[eventID];

    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Cancel VIP tickets" << setw(19) << "|\n";
        cout << "\t\t| [2]   |     Cancel Regular tickets" << setw(16) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        // Read user choice
        string choice;
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1" || choice == "2")
        {
            int vipTicketsBooked = users[username].canceledVipTickets[eventID];
            int regularTicketsBooked = users[username].canceledRegularTickets[eventID];

            if ((choice == "1" && vipTicketsBooked == 0) || (choice == "2" && regularTicketsBooked == 0))
            {
                cout << "You do not have any " << (choice == "1" ? "VIP" : "Regular") << " tickets for this event.\n";
                continue;
            }

            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets to cancel: ";
            getline(cin, numberOfTicketsStr);

            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }

            numberOfTickets = stoi(numberOfTicketsStr);

            if (choice == "1" && vipTicketsBooked < numberOfTickets)
            {
                cout << "You don't have that many VIP tickets.\n";
                return;
            }
            if (choice == "2" && regularTicketsBooked < numberOfTickets)
            {
                cout << "You don't have that many Regular tickets.\n";
                return;
            }

            if (choice == "1")
            {
                // Cancel VIP tickets
                int seatsToCancel = min(numberOfTickets, vipTicketsBooked);

                // Release VIP seats
                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.vipSeatsQueue.push(0); // Simulate releasing a VIP seat
                }

                // Update user's tickets
                users[username].canceledVipTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "VIP tickets cancelled successfully!\n";
                return;
            }
            else if (choice == "2")
            {
                // Cancel Regular tickets
                int seatsToCancel = min(numberOfTickets, regularTicketsBooked);

                // Release Regular seats
                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.regularSeatsQueue.push(0); // Simulate releasing a regular seat
                }

                // Update user's tickets
                users[username].canceledRegularTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "Regular tickets cancelled successfully!\n";
                return;
            }
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
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

    if (user.tickets.empty())
    {
        cout << "You have not purchased any tickets.\n";
        return;
    }

    cout << "Your tickets:\n";
    for (const auto &ticket : user.tickets)
    {
        int eventID = ticket.first;
        int totalTickets = ticket.second;

        // Check if the event still exists
        if (events.find(eventID) == events.end())
        {
            cout << "Event ID " << eventID << " has been deleted and no longer exists.\n";
            continue; // Skip to the next ticket
        }

        int vipTickets = 0;
        int regularTickets = 0;

        if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
        {
            vipTickets = user.canceledVipTickets.at(eventID);
        }

        if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
        {
            regularTickets = user.canceledRegularTickets.at(eventID);
        }

        cout << "Event ID: " << eventID
             << ", Event Name: " << events[eventID].eventName
             << ", VIP Tickets: " << vipTickets
             << ", Regular Tickets: " << regularTickets << '\n';
    }
}

void displayUserMenu(const string &username)
{
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Buy Tickets" << setw(24) << "|\n";
        cout << "\t\t| [2]   |     View Tickets" << setw(23) << "|\n";
        cout << "\t\t| [3]   |     Cancel Tickets" << setw(21) << "|\n";
        cout << "\t\t| [4]   |     View All Events" << setw(20) << "|\n";
        cout << "\t\t| [5]   |     Update Profile" << setw(21) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

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
            displayAllEvents();
        }
        else if (option == "5")
        {
            updateUserProfile(username);
        }
        else if (option == "0")
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
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update own Password" << setw(16) << "|\n";
        cout << "\t\t| [2]   |     Add New Admin" << setw(22) << "|\n";
        cout << "\t\t| [3]   |     Remove an Admin" << setw(20) << "|\n";
        cout << "\t\t| [4]   |     Exit Admin Panel" << setw(19) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

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
        if (errorHandler.idValidation(eventIDStr))
        {
            eventID = stoi(eventIDStr);
            break;
        }
        cout << "\n\tInvalid event ID. Please enter a numeric event ID with no spaces.\n";
    }

    if (events.find(eventID) != events.end())
    {
        cout << "\n\tEvent ID already exists. Please choose a different event ID.\n";
        return;
    }

    while (true)
    {
        cout << "\nEnter Event Name: ";
        getline(cin, eventName);
        if (errorHandler.nameValidation(eventName)) // Validate the username
        {
            break; // Exit loop if username is valid
        }
        else
        {
            cout << "\n\n\tInvalid Event Name. Please try again.\n";
        }
    }

    // Validate Total Seats
    while (true)
    {
        cout << "Enter total seats: ";
        getline(cin, totalSeatsStr);

        if (errorHandler.idValidation(totalSeatsStr)) // Check for valid total seats input
        {
            totalSeats = stoi(totalSeatsStr); // Convert the valid total seats string to an integer

            if (totalSeats > 0) // Ensure total seats is a positive number
            {
                break; // Break the loop if valid
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

        if (errorHandler.idValidation(vipSeatsStr)) // Check for valid VIP seats input
        {
            vipSeats = stoi(vipSeatsStr); // Convert the valid VIP seats string to an integer

            if (vipSeats >= 0 && vipSeats <= totalSeats) // Ensure VIP seats is between 0 and total seats
            {
                regularSeats = totalSeats - vipSeats;
                break; // Break the loop if valid
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
    string eventIDStr;
    int eventID;
    cout << "Enter event ID to update or delete: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update the event" << setw(19) << "|\n";
        cout << "\t\t| [2]   |     Delete the event" << setw(19) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        // Read user choice
        string choice;
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1")
        {
            string newEventName;
            while (true)
            {
                cout << "\nEnter new Event Name: ";
                getline(cin, newEventName);
                if (errorHandler.nameValidation(newEventName)) // Validate the event name
                {
                    break; // Exit loop if event name is valid
                }
                else
                {
                    cout << "\n\n\tInvalid Event Name. Please try again.\n";
                }
            }

            events[eventID].eventName = newEventName;
            cout << "Event updated successfully!\n";
        }
        else if (choice == "2")
        {
            // Remove tickets associated with the event from each user's record
            for (auto &userPair : users)
            {
                User &user = userPair.second;
                user.tickets.erase(eventID);
                user.canceledVipTickets.erase(eventID);
                user.canceledRegularTickets.erase(eventID);
            }

            events.erase(eventID);
            cout << "Event deleted successfully!\n";
            break; // Exit loop after deleting the event
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void manageTickets()
{
    string eventIDStr;
    int eventID;

    while (true)
    {
        cout << "Enter event ID to manage tickets: ";
        getline(cin, eventIDStr);

        if (errorHandler.idValidation(eventIDStr))
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

    Event &event = events[eventID];
    cout << "Managing tickets for Event ID: " << event.eventId << ", Event Name: " << event.eventName << '\n';
    cout << "Total seats: " << event.totalSeats << ", VIP seats: " << event.vipSeats << ", Regular seats: " << event.regularSeats << '\n';
    cout << "Available VIP seats: " << event.vipSeatsQueue.size() << ", Available Regular seats: " << event.regularSeatsQueue.size() << '\n';

    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Add seats" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Remove seats" << setw(23) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (choice == "0" || errorHandler.menuChoice(choice)) // Allow exit option
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "0") // Exit from the manageTickets menu
        {
            cout << "Exiting ticket management.\n";
            break;
        }

        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     VIP Seats" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Regular Seats" << setw(22) << "|\n";
        cout << "\t\t| [0]   |     Back to Main Menu" << setw(19) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        string option;
        while (true)
        {
            cout << "Enter seat type: ";
            getline(cin, option);
            if (option == "0" || errorHandler.menuChoice(option)) // Allow back option
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (option == "0") // Go back to the main action menu
        {
            cout << "Returning to main menu.\n";
            continue;
        }

        string seatsStr;
        int seats;

        while (true) // Keep asking until a valid number of seats is entered
        {
            cout << "Enter number of seats to " << (choice == "1" ? "add: " : "remove: ");
            getline(cin, seatsStr);

            if (errorHandler.idValidation(seatsStr)) // Check if input is valid
            {
                seats = stoi(seatsStr); // Convert the valid string to an integer
                break;                  // Exit loop if input is valid
            }
            else
            {
                cout << "Invalid number of seats. Please enter a numeric value with no spaces.\n";
            }
        }

        if (choice == "1") // Add seats
        {
            if (option == "1") // Add VIP seats
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].vipSeatsQueue.push(events[eventID].vipSeats + 1 + i);
                }
                events[eventID].vipSeats += seats;
            }
            else if (option == "2") // Add Regular seats
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].regularSeatsQueue.push(events[eventID].regularSeats + 1 + i);
                }
                events[eventID].regularSeats += seats;
            }
            events[eventID].totalSeats += seats;
            cout << "Seats added successfully.\n";
        }
        else if (choice == "2") // Remove seats
        {
            if (option == "1") // Remove VIP seats
            {
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
            else if (option == "2") // Remove Regular seats
            {
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
        }

        // Re-display the current state after each operation
        cout << "Current state for Event ID: " << event.eventId << ", Event Name: " << event.eventName << '\n';
        cout << "Total seats: " << event.totalSeats << ", VIP seats: " << event.vipSeats << ", Regular seats: " << event.regularSeats << "\n";
        cout << "Available VIP seats: " << event.vipSeatsQueue.size() << ", Available Regular seats: " << event.regularSeatsQueue.size() << "\n";
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
            if (e.vipSeatsQueue.empty() && e.regularSeatsQueue.empty())
            {
                cout << "No seats left.\n";
            }
            else
            {
                cout << "VIP Seats Available: " << e.vipSeatsQueue.size() << ", Regular Seats Available: " << e.regularSeatsQueue.size() << "\n";
            }
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
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Add Event" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Update Event" << setw(23) << "|\n";
        cout << "\t\t| [3]   |     Manage Tickets" << setw(21) << "|\n";
        cout << "\t\t| [4]   |     View All Users" << setw(21) << "|\n";
        cout << "\t\t| [5]   |     View All Events" << setw(20) << "|\n";
        cout << "\t\t| [6]   |     View All Users Data" << setw(16) << "|\n";
        cout << "\t\t| [7]   |     Manage Admin" << setw(23) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

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
            manageTickets();
        }
        else if (option == "4")
        {
            viewAllUsers();
        }
        else if (option == "5")
        {
            displayAllEvents();
        }
        else if (option == "6")
        {
            displayAllUserData();
        }
        else if (option == "7")
        {
            updateadminPanel(loggedInAdmin);
        }
        else if (option == "0")
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

void displayAllEvents()
{
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    cout << "All events:\n";
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        cout << "Event ID: " << eventID
             << ", Event Name: " << e.eventName
             << ", VIP Seats Available: " << e.vipSeatsQueue.size()
             << ", Regular Seats Available: " << e.regularSeatsQueue.size() << '\n';
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
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Sign Up" << setw(28) << "|\n";
        cout << "\t\t| [2]   |     Login" << setw(30) << "|\n";
        cout << "\t\t| [3]   |     Admin Panel" << setw(24) << "|\n";
        cout << "\t\t| [4]   |     Search Events" << setw(22) << "|\n";
        cout << "\t\t| [5]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

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

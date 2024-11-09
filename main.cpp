#include <iostream>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <regex> // Add this at the top for email validation

using namespace std;

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
    int event_id;
    string event_name;
    int total_seats;
    priority_queue<int> available_seats; // available seats
};

// Global maps to store users and events
unordered_map<string, User> users;
unordered_map<int, Event> events;

// Function declarations
void userSignUp();
void userLogin();
void updateUserProfile(const string &username);
void buyTickets(const string &username);
void viewTickets(const string &username);
void cancelTicket(const string &username);
void searchEvents();
void adminLogin();
void addEvent();
void updateEvent();
void viewAllUsers();
void manageTickets();
void displayUserMenu(const string &username);
void displayAdminMenu();
void clearInputBuffer();

bool isValidUsername(const string &username)
{
    return !username.empty() && username.length() >= 3;
}

bool isValidPassword(const string &password)
{
    return password.length() >= 6; // Minimum length of 6
}

bool isValidEmail(const string &email)
{
    const regex pattern(R"((\w+)(\.{0,1})(\w*)@(\w+)(\.(\w+))+)");
    return regex_match(email, pattern);
}

bool isPositiveInteger(int number)
{
    return number > 0;
}
// Modified userSignUp function with validation
void userSignUp()
{
    string username, password, email;

    cout << "Enter username: ";
    getline(cin, username);
    if (!isValidUsername(username))
    {
        cout << "Invalid username. Must be at least 3 characters long.\n";
        return;
    }

    if (users.find(username) != users.end())
    {
        cout << "Username already exists. Please choose a different username.\n";
        return;
    }

    cout << "Enter password: ";
    getline(cin, password);
    if (!isValidPassword(password))
    {
        cout << "Invalid password. Must be at least 6 characters long.\n";
        return;
    }

    cout << "Enter email: ";
    getline(cin, email);
    if (!isValidEmail(email))
    {
        cout << "Invalid email format.\n";
        return;
    }

    User newUser = {username, password, email, {}};
    users[username] = newUser;

    cout << "User signed up successfully!\n";
}

void userLogin()
{
    string username, password;
    cout << "Enter username: ";
    getline(cin, username);

    if (users.find(username) == users.end())
    {
        cout << "Username does not exist. Please sign up first.\n";
        return;
    }

    cout << "Enter password: ";
    getline(cin, password);

    if (users[username].password == password)
    {
        cout << "Login successful! Welcome, " << username << ".\n";
        displayUserMenu(username);
    }
    else
    {
        cout << "Incorrect password. Please try again.\n";
    }
}

void updateUserProfile(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }

    string newEmail;
    cout << "Enter new email: ";
    getline(cin, newEmail);

    users[username].email = newEmail;
    cout << "Profile updated successfully!\n";
}

// Modified buyTickets function with validation
void buyTickets(const string &username)
{
    int eventID, numberOfTickets;
    cout << "Enter event ID: ";
    cin >> eventID;
    clearInputBuffer();

    if (!isPositiveInteger(eventID) || events.find(eventID) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    cout << "Enter number of tickets: ";
    cin >> numberOfTickets;
    clearInputBuffer();

    if (!isPositiveInteger(numberOfTickets))
    {
        cout << "Number of tickets must be a positive integer.\n";
        return;
    }

    Event &event = events[eventID];
    if (event.available_seats.size() < numberOfTickets)
    {
        cout << "Not enough tickets available.\n";
        return;
    }

    for (int i = 0; i < numberOfTickets; ++i)
    {
        event.available_seats.pop(); // Simulating seat reservation
    }
    users[username].tickets[eventID] += numberOfTickets;

    cout << "Tickets booked successfully!\n";
}

// Similar validation can be added for other functions like updateUserProfile, cancelTicket, etc.

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
    clearInputBuffer();

    cout << "Enter number of tickets to cancel: ";
    cin >> numberOfTickets;
    clearInputBuffer();

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

    for (int i = 0; i < numberOfTickets; ++i)
    {
        events[eventID].available_seats.push(1); // Simulating seat release
    }
    users[username].tickets[eventID] -= numberOfTickets;
    if (users[username].tickets[eventID] == 0)
    {
        users[username].tickets.erase(eventID);
    }

    cout << "Tickets cancelled successfully!\n";
}

void searchEvents()
{
    string query;
    cout << "Enter event name to search: ";
    getline(cin, query);

    cout << "Search results:\n";
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        if (e.event_name.find(query) != string::npos)
        {
            cout << "Event ID: " << eventID << ", Event Name: " << e.event_name << '\n';
        }
    }
}

// Admin functions
void adminLogin()
{
    string username, password;
    cout << "Enter admin username: ";
    getline(cin, username);

    if (username != "admin")
    {
        cout << "Invalid admin username.\n";
        return;
    }

    cout << "Enter admin password: ";
    getline(cin, password);

    if (password == "admin_password")
    { // Replace with actual admin password
        cout << "Admin login successful! Welcome, Admin.\n";
        displayAdminMenu();
    }
    else
    {
        cout << "Incorrect password. Please try again.\n";
    }
}

void addEvent()
{
    int eventID;
    string eventName;
    int totalSeats;

    cout << "Enter event ID: ";
    cin >> eventID;
    cin.ignore(); // To ignore leftover newline character in input buffer

    // Check if the event ID already exists
    if (events.find(eventID) != events.end())
    {
        cout << "Event ID already exists. Please choose a different event ID.\n";
        return;
    }

    cout << "Enter event name: ";
    getline(cin, eventName);
    cout << "Enter total number of seats: ";
    cin >> totalSeats;
    cin.ignore(); // To ignore leftover newline character in input buffer

    // Create a new priority queue for available seats
    priority_queue<int> availableSeats;
    for (int i = 1; i <= totalSeats; ++i)
    {
        availableSeats.push(i);
    }

    // Create new event and add to the events map
    Event newEvent = {eventID, eventName, totalSeats, availableSeats};
    events[eventID] = newEvent;

    cout << "Event added successfully!\n";
}

void updateEvent()
{
    int eventID;
    cout << "Enter event ID to update: ";
    cin >> eventID;
    clearInputBuffer();

    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    string newEventName;
    cout << "Enter new event name: ";
    getline(cin, newEventName);

    events[eventID].event_name = newEventName;
    cout << "Event updated successfully!\n";
}

void viewAllUsers()
{
    cout << "All registered users:\n";
    for (const auto &user : users)
    {
        const string &username = user.first;
        const User &u = user.second;
        cout << "Username: " << username << ", Email: " << u.email << '\n';
    }
}

void manageTickets()
{
    int eventID;
    cout << "Enter event ID to manage tickets: ";
    cin >> eventID;
    clearInputBuffer();

    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    const Event &event = events[eventID];
    cout << "Managing tickets for Event ID: " << eventID << ", Event Name: " << event.event_name << '\n';
    cout << "Total seats: " << event.total_seats << ", Available seats: " << event.available_seats.size() << '\n';

    int choice;
    cout << "1. Add seats\n2. Remove seats\nChoose an option: ";
    cin >> choice;
    clearInputBuffer();

    int seats;
    if (choice == 1)
    {
        cout << "Enter number of seats to add: ";
        cin >> seats;
        clearInputBuffer();
        for (int i = 0; i < seats; ++i)
        {
            events[eventID].available_seats.push(events[eventID].total_seats + 1 + i);
        }
        events[eventID].total_seats += seats;
        cout << "Seats added successfully.\n";
    }
    else if (choice == 2)
    {
        cout << "Enter number of seats to remove: ";
        cin >> seats;
        clearInputBuffer();
        if (seats <= event.available_seats.size())
        {
            for (int i = 0; i < seats; ++i)
            {
                events[eventID].available_seats.pop();
            }
            events[eventID].total_seats -= seats;
            cout << "Seats removed successfully.\n";
        }
        else
        {
            cout << "Not enough seats available to remove.\n";
        }
    }
    else
    {
        cout << "Invalid option selected.\n";
    }
}

// Utility functions
void clearInputBuffer()
{
    cin.clear();                                         // Clear any error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore until newline
}

void displayUserMenu(const string &username)
{
    int choice;
    while (true)
    {
        cout << "\nUser Menu:\n";
        cout << "1. Update Profile\n";
        cout << "2. Buy Tickets\n";
        cout << "3. View Tickets\n";
        cout << "4. Cancel Ticket\n";
        cout << "5. Search Events\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            updateUserProfile(username);
            break;
        case 2:
            buyTickets(username);
            break;
        case 3:
            viewTickets(username);
            break;
        case 4:
            cancelTicket(username);
            break;
        case 5:
            searchEvents();
            break;
        case 6:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void displayAdminMenu()
{
    int choice;
    while (true)
    {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Event\n";
        cout << "2. Update Event\n";
        cout << "3. View All Users\n";
        cout << "4. Manage Tickets\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addEvent();
            break;
        case 2:
            updateEvent();
            break;
        case 3:
            viewAllUsers();
            break;
        case 4:
            manageTickets();
            break;
        case 5:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main()
{
    int choice;
    while (true)
    {
        cout << "\nWelcome to the Event Ticket Reservation System!\n";
        cout << "1. User Login\n";
        cout << "2. User Signup\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            userLogin();
            break;
        case 2:
            userSignUp();
            break;
        case 3:
            adminLogin();
            break;
        case 4:
            cout << "Thank you for using the Event Ticket Reservation System. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
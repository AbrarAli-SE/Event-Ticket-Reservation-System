#include <iostream>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <regex> // For email validation

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
    int vip_seats; // Number of VIP seats
    int regular_seats; // Number of regular seats
    priority_queue<int> vip_seats_queue; // VIP seats with higher priority
    priority_queue<int, vector<int>, greater<int>> regular_seats_queue; // Regular seats with lower priority
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
    const regex pattern(R"((\w+)(\.{0,1})(\w*)@(\w+)(\.(\w+))+)"); // Basic email validation regex
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

// Modified buyTickets function with VIP and Regular seat handling
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
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    // First allocate VIP seats if available
    while (vipTicketsBooked < numberOfTickets && !event.vip_seats_queue.empty())
    {
        event.vip_seats_queue.pop();
        vipTicketsBooked++;
    }

    // Allocate regular seats if VIP seats are less than required
    while (regularTicketsBooked < numberOfTickets - vipTicketsBooked && !event.regular_seats_queue.empty())
    {
        event.regular_seats_queue.pop();
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

    Event &event = events[eventID];

    // Release VIP seats
    int vipSeatsToCancel = min(numberOfTickets, users[username].tickets[eventID]);
    for (int i = 0; i < vipSeatsToCancel; ++i)
    {
        event.vip_seats_queue.push(1); // Simulate releasing a VIP seat
    }

    // Release Regular seats
    int regularSeatsToCancel = numberOfTickets - vipSeatsToCancel;
    for (int i = 0; i < regularSeatsToCancel; ++i)
    {
        event.regular_seats_queue.push(1); // Simulate releasing a regular seat
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
            cout << "Event ID: " << eventID << ", Event Name: " << e.event_name << ", VIP Seats: " << e.vip_seats << ", Regular Seats: " << e.regular_seats << '\n';
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
    int totalSeats, vipSeats, regularSeats;

    cout << "Enter event ID: ";
    cin >> eventID;
    clearInputBuffer();

    if (events.find(eventID) != events.end())
    {
        cout << "Event ID already exists.\n";
        return;
    }

    cout << "Enter event name: ";
    getline(cin, eventName);

    cout << "Enter total seats: ";
    cin >> totalSeats;
    clearInputBuffer();

    cout << "Enter number of VIP seats: ";
    cin >> vipSeats;
    clearInputBuffer();

    regularSeats = totalSeats - vipSeats;
    Event newEvent = {eventID, eventName, totalSeats, vipSeats, regularSeats};
    for (int i = 0; i < vipSeats; ++i)
    {
        newEvent.vip_seats_queue.push(1); // Simulate VIP seats
    }
    for (int i = 0; i < regularSeats; ++i)
    {
        newEvent.regular_seats_queue.push(1); // Simulate regular seats
    }

    events[eventID] = newEvent;
    cout << "Event added successfully!\n";
}

void updateEvent()
{
    // Simplified version, you can expand based on requirements
}

void viewAllUsers()
{
    cout << "All registered users:\n";
    for (const auto &user : users)
    {
        cout << "Username: " << user.first << ", Email: " << user.second.email << '\n';
    }
}

void manageTickets()
{
    // Simplified ticket management for now
}

void displayUserMenu(const string &username)
{
    int choice;
    do
    {
        cout << "\n1. Buy Tickets\n2. View Tickets\n3. Cancel Ticket\n4. Update Profile\n5. Log out\nEnter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            buyTickets(username);
            break;
        case 2:
            viewTickets(username);
            break;
        case 3:
            cancelTicket(username);
            break;
        case 4:
            updateUserProfile(username);
            break;
        case 5:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (true);
}

void displayAdminMenu()
{
    int choice;
    do
    {
        cout << "\n1. Add Event\n2. Update Event\n3. View All Users\n4. Manage Tickets\n5. Log out\nEnter your choice: ";
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
            break;
        }
    } while (true);
}

void clearInputBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
    int choice;
    do
    {
        cout << "\n1. User Sign Up\n2. User Login\n3. Admin Login\n4. Search Events\n5. Exit\nEnter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            userSignUp();
            break;
        case 2:
            userLogin();
            break;
        case 3:
            adminLogin();
            break;
        case 4:
            searchEvents();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}

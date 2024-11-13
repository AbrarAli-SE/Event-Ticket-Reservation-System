#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <Windows.h>
#include <vector>
using namespace std;

// Error Handling Class
class Validations // c;ass for input validations
{
public:
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

    bool stringValidation(string str)
    {
        if (str.empty())
        {
            return false;
        }

        bool containSpacesOnly = false;
        for (int i = 0; i < str.size(); ++i)
        {
            if (str[i] != ' ')
            {
                containSpacesOnly = true;
                break;
            }
        }

        if (!containSpacesOnly)
        {
            return false;
        }

        return true;
    }

    bool phoneValidation(string input)
    {
        if (input.empty())
        {
            return false;
        }
        return true;
    }
};

// Class for contact details
class Contact
{
public:
    string name;
    string number;
    string email;
    string type;
    bool isFavorite;
    Contact(string name = "", string number = "", string email = "", string type = "")
        : name(name), number(number), email(email), type(type), isFavorite(false) {}

    friend ostream &operator<<(ostream &os, const Contact &contact)
    {
        os << "\nName: " << contact.name
           << "\nNumber: " << contact.number
           << "\nEmail: " << contact.email
           << "\nType: " << contact.type
           << "\nFavorite: " << (contact.isFavorite ? "Yes" : "No") << endl;
        return os;
    }
};

// Node class for Binary Search Tree (BST)
class Node
{
public:
    Contact contact;
    Node *left;
    Node *right;

    Node(Contact contact) : contact(contact), left(nullptr), right(nullptr) {}
    ~Node()
    {
        delete left;
        delete right;
    }
};

// Binary Search Tree (BST) class to store contacts

class BST

{
private:
    Node *root;

    Node *insert(Node *node, Contact contact)
    {
        if (node == nullptr)
        {
            return new Node(contact);
        }

        if (contact.name < node->contact.name)
        {
            node->left = insert(node->left, contact);
        }
        else if (contact.name > node->contact.name)
        {
            node->right = insert(node->right, contact);
        }
        else
        {
            if (contact.number != node->contact.number || contact.email != node->contact.email)
            {
                node->right = insert(node->right, contact); // Insert with the same name but different info
            }
            else
            {
                cout << "\nContact already exists with the same name, number, and email.\n";
            }
        }
        return node;
    }

    Node *minValueNode(Node *node) const
    {
        Node *current = node;
        while (current && current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    void displayFavorites(Node *node) const
    {
        if (node == nullptr)
            return;
        displayFavorites(node->left);
        if (node->contact.isFavorite)
        {
            cout << node->contact;
        }
        displayFavorites(node->right);
    }

    Node *search(Node *node, const string &name) const
    {
        if (node == nullptr || node->contact.name == name)
        {
            return node;
        }
        if (name < node->contact.name)
        {
            return search(node->left, name);
        }
        return search(node->right, name);
    }

    // Updated inOrder function to handle file output
    void inOrder(Node *node, ostream &os) const
    {
        if (!node)
            return;
        inOrder(node->left, os);
        os << "Name: " << node->contact.name << "\n"
           << "Number: " << node->contact.number << "\n"
           << "Email: " << node->contact.email << "\n"
           << "Type: " << node->contact.type << "\n"
           << "Favorite: " << (node->contact.isFavorite ? "Yes" : "No") << "\n\n";
        inOrder(node->right, os);
    }

public:
    BST() : root(nullptr) {}
    ~BST()
    {
        delete root;
    }

    bool isValidEmail(const string &email) const
    {
        return email.size() > 10 && email.substr(email.size() - 10) == "@gmail.com";
    }
    Node *deleteNode(Node *node, const string &name)
    {
        if (node == nullptr)
            return node;

        if (name < node->contact.name)
        {
            node->left = deleteNode(node->left, name);
        }
        else if (name > node->contact.name)
        {
            node->right = deleteNode(node->right, name);
        }
        else
        {
            if (node->left == nullptr)
            {
                Node *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                Node *temp = node->left;
                delete node;
                return temp;
            }
            Node *temp = minValueNode(node->right);
            node->contact = temp->contact;
            node->right = deleteNode(node->right, temp->contact.name);
        }
        return node;
    }

    void insert(Contact contact)
    {
        root = insert(root, contact);
    }
    void saveToFile(const string &filename) const
    {
        ofstream file(filename);
        if (file.is_open())
        {
            inOrder(root, file); // Call inOrder with file output stream
            file.close();
            cout << "\nContacts saved successfully to " << filename << "\n";
        }
        else
        {
            cout << "\nFailed to open file for saving.\n";
        }
    }

    void loadFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "\nFailed to open file for loading.\n";
            return;
        }

        // Clear the current BST to avoid duplication if reloaded
        delete root;
        root = nullptr;

        string line, name, number, email, type, favorite;
        getline(file, line); // Skip header line

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, name, ',');
            getline(ss, number, ',');
            getline(ss, email, ',');
            getline(ss, type, ',');
            getline(ss, favorite, ',');

            Contact contact(name, number, email, type);
            contact.isFavorite = (favorite == "Yes");
            insert(contact);
        }

        file.close();
        cout << "\nContacts loaded successfully from " << filename << "\n";
    }

    void deleteMultipleContacts(const vector<string> &names)
    {
        for (const auto &name : names)
        {
            root = deleteNode(root, name);
        }
        cout << "\nSpecified contacts have been deleted.\n";
    }

    // Function to delete all contacts
    void deleteAllContacts()
    {
        delete root;
        root = nullptr;
        cout << "\nAll contacts have been deleted.\n";
    }

    Node *searchContact(const string &name) const
    {
        return search(root, name);
    }

    void editContact(const string &name)
    {
        Node *contactNode = searchContact(name);
        if (contactNode)
        {
            cout << "\nEditing contact details for " << name << ":\n";
            cout << "Enter new number (or press enter to keep current): ";
            string newNumber;
            getline(cin, newNumber);
            if (!newNumber.empty())
                contactNode->contact.number = newNumber;

            cout << "Enter new email (or press enter to keep current): ";
            string newEmail;
            getline(cin, newEmail);
            if (!newEmail.empty() && isValidEmail(newEmail))
                contactNode->contact.email = newEmail;

            cout << "Enter new type (or press enter to keep current): ";
            string newType;
            getline(cin, newType);
            if (!newType.empty())
                contactNode->contact.type = newType;

            cout << "\nContact updated successfully!\n";
        }
        else
        {
            cout << "\nContact not found.\n";
        }
    }

    void markAsFavorite(const string &name)
    {
        Node *contactNode = searchContact(name);
        if (contactNode)
        {
            contactNode->contact.isFavorite = true;
            cout << "\n"
                 << name << " has been added to favorites.\n";
        }
        else
        {
            cout << "\nContact not found.\n";
        }
    }

    void removeFromFavorite(const string &name)
    {
        Node *contactNode = searchContact(name);
        if (contactNode && contactNode->contact.isFavorite)
        {
            contactNode->contact.isFavorite = false;
            cout << "\n"
                 << name << " has been removed from favorites.\n";
        }
        else
        {
            cout << "\nContact not found or is not a favorite.\n";
        }
    }

    void displayAllContacts() const
    {
        if (root == nullptr)
        {
            cout << "\nNo contacts available.\n";
        }
        else
        {
            cout << "\nDisplaying all contacts:\n";
            inOrder(root, cout); // Use cout to display to console
        }
    }

    void displayFavoriteContacts() const
    {
        if (root == nullptr)
        {
            cout << "\nNo contacts available.\n";
        }
        else
        {
            cout << "\nDisplaying favorite contacts:\n";
            displayFavorites(root);
        }
    }
};
// Application class that manages user interaction
class PhoneBookApp : public Validations
{
private:
    BST bst;
    string password;
    string securityAnswer;

    void changePassword()
    {
        while (true)
        {
            cout << "\nEnter new password: ";
            getline(cin, password);
            if (stringValidation(password)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\t Invalid Password Patteren" << endl;
            }
        }
        cout << "\nPassword changed successfully!\n";
    }

    void restorePassword()
    {
        string answer;
        cout << "\nTo restore your password, answer the security question.";
        while (true)
        {
            cout << "\nWhat is your favorite color? ";
            getline(cin, answer);
            if (stringValidation(answer)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\nInvalid Answer. please try again\n";
            }
        }
        if (answer == securityAnswer)
        {
            cout << "\nYour password is: " << password << endl;
        }
        else
        {
            cout << "\nIncorrect answer. Cannot restore password.\n";
        }
    }

    bool authenticate()
    {
        int attempts = 0;
        string inputPassword;
        while (attempts < 3)
        {
            cout << "\nEnter password: ";
            getline(cin, inputPassword);
            if (inputPassword == password)
            {
                return true;
            }
            else
            {
                cout << "\nIncorrect password. Try again.\n";
                attempts++;
            }
        }
        cout << "\nToo many failed attempts. Exiting settings.\n";
        return false;
    }

public:
    PhoneBookApp() : password("123"), securityAnswer("blue") {}

    // Main settings menu
    void settingsMenu()
    {
        if (!authenticate())
            return; // Exit if authentication fails
        string choice;
        do
        {
            cout << "\n--- Settings Menu ---";
            cout << "\n1. Change Password";
            cout << "\n2. Backup Contacts";
            cout << "\n3. Restore Password";
            cout << "\n0. Return to Main Menu";
            cout << "\nSelect an option: ";
             while (true)
            {
                cout << "\nSelect an option: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\tInvalid option! Please try again.\n\n";
            }

            if (choice == "1")
            {
                system("cls");
                changePassword();
            }
            else if (choice == "2")
            {
                system("cls");
                // backupContacts();  // Uncomment if backupContacts function is implemented
            }
            else if (choice == "3")
            {
                system("cls");
                restorePassword();
            }
        } while (choice != "0");
    }

    void startApp()
    {
        if (!authenticate())
        {
            return; // Exit if authentication fails
        }
        // Load contacts immediately when the app starts
        bst.loadFromFile("contacts.txt");

        string choice;
        do
        {
            displayMenu();
            while (true)
            {
                cout << "\nSelect an option: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\tInvalid option! Please try again.\n\n";
            }
            if (choice == "1")
            {
                system("cls");
                addContact();
            }
            else if (choice == "2")
            {
                system("cls");
                editContact();
            }
            else if (choice == "3")
            {
                system("cls");
                searchContact();
            }
            else if (choice == "4")
            {
                system("cls");
                batchDelete();
            }
            else if (choice == "5")
            {
                system("cls");
                manageFavorites();
            }
            else if (choice == "6")
            {
                system("cls");
                displayAllContacts();
            }
            else if (choice == "7")
            {
                system("cls");
                settingsMenu();
            }
        } while (choice != "0");
    }

    void batchDelete()
    {
        cout << "\n--- Batch Delete Menu ---";
        cout << "\n1. Delete multiple contacts by name";
        cout << "\n2. Delete all contacts";
        string choice;
        while (true)
        {
            cout << "\nSelect an option: ";
            getline(cin, choice);
            if (menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1")
        {
            vector<string> names;
            string name;
            cout << "\nEnter names to delete (type 'done' to finish):\n";
            while (true)
            {
                getline(std::cin, name);
                if (name == "done")
                    break;
                names.push_back(name);
            }
            bst.deleteMultipleContacts(names);
            bst.saveToFile("contacts.txt"); // Save after batch delete
        }
        else if (choice == "2")
        {
            bst.deleteAllContacts();
            bst.saveToFile("contacts.txt"); // Save after deleting all
        }
        else
        {
            cout << "\nInvalid choice. Returning to main menu.\n";
        }
    }

    void addContact()
    {
        string name, number, email, type;

        while (true)
        {
            cout << "\nEnter the Name ";
            getline(cin, name);
            if (nameValidation(name)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Name. Please try again.\n";
            }
        }

        while (true)
        {
            cout << "Enter number: ";
            getline(cin, number);
            if (phoneValidation(number))
            {
                break;
            }
            else
            {
                cout << "\n\n\tInvalid Number. Please try again.\n";
            }
        }
        while (true)
        {
            cout << "Enter email (must end with '@gmail.com'): ";
            getline(cin, email);
            if (bst.isValidEmail(email))
            {
                break;
            }
            else
            {
                cout << "\nInvalid email. Please ensure the email ends with '@gmail.com'.\n";
            }
        }

        while (true)
        {
            cout << "Enter type (PTCL, Local, Emergency): ";
            getline(cin, type);
            if (nameValidation(type)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Type. Please try again.\n";
            }
        }

        Contact newContact(name, number, email, type);
        bst.insert(newContact);
        bst.saveToFile("contacts.txt"); // Save after adding contact
        cout << "\nContact added successfully!\n";
    }

    void markAsFavorite()
    {
        string name;
        while (true)
        {
            cout << "\nEnter the name of the contact to mark as favorite: ";
            getline(cin, name);
            if (nameValidation(name)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Name. Please try again.\n";
            }
        }
        bst.markAsFavorite(name);
        bst.saveToFile("contacts.txt"); // Save after marking as favorite
    }

    void removeFromFavorite()
    {
        string name;
        while (true)
        {
            cout << "\nEnter the name of the contact to remove from favorites: ";
            getline(cin, name);
            if (nameValidation(name)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Name. Please try again.\n";
            }
        }
        bst.removeFromFavorite(name);
        bst.saveToFile("contacts.txt"); // Save after removing from favorite
    }

    void displayFavoriteContacts()
    {
        bst.displayFavoriteContacts();
    }

    void editContact()
    {
        string name;
        while (true)
        {
            cout << "\nEnter the name of the contact to edit: ";
            getline(cin, name);
            if (nameValidation(name)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Name. Please try again.\n";
            }
        }
        bst.editContact(name);
        bst.saveToFile("contacts.txt"); // Save after editing contact
    }

    void searchContact()
    {
        string name;
        while (true)
        {
            cout << "\nEnter the name of the contact to search: ";
            getline(cin, name);
            if (nameValidation(name)) // Validate the username
            {
                break; // Exit loop if username is valid
            }
            else
            {
                cout << "\n\n\tInvalid Name. Please try again.\n";
            }
        }
        Node *result = bst.searchContact(name);
        if (result)
        {
            cout << "\nContact found:\n"
                 << result->contact;
        }
        else
        {
            cout << "\nNo contact found with the name " << name << endl;
        }
    }

    void displayAllContacts()
    {
        bst.displayAllContacts();
    }

    void manageFavorites()
    {
        string choice;
        do
        {
            favoriteMenu();
            while (true)
            {
                cout << "\nSelect an option: ";
                getline(cin, choice);
                if (menuChoice(choice))
                {
                    break;
                }
                cout << "\n\tInvalid option! Please try again.\n\n";
            }
            if (choice == "1")
            {
                system("cls");
                markAsFavorite();
            }
            else if (choice == "2")
            {
                system("cls");
                removeFromFavorite();
            }
            else if (choice == "3")
            {
                system("cls");
                displayFavoriteContacts();
            }
        } while (choice != "0");
    }

    void favoriteMenu()
    {
        cout << "\n--- Favorite Contacts Menu ---";
        cout << "\n1. Add to Favorite";
        cout << "\n2. Remove from Favorite";
        cout << "\n3. Display Favorite Contacts";
        cout << "\n0. Exit";
    }

    void displayMenu()
    {
        cout << "\n--- Phone Book Management System ---";
        cout << "\n1. Add Contact";
        cout << "\n2. Edit Contact";
        cout << "\n3. Search Contact";
        cout << "\n4. Delete Contact";
        cout << "\n5. Manage Favorite Contacts";
        cout << "\n6. Display All Contacts";
        cout << "\n7. Settings";
        cout << "\n0. Exit";
    }
};

int main()
{
    PhoneBookApp app;
    app.startApp();
    return 0;
}
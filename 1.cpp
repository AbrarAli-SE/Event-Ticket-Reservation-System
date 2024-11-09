#include <iostream>
#include <string>
#include <cctype>

bool passwordValidation(const std::string& str, bool& hasSpecialChar)
{
    // Flag to check if the password is at least 6 characters long
    bool isLengthValid = str.length() >= 6;

    // Iterate through the password string to check for special characters
    hasSpecialChar = false;
    for (int i = 0; i < str.size(); ++i)
    {
        if (!isalnum(str[i]))  // Check if the character is not alphanumeric (i.e., a special character)
        {
            hasSpecialChar = true;
            break;
        }
    }

    return isLengthValid && hasSpecialChar;
}

int main()
{
    std::string password;

    while (true)  // Infinite loop until the user enters a valid password
    {
        std::cout << "Enter password: ";
        std::cin >> password;

        bool hasSpecialChar = false;
        if (passwordValidation(password, hasSpecialChar))
        {
            std::cout << "Password is strong." << std::endl;
            break;  // Exit the loop if the password is valid
        }
        else
        {
            // Check for which condition failed and display specific error messages
            if (password.length() < 6)
            {
                std::cout << "Password must be at least 6 characters long." << std::endl;
            }
            else if (!hasSpecialChar)
            {
                std::cout << "Password must contain at least one special character." << std::endl;
            }
        }
    }

    return 0;
}

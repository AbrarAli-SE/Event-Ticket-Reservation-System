#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <Windows.h>
using namespace std;

// Contact Book Validation

class Validations // class for input validations
{
public:
    bool idValidation(const string &idString)
    {
        if (idString.empty() || idString.find(' ') != string::npos)
        {
            return false;
        }

        for (char ch : idString)
        {
            if (!isdigit(ch))
            {
                return false;
            }
        }
        return true;
    }

    bool stringValidation(const string &str)
    {
        if (str.empty())
        {
            return false;
        }

        for (char ch : str)
        {
            if (!isspace(ch))
            {
                return true;
            }
        }
        return false;
    }

    bool phoneNumberValidation(const string &number)
    {
        if (number.size() !=8)
        {
            return false;
        }

        for (char ch : number)
        {
            if (!isdigit(ch))
            {
                return false;
            }
        }
        return true;
    }

    bool emailValidation(const string &email)
    {
        const string gmailSuffix = "@gmail.com";
        if (email.empty())
        {
            return false;
        }

        for (char ch : email)
        {
            if (isspace(ch))
            {
                return false;
            }
        }
        if (email.size() < gmailSuffix.size() + 5)
        {
            return false;
        }

        string prefix = email.substr(0, email.size() - gmailSuffix.size());
        for (char ch : prefix)
        {
            if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
            {
                return false;
            }
        }

        string suffix = email.substr(email.size() - gmailSuffix.size());
        return suffix == gmailSuffix;
    }
};


//          E - Commerence validation

class Validations  //c;ass for input validations 
{
public:
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
    bool adminStringValidation(string str)
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
};



// Vehicle manangement system validation 

class ErrorHandling
{
private:
    int lettersCount;
    int digitsCount;
    bool spaceEncountered;

public:
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

    bool plateNoValidation(string plateNo)
    {
        if (plateNo.empty())
        {
            return false;
        }

        lettersCount = 0;
        digitsCount = 0;
        spaceEncountered = false;

        for (int i = 0; i < plateNo.size(); i++)
        {
            if (isalpha(plateNo[i]))
            {
                lettersCount++;
            }
            else if (isdigit(plateNo[i]))
            {
                digitsCount++;
            }
            else if (plateNo[i] == ' ')
            {
                if (spaceEncountered || lettersCount < 2 || lettersCount > 6)
                {
                    return false;
                }
                spaceEncountered = true;
            }
            else
            {
                return false;
            }
        }

        if (!spaceEncountered || lettersCount < 2 || lettersCount > 6 || digitsCount < 3 || digitsCount > 4)
        {
            return false;
        }

        return true;
    }

    bool hoursValidation(string idString)
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
        if (stoi(idString) <= 24 && stoi(idString) >= 1)
        {
            return true;
        }
        return false;
    }

    bool daysValidation(string days)
    {
        for (int i = 0; i < days.size(); ++i)
        {
            if (!isdigit(days[i]))
            {
                return false;
            }
        }
        if (days.empty())
        {
            return false;
        }
        for (int i = 0; i < days.size(); ++i)
        {
            if (days[i] == ' ')
            {
                return false;
            }
        }
        if (stoi(days) <= 31 && stoi(days) >= 1)
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

        bool hasDecimalPoint = false;
        for (size_t i = 0; i < amount.size(); ++i)
        {
            if (amount[i] == '.')
            {
                if (hasDecimalPoint) // More than one decimal point is invalid
                    return false;
                hasDecimalPoint = true;
            }
            else if (!isdigit(amount[i]))
            {
                return false;
            }
        }

        return true;
    }

    bool tokenValid(string token)
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
        if (token.size() == 6)
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

    void upperCase(string &plateNo)
    {
        for (int i = 0; i < plateNo.size(); i++)
        {
            plateNo[i] = toupper(plateNo[i]);
        }
    }
};


#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <filesystem>
#include <stdexcept>

using namespace std;

// global declartion
struct User
{
    string username;
    string password;
    string email;
    string role;
    double accountBalance;
    string accountNumber;
    double loanBalance;
};

// FUNCTION PROTOTYPE
void writeUserData(const string &filename, const vector<User> &users);
vector<User> readUserData(const string &filename);
void transferFunds(User &sender, User &recipient, double amount);
void createUser(vector<User> &users);
double calculateTotalRepayment(double loanAmount, int loanTerm);

string currentDateTime();

string generateReceipt(User user, int transactType, double amountProcessed, User *recipient = nullptr);
void printReceipt(string data);
vector<string> readTransactionHistory(User user);
bool printTransactionLog(vector<string> list);

// GLOBAL VARIABLES
const double INTEREST_RATE = 0.05;
basic_string parentDir = filesystem::current_path().string();
basic_string transactionLogDir = parentDir + "\\transaction_log";

// This function is used to read the data informations
vector<User> readUserData(const string &filename)
{
    vector<User> users;
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        istringstream ss(line);
        User user;
        char delimiter;

        if (getline(ss, user.username, ',') && getline(ss, user.password, ',') && getline(ss, user.email, ',') &&
            getline(ss, user.role, ',') && (ss >> user.accountBalance) && (ss >> delimiter) &&
            getline(ss, user.accountNumber, ',') && (ss >> user.loanBalance))
        {
            users.push_back(user);
        }
    }

    return users;
}

// This function is used to write a user data
void writeUserData(const string &filename, const vector<User> &users)
{
    ofstream file(filename);
    file << fixed << setprecision(2);

    for (const User &user : users)
    {
        file << user.username << "," << user.password << "," << user.email << "," << user.role << "," << user.accountBalance << ","
             << user.accountNumber << "," << user.loanBalance << "\n";
    }
}

// This function is used to transfer funds to the recipient
void transferFunds(User &sender, User &recipient, double amount)
{
    if (sender.accountBalance >= amount)
    {
        sender.accountBalance -= amount;
        recipient.accountBalance += amount;
    }
}

// This function is used to check if the email already exists
bool emailExists(const vector<User> &users, const string &email)
{
    for (const auto &user : users)
    {
        if (user.email == email)
        {
            return true;
        }
    }

    ifstream file("user_data.csv");
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string storedEmail;

        getline(iss, storedEmail, ',');
        if (storedEmail == email)
        {
            file.close();
            return true;
        }
    }
    file.close();

    return false;
}

// This function is used to create a new user
void createUser(vector<User> &users)
{
    User newUser;
    stringstream ss;

    cout << "Welcome! Let's create your account." << endl;
    cout << "Enter a username: ";
    cin >> newUser.username;

    cout << endl
         << "Enter a password: ";
    cin >> newUser.password;

    do
    {
        cout << endl
             << "Enter a preferred email address: ";
        cin >> newUser.email;

        if (newUser.email.find("@") == string::npos || newUser.email.find(".com") == string::npos)
        {
            cout << "\033[1mOops! Please enter a valid email address (e.g., user@example.com).\033[0m" << endl;
            continue;
        }

        if (emailExists(users, newUser.email))
        {
            cout << "\033[1mEmail already exists. Please enter a different email address.\033[0m\n";
            continue;
        }

        break;

    } while (true);

    newUser.role = "user";

    // Generate account number. Uses the following format: "ACCT-(Security PIN)-(Account Number)"
    // (e.g. ACCT-1234-0001)
    // Generate 4-digit random number for security PIN
    ss << setw(4) << setfill('0') << to_string(rand() % 9999) << "-";
    // Format the account number to 4-digits (e.g. 0001, 0002, etc.)

    ss << setw(4) << setfill('0') << to_string(users.size() + 1);
    newUser.accountNumber = "ACCT-" + ss.str();

    users.push_back(newUser);

    std::cout << "\033[1mPLEASE TAKE NOTE OF YOUR ACCOUNT NUMBER FOR LOG IN: " << newUser.accountNumber << "\033[0m" << std::endl;
    cout << "User created successfully!\n\n";
}

// Used to delete the user
void deleteUser(vector<User> &users, const string &usernameToDelete)
{
    users.erase(remove_if(users.begin(), users.end(),
                          [usernameToDelete](const User &user)
                          { return user.username == usernameToDelete; }),
                users.end());

    cout << "User deleted successfully!\n";
}

// This function is used to calculate the total repayment of loan
double calculateTotalRepayment(double loanAmount, int loanTerm)
{
    double interestAmount = loanAmount * INTEREST_RATE * loanTerm;

    double totalRepayment = loanAmount + interestAmount;

    return totalRepayment;
}

// Used to get the current date and time. Returns a formatted string
string currentDateTime()
{
    auto now = chrono::system_clock::now();
    auto t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&t), "%Y-%m-%d %X");
    return ss.str();
}

// Used to generate a receipt containing relevant information about a
// transaction. Must be called whenever the user performs a transaction.
// This function does two things: (1) Save receipt to file, and ...
string generateReceipt(User user, int transactType, double amountProcessed, User *recipient)
{
    string dateTime = currentDateTime();
    stringstream ss;

    if (!filesystem::exists(transactionLogDir))
    {
        filesystem::create_directory(transactionLogDir);
    }
    filesystem::current_path(transactionLogDir);
    fstream fTransactionLog(user.accountNumber + ".csv", ios::app);
    fTransactionLog << fixed << setprecision(2);

    switch (transactType)
    {
    case 0:
        ss << "Transfer";
        break;
    case 2:
        ss << "Cash in";
        break;
    case 3:
        ss << "Cash out";
        break;
    case 4:
        ss << "Loan";
        break;
    case 5:
        ss << "Pay loan";
        break;
    }

    ss << "," << dateTime << "," << user.username << "," << user.accountNumber << "," << amountProcessed;

    if (recipient)
    {
        ss << "," << recipient->username << "," << recipient->accountNumber;
    }
    else
    {
        ss << ",,";
    }
    ss << "\n";

    fTransactionLog << ss.str();

    filesystem::current_path(parentDir);

    return ss.str();
}

// Used to read and parse a specified user's transaction history
vector<string> readTransactionHistory(User user)
{
    vector<string> receiptList; // Stores the parsed data. Returned at the end of the function
    string buf;                 // Temporary string buffer for parsing

    // Check if the directory ./transaction_log exists
    // If it exists, move the current working path there
    // If not, create it
    if (!filesystem::exists(transactionLogDir))
    {
        filesystem::create_directory(transactionLogDir);
    }
    filesystem::current_path(transactionLogDir);
    ifstream fTransactionLog(user.accountNumber + ".csv"); // Open csv file

    // Store each line in the file to receiptList
    while (getline(fTransactionLog, buf))
    {
        receiptList.push_back(buf);
    }

    // Change working path back to parent directory
    filesystem::current_path(parentDir);

    return receiptList;
}

// Prints a formatted list of user's transaction logs
// Takes a list of receipts (vector<string>) as parameter
// Returns true or false if the list has contents or not
bool printTransactionLog(vector<string> list)
{
    stringstream line;   // Stores each receipt in the list for parsing
    string temp;         // Temporary string buffer
    stringstream buffer; // Temporary stringstream buffer

    cout << endl; // Print a new line before displaying the list

    if (list.empty())
    {
        cout << "No previous transactions" << endl;
        return false;
    }
    else
    {
        // Iterate over the list going over each receipt
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            line << *it; // Store the current receipt in stringstream

            // Take the first 2 data in the stringstream (transaction type, date time)
            for (int i = 0; i < 2; i++)
            {
                getline(line, temp, ',');
                temp.append("\t");
                buffer << temp;
            }
            // Format the string in the following format:
            // (receiptNumber) transactionType, dateTime
            cout << "(" << it - list.begin() + 1 << ") " << buffer.str() << endl;
            buffer.str(""); // Clear buffer for next iteration
            line.str("");
        }
        return true;
    }
}

void performTransaction(User *currentUser, int userChoice, double cashInAmount, vector<User> &users)
{
    string choiceStr;
    cout << "Do you want to continue transaction? (1 for YES, 0 for NO): ";
    cin >> choiceStr;

    try
    {
        int choice = stoi(choiceStr);

        if (choice == 1)
        {
            currentUser->accountBalance += cashInAmount;
            writeUserData("user_data.csv", users);
            printReceipt(generateReceipt(*currentUser, userChoice, cashInAmount));
        }
        else if (choice == 0)
        {
            cout << "Cash in cancelled\n";
        }
        else
        {
            cout << "Invalid user input. Please try again.\n";
        }
    }
    catch (const invalid_argument &)
    {
        std::cout << "Input must be an integer. Please try again.\n";
    }
}

void returnToMainMenu()
{
    while (true)
    {
        cout << "\nChoose an option: \n";
        cout << "1. Return to main menu\n";
        cout << "Enter your option: ";

        string input;
        cin >> input;

        try
        {
            int balanceOption = stoi(input);

            if (balanceOption == 1)
            {
                cout << "Returning to the main menu...\n\n";
                break;
            }
            else
            {
                cout << "Invalid option, Please press 1 to return to the main menu.\n";
            }
        }
        catch (const invalid_argument &)
        {
            cout << "Invalid input. Please enter a valid integer.\n";
            // Clear the input stream to avoid an infinite loop on invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (const out_of_range &)
        {
            cout << "Input out of range for integer conversion.\n";
            // Clear the input stream to avoid an infinite loop on invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// This function is used to print receipt
void printReceipt(string data)
{
    vector<string> info;
    stringstream ss(data);
    string str;

    while (getline(ss, str, ','))
    {
        info.push_back(str);
    }

    cout << "\n\n----------TRANSACTION COMPLETED----------\n";
    cout << "Type of transaction:\t" << info[0] << endl;
    cout << "Date Time:\t\t" << info[1] << endl;
    cout << "Account name:\t\t" << info[2] << endl;
    cout << "Account number:\t\t" << info[3] << endl;
    cout << "Amount processed:\t" << info[4] << endl;

    // Displays the recipient's details if the transaction has a recipient
    if (!info[5].empty())
    {
        cout << "Recipient:\t\t" << info[5] << endl;
        cout << "Account Number:\t\t" << info[6] << endl;
    }

    cout << endl;

    returnToMainMenu();
}

bool askUserToContinue()
{
    string input;

    cout << "Do you want to continue with the transaction? (1 for YES, 0 for NO): ";
    cin >> input;

    try
    {
        int choice = stoi(input);
        return (choice == 1);
    }
    catch (const invalid_argument &e)
    {
        cerr << "Invalid input. Please enter a valid integer." << endl;
        return false;
    }
    catch (const out_of_range &e)
    {
        cerr << "Input out of range for integer." << endl;
        return false;
    }
}

int getInputAsInteger(const string &prompt)
{
    string userInput;
    int result;

    while (true)
    {
        cout << prompt;
        cin >> userInput;

        try
        {
            result = stoi(userInput);
            break; // Break out of the loop if conversion is successful
        }
        catch (const invalid_argument &)
        {
            cout << "Invalid input. Please enter a valid integer.\n";
        }
    }

    return result;
}

#endif
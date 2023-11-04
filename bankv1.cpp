#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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
void transferFunds(User &sender, User &recipient, double amount)
{
    if (sender.accountBalance >= amount)
    {
        sender.accountBalance -= amount;
        recipient.accountBalance += amount;
    }
}

void createUser(vector<User> &users)
{
    User newUser;

    cout << "Enter a new username: ";
    cin >> newUser.username;

    cout << "Enter a password: ";
    cin >> newUser.password;

    cout << "Enter an email address: ";
    cin >> newUser.email;

    cout << "Enter a role (e.g., 'user' or 'admin'): ";
    cin >> newUser.role;

    cout << "Enter an initial account balance: ₱";
    cin >> newUser.accountBalance;

    newUser.accountNumber = "ACCT" + to_string(users.size() + 1);
    users.push_back(newUser);

    cout << "User created successfully!\n";
}

const double INTEREST_RATE = 0.05;

double calculateTotalRepayment(double loanAmount, int loanTerm)
{
    double interestAmount = loanAmount * INTEREST_RATE * loanTerm;

    double totalRepayment = loanAmount + interestAmount;

    return totalRepayment;
}

int main()
{
    vector<User> users = readUserData("user_data.csv");

    while (true)
    {
        cout << "Choose an option:\n";
        cout << "1. Login\n";
        cout << "2. Create a new user\n";
        cout << "3. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter your username: ";
            string username;
            cin >> username;

            cout << "Enter your password: ";
            string password;
            cin >> password;

            User *currentUser = nullptr;
            for (User &user : users)
            {
                if (username == user.username && password == user.password)
                {
                    currentUser = &user;
                    break;
                }
            }

            if (currentUser)
            {
                while (true)
                {
                    cout << "Welcome, " << currentUser->username << "!\n";
                    cout << "Choose an option:\n";
                    cout << "1. Transfer funds\n";
                    cout << "2. Check Account balances\n";
                    cout << "3. Cash in\n";
                    cout << "4. Cash out\n";
                    cout << "5. Loan\n";
                    cout << "6. Pay Loan Balance\n";
                    cout << "7. Logout\n";

                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1)
                    {
                        cout << "Enter the username of the recipient: ";
                        string recipientUsername;
                        cin >> recipientUsername;

                        User *recipientUser = nullptr;
                        for (User &user : users)
                        {
                            if (recipientUsername == user.username)
                            {
                                recipientUser = &user;
                                break;
                            }
                        }

                        if (recipientUser)
                        {
                            cout << "Enter the amount to transfer: ₱";
                            double transferAmount;
                            cin >> transferAmount;

                            transferFunds(*currentUser, *recipientUser, transferAmount);

                            writeUserData("user_data.csv", users);

                            cout << "Transfer successful!\n";
                        }
                        else
                        {
                            cout << "Error: Recipient not found.\n";
                        }
                    }
                    else if (userChoice == 2)
                    {
                        cout << "Account Balance: ₱" << currentUser->accountBalance << "\n";

                        if (currentUser->loanBalance == 0)
                        {
                            cout << "Loan Balance: ₱0\n";
                        }
                        else
                        {
                            cout << "Loan Balance: ₱" << currentUser->loanBalance << "\n";
                        }
                    }
                    else if (userChoice == 3)
                    {
                        cout << "Enter the amount to cash in: ₱";
                        double cashInAmount;
                        cin >> cashInAmount;

                        currentUser->accountBalance += cashInAmount;

                        writeUserData("user_data.csv", users);

                        cout << "Cash in successful!\n";
                    }
                    else if (userChoice == 4)
                    {
                        cout << "Enter the amount to cash out: ₱";
                        double cashOutAmount;
                        cin >> cashOutAmount;

                        if (currentUser->accountBalance >= cashOutAmount)
                        {
                            currentUser->accountBalance -= cashOutAmount;

                            writeUserData("user_data.csv", users);

                            cout << "Cash out successful!\n";
                        }
                        else
                        {
                            cout << "Error: Insufficient balance for cash out.\n";
                        }
                    }

                    else if (userChoice == 5)
                    {
                        cout << "Enter the loan amount: ₱";
                        double loanAmount;
                        cin >> loanAmount;

                        if (loanAmount > 0)
                        {
                            // Ask the user for the loan term (in months)
                            cout << "Enter the loan term (in months): ";
                            int loanTerm;
                            cin >> loanTerm;

                            double totalRepayment = calculateTotalRepayment(loanAmount, loanTerm);

                            cout << "Total amount to be repaid (including interest): ₱" << totalRepayment << endl;

                            // Check if the user wants to proceed with the loan
                            cout << "Do you want to proceed with the loan? (1 for yes, 0 for no): ";
                            int proceedLoan;
                            cin >> proceedLoan;

                            if (proceedLoan == 1)
                            {
                                currentUser->accountBalance += loanAmount;
                                currentUser->loanBalance += loanAmount;

                                writeUserData("user_data.csv", users);

                                cout << "Loan of ₱" << loanAmount << " taken successfully!\n";
                            }
                            else
                            {
                                cout << "Loan request canceled.\n";
                            }
                        }
                        else
                        {
                            cout << "Error: Invalid loan amount.\n";
                        }
                    }
                    else if (userChoice == 6)
                    {
                        cout << "Loan Balance: $" << currentUser->loanBalance << "\n"; // Display the current loan balance

                        cout << "Enter the amount to pay towards the loan balance: $";
                        double paymentAmount;
                        cin >> paymentAmount;

                        if (paymentAmount > 0 && paymentAmount <= currentUser->loanBalance)
                        {
                            // Check if the user has enough funds to make the payment
                            if (paymentAmount <= currentUser->accountBalance)
                            {
                                currentUser->accountBalance -= paymentAmount;
                                currentUser->loanBalance -= paymentAmount;

                                writeUserData("user_data.csv", users);

                                cout << "Remaining Loan Balance: $" << currentUser->loanBalance << "\n";

                                cout << "Loan payment of $" << paymentAmount << " successful!\n";
                            }
                            else
                            {
                                cout << "Error: Insufficient account balance to make the payment.\n";
                            }
                        }
                        else if (paymentAmount > currentUser->loanBalance)
                        {
                            cout << "Error: Payment amount exceeds the loan balance.\n";
                        }
                        else
                        {
                            cout << "Error: Invalid payment amount. Please enter a valid amount.\n";
                        }
                    }

                    else if (userChoice == 7)
                    {
                        cout << "Logging out...\n";
                        break;
                    }
                }
            }
            else
            {
                cout << "Error: User not found or incorrect credentials.\n";
            }
        }
        else if (choice == 2)
        {
            createUser(users);
            writeUserData("user_data.csv", users);
        }
        else if (choice == 3)
        {
            cout << "Exiting the program...\n";
            break;
        }
    }

    return 0;
}

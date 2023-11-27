#include "banksystem.hpp"

int main()
{
    // Provide a seed value for generating random numbers
    srand((unsigned)time(NULL));

    vector<User> users = readUserData("user_data.csv");

    // MAIN PROGRAM LOOP
    while (true)
    {
        // Home page
        cout << "\n---------------------------------------------------------------------------\n";
        cout << "  ____              _    _                _____           _                 \n";
        cout << " |  _ \\            | |  (_)              / ____|         | |                \n";
        cout << " | |_) | __ _ _ __ | | ___ _ __   __ _  | (___  _   _ ___| |_ ___ _ __ ___  \n";
        cout << " |  _ < / _` | '_ \\| |/ / | '_ \\ / _` |  \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ \n";
        cout << " | |_) | (_| | | | |   <| | | | | (_| |  ____) | |_| \\__ \\ ||  __/ | | | | |\n";
        cout << " |____/ \\__,_|_| |_|_|\\_\\_|_| |_|\\__, | |_____/ \\__, |___/\\__\\___|_| |_| |_|\n";
        cout << "                                  __/ |          __/ |                      \n";
        cout << "                                 |___/          |___/                       \n";
        cout << "\n---------------------------------------------------------------------------\n";
        cout << "\nChoose an option:\n";
        cout << "1. Login\n";
        cout << "2. Create a new user\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1, 2, or 3): ";

        int homeChoice = getInputAsInteger("");
        // Login screen
        if (homeChoice == 1)
        {
            cout << "\n---------------------------------------------------------------------------\n";
            cout << " _            _     \n";
            cout << "| |___ ___   |_|___ \n";
            cout << "| | . | . |  | |   |\n";
            cout << "|_|___|_  |  |_|_|_|\n";
            cout << "      |___|         \n\n";

            cout << "Enter your account number: ";
            string acct;
            cin >> acct;

            cout << "Enter your password: ";
            string password;
            cin >> password;

            // Find user on list of registered users
            // Using the inputted login credentials.
            // If found, store it in a pointer
            User *currentUser = nullptr;
            for (User &user : users)
            {
                if (acct == user.accountNumber && password == user.password)
                {
                    currentUser = &user;
                    break;
                }
            }

            if (currentUser)
            {
                // User home page
                while (true)
                {
                    cout << "\n---------------------------------------------------------------------------\n";
                    cout << "                                  __ \n";
                    cout << " _ _ _     _                     |  |\n";
                    cout << "| | | |___| |___ ___ _____ ___   |  |\n";
                    cout << "| | | | -_| |  _| . |     | -_|  |__|\n";
                    cout << "|_____|___|_|___|___|_|_|_|___|  |__|\n\n";

                    cout << "Glad to have you back, " << currentUser->username << " (" << currentUser->accountNumber << ")\n";
                    cout << "Choose an option:\n";
                    cout << "0. Transfer Funds              5. Pay Loan Balance\n";
                    cout << "1. Check Account Balances      6. View " << currentUser->username << ""
                            << " informations\n";
                    cout << "2. Cash In                     7. View Transaction History\n";
                    cout << "3. Cash Out                    8. Log out\n";
                    cout << "4. Loan                        9. Terminate Account\n";
                    cout << "Enter your options (0-9): ";
                    string input;
                    int userChoice;
                    cin >> input;

                    try
                    {
                        userChoice = stoi(input);
                    }
                    catch (const invalid_argument &)
                    {
                        cout << "Input must be an integer between 1 to 9";
                        continue;
                    }

                    // Transfer funds
                    if (userChoice == 0)
                    {
                        if (currentUser->accountBalance <= 100)
                        {
                            cout << "Balance is too low to transfer funds";
                        }
                        else
                        {
                            cout << "\n---------------------------------------------------------------------------\n";
                            cout << " _____                 ___            _____           _     \n";
                            cout << "|_   _|___ ___ ___ ___|  _|___ ___   |   __|_ _ ___ _| |___ \n";
                            cout << "  | | |  _| .'|   |_ -|  _| -_|  _|  |   __| | |   | . |_ -|\n";
                            cout << "  |_| |_| |__,|_|_|___|_| |___|_|    |__|  |___|_|_|___|___|\n\n";

                            cout << "Enter the Account number of the recipient: ";
                            string recipientAccountNumber;
                            cin >> recipientAccountNumber;

                            User *recipientUser = nullptr;
                            for (User &user : users)
                            {
                                if (recipientAccountNumber == user.accountNumber)
                                {
                                    recipientUser = &user;
                                    break;
                                }
                            }

                            if (recipientUser)
                            {
                                cout << "Enter the amount to transfer: ";
                                string transferAmountInput;
                                cin >> transferAmountInput;

                                try
                                {
                                    double transferAmount = stoi(transferAmountInput);
                                    if (transferAmount < 100)
                                    {
                                        cout << "Transfer amount minimum of 100: ";
                                    }

                                    else
                                    {
                                        string confirmTransactionInput;
                                        cout << "Are you sure you want to continue the transaction (1 for YES, 0 for NO): ";
                                        cin >> confirmTransactionInput;

                                        try
                                        {
                                            int confirmTransaction = stoi(confirmTransactionInput);
                                            if (confirmTransaction == 1)
                                            {

                                                transferFunds(*currentUser, *recipientUser, transferAmount);
                                                writeUserData("user_data.csv", users);
                                                cout << "Transfer successful! Here is your receipt:\n";
                                                printReceipt(generateReceipt(*currentUser, userChoice, transferAmount, recipientUser));
                                            }
                                            else
                                            {
                                                cout << "Transfer funds cancelled";
                                            }
                                        }
                                        catch (const invalid_argument)
                                        {
                                            cout << "ERROR: INVALID INPUT";
                                        }
                                    }
                                }
                                catch (const invalid_argument)
                                {
                                }
                            }
                            else
                            {
                                cout << "Error:  Recipient not found.\n";
                            }
                        }
                    }

                    // Check account balance
                    else if (userChoice == 1)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " _____     _                 \n";
                        cout << "| __  |___| |___ ___ ___ ___ \n";
                        cout << "| __ -| .'| | .'|   |  _| -_|\n";
                        cout << "|_____|__,|_|__,|_|_|___|___|\n\n";

                        cout << "Account Balance: " << currentUser->accountBalance << "\n";

                        if (currentUser->loanBalance == 0)
                        {
                            cout << "Loan Balance: 0\n";
                        }
                        else
                        {
                            cout << "Loan Balance: " << fixed << setprecision(2) << currentUser->loanBalance << "\n";
                        }

                        returnToMainMenu();
                    }

                    // Cash in
                    else if (userChoice == 2)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " _____         _      _     \n";
                        cout << "|     |___ ___| |_   |_|___ \n";
                        cout << "|   --| .'|_ -|   |  | |   |\n";
                        cout << "|_____|__,|___|_|_|  |_|_|_|\n\n";
                        cout << "Account Balance: " << currentUser->accountBalance << endl;

                        int cashInAmount; // Declare the variable to store the cash in amount.

                        cout << "Enter the amount to cash in: ";
                        string cashInput;
                        cin >> cashInput;

                        try
                        {
                            cashInAmount = stoi(cashInput); // Convert input to integer.

                            if (cashInAmount < 100)
                            {
                                cout << "\033[1mInvalid amount! Please enter a value equal to or greater than 100 Philippine pesos.\033[0m\n";
                                continue; // Continue to the next iteration of the loop if the amount is invalid.
                            }

                            // Call the performTransaction function after validating the input.
                            performTransaction(currentUser, userChoice, cashInAmount, users);
                        }
                        catch (const invalid_argument &)
                        {
                            cout << "Input must be an integer.\n";
                            continue; // Continue to the next iteration of the loop if the input is not an integer.
                        }
                    }

                    else if (userChoice == 3)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " _____         _              _   \n";
                        cout << "|     |___ ___| |_    ___ _ _| |_ \n";
                        cout << "|   --| .'|_ -|   |  | . | | |  _|\n";
                        cout << "|_____|__,|___|_|_|  |___|___|_|  \n";
                        cout << "Account Balance: " << currentUser->accountBalance << endl;
                        cout << "Enter the amount to cash out: ";

                        string cashOutInput;
                        cin >> cashOutInput;

                        try
                        {
                            int cashOutAmount = stoi(cashOutInput);

                            if (askUserToContinue())
                            {
                                cout << "Transaction continued";
                                if (cashOutAmount < 100)
                                {
                                    cout << "\033[1mInvalid amount! Please enter a value equal to or greater than 100 Philippine pesos.\033[0m\n";
                                }
                                else
                                {
                                    if (currentUser->accountBalance >= cashOutAmount)
                                    {
                                        currentUser->accountBalance -= cashOutAmount;
                                        writeUserData("user_data.csv", users);

                                        cout << "Cash out successful! Here is your receipt:\n";
                                        printReceipt(generateReceipt(*currentUser, userChoice, cashOutAmount));
                                    }
                                    else
                                    {
                                        cout << "Error: Insufficient balance for cash out.\n";
                                    }
                                }
                            }
                            else
                            {
                                cout << "Transaction Canceled";
                            }
                        }
                        catch (const invalid_argument &)
                        {
                            cout << "Input must be an integer.";
                            continue;
                        }
                    }

                    // Take out loan
                    else if (userChoice == 4)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " __                \n";
                        cout << "|  |   ___ ___ ___ \n";
                        cout << "|  |__| . | .'|   |\n";
                        cout << "|_____|___|__,|_|_|\n";
                        cout << "Enter the loan amount: ";

                        // Use string to read user input as it's more appropriate for handling non-integer input
                        string loanAmountInput;
                        cin >> loanAmountInput;

                        try
                        {
                            int loanAmount = stoi(loanAmountInput);

                            if (loanAmount >= 1000 && loanAmount <= 500000)
                            {
                                // Ask the user for the loan term (in months)
                                cout << "Enter the loan term (in months): ";
                                string LoanTermAmount;
                                cin >> LoanTermAmount;

                                try
                                {
                                    int loanTerm = stoi(LoanTermAmount);
                                    double totalRepayment = calculateTotalRepayment(loanAmount, loanTerm);

                                    cout << "Total amount to be repaid (including interest 5%): " << totalRepayment << endl;

                                    cout << "Do you want to proceed with the loan? (1 for yes, 0 for no): ";
                                    string proceedLoanChoice;
                                    cin >> proceedLoanChoice;
                                    try
                                    {
                                        int proceedLoan = stoi(proceedLoanChoice);
                                        if (proceedLoan == 1)
                                        {
                                            currentUser->loanBalance += totalRepayment;

                                            writeUserData("user_data.csv", users);

                                            cout << "Loan of " << loanAmount << " taken successfully! Here is your receipt:\n";
                                            printReceipt(generateReceipt(*currentUser, userChoice, loanAmount));
                                        }
                                        else
                                        {
                                            cout << "Loan request canceled.\n";
                                        }
                                    }
                                    catch (const std::invalid_argument)
                                    {
                                        cout << "Error: Invalid input for loan Term. Please enter a valid number.\n";
                                    }
                                }
                                catch (const std::invalid_argument)
                                {
                                    cout << "Error: Invalid input for loan Term. Please enter a valid number.\n";
                                }
                            }
                            else
                            {
                                cout << "Error: Invalid loan amount (Min 1000 || Max 500000).\n";
                            }
                        }
                        catch (const std::invalid_argument &e)
                        {
                            cout << "Error: Invalid input for loan amount. Please enter a valid number.\n";
                        }
                    }

                    // Pay loan balance
                    else if (userChoice == 5)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " _____            __                \n";
                        cout << "|  _  |___ _ _   |  |   ___ ___ ___ \n";
                        cout << "|   __| .'| | |  |  |__| . | .'|   |\n";
                        cout << "|__|  |__,|_  |  |_____|___|__,|_|_|\n";
                        cout << "          |___|                     \n";
                        cout << "Account Balance: " << currentUser->accountBalance << endl;
                        cout << "Loan Balance: " << currentUser->loanBalance << "\n"; // Display the current loan balance

                        cout << "Enter the amount to pay towards the loan balance: ";
                        string paymentAmountInput;
                        cin >> paymentAmountInput;
                        try
                        {
                            double paymentAmount = stoi(paymentAmountInput);
                            string loanChoiceInput;
                            cout << "Do you want to continue the transaction (1 if YES, 0 if NO): ";
                            cin >> loanChoiceInput;

                            try
                            {
                                int loanChoice = stoi(loanChoiceInput);

                                if (loanChoice == 1)
                                {
                                    if (paymentAmount > 0 && paymentAmount <= currentUser->loanBalance)
                                    {
                                        // Check if the user has enough funds to make the payment
                                        if (paymentAmount <= currentUser->accountBalance)
                                        {
                                            currentUser->accountBalance -= paymentAmount;
                                            currentUser->loanBalance -= paymentAmount;

                                            writeUserData("user_data.csv", users);

                                            cout << "Remaining Loan Balance: " << currentUser->loanBalance << "\n";

                                            cout << "Loan payment of " << paymentAmount << " successful! Here is your receipt:\n";
                                            printReceipt(generateReceipt(*currentUser, userChoice, paymentAmount));
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
                                else if (loanChoice == 0)
                                {
                                    cout << "Transaction canceled";
                                }
                                else
                                {
                                    cout << "Invalid input";
                                }
                            }
                            catch (const invalid_argument)
                            {
                                cout << "Error: Invalid user input";
                            }
                        }
                        catch (const std::invalid_argument)
                        {
                            cout << "Error: Invalid user input";
                        }
                    }

                    // View account information
                    else if (userChoice == 6)
                    {
                        cout << "\n-------------------------------------------------------------------------------------------------------------------------------\n";
                        cout << " _____ _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ \n";
                        cout << "|  _  |     |     |     |  |  |   | |_   _|  |     |   | |   __|     | __  |     |  _  |_   _|     |     |   | |   __|\n";
                        cout << "|     |   --|   --|  |  |  |  | | | | | |    |-   -| | | |   __|  |  |    -| | | |     | | | |-   -|  |  | | | |__   |\n";
                        cout << "|__|__|_____|_____|_____|_____|_|___| |_|    |_____|_|___|__|  |_____|__|__|_|_|_|__|__| |_| |_____|_____|_|___|_____|\n";
                        cout << "\nUsername: " << currentUser->username << endl;
                        cout << "Password: " << currentUser->password << endl;
                        cout << "Email: " << currentUser->email << endl;
                        cout << "Role: " << currentUser->role << endl;
                        cout << "Account number: " << currentUser->accountNumber << endl;

                        returnToMainMenu();
                    }

                    // View transaction history
                    else if (userChoice == 7)
                    {
                        string receiptChoiceInput;
                        int receiptChoice;
                        vector<string> receiptList = readTransactionHistory(*currentUser);

                        while (true)
                        {
                            if (printTransactionLog(receiptList))
                            {
                                cout << "Choose receipt to view (Enter 0 to exit): ";
                                receiptChoice = getInputAsInteger(receiptChoiceInput);

                                if (receiptChoice > 0 && receiptChoice < receiptList.size() + 1)
                                {
                                    printReceipt(receiptList[receiptChoice - 1]);
                                }
                                else if (receiptChoice == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    cout << "Please input a number between 1 and " << receiptList.size() << endl;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                    // Log out
                    else if (userChoice == 8)
                    {
                        cout << "Logging out...\n";
                        break;
                    }

                    // Option to delete account
                    else if (userChoice == 9)
                    {
                        cout << "\n---------------------------------------------------------------------------\n";
                        cout << " ____      _     _                                  _   \n";
                        cout << "|    \\ ___| |___| |_ ___    ___ ___ ___ ___ _ _ ___| |_ \n";
                        cout << "|  |  | -_| | -_|  _| -_|  | .'|  _|  _| . | | |   |  _|\n";
                        cout << "|____/|___|_|___|_| |___|  |__,|___|___|___|___|_|_|_|  \n";

                        cout << "Are you sure you want to delete your account? (1 for yes, 0 for no): ";
                        string confirmDeleteInput;
                        cin >> confirmDeleteInput;

                        try
                        {
                            int confirmDelete = stoi(confirmDeleteInput);
                            if (currentUser->accountBalance < 100)
                            {
                                if (confirmDelete == 1)
                                {
                                    string confirmPass;
                                    cout << "Please enter your password to confirm the deletion: ";
                                    cin >> confirmPass;

                                    if (confirmPass == currentUser->password)
                                    {
                                        if (!filesystem::exists(transactionLogDir))
                                        {
                                            filesystem::create_directory(transactionLogDir);
                                        }
                                        filesystem::current_path(transactionLogDir);

                                        // Delete transaction history
                                        try
                                        {
                                            filesystem::remove(currentUser->accountNumber + ".csv");
                                        }
                                        catch (const filesystem::filesystem_error &err)
                                        {
                                            ; // Do nothing
                                        }
                                        filesystem::current_path(parentDir);

                                        deleteUser(users, currentUser->username);
                                        writeUserData("user_data.csv", users);
                                        cout << "Account deleted. Logging out...\n";
                                        break;
                                    }
                                    else
                                    {
                                        cout << "Wrong password, Deletion canceled";
                                    }
                                }
                                else
                                {
                                    cout << "Account deletion canceled.\n";
                                }
                            }
                            else
                            {
                                cout << "Error: Unable to delete user. Ensure the account balance is less than 100.\n";
                            }
                        }
                        catch (const invalid_argument)
                        {
                            cout << "ERROR: INVALID INPUT";
                        }
                    }
                }
            }
            else
            {
                cout << "Error: User not found or incorrect credentials.\n";
            }
        } // End of login screen

        // Account creation page
        else if (homeChoice == 2)
        {
            cout << "\n---------------------------------------------------------------------------\n";
            cout << " _____             _                                  _   \n";
            cout << "|     |___ ___ ___| |_ ___    ___ ___ ___ ___ _ _ ___| |_ \n";
            cout << "|   --|  _| -_| .'|  _| -_|  | .'|  _|  _| . | | |   |  _|\n";
            cout << "|_____|_| |___|__,|_| |___|  |__,|___|___|___|___|_|_|_|  \n";
            createUser(users);
            writeUserData("user_data.csv", users);
        }

        // Program exit
        else if (homeChoice == 3)
        {
            cout << "Exiting the program...\n";
            break;
        }
    }

    return 0;
}

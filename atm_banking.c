#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100 // Maximum number of users in the database

// Function declarations
void withdrawal(float *balance);
void deposit(float *balance);
void check_balance(float balance);
int authenticate_admin();
void read_account_details(float *balance, int *pin, char *username);
void write_account_details(float balance, int pin, char *username);
void view_all_users();
void modify_user_balance(char *username);
void modify_user_pin(char *username);
void add_user();
void delete_user(char *username);

int main() {
    int pin, entered_pin, count = 0, amount = 1, continue_transaction = 1;
    float balance;
    char username[30];
	int option;

    // Read account details from file
    read_account_details(&balance, &pin, username);

    time_t now;
    time(&now);

    printf("\n");
    printf("\t\t\t\t\t       %s", ctime(&now)); // Current time and date at the top
    printf("\n\t\t\t******************WELCOME TO THE ATM*******************");

    // PIN Verification
    while (pin != entered_pin) {
        printf("\nPlease enter your pin: ");
        scanf("%d", &entered_pin);

        if (entered_pin != pin) {
            Beep(500, 450);
            printf("Invalid pin!!!\n");
        }

        count++;
        if (count == 3 && pin != entered_pin) {
            printf("You have entered the wrong PIN 3 times. Exiting.\n");
            exit(0);
        }
    }

    // Main transaction loop
    while (continue_transaction != 0) {
        printf("\n\t\t\t*************Available Transactions************");
        printf("\n\t\t1. Withdrawal");
        printf("\n\t\t2. Deposit");
        printf("\n\t\t3. Check Balance");
        printf("\n\t\t4. Admin Access");
        printf("\n\t\t5. Exit");
        printf("\n\n\tPlease select an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                withdrawal(&balance);
                break;

            case 2:
                deposit(&balance);
                break;

            case 3:
                check_balance(balance);
                break;

            case 4:
                if (authenticate_admin()) {
                    printf("\nAdmin access granted.\n");
                    int admin_option;
                    do {
                        printf("\n\t\tAdmin Menu");
                        printf("\n\t\t1. View all users");
                        printf("\n\t\t2. Modify user balance");
                        printf("\n\t\t3. Modify user PIN");
                        printf("\n\t\t4. Add a new user");
                        printf("\n\t\t5. Delete a user");
                        printf("\n\t\t6. Exit admin menu");
                        printf("\n\n\tPlease select an option: ");
                        scanf("%d", &admin_option);

                        switch (admin_option) {
                            case 1:
                                view_all_users();
                                break;
                            case 2:
                                {
                                    char modify_username[30];
                                    printf("\nEnter username to modify balance: ");
                                    scanf("%s", modify_username);
                                    modify_user_balance(modify_username);
                                }
                                break;
                            case 3:
                                {
                                    char modify_username[30];
                                    printf("\nEnter username to modify PIN: ");
                                    scanf("%s", modify_username);
                                    modify_user_pin(modify_username);
                                }
                                break;
                            case 4:
                                add_user();
                                break;
                            case 5:
                                {
                                    char delete_username[30];
                                    printf("\nEnter username to delete: ");
                                    scanf("%s", delete_username);
                                    delete_user(delete_username);
                                }
                                break;
                            case 6:
                                printf("\nExiting admin menu.\n");
                                break;
                            default:
                                printf("\nInvalid option. Try again.\n");
                        }
                    } while (admin_option != 6);
                } else {
                    printf("\nAdmin authentication failed.\n");
                }
                break;

            case 5:
                printf("\n\tThank you for using the ATM. Goodbye!\n");
                write_account_details(balance, pin, username); // Save changes before exit
                exit(0);

            default:
                Beep(500, 450);
                printf("\n\t\tInvalid Option!!! Please select a valid transaction.\n");
        }

        printf("\n\t\tDo you wish to perform another transaction? Press 1[Yes], 0[No]: ");
        scanf("%d", &continue_transaction);

        if (continue_transaction == 0) {
            printf("\n\t\tThank you for banking with us. Have a great day!\n");
            write_account_details(balance, pin, username); // Save changes before exit
        }
    }

    return 0;
}

// Function for withdrawal
void withdrawal(float *balance) {
    int amount;
    printf("\n\t\tEnter the amount to withdraw: ");
    scanf("%d", &amount);

    if (*balance < amount) {
        printf("\n\tSorry, insufficient balance.\n");
    } else {
        *balance -= amount;
        printf("\n\t\tYou have withdrawn Rs.%d. Your new balance is Rs.%.2f\n", amount, *balance);
    }
}

// Function for deposit
void deposit(float *balance) {
    int amount;
    printf("\n\t\tEnter the amount to deposit: ");
    scanf("%d", &amount);

    *balance += amount;
    printf("\n\t\tYou have deposited Rs.%d. Your new balance is Rs.%.2f\n", amount, *balance);
}

// Function to check balance
void check_balance(float balance) {
    printf("\n\t\tYour balance is Rs.%.2f\n", balance);
}

// Function to authenticate admin access
int authenticate_admin() {
    int admin_pin = 1234, entered_admin_pin;
    printf("\n\t\tEnter Admin PIN: ");
    scanf("%d", &entered_admin_pin);

    if (entered_admin_pin == admin_pin) {
        return 1;  // Admin authenticated
    } else {
        return 0;  // Admin authentication failed
    }
}

// Function to read account details from file
void read_account_details(float *balance, int *pin, char *username) {
    FILE *file = fopen("account_details.txt", "r");

    if (file == NULL) {
        printf("\nUnable to read account details. Creating a new account.\n");
        *pin = 9876; // Default pin
        *balance = 1000.0; // Default balance
        strcpy(username, "user1"); // Default username
        write_account_details(*balance, *pin, username); // Write the details to the file
    } else {
        fscanf(file, "%d %f %s", pin, balance, username);
        fclose(file);
    }
}

// Function to write account details to file
void write_account_details(float balance, int pin, char *username) {
    FILE *file = fopen("account_details.txt", "w");

    if (file != NULL) {
        fprintf(file, "%d %.2f %s", pin, balance, username); // Save pin, balance, and username
        fclose(file);
    } else {
        printf("\nError writing to account details file.\n");
    }
}

// Admin Functions

// Function to view all users (showing stored users from the file)
void view_all_users() {
    FILE *file = fopen("users.txt", "r");
    char username[30];
    int pin;
    float balance;

    if (file == NULL) {
        printf("\nNo users found.\n");
        return;
    }

    printf("\n\t\tAll Users:\n");
    printf("\n\t\tUsername\tPIN\tBalance\n");

    while (fscanf(file, "%s %d %f", username, &pin, &balance) != EOF) {
        printf("\n\t\t%s\t%d\t%.2f", username, pin, balance);
    }

    fclose(file);
}

// Function to modify a user's balance
void modify_user_balance(char *username) {
    FILE *file = fopen("users.txt", "r+");
    char file_username[30];
    int pin;
    float balance;
    int found = 0;

    while (fscanf(file, "%s %d %f", file_username, &pin, &balance) != EOF) {
        if (strcmp(file_username, username) == 0) {
            printf("\n\tEnter new balance for %s: ", username);
            scanf("%f", &balance);
            found = 1;
            break;
        }
    }

    if (found) {
        fseek(file, -sizeof(float), SEEK_CUR);
        fprintf(file, "%.2f", balance);
        printf("\n\tBalance updated successfully.\n");
    } else {
        printf("\n\tUser not found.\n");
    }

    fclose(file);
}

// Function to modify a user's PIN
void modify_user_pin(char *username) {
    FILE *file = fopen("users.txt", "r+");
    char file_username[30];
    int pin;
    float balance;
    int found = 0;

    while (fscanf(file, "%s %d %f", file_username, &pin, &balance) != EOF) {
        if (strcmp(file_username, username) == 0) {
            printf("\n\tEnter new PIN for %s: ", username);
            scanf("%d", &pin);
            found = 1;
            break;
        }
    }

    if (found) {
        fseek(file, -sizeof(int), SEEK_CUR);
        fprintf(file, "%d", pin);
        printf("\n\tPIN updated successfully.\n");
    } else {
        printf("\n\tUser not found.\n");
    }

    fclose(file);
}

// Function to add a new user
void add_user() {
    FILE *file = fopen("users.txt", "a");
    char username[30];
    int pin;
    float balance;

    if (file == NULL) {
        printf("\nError opening user file.\n");
        return;
    }

    printf("\nEnter new username: ");
    scanf("%s", username);
    printf("\nEnter initial PIN: ");
    scanf("%d", &pin);
    printf("\nEnter initial balance: ");
    scanf("%f", &balance);

    fprintf(file, "%s %d %.2f\n", username, pin, balance);
    printf("\nNew user added successfully.\n");

    fclose(file);
}

// Function to delete a user
void delete_user(char *username) {
    FILE *file = fopen("users.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char file_username[30];
    int pin;
    float balance;
    int found = 0;

    if (file == NULL || temp_file == NULL) {
        printf("\nError opening files.\n");
        return;
    }

    while (fscanf(file, "%s %d %f", file_username, &pin, &balance) != EOF) {
        if (strcmp(file_username, username) != 0) {
            fprintf(temp_file, "%s %d %.2f\n", file_username, pin, balance);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("\nUser %s deleted successfully.\n", username);
    } else {
        printf("\nUser not found.\n");
        remove("temp.txt");
    }
}

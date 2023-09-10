#include <stdio.h>
#include <string.h>
int isLoggedIn = 0; // global variable

struct Credential
{
    char phNo[11];
    char password[30];
};

struct Profile
{
    char name[25];
    char phNo[11];
    char userId[10];
    char address[20];
};

struct Order
{
    char orderId[10];
    char userId[10];
    int qty;
    char pid[10];
    double amount;
};

int validateAuth(const char *filename, const struct Credential *targetCredential)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1; // Return an error code
    }

    struct Credential currentCredential;
    int found = 0;

    while (fread(&currentCredential, sizeof(struct Credential), 1, file))
    {
        if (strcmp(currentCredential.phNo, targetCredential->phNo) == 0 &&
            strcmp(currentCredential.password, targetCredential->password) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(file);

    return found;
}

void displayMainMenu()
{
}

/**
 * The login function prompts the user to enter their phone number and password, validates the
 * credentials against a database, and either logs the user in or displays an error message.
 */
void login()
{
    char phNo[11];
    char password[11];
    printf("Enter phone number\n");
    scanf("%s", phNo);
    printf("Enter password\n");
    scanf("%s", password);
    struct Credential current;
    strcpy(current.phNo, phNo);
    strcpy(current.password, password);
    const char *filename = "cred_db.txt";
    if (validateAuth(filename, &current) == 1)
    {
        isLoggedIn = 1;
        printf("You are logged in !!\n");
        displayMainMenu();
    }
    else
    {
        // not validated
        printf("You are not logged in !!\n");
    }
}

/**
 * The function `registerUser` takes a filename and a new credential as input, opens the file in append
 * binary mode, writes the new credential to the file, and returns a success code if the operation is
 * successful.
 *
 * @param filename The filename parameter is a string that represents the name of the file where the
 * new user credential will be registered.
 * @param newCredential The newCredential parameter is a pointer to a struct Credential object. This
 * struct likely contains information about a user's credentials, such as a username and password.
 *
 * @return an integer value. If the file is successfully opened and the newCredential is successfully
 * written to the file, the function will return 1, indicating success. If there is an error opening
 * the file or writing to the file, the function will return 0, indicating failure.
 */
int registerUser(const char *filename, const struct Credential *newCredential)
{
    FILE *file = fopen(filename, "ab"); // Open the file in append binary mode

    if (file == NULL)
    {
        perror("Error opening file");
        return 0; // Return failure code
    }

    // Write the newCredential to the file
    if (fwrite(newCredential, sizeof(struct Credential), 1, file) != 1)
    {
        perror("Error writing to file");
        fclose(file);
        return 0; // Return failure code
    }

    fclose(file);
    return 1; // Return success code
}

int main()
{

    printf("Welcome to All Basket\n\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Quit\n");
    int authChoice;
    scanf("%d", &authChoice);
    if (authChoice == 1)
    {
        login();
    }
    else if (authChoice == 2)
    {
        struct Credential newCredential;
        strcpy(newCredential.phNo, "3");
        strcpy(newCredential.password, "new_password");
        const char *filename = "cred_db.txt";
        if (registerUser(filename, &newCredential))
            printf("Credential successfully written to the file.\n");
        else
            printf("Error occurred while writing the credential to the file.\n");
    }
    else
    {
        return 0;
    }
    return 0;
}
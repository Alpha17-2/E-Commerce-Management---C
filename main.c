#include <stdio.h>
#include <string.h>

struct Credential
{
    char phNo[11];
    char password[30];
};

struct Profile
{
    char name[25];
    char phNo[11];
    char address[20];
};

struct Product
{
    int pid;
    char pname[25];
    int price;
};

struct Order
{
    char orderId[10];
    char phNo[11];
    int qty;
    char pid[10];
    double amount;
};
int isLoggedIn = 0; // global variable

void addToCategory()
{

    printf("Insert product id , product name and price:\n");
    int pid;
    char pname[25];
    int price;
    scanf("%d %s %d", &pid, pname, &price);
    struct Product product;
    product.pid = pid;
    strcpy(product.pname, pname);
    product.price = price;
    FILE *file = fopen("products.txt", "ab"); // Open the file in append binary mode

    if (file == NULL)
    {
        perror("Error opening file");
    }

    if (fwrite(&product, sizeof(struct Product), 1, file) != 1)
    {
        perror("Error writing to file");
        fclose(file);
    }
}

void showOrders(char *phNo)
{
}

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

int registerUser(const char *filename, const struct Credential *newCredential, struct Profile *profile)
{
    FILE *file = fopen(filename, "ab"); // Open the file in append binary mode
    FILE *profileFile = fopen("profile_db.txt", "ab");
    if (file == NULL)
    {
        perror("Error opening file");
        return 0; // Return failure code
    }

    if (fwrite(profile, sizeof(struct Profile), 1, profileFile) != 1)
    {
        perror("Error writing to file");
        fclose(file);
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

/* The login function prompts the user to enter their phone number and password, validates the
 credentials against a database, and either logs the user in or displays an error message.
*/

void shop(char *phNo)
{
    printf("Shop using categories\n\n");
    printf("1. Mobiles\n");
    printf("2. Laptops\n");
    printf("3. Television\n");
    printf("4. Headphones\n");
    int shopChoice;
    printf("Enter your choice\n");
    scanf("%d", &shopChoice);
    switch (shopChoice)
    {
    case 1:

        break;

    default:
        break;
    }
}

void displayProducts()
{
    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
       
    }

    struct Product product;
    int found = 0;
    printf("Products\n\n");
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        printf("Name %s\n", product.pname);
        printf("Id %d\n", product.pid);
        printf("Price %d\n", product.price);
        printf("\n");
    }
    

    fclose(file);
}

void displayProfile(char *phNo)
{

    FILE *file = fopen("profile_db.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
       
    }

    struct Profile profile;
    while (fread(&profile, sizeof(struct Profile), 1, file))
    {
        if (strcmp(profile.phNo, phNo) == 0)
        {
            printf("Name : %s\n", profile.name);
            printf("Phone : %s\n", profile.phNo);
            break;
        }
    }
}

void displayMainMenu(char *phNo)
{
    printf("Main menu\n\n");
    printf("1. View Profile\n");
    printf("2. My Orders\n");
    printf("3. Add to category\n");
    printf("4. Display products\n");
    printf("5. Logout\n");
    int menuChoice;
    scanf("%d", &menuChoice);
    switch (menuChoice)
    {
    case 1:
        displayProfile(phNo);
        break;
    case 2:
        showOrders(phNo);
        break;
    case 3:
        addToCategory();
        break;
    case 4:
        displayProducts();
        break;
    default:
        break;
    }
}

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
        printf("You are logged in !!\n");
        displayMainMenu(phNo);
    }
    else
    {
        // not validated
        printf("You are not logged in !!\n");
    }
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
        struct Profile profile;
        char phNo[11];
        char password[11];
        char name[25];
        char address[20];
        printf("Enter Name\n");
        scanf("%s", name);
        printf("Enter address\n");
        scanf("%s", address);
        printf("Enter phone number\n");
        scanf("%s", phNo);
        printf("Enter password\n");
        scanf("%s", password);
        strcpy(newCredential.phNo, phNo);
        strcpy(newCredential.password, password);
        strcpy(profile.address, address);
        strcpy(profile.name, name);
        strcpy(profile.phNo, phNo);
        const char *filename = "cred_db.txt";
        if (registerUser(filename, &newCredential, &profile))
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
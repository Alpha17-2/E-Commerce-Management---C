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

int total = 0;

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

// Return 1 if found else 0
int addItemsIfFound(int pid, int q)
{
    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
    }

    struct Product product;
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (product.pid == pid)
        {
            total += (q * product.price);
            return 1;
        }
    }
    return 0;
}

struct Product fetechProduct(int pid)
{
    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
    }

    struct Product product;
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (product.pid == pid)
        {
            return product;
        }
    }
}

void addToCart()
{
    struct Product cart[20];
    int numberOfItems = 0;
    int quantity[20];
    char choice = 1;
    while (choice == 1)
    {
        printf("Enter id of the product and quantity\n");
        int id, q;
        scanf("%d %d", &id, &q);

        if (addItemsIfFound(id, q) == 1)
        {
            printf("Item successfully added to your cart\n");
            cart[numberOfItems] = fetechProduct(id);
            quantity[numberOfItems] = q;
            numberOfItems++;
        }
        else
        {
            printf("No such item found\n");
        }
        printf("\nDo you want to shop more ? Press 1 to continue or 0 to quit. \n");
        scanf("%d", &choice);
    }

    if (total > 0)
    {
        printf("\nYour Cart : \n");
        printf("\nPID\tPName\tPrice\tQty\tAmount\n");
        for (int i = 0; i < numberOfItems; i++)
        {
            printf("%d\t%s\t%d\t%d\t%d\n", cart[i].pid, cart[i].pname, cart[i].price, quantity[i], (quantity[i] * cart[i].price));
        }
        printf("\nTotal : %d\n", total);
        printf("\nPress 1 to checkout or 0 to cancel\n");
        int checkout;
        scanf("%d", &checkout);
        if (checkout == 1)
        {
            struct Order order;

            printf("Thank you for shopping !\n");
        }
        else
        {
            printf("Shop again !");
        }
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
    printf("\n******Products******\n\n");
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        printf("Name : %s\n", product.pname);
        printf("Id:  %d\n", product.pid);
        printf("Price : %d\n", product.price);
        printf("\n");
    }
    addToCart();

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
    printf("\nMain menu\n\n");
    printf("1. View Profile\n");
    printf("2. Display Products\n");
    printf("3. Logout\n");
    int menuChoice;
    scanf("%d", &menuChoice);
    switch (menuChoice)
    {
    case 1:
        displayProfile(phNo);
        break;
    case 2:
        displayProducts();
        break;
    default:
        break;
    }
}

// Function to search for a product by its ID
void searchProductByID()
{
    int pid;
    printf("Enter the Product ID to search for: ");
    scanf("%d", &pid);

    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;
    int found = 0;

    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (product.pid == pid)
        {
            found = 1;
            printf("Product found:\n");
            printf("ID: %d\n", product.pid);
            printf("Name: %s\n", product.pname);
            printf("Price: %d\n", product.price);
            break;
        }
    }

    if (!found)
    {
        printf("Product with ID %d not found.\n", pid);
    }

    fclose(file);
}

// Function to search for a product by its name
void searchProductByName()
{
    char pname[25];
    printf("Enter the Product name to search for: ");
    scanf("%s", pname);

    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;
    int found = 0;

    printf("Products found with name '%s':\n", pname);
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (strcmp(product.pname, pname) == 0)
        {
            found = 1;
            printf("ID: %d\n", product.pid);
            printf("Name: %s\n", product.pname);
            printf("Price: %d\n", product.price);
            printf("\n");
        }
    }

    if (!found)
    {
        printf("No products found with the name '%s'.\n", pname);
    }

    fclose(file);
}

void editProduct()
{
    int pid;
    printf("Enter the Product ID to edit: ");
    scanf("%d", &pid);

    FILE *file = fopen("products.txt", "rb+");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct Product product;
    int found = 0;

    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (product.pid == pid)
        {
            found = 1;

            // Modify the product details
            printf("Enter the new name for the product: ");
            scanf("%s", product.pname);
            printf("Enter the new price for the product: ");
            scanf("%d", &product.price);

            // Seek to the position of the product in the file
            fseek(file, -sizeof(struct Product), SEEK_CUR);

            // Write the updated product back to the file
            if (fwrite(&product, sizeof(struct Product), 1, file) != 1)
            {
                perror("Error writing to file");
            }

            printf("Product with ID %d has been updated.\n", pid);
            break;
        }
    }

    if (!found)
    {
        printf("Product with ID %d not found.\n", pid);
    }

    fclose(file);
}

// Function to delete a product by its ID
void deleteProduct()
{
    int pid;
    printf("Enter the Product ID to delete: ");
    scanf("%d", &pid);

    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL)
    {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    struct Product product;
    int found = 0;

    while (fread(&product, sizeof(struct Product), 1, file))
    {
        if (product.pid == pid)
        {
            found = 1;
            continue; // Skip the product to be deleted
        }

        // Write other products to the temporary file
        if (fwrite(&product, sizeof(struct Product), 1, tempFile) != 1)
        {
            perror("Error writing to temporary file");
            fclose(file);
            fclose(tempFile);
            return;
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file
    if (remove("products.txt") != 0)
    {
        perror("Error deleting original file");
        return;
    }

    if (rename("temp.txt", "products.txt") != 0)
    {
        perror("Error renaming temporary file");
        return;
    }

    if (found)
    {
        printf("Product with ID %d has been deleted.\n", pid);
    }
    else
    {
        printf("Product with ID %d not found.\n", pid);
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
        printf("\nYou are logged in !!\n");
        displayMainMenu(phNo);
    }
    else
    {
        printf("\nWrong credentials! Please try again.\n\n");
    }
}
void displayProductsAsAdmin()
{
    FILE *file = fopen("products.txt", "rb");
    if (file == NULL)
    {
        perror("Error opening file");
    }

    struct Product product;
    int found = 0;
    printf("\n******Products******\n\n");
    while (fread(&product, sizeof(struct Product), 1, file))
    {
        printf("Name : %s\n", product.pname);
        printf("Id:  %d\n", product.pid);
        printf("Price : %d\n", product.price);
        printf("\n");
    }
}

void loginAsAdmin()
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
    const char *filename = "cred_db_admin.txt";
    if (validateAuth(filename, &current) == 1)
    {
        printf("\nYou are logged in !!\n\n");
        int choice = 0;
        while (choice != 7)
        {
            printf("Select an admin task\n");
            printf("1. Add Product\n");
            printf("2. Edit Product\n");
            printf("3. View All Products\n");
            printf("4. Remove Product\n");
            printf("5. Search Product by ID\n");
            printf("6. Search Product by Name\n");
            printf("7. Exit\n");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                addToCategory();
                break;
            case 2:
                editProduct();
                break;
            case 3:
                displayProductsAsAdmin();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                searchProductByID();
                break;
            case 6:
                searchProductByName();
                break;
            case 7:
                printf("Exiting admin menu.\n");
                break;
            }
        }
    }
    else
    {
        printf("\nWrong credentials! Please try again.\n\n");
    }
}

void displayUserMenu()
{
    printf("*******************************\n");
    int authChoice;
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Quit\n");

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
        return;
    }
}

void displayAdminMenu()
{

    printf("*******************************\n");
    int authChoice;
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Quit\n");

    scanf("%d", &authChoice);
    if (authChoice == 1)
    {
        loginAsAdmin();
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
        const char *filename = "cred_db_admin.txt";
        if (registerUser(filename, &newCredential, &profile))
            printf("Credential successfully written to the file.\n");
        else
            printf("Error occurred while writing the credential to the file.\n");
    }
    else
    {
        return;
    }
}

int main()
{
    int exitChoice = 1;
    int userChoice, authChoice;
    while (exitChoice != 0)
    {
        printf("\nWelcome to All Basket\n\n");
        printf("1. User\n");
        printf("2. Admin\n");
        scanf("%d", &userChoice);
        if (userChoice == 1)
        {
            displayUserMenu();
        }
        else
        {
            displayAdminMenu();
        }
        printf("Enter 0 to exit or any key to continue\n");
        scanf("%d", &exitChoice);
    }
    return 0;
}

/*

#include <stdio.h>
#include <string.h>

// ... (Existing code with structures and functions)

// Function to edit a product by its ID


// ... (Rest of your code with displayProductsAsAdmin() function)

int main()
{
    // ... (Rest of your main function)

    int choice = 0;
    while (choice != 5)
    {
        printf("Select an admin task\n");
        printf("1. Add Product\n");
        printf("2. Edit Product\n");
        printf("3. View All Products\n");
        printf("4. Remove Product\n");
        printf("5. Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addToCategory();
            break;
        case 2:
            editProduct();
            break;
        case 3:
            displayProductsAsAdmin();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            printf("Exiting admin menu.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}



*/
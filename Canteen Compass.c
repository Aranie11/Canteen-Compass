#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_ITEMS 50
#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define ITEM_NAME_LEN 100

struct User{
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    int isSeller;
};

struct FoodItem {
    char name[ITEM_NAME_LEN];
    float price;
};

struct Order {
    char buyer[USERNAME_LEN];
    struct FoodItem items[MAX_ITEMS];
    int itemCount;
    float total;
    int isPaid;
};

void saveUsersToFile();
void loadUsersFromFile();
void saveOrderToFile(struct Order *order);
void loadOrdersFromFile();
void registerUser();
int authenticate(char *username, char *password);
void addFoodItem();
void deleteFoodItem();
void viewOrders();
void viewMenu();
void placeOrder(char *buyer);
void payForOrder(struct Order *order);
void sellerMenu();
void buyerMenu(char *username);

struct User users[MAX_USERS];
struct FoodItem menu[MAX_ITEMS];
struct Order orders[MAX_USERS];

int userCount = 0;
int menuCount = 0;
int orderCount = 0;

char *usersFile = "users.txt";
char *ordersFile = "orders.txt";

void saveUsersToFile() {
    FILE *fp = fopen(usersFile, "w");
    if (fp == NULL) {
        perror("Error saving users");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(fp,"Username\tPassword\tSeller or Buyer(1 for Seller 0 for Buyer)\n");
        fprintf(fp, "%s\t\t%s\t\t%d\n", users[i].username, users[i].password, users[i].isSeller);
    }
    fclose(fp);
}

void loadUsersFromFile() {
    FILE *fp = fopen(usersFile, "r");
    if (fp == NULL) {

        return;
    }
    userCount = 0;
    while (fscanf(fp, "%s %s %d", users[userCount].username, users[userCount].password, &users[userCount].isSeller) != -1) {
        userCount++;
    }
    fclose(fp);
    printf("Users loaded from file successfully.\n\n");
}

void saveOrderToFile(struct Order *order) {
    FILE *fp = fopen(ordersFile, "a");
    if (fp == NULL) {
            printf("Error of file");
        return;
    }
    fprintf(fp,"-----------Orders-----------\n\n");
    fprintf(fp, "Buyer Name: %s\n", (*order).buyer);
    fprintf(fp, "Payment Status: %s\n", ((*order).isPaid==1)? "Paid" : "Pending");
    fprintf(fp,"\tItem\tQuantity\tUnit Price\n");

    for (int i = 0; i < (*order).itemCount; i++) {
    fprintf(fp, "\t%s\t%d\t%.2f\n", (*order).items[i].name,1,(*order).items[i].price);
    }
    fprintf(fp, "Total Order Amount: %.2f\n", (*order).total);
    fprintf(fp, "END\n");
    fclose(fp);
    printf("Order saved to file successfully.\n");
}

void loadOrdersFromFile() {
    FILE *fp = fopen(ordersFile, "r");
    if (fp == NULL) {

        return;
    }
    orderCount = 0;
    while (fscanf(fp, "%s %d %f %d", orders[orderCount].buyer, &orders[orderCount].itemCount,&orders[orderCount].total,
                  &orders[orderCount].isPaid)!= -1) {
        for (int i = 0; i < orders[orderCount].itemCount; i++) {
            fscanf(fp, "%s %f", orders[orderCount].items[i].name, &orders[orderCount].items[i].price);
        }
        char endMarker[10];
        fscanf(fp, "%s", endMarker);
        if (strcmp(endMarker, "END") == 0) {
            orderCount++;
        }
    }
    fclose(fp);
    printf("Orders loaded from file successfully.\n");
}
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot register more users.\n");
        return;
    }
    struct User newUser;
    printf("Enter username: ");
    scanf("%s", newUser.username);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Username already exists. Please choose another.\n");
            return;
        }
    }
    printf("Enter password: ");
    scanf("%s", newUser.password);

    printf("Are you a seller? (1 for Yes, 0 for No): ");
    scanf("%d", &newUser.isSeller);

    users[userCount++] = newUser;
    saveUsersToFile();
    printf("Registration successful!\n");
}
int authenticate(char *username, char *password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

void addFoodItem() {
    while (1) {
        if (menuCount >= MAX_ITEMS) {
            printf("Menu is full. Cannot add more items.\n");
            break;
        }

        struct FoodItem newItem;
        printf("Enter food item name (or type 'S' to stop adding items): ");
        getchar();
        scanf("%s", newItem.name);

        newItem.name[strcspn(newItem.name, "\n")] = '\0';

        if (strcmp(newItem.name, "s") == 0 || strcmp(newItem.name, "S") == 0) {
            printf("Exiting food item addition.\n");
            break;
        }

        printf("Enter price: ");
        scanf("%f", &newItem.price);

        menu[menuCount++] = newItem;
        printf("Food item '%s' added successfully at price RPS %.2f!\n", newItem.name, newItem.price);
    }
}

void deleteFoodItem() {
    char itemName[ITEM_NAME_LEN];
    printf("Enter food item name to delete: ");
    scanf("%s", itemName);

    for (int i = 0; i < menuCount; i++) {
        if (strcmp(menu[i].name, itemName) == 0) {
            for (int j = i; j < menuCount - 1; j++) {
                menu[j] = menu[j + 1];
            }
            menuCount--;
            printf("Food item deleted successfully!\n");
            return;
        }
    }
    printf("Food item not found.\n");
}

void viewOrders() {
    printf("\n--- Orders ---\n");
    for (int i = 0; i < orderCount; i++) {
        printf("Order by %s:\n", orders[i].buyer);
        for (int j = 0; j < orders[i].itemCount; j++) {
            printf("  %s - RPS %.2f\n", orders[i].items[j].name, orders[i].items[j].price);
        }
        printf("Total: RPS %.2f\n", orders[i].total);
        printf("Payment Status: %s\n", orders[i].isPaid ? "Paid" : "Pending");
        printf("----------------\n");
    }
}

void viewMenu() {
    printf("\n--- Menu ---\n");
    for (int i = 0; i < menuCount; i++) {
        printf("%d. %s - RPS %.2f\n", i + 1, menu[i].name, menu[i].price);
    }
    printf("-------------\n");
}

void placeOrder(char *buyer) {
    struct Order newOrder;
    strcpy(newOrder.buyer, buyer);
    newOrder.itemCount = 0;
    newOrder.total = 0.0;
    newOrder.isPaid = 0;

    printf("\n--- Place Your Order ---\n");

    while (1) {
        viewMenu();
        printf("Select item number to add to your order (or enter 0 to finish): ");
        int itemNumber;
        scanf("%d", &itemNumber);

        if (itemNumber == 0) {
            printf("Exiting item selection.\n");
            break;
        }

        if (itemNumber < 1 || itemNumber > menuCount) {
            printf("Invalid selection. Please try again.\n");
            continue;
        }

        newOrder.items[newOrder.itemCount++] = menu[itemNumber - 1];
        newOrder.total += menu[itemNumber - 1].price;
        printf("Added '%s' - RPS %.2f to your order.\n", menu[itemNumber - 1].name, menu[itemNumber - 1].price);

        if (newOrder.itemCount >= MAX_ITEMS) {
            printf("You have reached the maximum items per order.\n");
            break;
        }

        printf("Would you like to add another item? (1 for Yes, 0 for No): ");
        int choice;
        scanf("%d", &choice);
        if (choice == 0) {
            break;
        }
    }

    if (newOrder.itemCount == 0) {
        printf("No items were added to the order.\n");
        return;
    }
    orders[orderCount++] = newOrder;
    printf("\nOrder placed successfully! THANK YOU! Total: RPS %.2f\n", newOrder.total);
    payForOrder(&orders[orderCount - 1]);
}

void payForOrder(struct Order *order) {
    if ((*order).isPaid) {
        printf("This order has already been paid.\n");
        return;
    }

    printf("Total amount to be paid: RPS %.2f\n", (*order).total);
    printf("Enter payment amount: ");
    float payment;
    scanf("%f", &payment);

    if (payment >= (*order).total) {
        (*order).isPaid = 1;
        printf("Payment successful! Change: RPS %.2f\n", payment - (*order).total);
        saveOrderToFile(order);
    } else {
        printf("Insufficient payment. Please try again.\n");
    }
}

void sellerMenu() {
    int choice;
    do {
        printf("\n--- Seller Menu ---\n");
        printf("1. Add Food Item\n");
        printf("2. Delete Food Item\n");
        printf("3. View Orders\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addFoodItem();
                break;
            case 2:
                deleteFoodItem();
                break;
            case 3:
                viewOrders();
                break;
            case 4:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

void buyerMenu(char *username) {
    int choice;
    do {
        printf("\n--- Buyer Menu ---\n");
        printf("1. View Menu\n");
        printf("2. Place Order\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewMenu();
                break;
            case 2:
                placeOrder(username);
                break;
            case 3:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
}

int main() {
    loadUsersFromFile();
    int choice;
    printf("Welcome to Canteen Compass\n");
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2: {
                char username[USERNAME_LEN];
                char password[PASSWORD_LEN];
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);
                int userIndex = authenticate(username, password);
                if (userIndex == -1) {
                    printf("Invalid username or password.\n");
                } else if (users[userIndex].isSeller) {
                    sellerMenu();
                } else {
                    buyerMenu(users[userIndex].username);
                }
                break;
            }
            case 3:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);

    return 0;
}


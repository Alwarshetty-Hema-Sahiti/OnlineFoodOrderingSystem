#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LOCATIONS 5
#define MAX_RESTAURANTS 10
#define MAX_ORDERS 10

int loc;
typedef struct User {
    char username[50];
    char password[50];
    int locIndex;
    struct User* next;
} User;

User* users = NULL;


typedef struct Restaurant {
    char name[50];
    char location[50];
    char menu[100]; 
    float rating;
    struct Restaurant* next;
} Restaurant;

Restaurant* restaurants[MAX_LOCATIONS] = {NULL};

int graph[MAX_LOCATIONS][MAX_LOCATIONS];


typedef struct Order {
    int orderId;
    char restaurant[50];
    char items[100];
    float totalBill;
    struct Order* next;
} Order;

Order* front = NULL;
Order* rear = NULL;

void registerUser() {
    printf("WELCOME TO TOMATO!\n");
    char username[50], password[50];
    int locIndex;
    
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    printf("0-Tirupati\n1-Nellore\n2-Chennai\n");
    printf("Enter location index (0-%d): ", MAX_LOCATIONS - 1);
    scanf("%d", &locIndex);
    loc=locIndex;
    
    if (locIndex < 0 || locIndex >= MAX_LOCATIONS) {
        printf("Invalid location index!\n");
        return;
    }
    
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->locIndex = locIndex;
    newUser->next = users;
    users = newUser;
    printf("User Registered Successfully at location index %d!\n", locIndex);
}

void addRestaurant(char* name, char* location, char* menu, float rating, int locIndex) {
    Restaurant* newRestaurant = (Restaurant*)malloc(sizeof(Restaurant));
    strcpy(newRestaurant->name, name);
    strcpy(newRestaurant->location, location);
    strcpy(newRestaurant->menu, menu);
    newRestaurant->rating = rating;
    newRestaurant->next = restaurants[locIndex];
    restaurants[locIndex] = newRestaurant;
}

void displayRestaurants(int locIndex) {
    Restaurant* temp = restaurants[locIndex];
    if (!temp) {
        printf("No restaurants found in this location!\n");
        return;
    }
    printf("Restaurants available in your area:\n");
    while (temp) {
        printf("- %s (Rating: %.1f)\n  Menu:\n", temp->name, temp->rating);
        char menuCopy[100];
        strcpy(menuCopy, temp->menu);
        char* token = strtok(menuCopy, ",");
        while (token) {
            printf("  - %s\n", token);
            token = strtok(NULL, ",");
        }
        temp = temp->next;
    }
}

void placeOrder() {
    char restaurant[50], item[50];
    char cart[100] = "";
    float totalBill = 0.0;
    int addMore;

    printf("Enter restaurant name: ");
    scanf("%s", restaurant);
    
    Restaurant* rest = NULL;
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        Restaurant* temp = restaurants[i];
        while (temp) {
            if (strcmp(temp->name, restaurant) == 0) {
                rest = temp;
                break;
            }
            temp = temp->next;
        }
        if (rest) break;
    }
    
    if (!rest) {
        printf("Restaurant not found!\n");
        return;
    }
    
    do {
        printf("Enter item name: ");
        scanf("%s", item);
        
        char menuCopy[100];
        strcpy(menuCopy, rest->menu);
        char* token = strtok(menuCopy, ",");
        int found = 0;
        
        while (token) {
            char menuItem[50];
            float price;
            sscanf(token, "%s Rs.%f", menuItem, &price);
            if (strcmp(menuItem, item) == 0) {
                strcat(cart, item);
                strcat(cart, ", ");
                totalBill += price;
                found = 1;
                break;
            }
            token = strtok(NULL, ",");
        }
        
        if (!found) {
            printf("Item not available in the selected restaurant!\n");
        }
        
        printf("Do you want to add more items? (1-Yes, 0-No): ");
        scanf("%d", &addMore);
    } while (addMore);
    
    printf("Total Bill: Rs.%.2f\n", totalBill);
    printf("Available Coupons: 1) FLAT50 (Rs.50 off)\n 2) DISC10 (10%% off)\n 3) NONE\n");
    int couponChoice;
    printf("Select a coupon (1/2/3): ");
    scanf("%d", &couponChoice);

    if (couponChoice == 1) {
        totalBill -= 50;
    } else if (couponChoice == 2) {
        totalBill *= 0.9;
    }
    printf("Final Bill after discount: Rs.%.2f\n", totalBill);
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->orderId = rand() % 100;
    strcpy(newOrder->restaurant, restaurant);
    strcpy(newOrder->items, cart);
    newOrder->totalBill = totalBill;
    newOrder->next = NULL;

    if (!rear) {
        front = rear = newOrder;
    } else {
        rear->next = newOrder;
        rear = newOrder;
    }
    printf("Order placed successfully!\n");
}

void processOrder() {
    if (!front) {
        printf("No orders to process!\n");
        return;
    }
    Order* temp = front;
    front = front->next;
    if (!front) rear = NULL;
    printf("Processing Order #%d: %s from %s\nTotal Bill: Rs.%.2f\n", temp->orderId, temp->items, temp->restaurant, temp->totalBill);
    free(temp);
}

int main() {
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph[i][j] = (i == j) ? 0 : (rand() % 10 + 1);
        }
    }

    addRestaurant("Pizza Hut", "tirupati", "Pizza Rs.300, Pasta Rs.250, Coke Rs.100", 4.5, 0);
    addRestaurant("Subway", "tirupati", "Sandwich Rs.200, Salad Rs.150, Coke Rs.100", 4.2, 0);
    addRestaurant("McDonald's", "nellore", "Burger Rs.250, Fries Rs.100, Coke Rs.100", 4.0, 1);
    addRestaurant("KFC", "chennai", "Chicken Rs.350, Fries Rs.100, Coke Rs.100", 4.3, 2);

    int choice;
    while (1) {
        printf("\n1.Log in \n2. Show Restaurants\n3. Place Order\n4. Process Order\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                displayRestaurants(loc);
                break;
            case 3:
                placeOrder();
                break;
            case 4:
                processOrder();
                break;
            case 5:
                printf("Thank you,we would love to serve you again.-TOMATO");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}

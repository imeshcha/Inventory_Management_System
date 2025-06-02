#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50
#define MARGIN "                                              "

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_COLOR_LIGHT_RED    "\x1b[91m"
#define ANSI_COLOR_LIGHT_GREEN  "\x1b[92m"
#define ANSI_COLOR_LIGHT_YELLOW "\x1b[93m"
#define ANSI_COLOR_LIGHT_BLUE   "\x1b[94m"
#define ANSI_COLOR_LIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_LIGHT_CYAN   "\x1b[96m"

struct Product {
    int id;
    char name[50];
    int quantity;
    float price;
};

struct Product inventory[MAX];
int count = 0;
int nextId = 1001;

void addProduct();
void viewAllProducts();
void searchProducts();
void updateProduct();
void removeProduct();
void saveToFile();
void loadFromFile();
int findProductById(int id);

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    int choice;
    loadFromFile();

    const char *title = "Welcome again to you Inventory";
    int totalWidth = 75;
    int titleLen = strlen(title);
    int pad = (totalWidth - titleLen) / 2 + 10;

    printf("\n\n");
    printf("%*s" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "\n", pad + titleLen, "", title);
    printf("%*s", pad + titleLen, "");
    for(int i = 0; i < titleLen; i++) printf("-");
    printf("\n\n");

    do {
        printf("\n");

        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "1. Search Product\n" ANSI_COLOR_RESET);
        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "2. View All Products\n" ANSI_COLOR_RESET);
        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "3. Add Product\n" ANSI_COLOR_RESET);
        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "4. Update Product\n" ANSI_COLOR_RESET);
        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "5. Remove Product\n" ANSI_COLOR_RESET);
        printf(MARGIN ANSI_COLOR_LIGHT_YELLOW "6. Exit\n" ANSI_COLOR_RESET);
        printf(MARGIN "Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                searchProducts();
                break;
            case 2:
                viewAllProducts();
                break;
            case 3:
                addProduct();
                break;
            case 4:
                updateProduct();
                break;
            case 5:
                removeProduct();
                break;
            case 6:
                saveToFile();
                printf(MARGIN ANSI_COLOR_LIGHT_GREEN "Exiting... Data saved!\n" ANSI_COLOR_RESET);
                exit(0);
            default:
                printf(MARGIN ANSI_COLOR_RED "Invalid choice! Try again.\n" ANSI_COLOR_RESET);
        }
    } while(1);

    return 0;
}

void addProduct() {
    if(count >= MAX) {
        printf(MARGIN "Inventory full!\n");
        return;
    }

    struct Product newProduct;
    newProduct.id = nextId++;

    printf(MARGIN "Product ID: %03d\n", newProduct.id);

    printf(MARGIN "Enter Product Name: ");
    fgets(newProduct.name, sizeof(newProduct.name), stdin);
    newProduct.name[strcspn(newProduct.name, "\n")] = 0;

    for(int i = 0; i < count; i++) {
        if(strcmp(inventory[i].name, newProduct.name) == 0) {
            printf(MARGIN "Error: Product Name already exists!\n");
            return;
        }
    }

    do {
        printf(MARGIN "Enter Quantity: ");
        scanf("%d", &newProduct.quantity);
        clearInputBuffer();
        if(newProduct.quantity < 0) printf(MARGIN "Quantity can't be negative!\n");
    } while(newProduct.quantity < 0);

    do {
        printf(MARGIN "Enter Price: ");
        scanf("%f", &newProduct.price);
        clearInputBuffer();
        if(newProduct.price < 0) printf(MARGIN "Price can't be negative!\n");
    } while(newProduct.price < 0);

    inventory[count++] = newProduct;
    printf(MARGIN ANSI_COLOR_LIGHT_GREEN "Product added successfully! (ID: %03d)\n\n" ANSI_COLOR_RESET, newProduct.id);
}

void viewAllProducts() {
    if(count == 0) {
        printf(MARGIN ANSI_COLOR_RED "Inventory is empty!\n" ANSI_COLOR_RESET);
        return;
    }

    printf("\n");

    printf(MARGIN " %-5s %-20s %-10s %-15s\n", "ID", "Name", "Quantity", "Price");
    printf(MARGIN "------------------------------------------------------------\n");
    for(int i = 0; i < count; i++) {
        printf(MARGIN " %03d    %-20s %-10d LKR %-10.2f\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].quantity,
               inventory[i].price);
    }
}

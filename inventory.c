#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define MARGIN "  "

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
void searchProduct();
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

    const char *title = "Inventory Management System";
    int titleLen = strlen(title);
    int totalWidth = 80;
    int pad = (totalWidth - titleLen) / 2;

    printf("\n%*s" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "\n", pad, "", title);
    printf("%*s", pad, "");
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
                searchProduct();
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
        printf(MARGIN "Inventory is empty!\n");
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

void searchProduct() {
    int method, idx = -1;
    printf(MARGIN "Select search by:\n");
    printf(MARGIN "  1. Name\n");
    printf(MARGIN "  2. ID\n");
    printf(MARGIN "Enter choice: ");
    scanf("%d", &method);
    clearInputBuffer();

    if(method == 1) {
        char searchName[50];
        printf(MARGIN "Enter Product Name: ");
        fgets(searchName, sizeof(searchName), stdin);
        searchName[strcspn(searchName, "\n")] = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(inventory[i].name, searchName) == 0) {
                idx = i;
                break;
            }
        }
    } else if(method == 2) {
        int searchId;
        printf(MARGIN "Enter Product ID: ");
        scanf("%d", &searchId);
        clearInputBuffer();
        idx = findProductById(searchId);
    } else {
        printf(MARGIN ANSI_COLOR_RED "Invalid selection!\n" ANSI_COLOR_RESET);
        return;
    }

    if(idx != -1) {
        printf(MARGIN "\nProduct Found:\n");
        printf(MARGIN "ID: %03d\n" MARGIN "Name: %s\n" MARGIN "Quantity: %d\n" MARGIN "Price: LKR %.2f\n",
               inventory[idx].id, inventory[idx].name,
               inventory[idx].quantity, inventory[idx].price);
    } else {
        printf(MARGIN ANSI_COLOR_LIGHT_RED "Product not found!\n" ANSI_COLOR_RESET);
    }
}

int findProductById(int id) {
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) return i;
    }
    return -1;
}

void updateProduct() {
    int method, idx = -1;
    printf(MARGIN "Select product by:\n");
    printf(MARGIN "  1. Name\n");
    printf(MARGIN "  2. ID\n");
    printf(MARGIN "Enter choice: ");
    scanf("%d", &method);
    clearInputBuffer();

    if(method == 1) {
        char updateName[50];
        printf(MARGIN "Enter Product Name to update: ");
        fgets(updateName, sizeof(updateName), stdin);
        updateName[strcspn(updateName, "\n")] = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(inventory[i].name, updateName) == 0) {
                idx = i;
                break;
            }
        }
    } else if(method == 2) {
        int updateId;
        printf(MARGIN "Enter Product ID to update: ");
        scanf("%d", &updateId);
        clearInputBuffer();
        idx = findProductById(updateId);
    } else {
        printf(MARGIN ANSI_COLOR_RED "Invalid selection!\n" ANSI_COLOR_RESET);
        return;
    }

    if(idx != -1) {
        printf(MARGIN "Product ID: %03d\n", inventory[idx].id);
        printf(MARGIN "Name: %s\n", inventory[idx].name);
        printf(MARGIN "Current Quantity: %d\n", inventory[idx].quantity);
        printf(MARGIN "Enter new Quantity: ");
        scanf("%d", &inventory[idx].quantity);
        clearInputBuffer();

        printf(MARGIN "Current Price: LKR %.2f\n", inventory[idx].price);
        printf(MARGIN "Enter new Price: ");
        scanf("%f", &inventory[idx].price);
        clearInputBuffer();

        printf(MARGIN ANSI_COLOR_LIGHT_GREEN "Product updated successfully! (ID: %03d)\n" ANSI_COLOR_RESET, inventory[idx].id);
    } else {
        printf(MARGIN ANSI_COLOR_LIGHT_RED "Product not found!\n" ANSI_COLOR_RESET);
    }
}

void removeProduct() {
    int method, idx = -1;
    printf(MARGIN "Select product by:\n");
    printf(MARGIN "  1. Name\n");
    printf(MARGIN "  2. ID\n");
    printf(MARGIN "Enter choice: ");
    scanf("%d", &method);
    clearInputBuffer();

    if(method == 1) {
        char removeName[50];
        printf(MARGIN "Enter Product Name to remove: ");
        fgets(removeName, sizeof(removeName), stdin);
        removeName[strcspn(removeName, "\n")] = 0;
        for(int i = 0; i < count; i++) {
            if(strcmp(inventory[i].name, removeName) == 0) {
                idx = i;
                break;
            }
        }
    } else if(method == 2) {
        int removeId;
        printf(MARGIN "Enter Product ID to remove: ");
        scanf("%d", &removeId);
        clearInputBuffer();
        idx = findProductById(removeId);
    } else {
        printf(MARGIN ANSI_COLOR_RED "Invalid selection!\n" ANSI_COLOR_RESET);
        return;
    }

    if(idx != -1) {
        int removedId = inventory[idx].id;
        char removedName[50];
        strcpy(removedName, inventory[idx].name);
        for(int j = idx; j < count - 1; j++) {
            inventory[j] = inventory[j+1];
        }
        count--;
        printf(MARGIN ANSI_COLOR_LIGHT_GREEN "Product '%s' (ID: %03d) removed successfully!\n" ANSI_COLOR_RESET, removedName, removedId);
        printf(MARGIN "\n");
    } else {
        printf(MARGIN ANSI_COLOR_LIGHT_RED "Product not found!\n" ANSI_COLOR_RESET);
    }
}

void saveToFile() {
    FILE *fp = fopen("inventory.dat", "w");
    if(fp == NULL) {
        printf(MARGIN "Error saving file!\n");
        return;
    }

    fprintf(fp, "%d\n", nextId);

    for(int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %d %.2f\n",
                inventory[i].id,
                inventory[i].name,
                inventory[i].quantity,
                inventory[i].price);
    }
    fclose(fp);
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct MenuItem {
    char name[20];
    float price;
    int quantity;
};

struct Order {
    char customerName[50];
    char orderDate[50];
    int numItems;
    struct MenuItem items[50];
};

void printInvoiceHeader(char customerName[50], char orderDate[30]) {
    printf("\n\n");
    printf("\t    GPT Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", orderDate);
    printf("\nInvoice To: %s", customerName);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}

void printInvoiceItem(char itemName[30], int quantity, float price) {
    printf("%s\t\t", itemName);
    printf("%d\t\t", quantity);
    printf("%.2f\t\t", quantity * price);
    printf("\n");
}

void printInvoiceFooter(float total) {
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float cgst = 0.09 * netTotal, grandTotal = netTotal + 2 * cgst;
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", discount);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\nSGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

int main() {
    int option, numItems;
    struct Order order;
    struct Order currentOrder;
    char saveInvoice = 'y', continueFlag = 'y';
    char customerName[50];
    FILE *filePointer;

    while (continueFlag == 'y') {
        system("clear");
        float total = 0;
        int invoiceFound = 0;

        printf("\t============GPT RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1.Generate Invoice");
        printf("\n2.Show all Invoices");
        printf("\n3.Search Invoice");
        printf("\n4.Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &option);
        fflush(stdin);

        switch (option) {
            case 1:
                system("clear");
                printf("\nPlease enter the customer's name:\t");
                fgets(order.customerName, 50, stdin);
                order.customerName[strlen(order.customerName) - 1] = 0;
                strcpy(order.orderDate, __DATE__);
                printf("\nPlease enter the number of items:\t");
                scanf("%d", &numItems);
                order.numItems = numItems;

                for (int i = 0; i < numItems; i++) {
                    fflush(stdin);
                    printf("\n\n");
                    printf("Please enter the item %d:\t", i + 1);
                    fgets(order.items[i].name, 20, stdin);
                    order.items[i].name[strlen(order.items[i].name) - 1] = 0;
                    printf("Please enter the quantity:\t");
                    scanf("%d", &order.items[i].quantity);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &order.items[i].price);
                    total += order.items[i].quantity * order.items[i].price;
                }

                printInvoiceHeader(order.customerName, order.orderDate);
                for (int i = 0; i < order.numItems; i++) {
                    printInvoiceItem(order.items[i].name, order.items[i].quantity, order.items[i].price);
                }
                printInvoiceFooter(total);

                printf("\nDo you want to save the invoice [y/n]:\t");
                scanf(" %c", &saveInvoice);

                if (saveInvoice == 'y') {
                    filePointer = fopen("RestaurantInvoice.dat", "a+");
                    fwrite(&order, sizeof(struct Order), 1, filePointer);
                    if (fwrite != 0)
                        printf("\nSuccessfully saved");
                    else
                        printf("\nError saving");
                    fclose(filePointer);
                }
                break;

            case 2:
                system("clear");
                filePointer = fopen("RestaurantInvoice.dat", "r");
                printf("\n  *****Your Previous Invoices*****\n");
                while (fread(&currentOrder, sizeof(struct Order), 1, filePointer)) {
                    float totalAmount = 0;
                    printInvoiceHeader(currentOrder.customerName, currentOrder.orderDate);
                    for (int i = 0; i < currentOrder.numItems; i++) {
                        printInvoiceItem(currentOrder.items[i].name, currentOrder.items[i].quantity,
                                         currentOrder.items[i].price);
                        totalAmount += currentOrder.items[i].quantity * currentOrder.items[i].price;
                    }
                    printInvoiceFooter(totalAmount);
                }
                fclose(filePointer);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                fflush(stdin);
                fgets(customerName, 50, stdin);
                customerName[strlen(customerName) - 1] = 0;
                system("clear");
                filePointer = fopen("RestaurantInvoice.dat", "r");
                printf("\t*****Invoice of %s*****", customerName);
                while (fread(&currentOrder, sizeof(struct Order), 1, filePointer)) {
                    float totalAmount = 0;
                    if (!strcmp(currentOrder.customerName, customerName)) {
                        printInvoiceHeader(currentOrder.customerName, currentOrder.orderDate);
                        for (int i = 0; i < currentOrder.numItems; i++) {
                            printInvoiceItem(currentOrder.items[i].name, currentOrder.items[i].quantity,
                                             currentOrder.items[i].price);
                            totalAmount += currentOrder.items[i].quantity * currentOrder.items[i].price;
                        }
                        printInvoiceFooter(totalAmount);
                        invoiceFound = 1;
                    }

                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist", customerName);
                }
                fclose(filePointer);
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);
                break;

            default:
                printf("Sorry, invalid option");
                break;
        }
        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf(" %c", &continueFlag);
    }

    printf("\n\t\t Bye Bye :)\n\n");
    printf("\n\n");

    return 0;
}

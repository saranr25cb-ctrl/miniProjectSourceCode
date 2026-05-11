#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[20];
    char firstName[20];
    double balance;
};

unsigned int menu();

void createAccount(FILE *fPtr);
void displayAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);
void modifyAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void textFile(FILE *fPtr);

int main()
{
    FILE *cfPtr;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            return 1;
        }

        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
        }
    }

    unsigned int choice;

    while ((choice = menu()) != 8)
    {
        switch (choice)
        {
        case 1:
            createAccount(cfPtr);
            break;

        case 2:
            displayAccount(cfPtr);
            break;

        case 3:
            depositMoney(cfPtr);
            break;

        case 4:
            withdrawMoney(cfPtr);
            break;

        case 5:
            transferMoney(cfPtr);
            break;

        case 6:
            modifyAccount(cfPtr);
            break;

        case 7:
            deleteAccount(cfPtr);
            break;

        case 9:
            textFile(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nThank you for using Banking System.\n");

    return 0;
}

unsigned int menu()
{
    unsigned int choice;

    printf("\n====== BANKING SYSTEM ======\n");
    printf("1. Create Account\n");
    printf("2. Display Account\n");
    printf("3. Deposit Money\n");
    printf("4. Withdraw Money\n");
    printf("5. Transfer Money\n");
    printf("6. Modify Account\n");
    printf("7. Delete Account\n");
    printf("8. Exit\n");
    printf("9. Generate Report File\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}

void createAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number (1-100): ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter First Name: ");
    scanf("%19s", client.firstName);

    printf("Enter Last Name: ");
    scanf("%19s", client.lastName);

    printf("Enter Initial Balance: ");
    scanf("%lf", &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully.\n");
}

void displayAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("\nAccount Number : %u\n", client.acctNum);
    printf("First Name    : %s\n", client.firstName);
    printf("Last Name     : %s\n", client.lastName);
    printf("Balance       : %.2f\n", client.balance);
}

void depositMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Deposit successful.\n");
}

void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter Withdrawal Amount: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Withdrawal successful.\n");
}

void transferMoney(FILE *fPtr)
{
    struct clientData sender, receiver;

    unsigned int senderAcc, receiverAcc;
    double amount;

    printf("Enter Sender Account Number: ");
    scanf("%u", &senderAcc);

    printf("Enter Receiver Account Number: ");
    scanf("%u", &receiverAcc);

    printf("Enter Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr, (senderAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (receiverAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&receiver, sizeof(struct clientData), 1, fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid account.\n");
        return;
    }

    if (sender.balance < amount)
    {
        printf("Insufficient balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fPtr, (senderAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (receiverAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&receiver, sizeof(struct clientData), 1, fPtr);

    printf("Transfer successful.\n");
}

void modifyAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter Account Number: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter New First Name: ");
    scanf("%19s", client.firstName);

    printf("Enter New Last Name: ");
    scanf("%19s", client.lastName);

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account modified successfully.\n");
}

void deleteAccount(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number to Delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

void textFile(FILE *fPtr)
{
    FILE *writePtr;

    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be created.\n");
        return;
    }

    rewind(fPtr);

    fprintf(writePtr,
            "%-10s%-15s%-15s%-10s\n",
            "Account",
            "FirstName",
            "LastName",
            "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u%-15s%-15s%-10.2f\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt generated successfully.\n");
}

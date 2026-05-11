#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bank
{
    int accNo;
    char name[30];
    float balance;
};

void createAccount(FILE *fp);
void displayAccount(FILE *fp);
void depositMoney(FILE *fp);
void withdrawMoney(FILE *fp);
void transferMoney(FILE *fp);
void deleteAccount(FILE *fp);
void generateReport(FILE *fp);

int main()
{
    FILE *fp;
    int choice;

    fp = fopen("bank.dat", "rb+");

    if (fp == NULL)
    {
        fp = fopen("bank.dat", "wb+");

        struct Bank empty = {0, "", 0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&empty, sizeof(struct Bank), 1, fp);
        }
    }

    do
    {
        printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");
        printf("1. Create Account\n");
        printf("2. Display Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Transfer Money\n");
        printf("6. Delete Account\n");
        printf("7. Generate Report\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount(fp);
            break;

        case 2:
            displayAccount(fp);
            break;

        case 3:
            depositMoney(fp);
            break;

        case 4:
            withdrawMoney(fp);
            break;

        case 5:
            transferMoney(fp);
            break;

        case 6:
            deleteAccount(fp);
            break;

        case 7:
            generateReport(fp);
            break;

        case 8:
            printf("Thank You...\n");
            break;

        default:
            printf("Invalid Choice!\n");
        }

    } while (choice != 8);

    fclose(fp);

    return 0;
}

void createAccount(FILE *fp)
{
    struct Bank customer;

    printf("Enter Account Number: ");
    scanf("%d", &customer.accNo);

    fseek(fp, (customer.accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fread(&customer, sizeof(struct Bank), 1, fp);

    if (customer.accNo != 0)
    {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s", customer.name);

    printf("Enter Initial Balance: ");
    scanf("%f", &customer.balance);

    customer.accNo = customer.accNo;

    fseek(fp, (customer.accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fwrite(&customer, sizeof(struct Bank), 1, fp);

    printf("Account Created Successfully!\n");
}

void displayAccount(FILE *fp)
{
    struct Bank customer;
    int accNo;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp, (accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fread(&customer, sizeof(struct Bank), 1, fp);

    if (customer.accNo == 0)
    {
        printf("Account not found!\n");
    }
    else
    {
        printf("\nAccount Number : %d\n", customer.accNo);
        printf("Name           : %s\n", customer.name);
        printf("Balance        : %.2f\n", customer.balance);
    }
}

void depositMoney(FILE *fp)
{
    struct Bank customer;
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp, (accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fread(&customer, sizeof(struct Bank), 1, fp);

    if (customer.accNo == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Deposit Amount: ");
    scanf("%f", &amount);

    customer.balance += amount;

    fseek(fp, -sizeof(struct Bank), SEEK_CUR);

    fwrite(&customer, sizeof(struct Bank), 1, fp);

    printf("Amount Deposited Successfully!\n");
}

void withdrawMoney(FILE *fp)
{
    struct Bank customer;
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp, (accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fread(&customer, sizeof(struct Bank), 1, fp);

    if (customer.accNo == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Withdraw Amount: ");
    scanf("%f", &amount);

    if (amount > customer.balance)
    {
        printf("Insufficient Balance!\n");
        return;
    }

    customer.balance -= amount;

    fseek(fp, -sizeof(struct Bank), SEEK_CUR);

    fwrite(&customer, sizeof(struct Bank), 1, fp);

    printf("Withdrawal Successful!\n");
}

void transferMoney(FILE *fp)
{
    struct Bank sender, receiver;
    int senderAcc, receiverAcc;
    float amount;

    printf("Enter Sender Account Number: ");
    scanf("%d", &senderAcc);

    printf("Enter Receiver Account Number: ");
    scanf("%d", &receiverAcc);

    printf("Enter Amount to Transfer: ");
    scanf("%f", &amount);

    fseek(fp, (senderAcc - 1) * sizeof(struct Bank), SEEK_SET);
    fread(&sender, sizeof(struct Bank), 1, fp);

    fseek(fp, (receiverAcc - 1) * sizeof(struct Bank), SEEK_SET);
    fread(&receiver, sizeof(struct Bank), 1, fp);

    if (sender.accNo == 0 || receiver.accNo == 0)
    {
        printf("Invalid Account Number!\n");
        return;
    }

    if (sender.balance < amount)
    {
        printf("Insufficient Balance!\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fp, (senderAcc - 1) * sizeof(struct Bank), SEEK_SET);
    fwrite(&sender, sizeof(struct Bank), 1, fp);

    fseek(fp, (receiverAcc - 1) * sizeof(struct Bank), SEEK_SET);
    fwrite(&receiver, sizeof(struct Bank), 1, fp);

    printf("Transfer Successful!\n");
}

void deleteAccount(FILE *fp)
{
    struct Bank empty = {0, "", 0};
    struct Bank customer;

    int accNo;

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accNo);

    fseek(fp, (accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fread(&customer, sizeof(struct Bank), 1, fp);

    if (customer.accNo == 0)
    {
        printf("Account does not exist!\n");
        return;
    }

    fseek(fp, (accNo - 1) * sizeof(struct Bank), SEEK_SET);

    fwrite(&empty, sizeof(struct Bank), 1, fp);

    printf("Account Deleted Successfully!\n");
}

void generateReport(FILE *fp)
{
    FILE *report;

    struct Bank customer;

    report = fopen("report.txt", "w");

    rewind(fp);

    fprintf(report, "ACCOUNT REPORT\n\n");

    while (fread(&customer, sizeof(struct Bank), 1, fp))
    {
        if (customer.accNo != 0)
        {
            fprintf(report,
                    "Acc No: %d\nName: %s\nBalance: %.2f\n\n",
                    customer.accNo,
                    customer.name,
                    customer.balance);
        }
    }

    fclose(report);

    printf("Report Generated Successfully!\n");
}

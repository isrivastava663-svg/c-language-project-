#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANS 50
#define FILENAME "bank.dat"

typedef struct {
    char date[30];
    char type[20];
    double amount;
    double balance_after;
} Transaction;

typedef struct {
    int account_no;
    char name[100];
    char phone[15];
    char address[150];
    char account_type[20]; // Savings / Current
    double balance;
    char pin[10];
    int trans_count;
    Transaction history[MAX_TRANS];
} Account;

Account accounts[MAX_ACCOUNTS];
int acc_count = 0;

void clear_screen() { system("clear || cls"); }

void get_current_time(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, 30, "%Y-%m-%d %H:%M:%S", tm_info);
}

void load_data() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) return;
    fread(&acc_count, sizeof(int), 1, fp);
    fread(accounts, sizeof(Account), acc_count, fp);
    fclose(fp);
}

void save_data() {
    FILE *fp = fopen(FILENAME, "wb");
    if (!fp) { printf("Error saving!\n"); return; }
    fwrite(&acc_count, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(Account), acc_count, fp);
    fclose(fp);
}

void print_line() { printf("============================================================\n"); }

void print_header() {
    print_line();
    printf("           🏦  BANK MANAGEMENT SYSTEM  🏦\n");
    print_line();
}

int find_account(int acc_no) {
    for (int i = 0; i < acc_count; i++)
        if (accounts[i].account_no == acc_no) return i;
    return -1;
}

int verify_pin(int idx) {
    char pin[10];
    printf("Enter PIN: ");
    scanf("%s", pin);
    return strcmp(accounts[idx].pin, pin) == 0;
}

void create_account() {
    if (acc_count >= MAX_ACCOUNTS) { printf("Bank full!\n"); return; }

    Account a;
    a.account_no = 1000 + acc_count + 1;
    a.balance = 0;
    a.trans_count = 0;

    printf("\nEnter Full Name     : "); scanf(" %[^\n]", a.name);
    printf("Enter Phone Number  : "); scanf("%s", a.phone);
    printf("Enter Address       : "); scanf(" %[^\n]", a.address);
    printf("Account Type (Savings/Current): "); scanf("%s", a.account_type);

    double initial;
    printf("Initial Deposit (min 500): ₹");
    scanf("%lf", &initial);
    if (initial < 500) { printf("❌ Minimum deposit is ₹500\n"); return; }
    a.balance = initial;

    printf("Set 4-digit PIN     : "); scanf("%s", a.pin);

    // Record first transaction
    char dt[30]; get_current_time(dt);
    strcpy(a.history[0].date, dt);
    strcpy(a.history[0].type, "Initial Deposit");
    a.history[0].amount = initial;
    a.history[0].balance_after = initial;
    a.trans_count = 1;

    accounts[acc_count++] = a;
    save_data();

    printf("\n✅ Account Created Successfully!\n");
    printf("   Account Number : %d\n", a.account_no);
    printf("   Name           : %s\n", a.name);
    printf("   Balance        : ₹%.2f\n", a.balance);
}

void deposit() {
    int acc_no;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) { printf("❌ Account not found!\n"); return; }
    if (!verify_pin(idx)) { printf("❌ Wrong PIN!\n"); return; }

    double amount;
    printf("Enter Deposit Amount: ₹");
    scanf("%lf", &amount);
    if (amount <= 0) { printf("❌ Invalid amount!\n"); return; }

    accounts[idx].balance += amount;
    char dt[30]; get_current_time(dt);
    int t = accounts[idx].trans_count;
    if (t < MAX_TRANS) {
        strcpy(accounts[idx].history[t].date, dt);
        strcpy(accounts[idx].history[t].type, "Deposit");
        accounts[idx].history[t].amount = amount;
        accounts[idx].history[t].balance_after = accounts[idx].balance;
        accounts[idx].trans_count++;
    }
    save_data();
    printf("✅ ₹%.2f deposited. New Balance: ₹%.2f\n", amount, accounts[idx].balance);
}

void withdraw() {
    int acc_no;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) { printf("❌ Account not found!\n"); return; }
    if (!verify_pin(idx)) { printf("❌ Wrong PIN!\n"); return; }

    double amount;
    printf("Enter Withdrawal Amount: ₹");
    scanf("%lf", &amount);
    if (amount <= 0 || amount > accounts[idx].balance) {
        printf("❌ Invalid amount or insufficient balance!\n"); return;
    }

    accounts[idx].balance -= amount;
    char dt[30]; get_current_time(dt);
    int t = accounts[idx].trans_count;
    if (t < MAX_TRANS) {
        strcpy(accounts[idx].history[t].date, dt);
        strcpy(accounts[idx].history[t].type, "Withdrawal");
        accounts[idx].history[t].amount = amount;
        accounts[idx].history[t].balance_after = accounts[idx].balance;
        accounts[idx].trans_count++;
    }
    save_data();
    printf("✅ ₹%.2f withdrawn. Remaining Balance: ₹%.2f\n", amount, accounts[idx].balance);
}

void check_balance() {
    int acc_no;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) { printf("❌ Account not found!\n"); return; }
    if (!verify_pin(idx)) { printf("❌ Wrong PIN!\n"); return; }

    printf("\n--- Account Details ---\n");
    printf("Account No   : %d\n", accounts[idx].account_no);
    printf("Name         : %s\n", accounts[idx].name);
    printf("Account Type : %s\n", accounts[idx].account_type);
    printf("Balance      : ₹%.2f\n", accounts[idx].balance);
}

void view_transactions() {
    int acc_no;
    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) { printf("❌ Account not found!\n"); return; }
    if (!verify_pin(idx)) { printf("❌ Wrong PIN!\n"); return; }

    printf("\n--- Transaction History for Account %d ---\n", acc_no);
    printf("%-25s %-20s %-12s %-12s\n", "Date", "Type", "Amount(₹)", "Balance(₹)");
    print_line();
    for (int i = 0; i < accounts[idx].trans_count; i++) {
        Transaction *t = &accounts[idx].history[i];
        printf("%-25s %-20s %-12.2f %-12.2f\n",
               t->date, t->type, t->amount, t->balance_after);
    }
}

void transfer() {
    int from, to;
    printf("\nFrom Account No: "); scanf("%d", &from);
    int fidx = find_account(from);
    if (fidx == -1) { printf("❌ Source account not found!\n"); return; }
    if (!verify_pin(fidx)) { printf("❌ Wrong PIN!\n"); return; }

    printf("To Account No  : "); scanf("%d", &to);
    int tidx = find_account(to);
    if (tidx == -1) { printf("❌ Destination account not found!\n"); return; }

    double amount;
    printf("Transfer Amount: ₹"); scanf("%lf", &amount);
    if (amount <= 0 || amount > accounts[fidx].balance) {
        printf("❌ Invalid amount or insufficient balance!\n"); return;
    }

    accounts[fidx].balance -= amount;
    accounts[tidx].balance += amount;
    char dt[30]; get_current_time(dt);

    // Log for sender
    int t1 = accounts[fidx].trans_count;
    if (t1 < MAX_TRANS) {
        strcpy(accounts[fidx].history[t1].date, dt);
        strcpy(accounts[fidx].history[t1].type, "Transfer Out");
        accounts[fidx].history[t1].amount = amount;
        accounts[fidx].history[t1].balance_after = accounts[fidx].balance;
        accounts[fidx].trans_count++;
    }
    // Log for receiver
    int t2 = accounts[tidx].trans_count;
    if (t2 < MAX_TRANS) {
        strcpy(accounts[tidx].history[t2].date, dt);
        strcpy(accounts[tidx].history[t2].type, "Transfer In");
        accounts[tidx].history[t2].amount = amount;
        accounts[tidx].history[t2].balance_after = accounts[tidx].balance;
        accounts[tidx].trans_count++;
    }
    save_data();
    printf("✅ ₹%.2f transferred from Acc %d to Acc %d\n", amount, from, to);
}

int main() {
    load_data();
    int choice;
    do {
        clear_screen();
        print_header();
        printf("  1. Create Account\n");
        printf("  2. Deposit Money\n");
        printf("  3. Withdraw Money\n");
        printf("  4. Check Balance\n");
        printf("  5. Transfer Money\n");
        printf("  6. View Transactions\n");
        printf("  0. Exit\n");
        print_line();
        printf("  Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: check_balance(); break;
            case 5: transfer(); break;
            case 6: view_transactions(); break;
            case 0: printf("\n👋 Thank you for banking with us!\n"); break;
            default: printf("Invalid choice!\n");
        }
        if (choice != 0) { printf("\nPress Enter to continue..."); getchar(); getchar(); }
    } while (choice != 0);

    return 0;
}

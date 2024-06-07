#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define buff 100

typedef struct card{
    char ownerName[20];
    char cardNumber[10];
    char orderDate[20];
    char expDate[20];
    char pin[5];
    int cvv;
    int balance;
} card;

card cardArr[buff];
int cardCount = 0;
char* history_file = "history.txt";

void append(char* file_name, card exp);
void write_file(char* file_name);
char* get_date();
char* get_end_date();
void transferMoney(char* file_name, char from_card[10]);
void add_balance(char* file_name);
void read(char* file_name);
bool is_equivalent_cardNumber(char* file_name, char card_number[10]);
bool is_correct_pin_code(char* file_name, char card_number[10], char pin_code[5]);
int get_random_int(int begin, int end);
char* select_process(int process_number);
void add_to_history(char card_number[10], int process_number, bool is_successfull);
void add_transactions_to_history(char card_number[10], int process_number, char sign, int amount, bool is_successfull);
void print_card_info(char* file_name, char card_number[10]);
void print_card_history(char* file_name, char card_number[10]);
void sign_in(char* file_name, char card_number[10]);
void sign_up(char* file_name, char card_number[10]);
void log_in(char* file_name);

int main()
{
    log_in("cards.txt");
    return 0;
}

void append(char* file_name, card exp){
    FILE* fptr = fopen(file_name, "a");

    if (!is_equivalent_cardNumber(file_name, exp.cardNumber)){
        fprintf(fptr, "%s %s %s %s %s %d %d\n",
             exp.ownerName,
             exp.cardNumber,
             exp.orderDate,
             exp.expDate,
             exp.pin,
             exp.cvv,
             exp.balance);
        strcpy(cardArr[cardCount].ownerName, exp.ownerName);
        strcpy(cardArr[cardCount].cardNumber, exp.cardNumber);
        strcpy(cardArr[cardCount].orderDate, exp.orderDate);
        strcpy(cardArr[cardCount].expDate, exp.expDate);
        strcpy(cardArr[cardCount].pin, exp.pin);
        cardArr[cardCount].cvv = exp.cvv;
        cardArr[cardCount].balance = exp.balance;
        cardCount += 1;
    }
    fclose(fptr);
}

void write_file(char* file_name){
    FILE* fptr = fopen(file_name, "w");
    for (int i = 0; i < cardCount; i++){
        fprintf(fptr, "%s %s %s %s %s %d %d\n",
             cardArr[i].ownerName,
             cardArr[i].cardNumber,
             cardArr[i].orderDate,
             cardArr[i].expDate,
             cardArr[i].pin,
             cardArr[i].cvv,
             cardArr[i].balance);
    }
    fclose(fptr);
}

char* get_date(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* time_str = malloc(20 * sizeof(char));
    sprintf(time_str, "%d-%02d-%02d/%02d:%02d:%02d",
        tm -> tm_year + 1900,
        tm -> tm_mon + 1,
        tm -> tm_mday,
        tm -> tm_hour,
        tm -> tm_min,
        tm -> tm_sec);
    return time_str;
}

char* get_end_date(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* time_str = malloc(20 * sizeof(char));
    sprintf(time_str, "%d-%02d-%02d/%02d:%02d:%02d",
        tm -> tm_year + 1905,
        tm -> tm_mon + 1,
        tm -> tm_mday,
        tm -> tm_hour,
        tm -> tm_min,
        tm -> tm_sec);
    return time_str;
}

void transferMoney(char* file_name, char from_card[10]){
    read(file_name);
    bool is_from_card_valid = false;
    bool is_to_card_valid = false;
    int from_card_index, to_card_index, amount;
    char to_card[10];
    printf("To card:");
    scanf("%s", to_card);
    printf("Amount: ");
    scanf("%d", &amount);
    for (int i = 0; i < cardCount; i++){
        if (strcmp(cardArr[i].cardNumber, from_card) == 0){
            from_card_index = i;
            is_from_card_valid = true;
        }
        else if (strcmp(cardArr[i].cardNumber, to_card) == 0){
            to_card_index = i;
            is_to_card_valid = true;
        }
    }
    if (is_from_card_valid && is_to_card_valid){
        if (cardArr[from_card_index].balance >= amount){
            cardArr[from_card_index].balance -= amount;
            cardArr[to_card_index].balance += amount;
            write_file(file_name);
            add_transactions_to_history(from_card, 1, '-', amount, true);
            add_transactions_to_history(to_card, 1, '+', amount, true);
            system("cls");
            int i;
            printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

            for (i = 0; i < 5; i++)
            {
                printf(".");
                fflush(stdout);
                usleep(300000);
            }
            system("cls");
            printf("Process successfully!\n");
        }
        else{
            add_to_history(from_card, 1, false);
            system("cls");
            int i;
            printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

            for (i = 0; i < 5; i++)
            {
                printf(".");
                fflush(stdout);
                usleep(300000);
            }
            system("cls");
            printf("Insufficient balance!\n");
        }
    }
    else{
        system("cls");
        int i;
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

        for (i = 0; i < 5; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(300000);
        }
        system("cls");
        printf("%s card number is not found", to_card);
    }
}

void add_balance(char* file_name){
    int amount;
    char to_card[10];
    printf("To card: ");
    scanf("%s", to_card);
    if (is_equivalent_cardNumber(file_name, to_card)){
        printf("Amount: ");
        scanf("%d", &amount);
        for (int i = 0; i < cardCount; i++){
            if(strcmp(cardArr[i].cardNumber, to_card) == 0){
                cardArr[i].balance += amount;
                write_file(file_name);
            }
            break;
        }
        add_transactions_to_history(to_card, 0, '+', amount, true);
        system("cls");
        int i;
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

        for (i = 0; i < 5; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(300000);
        }
        system("cls");
        printf("\nProcess completed successfully!\n");
    }
    else{
        add_to_history(to_card, 0, false);
        system("cls");
        int i;
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

        for (i = 0; i < 5; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(300000);
        }
        system("cls");
        printf("\nIncorrect card number!\n");
    }
}

void read(char* file_name){
    FILE* fptr =fopen(file_name, "r");
    cardCount = 0;
    while (fscanf(fptr, "%s %s %s %s %s %d %d",
                  cardArr[cardCount].ownerName,
                  cardArr[cardCount].cardNumber,
                  cardArr[cardCount].orderDate,
                  cardArr[cardCount].expDate,
                  cardArr[cardCount].pin,
                  &cardArr[cardCount].cvv,
                  &cardArr[cardCount].balance) != EOF) cardCount += 1;
    fclose(fptr);
}

bool is_equivalent_cardNumber(char* file_name, char card_number[10]){
    read(file_name);
    for (int i = 0; i < cardCount; i++){
        if (strcmp(cardArr[i].cardNumber, card_number) == 0) return true;
    }
    return false;
}

bool is_correct_pin_code(char* file_name, char card_number[10], char pin_code[5]){
    read(file_name);
    for (int i = 0; i < cardCount; i++){
        if (strcmp(cardArr[i].cardNumber, card_number) == 0 && strcmp(cardArr[i].pin, pin_code) == 0) return true;
    }
    return false;
}

int get_random_int(int begin, int end){
    srand(time(NULL));
    int random_int = rand() % end + begin;
    return random_int;
}

char* select_process(int process_number){
    if (process_number == -1) return "Account created";
    else if (process_number == 0) return "Add Balance";
    else if (process_number == 1) return "Transfer Money";
    else if (process_number == 2) return "Card Info";
    else if (process_number == 3) return "Payment History";
    return ".";
}

void add_to_history(char card_number[10], int process_number, bool is_successfull){
    FILE* fptr = fopen(history_file, "a");
    if (is_successfull) fprintf(fptr, "%s %s %s Successfull\n", get_date(), select_process(process_number), card_number);
    else fprintf(fptr, "%s %s %s Unsuccessfull\n", get_date(), select_process(process_number), card_number);
    fclose(fptr);
}

void add_transactions_to_history(char card_number[10], int process_number, char sign, int amount, bool is_successfull){
    FILE* fptr = fopen(history_file, "a");
    if (is_successfull) fprintf(fptr, "%s %c%d %s Successfull\n", select_process(process_number), sign, amount, card_number);
    else fprintf(fptr, "%s %c%d %s Unsuccessfull\n", select_process(process_number), sign, amount, card_number);
    fclose(fptr);
}

void print_card_info(char* file_name, char card_number[10]){
    for (int i = 0; i < cardCount; i++){
        if (strcmp(cardArr[i].cardNumber, card_number) == 0){
            system("cls");
            printf("Owner Name: %s\n", cardArr[i].ownerName);
            printf("Card Number: %s\n", cardArr[i].cardNumber);
            printf("Order Date: %s\n", cardArr[i].orderDate);
            printf("Expiration Date: %s\n", cardArr[i].expDate);
            printf("PIN: %s\n", cardArr[i].pin);
            printf("CVV: %d\n", cardArr[i].cvv);
            printf("Balance: %d\n\n\n", cardArr[i].balance);
            break;
        }
    }
}

void print_card_history(char* file_name, char card_number[10]){
    FILE* fptr = fopen(history_file, "r");
    char str[100];
    char card_num[10];
    system("cls");
    while (fgets(str, 100, fptr) != NULL){
        for (int i = strlen(str) - 1; i >= 0; i--){
            if (str[i] == ' '){
                for (int j = 8; j >= 0; j--){
                    card_num[j] = str[--i];
                }
                if (strcmp(card_num, card_number) == 0){
                    printf("%s\n", str);
                }
                break;
            }
        }
    }
    fclose(fptr);
}

void sign_in(char* file_name, char card_number[10]){
    char pin_code[5];
    bool is_correct_pin = false;
    int choice;
    for (int i = 3; i >= 0; i--){
        printf("Enter your pin:(%d times remaining):", i);
        scanf("%s", pin_code);
        if (is_correct_pin_code(file_name, card_number, pin_code)){
            is_correct_pin = true;
            break;
        }
    }
    if (!is_correct_pin) printf("Incorrect pin\n");
    else{
        for (int i = 0; i < cardCount; i++){
            if (strcmp(cardArr[i].cardNumber, card_number) == 0){
                printf("Welcome %s\n", cardArr[i].ownerName);
                break;
            }
        }
        system("cls");
        int i;
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  Loading");

        for (i = 0; i < 5; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(300000);
        }
        system("cls");
        printf("What you wanna do?\n");
        printf("0 - Add Balance to Card\n");
        printf("1 - Transfer Money\n");
        printf("2 - Card Information\n");
        printf("3 - Card History\n");
        printf("\nType the process you want to select: ");
        scanf("%d", &choice);
        if (choice == 0) add_balance(file_name);
        else if (choice == 1) transferMoney(file_name, card_number);
        else if(choice == 2) print_card_info(file_name, card_number);
        else if (choice == 3) print_card_history(file_name, card_number);
    }
}

void sign_up(char* file_name, char card_number[10]){
    char name[20], pin1[5], pin2[5];
    card usr;
    printf("\t\tSign up: \n");
    printf("Type your name: ");
    scanf("%s", name);
    printf("Create a pin: ");
    scanf("%s", pin1);
    printf("Type your pin again: ");
    scanf("%s", pin2);
    if (strcmp(pin1, pin2) == 0){
        strcpy(usr.cardNumber, card_number);
        strcpy(usr.ownerName, name);
        strcpy(usr.orderDate, get_date());
        strcpy(usr.expDate, get_end_date());
        strcpy(usr.pin, pin1);
        usr.cvv = get_random_int(100, 999);
        usr.balance = 0;
        append(file_name, usr);
        printf("You created account successfully\n");
        add_to_history(card_number, -1, true);
    }
}

void log_in(char* file_name){
    char card_number[10];
    char sign;
    printf(" ------------------------------\n");
    printf("|         Welcome to Bank      | \n");
    printf(" ------------------------------\n\n");
    printf("Press ENTER to continue . . .\n");
    scanf("%c", &sign);
    system("cls");
    printf("|     Sign in  /  Sign up      |\n");
    printf("\nEnter card number: ");
    scanf("%s", card_number);
    if (is_equivalent_cardNumber(file_name, card_number)){
       sign_in(file_name, card_number);
    }
    else{
        sign_up(file_name, card_number);
    }
}

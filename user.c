#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

struct User {
    char username[50];
    char name[50];
    char email[50];
    char password[50];
    char address[50];
    char gender[50];
    char noTelp[50];
};

void takeInput(char input[50]) {
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = 0;  
}

void takePassword(char password[50]) {
    int i = 0;     
    char input;

    while (1) {
        input = getch();
        if (input == ENTER || input == TAB) {
            password[i] = '\0';
            break;
        } else if (input == BCKSPC) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = input;
            printf("*");
        }
    }
    printf("\n");
}

void generateUsername(char email[50], char username[50]) {
    int i;
    for (i = 0; i < strlen(email); i++) {
        if (email[i] == '@')
            break;
        username[i] = email[i];
    }
    username[i] = '\0'; 
}

int main() {
    int opt;
    bool user_found;
    char confirm_password[50];
    struct User user;
    FILE *file_password;

    printf("1. Sign Up\n");
    printf("2. Login\n");
    printf("3. Keluar\n");
    printf("Masukan pilihan: ");
    scanf("%d", &opt);
    fgetc(stdin);

    switch (opt) {

    case 1:
        printf("Masukan nama lengkap: ");
        takeInput(user.name);
        printf("Masukan Email : ");
        takeInput(user.email);
        printf("Masukan Gender: ");
        takeInput(user.gender);
        printf("Masukan Alamat: ");
        takeInput(user.address);
        printf("Masukan No. Telepon: ");
        takeInput(user.noTelp);
        printf("Masukan Password: ");
        takePassword(user.password);
        printf("Konfirmasi Password: ");
        takePassword(confirm_password);

        if (strcmp(user.password, confirm_password) == 0) {

            generateUsername(user.email, user.username);

            file_password = fopen("User.dat", "ab");

            if (fwrite(&user, sizeof(user), 1, file_password) == 1)
                printf("\nUser Registration success, Your username is %s\n", user.username);
            else
                printf("\nSomething went wrong");

            fclose(file_password);
            return 0;
        } else {
            printf("\nYour password does not match\n");
            return 0;
        }
        break;

    case 2: {
        char username[50], password[50];
        struct User usr;
        int chances = 3;
        user_found = false;

        printf("Enter your username: ");
        takeInput(username);

        file_password = fopen("User.dat", "rb");

        if (file_password == NULL) {
            printf("\nNo user database found!\n");
            return 0;
            break;
        }

        while (fread(&usr, sizeof(usr), 1, file_password)) {
            if (strcmp(usr.username, username) == 0) {
                user_found = true;

                while (chances > 0) {
                    printf("Enter your Password: ");
                    takePassword(password);

                    if (strcmp(usr.password, password) == 0) {
                        printf("\nLogin Successful!\n");
                        printf("Welcome %s\n", usr.username);
                        printf("| Full Name : %s\n", usr.name);
                        printf("| Email     : %s\n", usr.email);
                        printf("| Username  : %s\n", usr.username);
                        printf("| Address   : %s\n", usr.address);
                        printf("| Nomor Telepon   : %s\n", usr.noTelp);
                        chances = -1;
                        return 1;
                        break;
                    } else {
                        printf("Wrong password! (%d chances left) ", chances);
                        chances--;
                    }
                }

                if (chances == 0)
                    printf("\nYou have used all 3 attempts! Login Failed.\n");

                break;
            }
        }

        if (!user_found)
            printf("\nUser is not registered!\n");
            fclose(file_password);
            return 0;
            break;
    }

    default:
        printf("Exiting...\n");
        break;
    }

    return 1;
}
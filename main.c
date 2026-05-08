#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define RESET   "\033[0m"   
#define BOLD    "\033[1m"   

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#define ENTER 13
#define TAB 9
#define BCKSPC 8

#define USERNAME "admin"
#define PASSWORD "admin123"


// STRUKTUR UNTUK ADMIN
struct Voucher { 
    char username[20];
    int voucher;
 };

struct Favorite { char username[20]; char rekomendasi[50]; char genre[30]; char nama_film[30]; };

struct Booking { char username[20]; char nama_film[30]; char jadwal[30]; char nomor_kursi[30]; };

struct Film { 
    char genre[30]; 
    char nama_film[50]; 
    int harga_tiket;    
    int stok;
};

int menuLoginAdmin(){
    char inputUser[20];
    char inputPass[20];

    printf("\n=====================================================================\n");
    printf("                         LOGIN ADMIN                      \n"); 
    printf("=====================================================================\n");
    printf("Masukkan Username: ");
    scanf("%19s", inputUser);
    printf("Masukkan Password: ");
    scanf("%19s", inputPass);

    if(strcmp(inputUser, USERNAME) == 0 && strcmp(inputPass, PASSWORD) == 0) {
        printf("Login berhasil! Selamat datang, %s.\n", USERNAME);
        return 0;
    } else {
        printf("Login gagal! Username atau password salah.\n");
        return 1;
    }
}

const char* getColorByGenre(const char* genre) {

    char lowerGenre[30];
    int i = 0;
    while (genre[i] && i < 29) {
        lowerGenre[i] = tolower(genre[i]);
        i++;
    }
    lowerGenre[i] = '\0';

    if (strstr(lowerGenre, "horror") != NULL) return RED;
    if (strstr(lowerGenre, "comedy") != NULL || strstr(lowerGenre, "komedi") != NULL) return YELLOW;
    if (strstr(lowerGenre, "action") != NULL || strstr(lowerGenre, "aksi") != NULL) return BLUE;
    if (strstr(lowerGenre, "romance") != NULL || strstr(lowerGenre, "romantis") != NULL) return MAGENTA;
    if (strstr(lowerGenre, "sci-fi") != NULL || strstr(lowerGenre, "scifi") != NULL) return CYAN;
    return GREEN; // Default color untuk genre lainnya
}

void tampilkanInfoFilm(char genre[30], char nama_film[50], int harga_tiket, int stok) {
    printf( GREEN "\n=== DETAIL FILM YANG DIINPUT ===\n" RESET);
    printf( YELLOW"Genre       : %s\n" RESET, genre);
    printf( MAGENTA"Nama Film   : %s\n" RESET, nama_film);
    printf( CYAN"Harga Tiket : Rp %d\n" RESET, harga_tiket);
    printf( GREEN"Stok        : %d\n" RESET, stok); 
}

void simpanDataFilm(struct Film dataSatuFilm) {
    FILE *file = fopen("dataFilm.txt", "a"); 
    
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file dataFilm.txt\n");
        return;
    }
    
    fprintf(file, "%s|%s|%d|%d\n", dataSatuFilm.genre, dataSatuFilm.nama_film, dataSatuFilm.harga_tiket, dataSatuFilm.stok);
    fclose(file);
}

void urutkanDanTampilkanByGenre() {
    struct Film daftarFilm[100]; 
    struct Film *ptrFilm[100]; 
    int jumlahFilm = 0;

    FILE *file = fopen("dataFilm.txt", "r");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file\n");
        return;
    }

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%d|%d", 
               daftarFilm[jumlahFilm].genre, 
               daftarFilm[jumlahFilm].nama_film, 
               &daftarFilm[jumlahFilm].harga_tiket,
               &daftarFilm[jumlahFilm].stok);

        ptrFilm[jumlahFilm] = &daftarFilm[jumlahFilm]; 
        jumlahFilm++; 
    }
    fclose(file);


    struct Film *tempPtr; 
    
    for (int i = 0; i < jumlahFilm - 1; i++) {
        for (int j = 0; j < jumlahFilm - i - 1; j++) {
  
            if (strcmp(ptrFilm[j]->genre, ptrFilm[j+1]->genre) > 0) {
        
                tempPtr = ptrFilm[j];
                ptrFilm[j] = ptrFilm[j+1];
                ptrFilm[j+1] = tempPtr;
            }
        }
    }


    printf( CYAN "\n=== DAFTAR FILM (DIURUTKAN BERDASARKAN GENRE A-Z) ===\n" RESET);
    for (int i = 0; i < jumlahFilm; i++) {
        const char *warna = getColorByGenre(ptrFilm[i]->genre);
        printf("%sGenre: %-15s | Judul: %-25s | Harga: Rp %d | Stok: %d\n" RESET, 
               warna,
               ptrFilm[i]->genre, 
               ptrFilm[i]->nama_film, 
               ptrFilm[i]->harga_tiket,
               ptrFilm[i]->stok);

    }
    printf(MAGENTA"=========================================================================\n" RESET);
}

void cariFilmBerdasarkanGenre() {
    char targetGenre[30];
    printf("\n--- CARI FILM BERDASARKAN GENRE ---\n");
    printf("Masukkan Genre yang dicari: ");
    scanf(" %[^\n]", targetGenre);

    FILE *file = fopen("dataFilm.txt", "r");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file dataFilm.txt\n");
        return;
    }

    char line[150];
    struct Film tempFilm;
    int ditemukan = 0;

    printf("\n=== HASIL PENCARIAN (GENRE: %s) ===\n", targetGenre);
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^|]|%[^|]|%d|%d", tempFilm.genre, tempFilm.nama_film, &tempFilm.harga_tiket, &tempFilm.stok) == 4) {

            if (strcmp(tempFilm.genre, targetGenre) == 0) {
                const char *warna = getColorByGenre(tempFilm.genre);
                printf("%sGenre: %-15s | Judul: %-25s | Harga: Rp %d | Stok: %d\n" RESET, 
                       warna,
                       tempFilm.genre, tempFilm.nama_film, tempFilm.harga_tiket, tempFilm.stok);
                ditemukan = 1;
            }
        }
    }
    fclose(file);

    if (!ditemukan) {
        printf("Tidak ditemukan film dengan genre '%s'.\n", targetGenre);
    }
    printf("=====================================================================\n");
}

int menuAdmin() {
    struct Film dataSatuFilm;
    int pilihan;

   
        printf("\n=====================================================================\n");
        printf("                         SELAMAT DATANG                      \n");
        printf("                         ADMINISTRATOR!                          \n");
        printf("=====================================================================\n");

        do {

            printf("\n1.INPUT FILM BARU");
            printf("\n2.TAMPILKAN SEMUA FILM");
            printf("\n3.CARI FILM BERDASARKAN GENRE");
            printf("\n4.LOGOUT");
            printf("\nMasukan Pilihanmu: ");
            scanf(" %d", &pilihan);
            
            if(pilihan==1){
                printf("\n--- INPUT FILM BARU ---\n");
                
                printf("Masukkan Genre Film   : ");
                scanf(" %[^\n]", dataSatuFilm.genre); 
                
                printf("Masukkan Nama Film    : ");
                scanf(" %[^\n]", dataSatuFilm.nama_film); 
                
                printf("Masukkan Harga Tiket  : ");
                scanf("%d", &dataSatuFilm.harga_tiket); 
                                
                printf("Masukkan Stok Tiket  : ");
                scanf("%d", &dataSatuFilm.stok); 

                simpanDataFilm(dataSatuFilm);
            }else if(pilihan==2){
                urutkanDanTampilkanByGenre();
            }else if(pilihan==3){
                cariFilmBerdasarkanGenre();
            }else if(pilihan==4){
                printf("Logout...\n");
            }else{
                printf("Pilihan tidak ada\n");
            }
        } while (pilihan != 4);

    

}



// STRUKTUR UNTUK USER

struct User {
        char username[50];
        char name[50];
        char email[50];
        char password[50];
        char address[50];
        char gender[50];
        char noTelp[50];
    };
    
struct User usr;

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
    
    int loginUser() {
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
            // struct User usr;
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
                            printf(BOLD GREEN "\n=============Login Successful!=============\n" RESET);
                            printf("==========Welcome %s==========\n", usr.username);
                            printf("| Full Name \t\t: %s\n", usr.name);
                            printf("| Email     \t\t: %s\n", usr.email);
                            printf("| Username  \t\t: %s\n", usr.username);
                            printf("| Address   \t\t: %s\n", usr.address);
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
            printf("Pilihan tidak valid...\n");
            break;
        }
    
        return 0;
    }

   
 void pesanTiket() {
    FILE *file = fopen("dataFilm.txt", "r");
    if (file == NULL) {
        printf(RED "Film tidak tersedia atau database kosong!\n" RESET);
        return;
    }


    struct Film *daftarFilm = NULL;
    int totalFilm = 0;
    char line[150];

   
    while (fgets(line, sizeof(line), file)) {
        daftarFilm = realloc(daftarFilm, (totalFilm + 1) * sizeof(struct Film));
        
        sscanf(line, "%[^|]|%[^|]|%d|%d", 
               (daftarFilm + totalFilm)->genre, 
               (daftarFilm + totalFilm)->nama_film, 
               &(daftarFilm + totalFilm)->harga_tiket,
               &(daftarFilm + totalFilm)->stok);
        totalFilm++;
    }
    fclose(file);

    if (totalFilm == 0) {
        printf(YELLOW "Belum ada film yang ditambahkan oleh Admin.\n" RESET);
        free(daftarFilm);
        return;
    }

    // 3. TAMPILKAN DAFTAR FILM & STOK
    printf(CYAN "\n=== DAFTAR FILM YANG TERSEDIA ===\n" RESET);
    for (int i = 0; i < totalFilm; i++) {
        printf("%d. %-25s | Harga: Rp %-7d | Stok: %d kursi\n", 
               i + 1, 
               (daftarFilm + i)->nama_film, 
               (daftarFilm + i)->harga_tiket, 
               (daftarFilm + i)->stok);
    }

    // 4. PROSES PEMILIHAN OLEH USER
    int pilihan, jumlahBeli;
    printf("\nPilih nomor film yang ingin dibeli (0 untuk batal): ");
    scanf("%d", &pilihan);

    if (pilihan == 0) {
        printf("Pemesanan dibatalkan.\n");
        free(daftarFilm);
        return;
    }

    if (pilihan < 1 || pilihan > totalFilm) {
        printf(RED "Pilihan tidak valid!\n" RESET);
        free(daftarFilm);
        return;
    }

    // Arahkan probe pointer tepat ke film yang dipilih
    struct Film *filmDipilih = (daftarFilm + (pilihan - 1));

    printf("Masukkan jumlah tiket: ");
    scanf("%d", &jumlahBeli);

    // 5. CEK STOK DAN LAKUKAN TRANSAKSI
    if (jumlahBeli > filmDipilih->stok) {
        printf(RED "\n>>> TRANSAKSI GAGAL: Stok kursi tidak mencukupi! (Sisa: %d)\n" RESET, filmDipilih->stok);
    } else {
        // Kurangi stok di memori
        filmDipilih->stok -= jumlahBeli;
        int totalBayar = jumlahBeli * filmDipilih->harga_tiket;

        printf(GREEN "\n>>> TRANSAKSI BERHASIL! <<<\n" RESET);
        printf("Pembeli: " BOLD "%s\n" RESET, usr.username); 
        printf("Film   : %s (%d tiket)\n", filmDipilih->nama_film, jumlahBeli);
        printf("Total  : Rp %d\n", totalBayar);

   
        file = fopen("dataFilm.txt", "w");
        for (int i = 0; i < totalFilm; i++) {
            fprintf(file, "%s|%s|%d|%d\n", 
                    (daftarFilm + i)->genre, 
                    (daftarFilm + i)->nama_film, 
                    (daftarFilm + i)->harga_tiket, 
                    (daftarFilm + i)->stok);
        }
        fclose(file);
    }

    // 7. BEBASKAN MEMORI (Wajib agar tidak memory leak)
    free(daftarFilm);
}

int menuPembeli() {
    int pilihan;


    while (1) {
        printf("\n=====================================================================\n");
        printf("                         SELAMAT DATANG                      \n");
        printf("                         %s!                          \n", usr.username);
        printf("=====================================================================\n");

        printf("\n1. Lihat Daftar Film");
        printf("\n2. Pesan Tiket");
        printf("\n3. Keluar");
        printf("\nMasukan Pilihanmu: ");
        scanf(" %d", &pilihan);

        switch (pilihan) {
            case 1:
                urutkanDanTampilkanByGenre();
                break;
            case 2:
                pesanTiket();
                break;
            case 3:
                printf("Terima kasih telah berkunjung! Sampai jumpa lagi.\n");
                return 0;
        }
    }
}









//GERBANG AWAL
int main(int argc, char const *argv[])
{
    int pilihan;

    printf(BOLD CYAN "\n\t\t\t\t============Silakan login terlebih dahulu=============\n" RESET);
    printf("1. Login Admin\n");
    printf("2. Login Pembeli\n");
    printf("Masukkan Pilihan Anda: ");
    scanf("%d", &pilihan);


    switch (pilihan) {
        case 1:
            if (menuLoginAdmin() != 0) {
                exit(1); 
            }
            menuAdmin();

            break;

        case 2:
          if (loginUser() == 1) {
                printf(BOLD CYAN "============SELAMAT DATANG DI BIOSKOP KAMI===========" RESET);
                menuPembeli();
            }
            
            break;
    
        default:
            printf("Pilihan tidak valid!\n");
            break;
    }

    return 0;
}




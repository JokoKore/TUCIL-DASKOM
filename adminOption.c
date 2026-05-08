#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define USERNAME "admin"
#define PASSWORD "admin123"

#define RESET   "\033[0m"   
#define BOLD    "\033[1m"   

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"


struct Voucher { char username[20]; int voucher; };

struct Favorite { char username[20]; char rekomendasi[50]; char genre[30]; char nama_film[30]; };

struct Booking { char username[20]; char nama_film[30]; char jadwal[30]; char nomor_kursi[30]; };

struct Film { 
    char genre[30]; 
    char nama_film[50]; 
    int harga_tiket;    
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
        return 1;
    } else {
        printf("Login gagal! Username atau password salah.\n");
        return 0;
    }
}

int menuLoginUser(){
    printf("NICEEEE");
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

void tampilkanInfoFilm(char genre[30], char nama_film[50], int harga_tiket) {
    printf( GREEN "\n=== DETAIL FILM YANG DIINPUT ===\n" RESET);
    printf( YELLOW"Genre       : %s\n" RESET, genre);
    printf( MAGENTA"Nama Film   : %s\n" RESET, nama_film);
    printf( CYAN"Harga Tiket : Rp %d\n" RESET, harga_tiket); 
}

void simpanDataFilm(struct Film dataSatuFilm) {
    FILE *file = fopen("dataFilm.txt", "a"); 
    
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file dataFilm.txt\n");
        return;
    }
    
    fprintf(file, "%s|%s|%d\n", dataSatuFilm.genre, dataSatuFilm.nama_film, dataSatuFilm.harga_tiket);
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
        sscanf(line, "%[^|]|%[^|]|%d", 
               daftarFilm[jumlahFilm].genre, 
               daftarFilm[jumlahFilm].nama_film, 
               &daftarFilm[jumlahFilm].harga_tiket);
        

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
        printf("%sGenre: %-15s | Judul: %-25s | Harga: Rp %d\n" RESET, 
               warna,
               ptrFilm[i]->genre, 
               ptrFilm[i]->nama_film, 
               ptrFilm[i]->harga_tiket);

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
        if (sscanf(line, "%[^|]|%[^|]|%d", tempFilm.genre, tempFilm.nama_film, &tempFilm.harga_tiket) == 3) {

            if (strcmp(tempFilm.genre, targetGenre) == 0) {
                const char *warna = getColorByGenre(tempFilm.genre);
                printf("%sGenre: %-15s | Judul: %-25s | Harga: Rp %d\n" RESET, 
                       warna,
                       tempFilm.genre, tempFilm.nama_film, tempFilm.harga_tiket);
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





int main() {
    struct Film dataSatuFilm;
    int pilihan;

    printf( BOLD "\n=== SISTEM INFORMASI BIOSKOP ===\n" RESET);
    printf( GREEN "1. LOGIN SEBAGAI ADMIN\n" RESET);
    printf(GREEN "2. LOGIN SEBAGAI USER\n" RESET);
    printf("Masukan Pilihanmu: ");
    scanf(" %d", &pilihan);
    
   

    if ( menuLoginAdmin() == 1)
    {
        printf("\n=====================================================================\n");
        printf("                         SELAMAT DATANG                      \n");
        printf("                         ADMINISTRATOR!                          \n");
        printf("=====================================================================\n");

        do {

            printf("\n1.INPUT FILM BARU");
            printf("\n2.TAMPILKAN SEMUA FILM");
            printf("\n3.CARI FILM BERDASARKAN GENRE");
            printf("\n4.KEMBALI KE MENU UTAMA");
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

                simpanDataFilm(dataSatuFilm);
            }else if(pilihan==2){
                urutkanDanTampilkanByGenre();
            }else if(pilihan==3){
                cariFilmBerdasarkanGenre();
            }else if(pilihan==4){
                printf("Kembali...\n");
            }else{
                printf("Pilihan tidak ada\n");
            }
        } while (pilihan != 4);

    }else if(pilihan == 2)
    {
        menuLoginUser();
    }
    
   
    return 0;
}
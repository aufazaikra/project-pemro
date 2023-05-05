#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAKS_PERTANYAAN 5
#define MAKS_NAMA 50
#define MAKS_USERNAME 20
#define MAKS_PASSWORD 20
#define MAKS_JAWABAN 4
#define MAKS_BARIS 256

typedef struct
{
    char pertanyaan[MAKS_BARIS];
    char jawaban[MAKS_JAWABAN][MAKS_BARIS];
    int jawaban_benar;
} Pertanyaan;

typedef struct
{
    char nama[MAKS_NAMA];
    int usia;
    int skor;
} Pemain;

typedef struct
{
    char username[MAKS_USERNAME];
    char password[MAKS_PASSWORD];
    char nama[MAKS_NAMA];
    int usia;
} Registrasi;

void daftar_pengguna(Registrasi *reg)
{
    printf("Silakan melakukan registrasi.\n");
    printf("Username: ");
    scanf("%s", reg->username);
    printf("Password: ");
    scanf("%s", reg->password);
    printf("Nama: ");
    scanf(" %[^\n]s", reg->nama);
    printf("Usia: ");
    scanf("%d", &reg->usia);

    FILE *fp = fopen("registrasi.bin", "ab");
    fwrite(reg, sizeof(Registrasi), 1, fp);
    fclose(fp);
}

int login(char *username, char *password)
{
    FILE *fp = fopen("registrasi.bin", "rb");
    Registrasi reg;
    while (fread(&reg, sizeof(Registrasi), 1, fp))
    {
        if (strcmp(reg.username, username) == 0 && strcmp(reg.password, password) == 0)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void load_pertanyaan(Pertanyaan pertanyaan[])
{
    FILE *fp = fopen("kuis.txt", "r");
    char baris[MAKS_BARIS];
    char *token;

    for (int i = 0; i < MAKS_PERTANYAAN; i++)
    {
        fgets(baris, MAKS_BARIS, fp);
        token = strtok(baris, "|");
        strcpy(pertanyaan[i].pertanyaan, token);
        for (int j = 0; j < MAKS_JAWABAN; j++)
        {
            token = strtok(NULL, "|");
            strcpy(pertanyaan[i].jawaban[j], token);
        }
        token = strtok(NULL, "|");
        pertanyaan[i].jawaban_benar = atoi(token);
    }

    fclose(fp);
}

void pertanyaan() {
    Pertanyaan pertanyaan[MAKS_PERTANYAAN];

    strcpy(pertanyaan[0].pertanyaan, "Siapakah Tokoh penemu yang mendapat julukan bapak komputer?");
    strcpy(pertanyaan[0].jawaban[0], "Charles Babbage");
    strcpy(pertanyaan[0].jawaban[1], "Michael Faraday");
    strcpy(pertanyaan[0].jawaban[2], "James Naismith");
    pertanyaan[0].jawaban_benar = 1;

    strcpy(pertanyaan[1].pertanyaan, "John Boyd Dunlop merupakan penemu yang berhasil menciptakan?");
    strcpy(pertanyaan[1].jawaban[0], "Ban yang berisi udara");
    strcpy(pertanyaan[1].jawaban[1], "Lampu lalu lintas");
    strcpy(pertanyaan[1].jawaban[2], "Kalkulator");
    pertanyaan[1].jawaban_benar = 1;

    strcpy(pertanyaan[2].pertanyaan, "Siapakah Tokoh Penemu arus listrik?");
    strcpy(pertanyaan[2].jawaban[0], "Carros Marocos");
    strcpy(pertanyaan[2].jawaban[1], "Michael Faraday");
    strcpy(pertanyaan[2].jawaban[2], "Pablo Picasso");
    pertanyaan[2].jawaban_benar = 2;

    strcpy(pertanyaan[3].pertanyaan, "Siapakah Tokoh Penemu Televisi?");
    strcpy(pertanyaan[3].jawaban[0], "Garbagge Thonny");
    strcpy(pertanyaan[3].jawaban[1], "Charles Martin");
    strcpy(pertanyaan[3].jawaban[2], "John Logie Baird");
    pertanyaan[3].jawaban_benar = 3;

    strcpy(pertanyaan[4].pertanyaan, "James Naismith merupakan tokoh yang berhasil menemukan?");
    strcpy(pertanyaan[4].jawaban[0], "Olahraga Sepak Bola");
    strcpy(pertanyaan[4].jawaban[1], "Olahraga Bola Voli");
    strcpy(pertanyaan[4].jawaban[2], "Olahraga Bola Basket");
    pertanyaan[4].jawaban_benar = 3;

    Pemain pemain;
    void tampilkan_skor_tertinggi();
}


int mainkan_permainan(Pertanyaan pertanyaan[], Pemain *pemain)
{
    int skor = 0;
    int jawaban;

    for (int i = 0; i < MAKS_PERTANYAAN; i++)
    {
        printf("\n\nPertanyaan %d:\n", i + 1);
        printf("%s\n", pertanyaan[i].pertanyaan);
        for (int j = 0; j < MAKS_JAWABAN; j++)
        {
            printf("%d. %s\n", j + 1, pertanyaan[i].jawaban[j]);
        }
        printf("Jawaban Anda: ");
        scanf("%d", &jawaban);

        if (jawaban == pertanyaan[i].jawaban_benar)
        {
            printf("Jawaban Anda benar!\n");
            skor++;
        }
        else
        {
            printf("Jawaban Anda salah! Permainan berakhir.\n");
            break;
        }
    }

    pemain->skor = skor;
    return skor;
}


void tampilkan_skor_tertinggi()
{
    FILE *fp = fopen("skor_tertinggi.bin", "rb");

    if (!fp)
    {
        printf("Belum ada skor tertinggi yang tersimpan.\n");
        return;
    }

    Pemain pemain;
    printf("Skor tertinggi:\n");
    while (fread(&pemain, sizeof(Pemain), 1, fp))
    {
        printf("%s: %d\n", pemain.nama, pemain.skor);
    }
    fclose(fp);
}

void simpan_skor_tertinggi(Pemain *pemain)
{
    FILE *fp = fopen("skor_tertinggi.bin", "ab");
    fwrite(pemain, sizeof(Pemain), 1, fp);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Penggunaan: %s <username> <password>\n", argv[0]);
        return 1;
    }

    char *username = argv[1];
    char *password = argv[2];

    if (!login(username, password))
    {
        printf("Login gagal. Silakan coba lagi.\n");
        return 1;
    }

    Pemain pemain;
    Registrasi reg;
    FILE *fp = fopen("registrasi.bin", "rb");
    while (fread(&reg, sizeof(Registrasi), 1, fp))
    {
        if (strcmp(reg.username, username) == 0)
        {
            strcpy(pemain.nama, reg.nama);
            pemain.usia = reg.usia;
            break;
        }
    }
    fclose(fp);

    printf("Selamat datang, %s!\n", pemain.nama);
    printf("Anda akan memainkan game kuis. Setiap pertanyaan memiliki 4 pilihan jawaban.\n");
    printf("Silakan pilih jawaban yang benar dengan mengetik nomor jawaban.\n");
    printf("Jika jawaban Anda salah, permainan akan berakhir.\n");
    printf("Anda akan menjawab %d pertanyaan.\n\n", MAKS_PERTANYAAN);

    Pertanyaan pertanyaan[MAKS_PERTANYAAN];
    load_pertanyaan(pertanyaan);
    int skor = mainkan_permainan(pertanyaan, &pemain);

    printf("\n\nPermainan selesai!\n");
    printf("Skor Anda: %d\n", skor);

    fp = fopen("skor.bin", "ab");
    fwrite(&pemain, sizeof(Pemain), 1, fp);
    fclose(fp);

    return 0;
}

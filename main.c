#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10
#define MAX_BOOKING 100

// ================= BST PELANGGAN =================
typedef struct Pelanggan {
    int id;
    char nama[50];
    struct Pelanggan *left, *right;
} Pelanggan;

Pelanggan* buatPelanggan(int id, char nama[]) {
    Pelanggan* baru = (Pelanggan*)malloc(sizeof(Pelanggan));
    baru->id = id;
    strcpy(baru->nama, nama);
    baru->left = baru->right = NULL;
    return baru;
}

Pelanggan* insertPelanggan(Pelanggan* root, int id, char nama[]) {
    if(root == NULL)
        return buatPelanggan(id, nama);

    if(id < root->id)
        root->left = insertPelanggan(root->left, id, nama);
    else if(id > root->id)
        root->right = insertPelanggan(root->right, id, nama);

    return root;
}

Pelanggan* cariPelanggan(Pelanggan* root, int id) {
    if(root == NULL || root->id == id)
        return root;

    if(id < root->id)
        return cariPelanggan(root->left, id);

    return cariPelanggan(root->right, id);
}

// ================= HASH TABLE LAPANGAN =================
typedef struct Lapangan {
    char kode[10];
    char nama[50];
    struct Lapangan *next;
} Lapangan;

Lapangan* hashTable[TABLE_SIZE];

int hashFunction(char kode[]) {
    return kode[0] % TABLE_SIZE;
}

void tambahLapangan(char kode[], char nama[]) {
    int index = hashFunction(kode);

    Lapangan* baru = (Lapangan*)malloc(sizeof(Lapangan));

    strcpy(baru->kode, kode);
    strcpy(baru->nama, nama);

    baru->next = hashTable[index];
    hashTable[index] = baru;
}

void cariLapangan(char kode[]) {
    int index = hashFunction(kode);

    Lapangan* temp = hashTable[index];

    while(temp != NULL) {
        if(strcmp(temp->kode, kode) == 0) {
            printf("Lapangan ditemukan: %s\n", temp->nama);
            return;
        }
        temp = temp->next;
    }

    printf("Lapangan tidak ditemukan\n");
}

// ================= PRIORITY QUEUE =================
typedef struct {
    char nama[50];
    int prioritas;
} WaitingList;

WaitingList heap[100];
int heapSize = 0;

void swap(WaitingList *a, WaitingList *b) {
    WaitingList temp = *a;
    *a = *b;
    *b = temp;
}

void insertHeap(char nama[], int prioritas) {
    heapSize++;

    strcpy(heap[heapSize].nama, nama);
    heap[heapSize].prioritas = prioritas;

    int i = heapSize;

    while(i > 1 &&
          heap[i].prioritas > heap[i/2].prioritas) {

        swap(&heap[i], &heap[i/2]);
        i /= 2;
    }
}

void tampilWaitingList() {
    printf("\n=== Waiting List ===\n");

    for(int i=1; i<=heapSize; i++) {
        printf("%s (%d)\n",
               heap[i].nama,
               heap[i].prioritas);
    }
}

// ================= QUICK SORT BOOKING =================
typedef struct {
    int idBooking;
    char nama[50];
} Booking;

Booking booking[MAX_BOOKING];
int jumlahBooking = 0;

void swapBooking(Booking *a, Booking *b) {
    Booking temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int low, int high) {
    int pivot = booking[high].idBooking;

    int i = low - 1;

    for(int j=low; j<high; j++) {
        if(booking[j].idBooking < pivot) {
            i++;
            swapBooking(&booking[i], &booking[j]);
        }
    }

    swapBooking(&booking[i+1], &booking[high]);

    return i+1;
}

void quickSort(int low, int high) {
    if(low < high) {
        int pi = partition(low, high);

        quickSort(low, pi-1);
        quickSort(pi+1, high);
    }
}

// ================= MENU =================
int main() {

    Pelanggan *root = NULL;

    int pilih;

    do {
        printf("\n=== SPORTIFY ARENA ===\n");
        printf("1. Tambah Pelanggan\n");
        printf("2. Cari Pelanggan\n");
        printf("3. Tambah Lapangan\n");
        printf("4. Cari Lapangan\n");
        printf("5. Tambah Waiting List\n");
        printf("6. Tampilkan Waiting List\n");
        printf("0. Keluar\n");
        printf("Pilihan : ");
        scanf("%d", &pilih);

        if(pilih == 1) {
            int id;
            char nama[50];

            printf("ID : ");
            scanf("%d", &id);

            printf("Nama : ");
            scanf(" %[^\n]", nama);

            root = insertPelanggan(root, id, nama);
        }

        else if(pilih == 2) {
            int id;

            printf("Masukkan ID : ");
            scanf("%d", &id);

            Pelanggan *hasil =
                cariPelanggan(root, id);

            if(hasil)
                printf("Ditemukan : %s\n",
                       hasil->nama);
            else
                printf("Tidak ditemukan\n");
        }

        else if(pilih == 3) {
            char kode[10], nama[50];

            printf("Kode : ");
            scanf("%s", kode);

            printf("Nama Lapangan : ");
            scanf(" %[^\n]", nama);

            tambahLapangan(kode, nama);
        }

        else if(pilih == 4) {
            char kode[10];

            printf("Kode : ");
            scanf("%s", kode);

            cariLapangan(kode);
        }

        else if(pilih == 5) {
            char nama[50];
            int prioritas;

            printf("Nama : ");
            scanf(" %[^\n]", nama);

            printf("Prioritas : ");
            scanf("%d", &prioritas);

            insertHeap(nama, prioritas);
        }

        else if(pilih == 6) {
            tampilWaitingList();
        }

    } while(pilih != 0);

    printf("\n=====================================\n");
    printf("      PROGRAM TELAH SELESAI\n");
    printf(" Sportify Arena - Booking Lapangan\n");
    printf(" Terima kasih telah menggunakan\n");
    printf(" aplikasi ini.\n");
    printf("=====================================\n");


    return 0;
}
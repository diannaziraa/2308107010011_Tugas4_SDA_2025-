#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

#define MAX_WORD_LENGTH 100

void merge_sort_int_wrapper(int* arr, int n) { merge_sort_int(arr, 0, n - 1); }
void quick_sort_int_wrapper(int* arr, int n) { quick_sort_int(arr, 0, n - 1); }
void merge_sort_str_wrapper(char** arr, int n) { merge_sort_str(arr, 0, n - 1); }
void quick_sort_str_wrapper(char** arr, int n) { quick_sort_str(arr, 0, n - 1); }

void benchmark_sorting_int(const char* name, void (*sort_fn)(int*, int), int* original, int n) {
    int* data = malloc(n * sizeof(int));
    memcpy(data, original, n * sizeof(int));
    clock_t start = clock();
    sort_fn(data, n);
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    double mem = n * sizeof(int);
    printf("| %-13s | %-11d | %10.3f detik | %9.2f MB |\n", name, n, duration, mem / (1024.0 * 1024));
    free(data);
}

void benchmark_sorting_str(const char* name, void (*sort_fn)(char**, int), char** original, int n) {
    char** data = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) data[i] = strdup(original[i]);
    clock_t start = clock();
    sort_fn(data, n);
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    double mem = 0;
    for (int i = 0; i < n; i++) mem += strlen(data[i]) + 1;
    mem += n * sizeof(char*);
    printf("| %-13s | %-11d | %10.3f detik | %9.2f MB |\n", name, n, duration, mem / (1024.0 * 1024));
    for (int i = 0; i < n; i++) free(data[i]);
    free(data);
}

int* load_numbers(const char* filename, int n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file angka");
        exit(EXIT_FAILURE);
    }
    int* data = malloc(n * sizeof(int));
    int count = 0;
    while (count < n && fscanf(file, "%d", &data[count]) == 1) {
        count++;
    }
    fclose(file);
    if (count < n) {
        fprintf(stderr, "File angka hanya memiliki %d data, dibutuhkan %d.\n", count, n);
        exit(EXIT_FAILURE);
    }
    return data;
}

char** load_words(const char* filename, int n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file kata");
        exit(EXIT_FAILURE);
    }
    char** data = malloc(n * sizeof(char*));
    char buffer[MAX_WORD_LENGTH];
    int count = 0;
    while (count < n && fscanf(file, "%s", buffer) == 1) {
        data[count] = strdup(buffer);
        count++;
    }
    fclose(file);
    if (count < n) {
        fprintf(stderr, "File kata hanya memiliki %d data, dibutuhkan %d.\n", count, n);
        exit(EXIT_FAILURE);
    }
    return data;
}

int main() {
    int pilihan;
    int opsi[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};

    printf("=== PILIH JUMLAH DATA UNTUK BENCHMARK ===\n");
    printf("1. 10.000\n");
    printf("2. 50.000\n");
    printf("3. 100.000\n");
    printf("4. 250.000\n");
    printf("5. 500.000\n");
    printf("6. 1.000.000\n");
    printf("7. 1.500.000\n");
    printf("8. 2.000.000\n");
    printf("Masukkan pilihan (1-8): ");
    scanf("%d", &pilihan);

    if (pilihan < 1 || pilihan > 8) {
        printf("Pilihan tidak valid!\n");
        return 1;
    }

    int jumlah_data = opsi[pilihan - 1];

    int* data_angka = load_numbers("data_angka.txt", jumlah_data);
    char** data_kata = load_words("data_kata.txt", jumlah_data);

    printf("\n>>> Pengujian performa algoritma sorting terhadap %d data *kata*\n", jumlah_data);
    printf("+---------------+-------------+------------------+--------------+\n");
    printf("| Algoritma     | Jumlah Data | Waktu Eksekusi   | Memori (MB)  |\n");
    printf("+---------------+-------------+------------------+--------------+\n");
    benchmark_sorting_str("BubbleSort", bubble_sort_str, data_kata, jumlah_data);
    benchmark_sorting_str("SelectionSort", selection_sort_str, data_kata, jumlah_data);
    benchmark_sorting_str("InsertionSort", insertion_sort_str, data_kata, jumlah_data);
    benchmark_sorting_str("MergeSort", merge_sort_str_wrapper, data_kata, jumlah_data);
    benchmark_sorting_str("QuickSort", quick_sort_str_wrapper, data_kata, jumlah_data);
    benchmark_sorting_str("ShellSort", shell_sort_str, data_kata, jumlah_data);
    printf("+---------------+-------------+------------------+--------------+\n");

    printf("\n>>> Pengujian performa algoritma sorting terhadap %d data *angka*\n", jumlah_data);
    printf("+---------------+-------------+------------------+--------------+\n");
    printf("| Algoritma     | Jumlah Data | Waktu Eksekusi   | Memori (MB)  |\n");
    printf("+---------------+-------------+------------------+--------------+\n");
    benchmark_sorting_int("BubbleSort", bubble_sort_int, data_angka, jumlah_data);
    benchmark_sorting_int("SelectionSort", selection_sort_int, data_angka, jumlah_data);
    benchmark_sorting_int("InsertionSort", insertion_sort_int, data_angka, jumlah_data);
    benchmark_sorting_int("MergeSort", merge_sort_int_wrapper, data_angka, jumlah_data);
    benchmark_sorting_int("QuickSort", quick_sort_int_wrapper, data_angka, jumlah_data);
    benchmark_sorting_int("ShellSort", shell_sort_int, data_angka, jumlah_data);
    printf("+---------------+-------------+------------------+--------------+\n");

    free(data_angka);
    for (int i = 0; i < jumlah_data; i++) free(data_kata[i]);
    free(data_kata);

    return 0;
}

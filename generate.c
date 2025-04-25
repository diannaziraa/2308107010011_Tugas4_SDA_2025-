#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Fungsi pembangkit angka
void generate_random_numbers(const char *filename, int count, int max_value) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File tidak dapat dibuka");
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int num = rand() % max_value;
        fprintf(fp, "%d\n", num);
    }

    fclose(fp);
}

// Fungsi pembangkit kata
void random_word(char *word, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        word[i] = charset[key];
    }
    word[length] = '\0';
}

void generate_random_words(const char *filename, int count, int max_word_length) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("File tidak dapat dibuka");
        return;
    }

    srand(time(NULL));
    char word[100];
    for (int i = 0; i < count; i++) {
        int length = (rand() % (max_word_length - 3)) + 3;
        random_word(word, length);
        fprintf(fp, "%s\n", word);
    }

    fclose(fp);
}

int main() {
    printf("Membuat file data_angka.txt dan data_kata.txt dengan 2.000.000 data...\n");
    generate_random_numbers("data_angka.txt", 2000000, 2000000);
    generate_random_words("data_kata.txt", 2000000, 20);
    printf("Selesai!\n");
    return 0;
}

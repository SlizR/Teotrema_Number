#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 120000
#define CHUNK_SIZE 10000

typedef struct {
    int digits[MAX_DIGITS];
    int size;
} BigInt;

void init(BigInt *a, int value) {
    memset(a->digits, 0, sizeof(a->digits));
    a->size = 0;
    while (value > 0) {
        a->digits[a->size++] = value % 10;
        value /= 10;
    }
}

void multiply(BigInt *a, int b) {
    int carry = 0;
    for (int i = 0; i < a->size; i++) {
        long long prod = (long long)a->digits[i] * b + carry;
        a->digits[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry > 0) {
        a->digits[a->size++] = carry % 10;
        carry /= 10;
    }
}

void power(BigInt *result, int base, int exp) {
    init(result, 1);
    for (int i = 0; i < exp; i++) {
        multiply(result, base);
        if (i % 1000 == 0)
            printf("Step %d/%d\n", i, exp);
    }
}

void save_chunks(BigInt *num) {
    int file_index = 0;
    for (int i = num->size - 1; i >= 0; i -= CHUNK_SIZE) {
        int start = (i - CHUNK_SIZE + 1 > 0) ? i - CHUNK_SIZE + 1 : 0;
        char filename[50];
        snprintf(filename, sizeof(filename), "chunk_%d.txt", file_index++);
        FILE *f = fopen(filename, "w");
        if (!f) { printf("Error opening file %s\n", filename); return; }
        for (int j = start; j <= i; j++)
            fprintf(f, "%d", num->digits[j]);
        fclose(f);
    }
}

int main() {
    BigInt num;
    int base = 847;
    int exp = 317;
    exp = exp * exp;

    printf("Calculating 847^(317^2) = 847^%d\n", exp);
    power(&num, base, exp);

    printf("Number of digits: %d\n", num.size);

    printf("Saving chunks to txt files...\n");
    save_chunks(&num);
    printf("Done! Files chunk_0.txt, chunk_1.txt, ... generated.\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define pn 16777216L // pruning number

long* read_file(char* filename, int* n);
long iterate(int x);
long prune(int x);
int hash(int* x);

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }

    int n;
    long* initials = read_file(argv[1], &n);
    int deltas[4];
    long totals[130321] = {0}; // max 4-bit number in base 19
    int seen[130321] = {0};

    // part 1 - also set up for part 2
    long total = 0;
    for (int i = 0; i < n; i++) {
        int count = 0;
        long secret = initials[i];
        int prev_price = secret % 10;

        for (int j = 0; j < 2000; j++) {

            // calculate next secret number and price difference
            secret = iterate(secret);
            int price = secret % 10;
            int delta = price - prev_price;

            prev_price = price;

            // fill first 4 deltas
            if (count < 4) {
                deltas[count++] = delta;

                if (count < 4) {
                    continue;
                }
            } else {
                // slide window
                deltas[0] = deltas[1];
                deltas[1] = deltas[2];
                deltas[2] = deltas[3];
                deltas[3] = delta;
            }

            int key = hash(deltas);
            if (seen[key] != i + 1) { // check if we've seen this sequence before
                seen[key] = i + 1;
                totals[key] += price;
            }
        }
        total += secret;
    }

    printf("Secret number sum = %ld\n", total);

    // part 2
    int maxPrice = 0;
    for (int i = 0; i < 130321; i++) {
        if (totals[i] > maxPrice) {
            maxPrice = totals[i];
        }
    }
    printf("Highest price possible = %d\n", maxPrice);

    free(initials);
    return 0;
}

int hash(int* x)
{
    int a = x[0] + 9;
    int b = x[1] + 9;
    int c = x[2] + 9;
    int d = x[3] + 9;
    return ((a*19 + b)*19 + c)*19 + d;
}

long iterate(int x)
{
    long temp;
    temp = x * 64;
    x ^= temp;
    x = prune(x);

    temp = x / 32;
    x ^= temp;
    x = prune(x);

    temp = x * 2048;
    x ^= temp;
    x = prune(x);

    return x;
}

// take the number mod pn
long prune(int x)
{
    return ((x %= pn) < 0 ? x + pn : x);
}

long* read_file(char* filename, int* n)
{
    FILE* fptr = fopen(filename, "r");

    fscanf(fptr, "%d\n", n);
    long* arr = malloc(sizeof(long) * (*n));

    for (int i = 0; i < *n; i++) {
        fscanf(fptr, "%ld\n", arr + i);
    }
    
    return arr;
}
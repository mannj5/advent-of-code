#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long solve(char* filename, int k);
long long get_max_digit(char* line, int l, int k);

int main(int argc, char** argv) {

    printf("part 1: %lld\n", solve(argv[1], 2));
    printf("part 2: %lld\n", solve(argv[1], 12));

    return 0;
}

long long get_max_digit(char* line, int l, int k)
{
    long long total = 0;
    int ix = 0;
    while (k) {
        int max = 0;
        int len = l - k;
        for (int i = ix; i < len; i++) {
            int num = line[i] - '0';
            if (num > max) {
                max = num;
                ix = i + 1;
            }
        }
        k--;
        total = (total * 10) + max;
    }

    return total;
}

long long solve(char* filename, int k)
{
    FILE* fptr = fopen(filename, "r");

    long long total = 0;
    size_t size;
    char* line = NULL;
    int nbytes;
    while ((nbytes = getline(&line, &size, fptr)) > 0) {
        total += get_max_digit(line, nbytes, k);
    }

    fclose(fptr);

    return total;
}
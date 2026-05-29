#include <stdio.h>
#include <stdlib.h>

int* parse(char* filename, int* size);
void part_one(int* arr, int size);
void part_two(int* arr, int size);


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: no file given\n");
        return 1;
    }

    int size;
    int* arr = parse(argv[1], &size);
    
    part_one(arr, size);
    part_two(arr, size);

    free(arr);

    return 0;
}

int mod(int x, int m) {
    return ((x %= m) < 0 ? x + m : x);
}

void part_two(int* arr, int size) {
    int pos = 50;
    int npos;
    int count = 0;

    int turn;
    for (int i = 0; i < size; i++) {
        turn = arr[i];
        npos = pos + turn;

        // count net turns
        count += abs(turn / 100);

        if (turn > 0) {
            if ((pos + (turn % 100)) >= 100) {
                count++;
            }
        } else if (turn < 0) {
            if ((pos + (turn % 100)) <= 0) {
                count++;
            }
            if (pos == 0) {
                count--;
            }
        }
        pos = mod(npos, 100);
    }

    printf("part 2: %d\n", count);
}

void part_one(int* arr, int size) {
    int pos = 50;
    int count = 0;

    for (int i = 0; i < size; i++) {
        pos = mod(pos + arr[i], 100);
        if (pos == 0) {
            count++;
        }
    }

    printf("part 1: %d\n", count);
}

int* parse(char* filename, int* size)
{
    FILE* fptr = fopen(filename, "r");

    fscanf(fptr, "%d\n", size);
    int* arr = malloc(sizeof(int) * (*size));

    int turn;
    char dir;
    int ix = 0;
    while (fscanf(fptr, "%c%d\n", &dir, &turn) == 2) {
        if (dir == 'L') {
            turn *= -1;
        }
        arr[ix++] = turn;
    }

    fclose(fptr);
    
    return arr;
}
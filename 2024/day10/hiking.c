#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * size of the grid for each input type
 *      test = 8    real = 45
 */

char* read_file(const char* filename, int size);
int find_trail(int* seen, const char* grid, int size, int x, int y, char height);
/*
int part_two(const char* buffer, long size);
int search_at(const char* buf, int x, int y);
*/

int main(int argc, char** argv)
{
    /* argument parsing */
    if (argc < 2) {
        fprintf(stderr, "Error: didn't specify enough arguments.\n");
        return 1;
    }
    int size = 0;
    char* filename;
    argv++;
    while (*argv) {
        if (!strcmp("real", *argv)) {
            filename = "input";
            size = 45;
        } else if (!strcmp("test", *argv)) {
            filename = "test";
            size = 8;
        } else {
            fprintf(stderr, "Unrecognised argument: \"%s\"\n", *argv);
            return 1;
        }
        argv++;
    }

    /* Read in the file to a buffer */
    char* grid = read_file(filename, size);
    if (grid == NULL) {
        return 1;
    }

    int sum = 0;
    int distinct = 0;
    for (int i = 0; i < size * size; i++) {
        if (grid[i] == '0') {
            int seen[size * size];
            for (int j = 0; j < size * size; j++) {
                seen[j] = 0;
            }
            distinct += find_trail(seen, grid, size, i % size, i / size, '1');
            for (int j = 0; j < size * size; j++) {
                if (seen[j] == 1) {
                    sum++;
                }
            }
        }
    }
    printf("\nsum of trailhead scores is: %d\n", sum);
    printf("\nnumber of distinct paths is: %d\n", distinct);

    free(grid);
    return 0;
}

/**
 * allocate memory to the buffer and then return the size of it
 */
char* read_file(const char* filename, int size) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        return NULL;
    }

    char* buffer = malloc(sizeof(char) * size * size);
    char c;
    for (int i = 0; i < size; i++) {
        fread(buffer + (i * size), sizeof(char), size, f);
        c = fgetc(f);
    }
    return buffer;
}

int find_trail(int* seen, const char* grid, int size, int x, int y, char height)
{
    if (grid[y * size + x] == '9') {
        printf("reached the end of a trail\n");
        seen[y * size + x] = 1;
        return 1;
    }
    int total = 0;
    if (x - 1 >= 0 && grid[y * size + (x - 1)] == height) {
        // printf("found \'%c\' at (%d, %d)\n", height, x - 1, y);
        total += find_trail(seen, grid, size, (x - 1), y, height + 1);
    }
    if (x + 1 < size && grid[y * size + (x + 1)] == height) {
        // printf("found \'%c\' at (%d, %d)\n", height, x + 1, y);
        total += find_trail(seen, grid, size, (x + 1), y, height + 1);
    }
    if (y - 1 >= 0 && grid[(y - 1) * size + x] == height) {
        // printf("found \'%c\' at (%d, %d)\n", height, x, y - 1);
        total += find_trail(seen, grid, size, x, (y - 1), height + 1);
    }
    if (y + 1 < size && grid[(y + 1) * size + x] == height) {
        // printf("found \'%c\' at (%d, %d)\n", height, x, y + 1);
        total += find_trail(seen, grid, size, x, (y + 1), height + 1);
    }
    return total;
}
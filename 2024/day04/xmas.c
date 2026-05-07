#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 140
#define COLS 141

char* read_file(const char* filename, long* size);
int part_one(const char* buffer, long size);
int part_two(const char* buffer, long size);
int search_at(const char* buf, int x, int y);

int main(int argc, char** argv)
{
    /* argument parsing */
    if (argc < 2) {
        fprintf(stderr, "Error: didn't specify which part to calculate.\n");
        return 1;
    }
    int method = 0;
    argv++;
    while (*argv && !strncmp("--", *argv, 2)) {
        if (!strcmp("p1", *argv + 2)) {
            method = 1;
        } else if (!strcmp("p2", *argv + 2)) {
            method = 2;
        } else {
            fprintf(stderr, "Unrecognised argument: \"%s\"\n", *argv);
            return 1;
        }
        argv++;
    }

    /* Read in the file to a buffer */
    long size;
    char* buffer = read_file("input", &size);
    if (buffer == NULL) {
        free(buffer);
        return 1;
    }

    int xmases;
    // Choose method
    if (method == 1) {
        xmases = part_one(buffer, size);
    } else { // part = 2
        xmases = part_two(buffer, size);
    }

    free(buffer);
    printf("number of xmas's were: %d\n", xmases);
    return 0;
}

/**
 * allocate memory to the buffer and then return the size of it
 */
char* read_file(const char* filename, long* size) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        return NULL;
    }

    // Go to end to get size
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    rewind(f);

    // Allocate buffer (+1 for null terminator)
    char* buffer = malloc(*size + 1);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    // Read entire file
    fread(buffer, 1, *size, f);
    buffer[*size] = '\0';

    fclose(f);
    return buffer;
}

/**
 * look for an 'X' and then check in all directions from that position
 */
int part_one(const char* buffer, long size)
{
    int total = 0;
    char c;
    for (long i = 0; i < size; i++) {
        c = buffer[i];
        if (c == 'X') {
            total += search_at(buffer, i % COLS, i / COLS);
        }
    }
    return total;
}

/**
 * check if x and y are within the bounds of the array
 */
bool is_valid(int x, int y)
{
    return (0 <= x && x < COLS) && (0 <= y && y < ROWS);
}

/**
 * try to match "mas" in direction specified by x_dir and y_dir
 */
int match_mas(const char* buf, int x, int y, int x_dir, int y_dir)
{
    char m, a, s;
    m = buf[(y + 1 * y_dir) * COLS + (x + 1 * x_dir)];
    a = buf[(y + 2 * y_dir) * COLS + (x + 2 * x_dir)];
    s = buf[(y + 3 * y_dir) * COLS + (x + 3 * x_dir)];
    // final check
    if (m == 'M' && a == 'A' && s == 'S') {
        return 1;
    }
    return 0;
}

/**
 * search for an "mas" in the 8 cardinal directions from an X
 */
int search_at(const char* buf, int x, int y)
{
    int total = 0;
    char m, a, s;
    if (is_valid(x + 3, y)) { // right
        total += match_mas(buf, x, y, 1, 0);
    }
    if (is_valid(x - 3, y)) { // left
        total += match_mas(buf, x, y, -1, 0);
    }
    if (is_valid(x, y - 3)) { // up
        total += match_mas(buf, x, y, 0, -1);
    }
    if (is_valid(x, y + 3)) { // down
        total += match_mas(buf, x, y, 0, 1);
    }
    if (is_valid(x + 3, y - 3)) { // up-right
        total += match_mas(buf, x, y, 1, -1);
    }
    if (is_valid(x + 3, y + 3)) { // down-right
        total += match_mas(buf, x, y, 1, 1);
    }
    if (is_valid(x - 3, y - 3)) { // up-left
        total += match_mas(buf, x, y, -1, -1);
    }
    if (is_valid(x - 3, y + 3)) { // down-left
        total += match_mas(buf, x, y, -1, 1);
    }

    return total;
}

/**
 * return false if the position is on the edge, true otherwise.
 */
bool check_around(const char* buf, long ix)
{
    int left = (ix - 1) % COLS;
    int right = (ix + 1) % COLS;
    int up = ix / COLS - 1;
    int down = ix / COLS + 1;
    return (0 <= left) && (right < COLS) &&
        (0 <= up) && (down < ROWS);
}

/**
 * check for M and S combinations by just adding up their integer values
 *    c1 c2
 *      A
 *    c3 c4
 * This is the format I check, and why I do:
 *      c1 + c4 == 160 && c2 + c3 == 160
 */
int part_two(const char* buffer, long size)
{
    int total = 0;
    char c;
    int c1, c2, c3, c4;
    for (long i = 0; i < size; i++) {
        c = buffer[i];
        if (c == 'A' && check_around(buffer, i)) {
            c1 = buffer[i - COLS - 1];
            c2 = buffer[i - COLS + 1];
            c3 = buffer[i + COLS - 1];
            c4 = buffer[i + COLS + 1];
            if (c1 + c4 == 160 && c2 + c3 == 160) { // we have a match!
                total += 1;
            }
        }
    }

    return total;
}

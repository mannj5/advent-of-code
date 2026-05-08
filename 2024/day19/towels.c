
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

enum Colour {
    WHITE,
    BLUE,
    BLACK,
    RED,
    GREEN,
    INVALID
};

enum State {
    UNKNOWN,
    POSSIBLE,
    IMPOSSIBLE
};

typedef char* string;

void _free(string** mem) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; mem[i][j] != NULL; j++) {
            free(mem[i][j]);
        }
        free(mem[i]);
    }
    free(mem);
}

uint8_t get_colour(char c) {
    switch (c) {
        case 'w': return WHITE;
        case 'u': return BLUE;
        case 'b': return BLACK;
        case 'r': return RED;
        case 'g': return GREEN;
    }
}

string** get_colour_array(FILE* fptr)
{
    string** arr = malloc(sizeof(string*) * 5);
    for (int i = 0; i < 5; i++) {
        arr[i] = malloc(sizeof(string) * 10);
    }
    
    int sizes[5] = {0};
    int caps[5] = {10, 10, 10, 10, 10};

    ssize_t len;
    char* token;
    size_t size;
    while (1) {
        token = NULL;
        len = getdelim(&token, &size, ',', fptr);
        token[len - 1] = '\0';
        uint8_t colour = get_colour(token[0]);

        if (sizes[colour] + 1 == caps[colour]) {
            caps[colour] <<= 1; // multiply by 2
            arr[colour] = realloc(arr[colour], sizeof(string) * caps[colour]); 
        }

        arr[colour][sizes[colour]++] = token;
        arr[colour][sizes[colour]] = NULL; // this lets me iterate through them

        if (fgetc(fptr) == '\n') {
            break;
        }
    }

    return arr;
}

// part 1
uint8_t possible(
    string** tokens, string line, int ix, long long* memo)
{
    // printf("-- parse %s", line + ix);
    if (line[ix] == '\n') {
        // printf("\t\tDONE\n");
        return POSSIBLE;
    }

    // memo:
    // 0 = unknown
    // 1 = possible
    // 2 = impossible
    if (memo[ix] != UNKNOWN) {
        return memo[ix];
    }

    uint8_t colour = get_colour(line[ix]);
    string* set = tokens[colour];

    for (int i = 0; set[i] != NULL; i++) {
        string token = set[i];
        size_t length = strlen(token);

        // try and match the current token
        if (!strncmp(token, line + ix, length)) {
            // printf("matched %s\n", token);
            if (possible(tokens, line, ix + length, memo) == POSSIBLE) {
                memo[ix] = POSSIBLE;
                return POSSIBLE;
            }
        }
    }
    // printf("no match\n");

    memo[ix] = IMPOSSIBLE;
    return IMPOSSIBLE;
}

// part 2
long long ways(
    string** tokens, string line, int ix, long long* memo)
{
    // printf("-- parse %s", line + ix);
    if (line[ix] == '\n') {
        // printf("\t\tDONE\n");
        memo[ix] = 1;
        return 1;
    }

    // memo:
    // -1 = unknown
    //  0 = impossible
    // >0 = impossible
    if (memo[ix] != -1) {
        return memo[ix];
    }

    uint8_t colour = get_colour(line[ix]);
    string* set = tokens[colour];
    long long total = 0;

    for (int i = 0; set[i] != NULL; i++) {
        string token = set[i];
        size_t length = strlen(token);

        // try and match the current token
        if (!strncmp(token, line + ix, length)) {
            // printf("matched %s\n", token);
            total += ways(tokens, line, ix + length, memo);
        }
    }
    // printf("no match\n");

    memo[ix] = total;
    return total;
}


// Try to parse a sentence (line) with the grammar in arr
int main(int argc, char** argv) {
    if (argc < 2) return 1;

    FILE* fptr = fopen(argv[1], "r");
    char*** tokens = get_colour_array(fptr);

    char c;
    c = fgetc(fptr); // consume the newline
    char* line = NULL;
    size_t size;
    long long total = 0;
    int count = 0;;
    while (getline(&line, &size, fptr) > 0) {
        long long* memo = calloc(size, sizeof(long long));
        if (possible(tokens, line, 0, memo) == POSSIBLE) {
            count++;
        }

        memset(memo, -1, size * sizeof(long long));
        total += ways(tokens, line, 0, memo);

        // printf("\n");
        free(memo);
    }
    printf("valid designs: %d\n", count);
    printf("total combinations: %lld\n", total);

    _free(tokens);
    fclose(fptr);
    return 0;
}
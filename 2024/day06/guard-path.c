#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ROWS 130
#define COLS 130

typedef enum {
    INVALID,
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

void trace_guard_path(char grid[ROWS][COLS + 1], int x, int y);
int check_for_loop(const char grid[ROWS][COLS + 1], int x, int y);

int main(int argc, char** argv) {

    if (argc < 3) {
        fprintf(stderr, "Error: not enough arguments given\n");
        return 1;
    }
    int method = atoi(argv[1]);

    FILE* fin = fopen(argv[2], "rb");
    if (!fin) {
        return 1;
    }
    
    // Starting position
    int start_x, start_y;

    /* Allocate the memory */
    char c;
    size_t nbytes;
    char grid[ROWS][COLS + 1];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            c = fgetc(fin);
            if (c == '^') {
                start_x = j; start_y = i;
            }
            grid[i][j] = c;
        }
        grid[i][COLS] = '\0';
        /* Skip the newline */
        c = fgetc(fin);
    }
    fclose(fin);

    /* Trace guard path with X's */
    trace_guard_path(grid, start_x, start_y);

    if (method == 1) {
        /* Count the X's */
        int total = 0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == 'X') {
                    total++;
                }
            }
        }
        printf("Guard visited %d unique tiles\n", total);
    } else if (method == 2) {
        /* For each 'X', place a '#' on that tile and simulate the guard to see if he loops */
        int total = 0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == 'X') {
                    grid[i][j] = '#';
                    total += check_for_loop(grid, start_x, start_y);
                    grid[i][j] = 'X';
                }
            }
        }
        printf("Found %d infinite loop positions\n", total);
    }

    return 0;
}

    /* Simulate the guard from his starting point and mark his path */
void trace_guard_path(char grid[ROWS][COLS + 1], int x, int y)
{
    Direction dir = UP;
    while (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        grid[y][x] = 'X';
        switch (dir) {
            case RIGHT:
                if (x + 1 < COLS && grid[y][x + 1] == '#') {
                    dir = DOWN;
                } else {
                    x++;
                }
                break;
            case LEFT:
                if (x - 1 >= 0 && grid[y][x - 1] == '#') {
                    dir = UP;
                } else {
                    x--;
                }
                break;
            case DOWN:
                if (y + 1 < ROWS && grid[y + 1][x] == '#') {
                    dir = LEFT;
                } else {
                    y++;
                }
                break;
            case UP:
                if (y - 1 >= 0 && grid[y - 1][x] == '#') {
                    dir = RIGHT;
                } else {
                    y--;
                }
                break;
        }
    }
}

    /* We detect a loop by checking if the guard has been at this tile facing the same direction before */
int check_for_loop(const char grid[ROWS][COLS + 1], int x, int y)
{
    Direction dirs[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            dirs[i][j] = INVALID;
        }
    }

    Direction dir = UP;
    while (x >= 0 && x < ROWS && y >= 0 && y < COLS) {
        if (dirs[y][x] & (1 << dir)) {
            return 1;
        }
        dirs[y][x] |= (1 << dir);

        switch (dir) {
            case RIGHT:
                if (x + 1 < COLS && grid[y][x + 1] == '#') {
                    dir = DOWN;
                } else {
                    x++;
                }
                break;
            case LEFT:
                if (x - 1 >= 0 && grid[y][x - 1] == '#') {
                    dir = UP;
                } else {
                    x--;
                }
                break;
            case DOWN:
                if (y + 1 < ROWS && grid[y + 1][x] == '#') {
                    dir = LEFT;
                } else {
                    y++;
                }
                break;
            case UP:
                if (y - 1 >= 0 && grid[y - 1][x] == '#') {
                    dir = RIGHT;
                } else {
                    y--;
                }
                break;
        }
    }
    return 0;
}

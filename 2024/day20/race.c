#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int8_t dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
//                      up     right   down    left

typedef struct {
    char** grid;
    uint8_t n;
    uint8_t x;
    uint8_t y;
} Maze;

int16_t** pathfind(Maze* maze);
Maze read_file(char* filename);

void _free(char** carr, int16_t** iarr, int n) {
    for (int i = 0; i < n; i++) {
        free(carr[i]);
        free(iarr[i]);
    }
    free(carr);
    free(iarr);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }

    Maze maze = read_file(argv[1]);
    int16_t** dist = pathfind(&maze);

    _free(maze.grid, dist, maze.n);

    return 0;
}

void print_maze(char** grid, int n, int curx, int cury, int x, int y) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == cury && j == curx) {
                printf("O");
            } else if (i == y && j == x) {
                printf("D");
            } else {
                printf("%c", grid[i][j]);
            }
        }
        printf("\n");
    }
}

int manhattan_distance(int x, int y, int nx, int ny) {
    return abs(x - nx) + abs(y - ny);
}

int find_cheats(Maze* maze, int16_t** dist, int time, int maxdist)
{
    int nx, ny;
    int x = maze->x;
    int y = maze->y;
    int total = 0;
    for (int dx = -maxdist; dx <= maxdist; dx++) {
        for (int dy = -maxdist; dy <= maxdist; dy++) {
            // if outside of range, skip
            nx = x + dx;
            ny = y + dy;
            int mandist = manhattan_distance(x, y, nx, ny);
            if (mandist > maxdist) {
                continue;
            }

            // if out of bounds, skip
            if (nx < 0 || nx >= maze->n) {
                continue;
            }
            if (ny < 0 || ny >= maze->n) {
                continue;
            }

            // not a valid cheat
            if (maze->grid[ny][nx] == '#') {
                continue;
            }

            // check if visited
            int oldtime = dist[ny][nx];
            if (oldtime == -1) {
                continue;
            }

            // check for above threshold
            if (time - oldtime - mandist >= 100) {
                total++;
            }
        }
    }
    return total;
}

// time is set to the time it will be after the step
int move(Maze* maze, int16_t** dist, int time)
{
    // figure out all the cheats from where you are first
    // part 1
    // int total = find_cheats(maze, dist, time, 2);
    // part 2
    int total = find_cheats(maze, dist, time, 20);
    
    int dx, dy;
    int x = maze->x;
    int y = maze->y;
    // now figure out where to move
    for (int i = 0; i < 4; i++) {
        dx = dirs[i][0];
        dy = dirs[i][1];
        
        // skip if already visited
        if (dist[y + dy][x + dx] != -1) {
            continue;
        }
        // skip if wall
        if (maze->grid[y + dy][x + dx] == '#') {
            continue;
        }
        // you only get here once
        maze->x += dx;
        maze->y += dy;
        dist[maze->y][maze->x] = time;
        break;
    }

    return total;
}


int16_t** pathfind(Maze* maze)
{
    int16_t** dist = malloc(sizeof(int16_t*) * maze->n);
    for (int i = 0; i < maze->n; i++) {
        dist[i] = malloc(sizeof(int16_t) * maze->n);
        memset(dist[i], -1, sizeof(int16_t) * maze->n);
    }

    int newx, newy;
    int time = 0;
    int total = 0;
    printf("starting pathfind...");
    dist[maze->y][maze->x] = 0;
    while (maze->grid[maze->y][maze->x] != 'E') {
        // try to move in every direction
        total += move(maze, dist, ++time);
        // printf("time at %d, %d is %d\n", maze->x, maze->y, dist[maze->y][maze->x]);
    }
    total += move(maze, dist, time);

    printf("done.\ntotal cheats found: %d\n", total);
    printf("time to complete course: %d\n", time);

    return dist;
}

Maze read_file(char* filename)
{
    FILE* fptr = fopen(filename, "r");

    Maze maze;
    int n;
    fscanf(fptr, "%d\n", &n);
    maze.grid = malloc(sizeof(char*) * n);

    char c;
    size_t size;
    for (int i = 0; i < n; i++) {
        maze.grid[i] = malloc(sizeof(char) * n);
        for (int j = 0; j < n; j++) {
            c = fgetc(fptr);
            maze.grid[i][j] = c;
            if (c == 'S') {
                maze.x = j;
                maze.y = i;
            }
        }
        c = fgetc(fptr);
    }
    fclose(fptr);
    maze.n = n;

    return maze;
}

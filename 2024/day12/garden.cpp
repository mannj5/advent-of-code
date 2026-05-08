#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <queue>
#include <utility>

typedef struct {
    int size;
    char* data;
} Grid;

const int diags[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

Grid* read_file(char* filename);
int solve(char* grid, int size,
        int (*func)(char*, int, int, int));
int count_edges(char* grid, int x, int y, int n);
int count_corners(char* grid, int x, int y, int n);
int bfs(char* grid, int* seen, int i, int n,
        int (*func)(char*, int, int, int));

int main(int argc, char** argv) {
    
    if (argc < 2) {
        return 1;
    }

    Grid* grid = read_file(argv[1]);
    if (grid == NULL) {
        return 1;
    }

    int ans = solve(grid->data, grid->size, count_edges);
    printf("part 1: %d\n", ans);
    ans = solve(grid->data, grid->size, count_corners);
    printf("part 2: %d\n", ans);

    delete grid->data;
    delete grid;

    return 0;
}

Grid* read_file(char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int size = (int) ftell(f);
    rewind(f);

    Grid* grid = new Grid;
    grid->data = new char[size];
    char c;
    for (int i = 0; i < size; i++) {
        c = fgetc(f);
        if (c == '\n') {
            c = fgetc(f);
            size--;
        }
        grid->data[i] = c;
    }
    grid->size = size;
    // printf("read %d bytes\n", size);
    fclose(f);

    return grid;
}

bool in_region(char* grid, int x, int y, int n, char c)
{
    return 0 <= x && x < n && 0 <= y && y < n && grid[y * n + x] == c;
}

bool in_visited(char* grid, int* seen, int x, int y, int n, char c)
{
    return in_region(grid, x, y, n, c) && seen[y * n + x] == 0;
}

int count_edges(char* grid, int x, int y, int n)
{
    char c = grid[y * n + x];
    int result = 4;

    int dx, dy;
    for (int i = 0; i < 4; i++) {
        dx = dirs[i][0];
        dy = dirs[i][1];
        if (in_region(grid, x + dx, y + dy, n, c)) {
            result--;
        }
    }

    return result;
}

int count_corners(char* grid, int x, int y, int n)
{
    char c = grid[y * n + x];
    int corners = 0;
    for (int i = 0; i < 4; i++) {

        int dx = diags[i][0];
        int dy = diags[i][1];

        bool a = in_region(grid, x + dx, y, n, c);
        bool b = in_region(grid, x, y + dy, n, c);
        bool diag = in_region(grid, x + dx, y + dy, n, c);

        if (!a && !b) {
            corners++;
        }
        if (a && b && !diag) {
            corners++;
        }
    }
    // printf("found %d corner in region %c\n", corners, c);
    
    return corners;
}

int bfs(char* grid, int* seen, int i, int n,
        int (*func)(char*, int, int, int))
{
    std::queue<std::pair<int, int>> q;
    int y = i / n;
    int x = i % n;
    
    // Push the first position onto the queue
    q.push({x, y});
    seen[y * n + x] = 1;
    // the letter of the region we will check
    char c = grid[y * n + x];
 
    std::pair<int, int> p;
    int area = 0;
    int edges = 0;
   
    int dx, dy;
    while (!q.empty()) {
        p = q.front();
        x = p.first;
        y = p.second;
        area++;
        edges += func(grid, x, y, n);
        q.pop();

        for (int i = 0; i < 4; i++) {
            dx = dirs[i][0];
            dy = dirs[i][1];
            if (in_visited(grid, seen, x + dx, y + dy, n, c)) {
                seen[(y + dy) * n + (x + dx)] = 1;
                q.push({x + dx, y + dy});
                // printf("pushed (%d, %d) into the queue\n", x + dx, y + dy);
            }
        }
    }

    // printf("block %c has area %d and perimeter %d\n", c, area, edges);

    return area * edges;
}

int solve(char* grid, int size,
        int (*func)(char*, int, int, int))
{
    int n = std::sqrt(size);
    int* seen = new int[size]();
    int total = 0;
    for (int i = 0; i < size; i++) {
        if (seen[i] == 0) {
            // printf("Performing bfs on block %c\n", grid[i]);
            total += bfs(grid, seen, i, n, func);
        }
    }

    return total;
}

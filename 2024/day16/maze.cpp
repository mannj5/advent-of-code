#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct State {
    int x, y, dir, cost;

    bool operator<(const State& other) const {
        return cost < other.cost;
    }
};

struct Position {
    int x, y;

    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct Grid {
    Position start;
    Position end;
    vector<vector<char>> grid;
    int size;

    Grid(int n) : grid(n, vector<char>(n)) {}
};

int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};

// Function Declarations //////////////////
Grid* read_file(char* filename);
void search(Position start, vector<vector<char>> grid,
    vector<vector<vector<int>>>& dist, int moveDir, int n);
///////////////////////////////////////////


int main(int argc, char** argv) {
    // put grid into a struct
    Grid* g = read_file(argv[1]);
    int n = g->size;
    vector<vector<vector<int>>> sdist(n,
        vector<vector<int>>(n,
        vector<int>(4, INT_MAX)));
    search(g->start, g->grid, sdist, 1, n);


    // Part 1
    int min = INT_MAX;
    for (auto &d : sdist[g->end.x][g->end.y]) {
        if (d < min) {
            min = d;
        }
    }
    cout << ">> lowest cost is " << min << endl;

    // Part 2
    vector<vector<vector<int>>> edist(n,
        vector<vector<int>>(n,
        vector<int>(4, INT_MAX)));
    search(g->end, g->grid, edist, -1, n);

    int tiles = 0;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            for (int d = 0; d < 4; d++) {
                if (sdist[x][y][d] + edist[x][y][d] == min) {
                    tiles++;
                    break;
                }
            }
        }
    }
    cout << ">> total number of valid tiles is " << tiles << endl;

    delete g;
    return 0;
}

void search(Position start, vector<vector<char>> grid,
    vector<vector<vector<int>>>& dist, int moveDir, int n)
{
    priority_queue<State> pq;

    // if doing the backwards traversal, init all directions
    if (moveDir == -1) {
        for (int d = 0; d < 4; d++) {
            dist[start.x][start.y][d] = 0;
            pq.push({start.x, start.y, d, 0});
        }
    }
    
    dist[start.x][start.y][EAST] = 0;
    pq.push({start.x, start.y, EAST, 0});

    State cur;
    while (!pq.empty()) {
        cur = pq.top();
        pq.pop();

        if (cur.cost > dist[cur.x][cur.y][cur.dir]) {
            continue;
        }

        // consider the case of taking a step forward (or backward)
        int nx = cur.x + moveDir * dx[cur.dir];
        int ny = cur.y + moveDir * dy[cur.dir];

        if (0 <= ny && ny < n && 0 <= nx && nx < n &&
            grid[ny][nx] != '#') {

            if (dist[nx][ny][cur.dir] > cur.cost + 1) {
                dist[nx][ny][cur.dir] = cur.cost + 1;
                pq.push({nx, ny, cur.dir, cur.cost + 1});
            }
        }

        // now consider turning first
        for (int turn : {-1, 1}) {
            int ndir = (cur.dir + turn + 4) % 4; // mod 4

            int ncost = cur.cost + 1000;
            if (dist[cur.x][cur.y][ndir] > ncost) {
                dist[cur.x][cur.y][ndir] = ncost;
                pq.push({cur.x, cur.y, ndir, ncost});
            }
        }
    }
}

Grid* read_file(char* filename)
{
    ifstream fptr(filename);
    int n;
    fptr >> n;
    fptr.ignore();

    Grid* g = new Grid(n);
    char c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fptr.get(c);
            g->grid[i][j] = c;
            if (c == 'S') {
                g->start = {j, i};
            } else if (c == 'E') {
                g->end = {j, i};
            }
        }
        fptr.ignore();
    }
    
    g->size = n;
    return g;
}
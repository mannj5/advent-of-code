
#include <iostream>
#include <fstream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

vector<vector<int>> bfs(vector<vector<char>> grid, int n, int startx, int starty);

int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "not enough arguments given\n";
        return 1;
    }

    ifstream fptr(argv[1]);
    if (!fptr.is_open()) {
        perror("fopen");
    }

    int n;
    fptr >> n;
    vector<vector<char>> grid(n, vector<char>(n));
    // build the grid
    char c;
    int x, y;
    for (int i = 0; i < 1024; i++) {
        fptr >> x >> c >> y;
        grid[y][x] = '#';
    }

    vector<vector<int>> sdist = bfs(grid, n, 0, 0);
    // part 1
    int mindist = sdist[n - 1][n - 1];
    printf("min distance: %d\n", mindist);


    vector<vector<int>> edist = bfs(grid, n, n - 1, n - 1);
    while (sdist[n - 1][n - 1] > 0) {
        // Check if the next byte is on the shortest path
        fptr >> x >> c >> y;
        grid[y][x] = '#';
        if (sdist[y][x] + edist[y][x] == mindist) {
            // reconstruct the shortest path
            sdist = bfs(grid, n, 0, 0);
            edist = bfs(grid, n, n - 1, n - 1);
            mindist = sdist[n - 1][n - 1];
        }
    }
    printf("the byte that cut off the path landed on (%d, %d)\n", x, y);

    fptr.close();
    return 0;
}

vector<vector<int>> bfs(vector<vector<char>> grid, int n, int startx, int starty)
{
    queue<pair<int, int>> q;
    vector<vector<int>> dist(n, vector<int>(n));

    q.push({startx, starty});
    dist[startx][starty] = 0;

    // (x, y) pair
    int x, y;
    pair<int, int> pos;
    while (!q.empty()) {
        pos = q.front();
        q.pop();

        x = pos.first;
        y = pos.second;
        
        int dx, dy;
        for (int i = 0; i < 4; i++) {
            int newx = x + dirs[i][0];
            if (newx < 0 || newx >= n) {
                continue;
            }

            int newy = y + dirs[i][1];
            if (newy < 0 || newy >= n) {
                continue;
            }

            if (grid[newy][newx] == '#') {
                continue;
            }

            if (dist[newy][newx] == 0) {
                dist[newy][newx] = dist[y][x] + 1;
                q.push({newx, newy});                
            }
        }
    }
    return dist;
}
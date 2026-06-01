#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>

using namespace std;

vector<string> parse(char* filename, int* n);
int solve1(vector<string> grid, int n);
int solve2(vector<string> grid, int n);


int main(int argc, char** argv) {

    int n;
    vector<string> grid = parse(argv[1], &n);

    cout << "part 1: " << solve1(grid, n) << endl;
    cout << "part 1: " << solve2(grid, n) << endl;


    return 0;
}

int check_pos(vector<string> grid, int x, int y, int n)
{
    int count = 0;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            if (i < 0 || i >= n) {
                continue;
            }
            if (j < 0 || j >= n) {
                continue;
            }
            if (i == y && j == x) {
                continue;
            }
            if (grid[i][j] == '@') {
                count++;
            }
        }
    }
    if (count < 4) {
        return 1;
    }
    return 0;
}

int solve2(vector<string> grid, int n)
{
    int total = 0;
    vector<tuple<int, int>> removed;
    do {
        removed.clear();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(grid[i][j] == '@' && check_pos(grid, j, i, n)) {
                    total++;
                    removed.push_back({j, i});
                }
            }
        }

        for (const auto& [x, y] : removed) {
            grid[y][x] = '.';
        }
    } while (removed.size() > 0);

    return total;
}

int solve1(vector<string> grid, int n)
{
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(grid[i][j] == '@') {
                total += check_pos(grid, j, i, n);
            }
        }
    }

    return total;
}

vector<string> parse(char* filename, int* n)
{
    ifstream fptr(filename);

    int size;
    fptr >> size;
    fptr.ignore();
    vector<string> grid(size);

    string line;
    int i = 0;
    while (getline(fptr, line)) {
        grid[i] = line;
        i++;
    }

    fptr.close();
    *n = size;
    return grid;
}
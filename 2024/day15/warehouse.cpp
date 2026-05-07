#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef struct {
    int x, y;
} Position;

void move(Position dir, vector<vector<char>>& grid, Position& pos)
{
    int x = pos.x;
    int y = pos.y;
    int n = grid.size();
    while (0 <= x && x < n && 0 <= y && y < n) {
        if (grid[y][x] == '#') {
            return;
        }
        if (grid[y][x] == '.') {
            // the robot is able to move in this direction
            // unwind steps and replace one step in that direction
            while (!(x == pos.x && y == pos.y)) {
                grid[y][x] = grid[y - dir.y][x - dir.x];
                x -= dir.x;
                y -= dir.y;
            }
            grid[y][x] = '.';
            pos.x += dir.x;
            pos.y += dir.y;
            // cout << "moved to " << pos.x << ", " << pos.y << endl;
            return;
        }
        x += dir.x;
        y += dir.y;
    }
    // cout << "Robot couldn't move\n";
}


int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "Error: not enough arguments given\n";
        return 1;
    }

    ifstream fptr(argv[1]);
    int n;
    fptr >> n;
    fptr.ignore();

    Position pos;
    char c;
    vector<vector<char>> grid(n, vector<char>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fptr.get(c);
            if (c == '@') {
                pos.y = i;
                pos.x = j;
            }
            grid[i][j] = c;
        }
        fptr.ignore();
    }

    fptr.ignore();
    while (fptr.get(c)) {
        switch (c) {
            case 'v':
                move({0, 1}, grid, pos);
                break;
            case '^':
                move({0, -1}, grid, pos);
                break;
            case '>':
                move({1, 0}, grid, pos);
                break;
            case '<':
                move({-1, 0}, grid, pos);
                break;
            default:
                break;
        }
    }
    
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // cout << grid[i][j];
            if (grid[i][j] == 'O') {
                total += (100 * i) + j;
            }
        }
        // cout << endl;
    }
    cout << ">> sum of box GPS coords is " << total << endl;

    fptr.close();
    return 0;
}
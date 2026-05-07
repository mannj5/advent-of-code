#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

struct Position {
    int x, y;

    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

void move_horizontal(int dir, vector<vector<char>>& grid, Position& pos)
{
    int y = pos.y;
    int x = pos.x;
    int n = grid[0].size();
    while (0 <= x && x < n) {
        if (grid[y][x] == '#') {
            return;
        }
        if (grid[y][x] == '.') {
            // the robot is able to move in this direction
            // unwind steps and replace one step in that direction
            while (x != pos.x) {
                grid[y][x] = grid[y][x - dir];
                x -= dir;
            }
            grid[y][x] = '.';
            pos.x += dir;
            // cout << "moved to " << pos.x << ", " << pos.y << endl;
            return;
        }
        x += dir;
    }
    // cout << "Robot couldn't move\n";
}

// Position is left-aligned
bool check_move(int dir, vector<vector<char>>& grid, Position pos, set<Position>& boxes)
{
    if (grid[pos.y][pos.x] == ']') {
        pos.x--;
    }

    Position left = {pos.x, pos.y + dir};
    Position right = {pos.x + 1, pos.y + dir};
    char nextl = grid[pos.y + dir][pos.x];
    char nextr = grid[pos.y + dir][pos.x + 1];

    if (nextl == '#' || nextr == '#') {
        return false;
    }

    if (nextl == '.' && nextr == '.') {
        boxes.insert(pos);
        return true;
    }

    if (nextl == '.') {
        if (check_move(dir, grid, right, boxes)) {
            boxes.insert(pos);
            return true;
        }
        return false;
    }

    if (nextr == '.') {
        if (check_move(dir, grid, left, boxes)) {
            boxes.insert(pos);
            return true;
        }
        return false;
    }

    if (check_move(dir, grid, left, boxes) && check_move(dir, grid, right, boxes)) {
        boxes.insert(pos);
        return true;
    }
    return false;
}

void move_boxes(int dir, vector<vector<char>>& grid, const set<Position>& boxes)
{
    for (auto &b : boxes) {
        grid[b.y][b.x] = '.';
        grid[b.y][b.x + 1] = '.';
    }
    for (auto &b : boxes) {
        grid[b.y + dir][b.x] = '[';
        grid[b.y + dir][b.x + 1] = ']';
    }
}

void move_robot(int dir, vector<vector<char>>& grid, Position& pos)
{
    grid[pos.y + dir][pos.x] = '@';
    grid[pos.y][pos.x] = '.';
    pos.y += dir;
}

void move_vertical(int dir, vector<vector<char>>& grid, Position& pos)
{
    int y = pos.y;
    int x = pos.x;
    // If you can move without moving boxes, do it
    if (grid[pos.y + dir][x] == '.') {
        move_robot(dir, grid, pos);
        return;
    }
    // The immediate neighbour was not free, check with boxes now
    int n = grid.size();
    while (0 <= y && y < n) {
        // There were no free spaces
        if (grid[y][x] == '#') {
            return;
        }
        if (grid[y][x] == '.') {
            // traverse the box 'tree'
            set<Position> boxes;
            if (check_move(dir, grid, {pos.x, pos.y + dir}, boxes)) {
                move_boxes(dir, grid, boxes);
                move_robot(dir, grid, pos);
                return;
            }
            // add the boxes to a vector
        }
        y += dir;
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

    // Read the file and create grid
    Position pos;
    char c;
    int ix;
    char tiles[4][2] = {{'#', '#'}, {'[', ']'}, {'@', '.'}, {'.', '.'}};
    vector<vector<char>> grid(n, vector<char>(2 * n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fptr.get(c);
            switch (c) {
                case '#':
                    ix = 0;
                    break;
                case 'O':
                    ix = 1;
                    break;
                case '@':
                    pos.x = 2 * j;
                    pos.y = i;
                    ix = 2;
                    break;
                default:
                    ix = 3;
                    break;
            }
            grid[i][2 * j] = tiles[ix][0];
            grid[i][2 * j + 1] = tiles[ix][1];
        }
        fptr.ignore();
    }

    fptr.ignore();
    while (fptr.get(c)) {
        switch (c) {
            case 'v':
                move_vertical(1, grid, pos);
                break;
            case '^':
                move_vertical(-1, grid, pos);
                break;
            case '>':
                move_horizontal(1, grid, pos);
                break;
            case '<':
                move_horizontal(-1, grid, pos);
                break;
            default:
                break;
        }
    }

    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 2*n; j++) {
            cout << grid[i][j];
            if (grid[i][j] == '[') {
                total += (100 * i) + j;
            }
        }
        cout << endl;
    }
    cout << ">> sum of box GPS coords is " << total << endl;

    fptr.close();
    return 0;
}
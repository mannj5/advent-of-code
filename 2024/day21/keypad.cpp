
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <climits>

using namespace std;

const char keypos[4][3] =
    {{'7', '8', '9'},
     {'4', '5', '6'},
     {'1', '2', '3'},
     {' ', '0', 'A'}};

const char arrowpos[2][3] =
    {{' ', '^', 'A'},
     {'<', 'v', '>'}};

map<char, int> int_values =
    {{' ', 0}, {'^', 1}, {'A', 2},
    {'<', 3}, {'v', 4}, {'>', 5}};

/* Map of shortest strings for each transition  */
map<pair<char, char>, vector<string>> keypaths;
map<pair<char, char>, vector<string>> arrowpaths;
long long memo[26][6][6] = {0};

/* Build the shortest paths for the number pad */
vector<string> get_num_path(int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;

    string horiz, vert;
    if (dy > 0) {
        vert = string(dy, 'v');
    } else {
        vert = string(-dy, '^');
    }
    if (dx > 0) {
        horiz = string(dx, '>');
    } else {
        horiz = string(-dx, '<');
    }

    vector<string> vec;

    // Pass for numpad

    // if it takes you through the empty space, skip
    if (dx != 0 && keypos[sy][sx + dx] != ' ') {
        vec.push_back(horiz + vert + "A");
    }
    if (dy != 0 && keypos[sy + dy][sx] != ' ') {
        vec.push_back(vert + horiz + "A");
    }

    return vec;
}

/* Build the shortest paths for the arrow pad */
vector<string> get_arrow_path(int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;

    string horiz, vert;
    if (dy > 0) {
        vert = string(dy, 'v');
    } else {
        vert = string(-dy, '^');
    }
    if (dx > 0) {
        horiz = string(dx, '>');
    } else {
        horiz = string(-dx, '<');
    }

    vector<string> vec;

    // Pass for numpad

    // if it takes you through the empty space, skip
    if (dx != 0 && arrowpos[sy][sx + dx] != ' ') {
        vec.push_back(horiz + vert + "A");
    }
    if (dy != 0 && arrowpos[sy + dy][sx] != ' ') {
        vec.push_back(vert + horiz + "A");
    }

    return vec;
}

void build_paths(void) {
    for (int sy = 0; sy < 4; sy++) {
        for (int sx = 0; sx < 3; sx++) {

            for (int ey = 0; ey < 4; ey++) {
                for (int ex = 0; ex < 3; ex++) {
                    vector<string> val = get_num_path(sx, sy, ex, ey);
                    pair<char, char> key = make_pair(keypos[sy][sx], keypos[ey][ex]);
                    keypaths[key] = val;

                    if (sy > 1 || ey > 1) {
                        continue;
                    }
                    val = get_arrow_path(sx, sy, ex, ey);
                    key = make_pair(arrowpos[sy][sx], arrowpos[ey][ex]);
                    arrowpaths[key] = val;
                }
            }
        }
    }
}

/* Solve the subsequent states (which use the arrow pad) */
long long solve_state(int depth, char src, char dst) {
    if (src == dst) {
        return 1;
    }

    int srcix = int_values[src];
    int dstix = int_values[dst];
    long long min = memo[depth][srcix][dstix];
    if (min != 0) {
        return min;
    }

    min = LLONG_MAX;
    if (depth == 0) {
        string str = arrowpaths.at({src, dst}).at(0);
        min = str.length();
        memo[depth][srcix][dstix] = min;
        return min;
    }

    long long total;
    for (auto &str : arrowpaths.at({src, dst})) {
        total = 0;
        char prev = 'A';

        for (auto &c : str) {
            total += solve_state(depth - 1, prev, c);
            prev = c;
        }

        if (total < min) {
            min = total;
        }
    }
    //
    memo[depth][srcix][dstix] = min;
    return min;
}

/* Solve the first state (uses the number pad) */
long long first_state(char src, char dst) {
    long long total;
    long long min = LLONG_MAX;

    for (auto &str : keypaths.at({src, dst})) {
        total = 0;
        char prev = 'A';

        for (auto &c : str) {
            total += solve_state(24, prev, c);
            prev = c;
        }

        if (total < min) {
            min = total;
        }
    }
    return min;
}

int main() {
    // char arr[5][5] = {"029A", "980A", "179A", "456A", "379A"};
    char arr[5][5] = {"129A", "176A", "169A", "805A", "208A"};
    build_paths();
    long long total = 0;
    for (int i = 0; i < 5; i++) {
        long long complexity = 0;
        char prev = 'A';
        char* ex = arr[i];
        while (*ex) {
            complexity += first_state(prev, *ex);
            prev = *ex;
            ex++;
        }
        ex -= 4;
        ex[3] = '\0';

        total += complexity * atoi(ex);
        
    }
    printf("Total complexity = %lld\n", total);

    return 0;
}
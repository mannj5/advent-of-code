#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int x, y;
    int dx, dy;
} Robot;

int W, H;

int step(vector<Robot>& vec)
{
    int q1 = 0; int q2 = 0; int q3 = 0; int q4 = 0;
    for (auto &r : vec) {
        // make a step
        r.x = (r.x + r.dx + W) % W;
        r.y = (r.y + r.dy + H) % H;
        
        if (r.x < W / 2) {
            if (r.y < H / 2) {
                q1++;
            } else if (r.y > H / 2) {
                q2++;
            }
        } else if (r.x > W / 2) {
            if (r.y < H / 2) {
                q3++;
            } else if (r.y > H / 2) {
                q4++;
            }
        }
    }

    return q1 * q2 * q3 * q4;
}

int calc_safety(const vector<Robot>& vec)
{
    int q1 = 0; int q2 = 0; int q3 = 0; int q4 = 0;
    for (auto &r : vec) {
        if (r.x < W / 2) {
            if (r.y < H / 2) {
                q1++;
            } else if (r.y > H / 2) {
                q2++;
            }
        } else if (r.x > W / 2) {
            if (r.y < H / 2) {
                q3++;
            } else if (r.y > H / 2) {
                q4++;
            }
        }
    }

    return q1 * q2 * q3 * q4;
}

void render(const vector<Robot>& vec)
{
    vector<string> grid(H, string(W, '.'));

    for (auto &r : vec) {
        grid[r.y][r.x] = '#';
    }

    // clear screen
    cout << "\033[H";

    for (auto &row : grid) {
        for (auto c : row) {
            cout << c << " ";
        }
        cout << "\n";
    }
}

int main(int argc, char** argv) {
    // setup
    if (argc < 4) {
        cerr << "Error: not enough arguments given\n";
    }
    H = stoi(argv[1]);
    W = stoi(argv[2]);
    ifstream fin(argv[3]);
    int x, y, dx, dy, n;
    char c;

    fin >> n; // number of entries in the file
    vector<Robot> robots(n);
    for (int i = 0; i < n; i++) {
        // weird c++ parsing lol
        fin >> c >> c >> x >> c >> y >> c >> c >> dx >> c >> dy;
        robots[i] = (Robot) {x, y, dx, dy};
    }

    // part 1
    int sf;
    /*
    for (int t = 0; t < 100; t++) {
        sf = step(robots);
    }

    cout << ">> Safety factor is " << sf << endl;
    */
    
    // part 2
    int best_t;
    int best_sf = INT_MAX;
    for (int t = 0; t < 101 * 103; t++) {
        sf = step(robots);
        if (sf < best_sf) {
            best_sf = sf;
            best_t = t;
            render(robots);
        }
    }
    cout << "The lowest safety factor was found on iteration " << (best_t + 1) << endl;

    // cleanup
    fin.close();
    return 0;
}
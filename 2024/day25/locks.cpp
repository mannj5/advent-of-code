#include <iostream>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

using arr5 = array<int, 5>;

void parse(char* filename, vector<arr5>& keys, vector<arr5>& locks);
int can_unlock(arr5 key, arr5 lock);

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Error: no file given\n";
        return 1;
    }

    vector<arr5> keys, locks;
    parse(argv[1], keys, locks);

    int total = 0;
    for (arr5 key : keys) {
        for (arr5 lock : locks) {
            total += can_unlock(key, lock);
        }
    }

    // part 1
    cout << total << " unique key/lock combinations\n";

    return 0;
}

int can_unlock(arr5 key, arr5 lock)
{
    for (int i = 0; i < 5; i++) {
        if (key[i] + lock[i] > 5) {
            return 0;
        }
    }
    return 1;
}

void parse(char* filename, vector<arr5>& keys, vector<arr5>& locks)
{
    ifstream fptr(filename);

    string line;
    while (getline(fptr, line)) {

        if (line.empty()) {
            continue;
        }

        // process locks
        if (line[0] == '#') {
            arr5 heights = {0, 0, 0, 0, 0};
            for (int r = 0; r < 6; r++) {
                getline(fptr, line);
                // process a line of the lock
                for (int c = 0; c < 5; c++) {
                    if (line[c] == '#') {
                        heights[c]++;
                    }
                }
            }
            locks.push_back(heights);
        } else if (line[0] == '.') { // process keys
            arr5 heights = {5, 5, 5, 5, 5};
            for (int r = 0; r < 6; r++) {
                getline(fptr, line);
                // process a line of the lock
                for (int c = 0; c < 5; c++) {
                    if (line[c] == '.') {
                        heights[c]--;
                    }
                }
            }
            keys.push_back(heights);
        }
    }

    fptr.close();
}
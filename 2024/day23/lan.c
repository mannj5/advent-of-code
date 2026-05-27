#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define size 676

bool adj[size][size] = {0};

void parse(char* filename);
int part_one(void);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: no file given\n");
        return 1;
    }

    // 26^2 possible combinations
    parse(argv[1]);

    // part one answer
    printf("T-triangles: %d\n", part_one());

    return 0;
}

int part_one(void)
{
    int triangles = 0;
    for (int i = 0; i < size - 2; i++) {
        for (int j = i + 1; j < size - 1; j++) {
            if (!adj[i][j]) { // check if connected
                continue;
            }
            for (int k = j + 1; k < size; k++) {
                if (adj[i][k] && adj[j][k]) {

                    // check that at least one of the
                    // computers starts with a t
                    bool t1 = (i / 26) == 19;
                    bool t2 = (j / 26) == 19;
                    bool t3 = (k / 26) == 19;

                    if (t1 || t2 || t3) {
                        triangles++;
                    } 
                }
            }
        }
    }

    return triangles;
}

void parse(char* filename)
{
    FILE* fptr = fopen(filename, "r");

    int A, B;
    char a1, a2, b1, b2;
    while (fscanf(fptr, "%c%c-%c%c\n", &a1, &a2, &b1, &b2) == 4) {

        A = (a1 - 'a') * 26 + (a2 - 'a');
        B = (b1 - 'a') * 26 + (b2 - 'a');

        adj[A][B] = true;
        adj[B][A] = true;
    }

    fclose(fptr);
}
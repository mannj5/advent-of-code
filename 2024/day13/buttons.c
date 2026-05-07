#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Error: not enough arguments given\n");
        return 1;
    }

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror(argv[1]);
        return 1;
    }

    // uncomment for part 2
    // long offset = 10000000000000UL;
    long offset = 0UL;

    long total = 0;
    long a, b, c, d, e, f;
    long D, Dx, Dy;
    /* Part one, use determinant trick
     * 
     * ax by = e
     * cx dy = f
     *
     */
    while (fscanf(fptr,
        "Button A: X+%ld, Y+%ld\n"
        "Button B: X+%ld, Y+%ld\n"
        "Prize: X=%ld, Y=%ld\n",
        &a, &c, &b, &d, &e, &f) == 6) {
            
        e += offset;
        f += offset;

        // printf("got the system:\n\t%ldA + %ldB = %ld\n", a, b, e);
        // printf("\t%ldA + %ldB = %ld\n", b, d, f);

        D = a * d - b * c;
        if (D == 0) {
            // printf("System is inconsistent\n");
            continue;
        }
        Dx = e * d - b * f;
        Dy = a * f - e * c;
        if (Dx == 0 && Dy == 0) {
            // printf("System is indeterminate\n");
            continue;
        }
        if (Dx % D != 0 || Dy % D != 0) {
            // printf("System does not have integer solutions\n");
            continue;
        }

        // printf("The solutions are x=%d, y=%d\n", Dx / D, Dy / D);
        total += 3 * (Dx / D) + (Dy / D);
    }

    printf("\n >> Total cost of presses is %ld\n", total);
    return 0;
}

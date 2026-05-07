#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CMD_SIZE 12

const char* const usageMsg = "Usage: ./day3 <filename>";
bool mulEnabled = true;

int parse_file(FILE* fin);
int parse_mul(FILE* fin);
void parse_toggle(FILE* fin);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s\n", usageMsg);
        return EXIT_FAILURE;
    }

    FILE* fin = fopen(argv[1], "r");
    if (fin == NULL) {
        perror("error");
        return EXIT_FAILURE;
    }

    printf("Total: %d\n", parse_file(fin));
    return EXIT_SUCCESS;
}

int parse_file(FILE* fin)
{
    int total = 0;
    int c = 0;
    while (c != EOF) {
        c = fgetc(fin);
        switch (c) {
            case 'd':
                parse_toggle(fin);
                break;
            case 'm':
                total += parse_mul(fin);
                break;
            default:
                break;
        }
    }
    return total;
}

int parse_mul(FILE* fin)
{
    if (mulEnabled == false) {
        return 0;
    }

    char p;
    int n1, n2;
    if (fscanf(fin, "ul(%d,%d%c", &n1, &n2, &p) == 3) {
        if (p == ')') {
            return n1 * n2;
        }
    }
    return 0;
}

void parse_toggle(FILE* fin)
{
    char c = fgetc(fin);
    if (c == 'o') {
        mulEnabled = true;
    }
    c = fgetc(fin);
    if (c == 'n') {
        mulEnabled = false;
    }
}
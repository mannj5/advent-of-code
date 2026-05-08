
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// for the real input
int program[16] = {2,4,1,6,7,5,4,6,1,4,5,5,0,3,3,0};

typedef struct {
    uint64_t A, B, C;
    int ip;
    int* program;
    int len;
} VM;

uint64_t combo(VM* vm, int operand) {
    switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4: return vm->A;
        case 5: return vm->B;
        case 6: return vm->C;
        case 7: exit(0);
    }
}

VM* init_vm(char* filename)
{
    FILE* fptr = fopen(filename, "r");

    VM* vm = malloc(sizeof(VM));
    fscanf(fptr, "Register A: %llu\n", &vm->A);
    fscanf(fptr, "Register B: %llu\n", &vm->B);
    fscanf(fptr, "Register C: %llu\n", &vm->C);
    fscanf(fptr, "%d\n", &(vm->len));
    vm->ip = 0;

    vm->program = malloc(sizeof(int) * vm->len);
    fscanf(fptr, "Program: %d", vm->program);

    int c;
    int ix = 1;
    while ((c = fgetc(fptr)) != EOF) {
        if (c == ',') {
            continue;
        }
        vm->program[ix] = c - '0';
        ix++;
    }

    fclose(fptr);
    return vm;
}

void run(VM* vm)
{
    int jump;
    int opcode, operand;
    while (vm->ip < vm->len) {
        opcode = vm->program[vm->ip];
        operand = vm->program[vm->ip + 1];
        jump = 0;

        switch (opcode) {
            case 0: // adv
                vm->A >>= combo(vm, operand);
                break;
            case 1: // bxl
                vm->B ^= operand;
                break;
            case 2: // bst
                vm->B = combo(vm, operand) & 7ULL;
                break;
            case 3: // jnz
                if (vm->A != 0) {
                    vm->ip = operand;
                    jump = 1;
                }
                break;
            case 4: // bxc
                vm->B ^= vm->C;
                break;
            case 5: // out
                printf("%d,", combo(vm, operand) & 7ULL);
                break;
            case 6: // bdv
                vm->B = vm->A >> combo(vm, operand);
                break;
            case 7: // cdv
                vm->C = vm->A >> combo(vm, operand);
                break;
        }

        vm->ip += 2;
        if (jump) {
            vm->ip -= 2;
        }
    }
    printf("\b \n");
}

int step(long long A) {
    int B = (A & 7) ^ 6;
    int C = A >> B;
    return (B ^ C ^ 4) & 7;
}

int solve(long long A, int ix) {
    if (ix < 0) {
        printf("%lld\n", A >> 3);
        return 1;
    }

    for (int i = 0; i < 8; i++) {
        if (step(A | i) == program[ix]) {
            if (solve((A | i) << 3, ix - 1)) {
                return 1;
            }
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    /*
    if (argc < 2) {
        return 1;
    }

    VM* vm = init_vm(argv[1]);
    run(vm);

    free(vm->program);
    free(vm);
    */

    // This is embarassing, but the program I was given is lowkey easy to decode
    // and you can evaluate it with this simple loop
    // but the commented code does work for general programs
    int A = 66171486;

    // part 1
    int out;
    while (A != 0) {
        printf("%d,", step(A));
        A >>= 3;
    }
    printf("\b \n");

    // part 2 over here
    // reverse engineer it using dfs
    solve(0, 15);

    return 0;
}
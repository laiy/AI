/*
 * this program is used for generate the majority of 8 digits problems and 8 queens problems
 */

#include <cstdio>
#include <cstdlib>

#define TESRCASE 100000
#define STEP 10000
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

void swap(int *a, int *b) {
    *a ^= *b ^= *a ^= *b;
}

void generate_8_digits_problem() {
    FILE *fp = fopen("testcase_8_digits_problem", "w");
    int testcase = TESRCASE, direction, position, steps;
    while (testcase--) {
        int state[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
        steps = STEP;
        position = 9;
        while (steps--) {
            direction = rand() % 4;
            switch (direction) {
                case UP:
                    if (position <= 3)
                        break;
                    else {
                        swap(&state[position - 1], &state[position - 4]), position -= 3;
                        break;
                    }
                case DOWN:
                    if (position >= 7)
                        break;
                    else {
                        swap(&state[position - 1], &state[position + 2]), position += 3;
                        break;
                    }
                case LEFT:
                    if (position % 3 == 1)
                        break;
                    else {
                        swap(&state[position - 1], &state[position - 2]), position--;
                        break;
                    }
                case RIGHT:
                    if (position % 3 == 0)
                        break;
                    else {
                        swap(&state[position - 1], &state[position]), position++;
                        break;
                    }
            }
        }
        fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7], state[8]);
    }
    fclose(fp);
}

void generate_8_queens_problem() {
    FILE *fp = fopen("testcase_8_queens_problem", "w");
    int testcase = TESRCASE, state[8], position, i;
    while (testcase--) {
        for (i = 0; i < 8; i++)
            position = rand() % 8 + 1, state[i] = position;
        fprintf(fp, "%d %d %d %d %d %d %d %d\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7]);
    }
    fclose(fp);
}

int main() {
    generate_8_digits_problem();
    generate_8_queens_problem();
    return 0;
}


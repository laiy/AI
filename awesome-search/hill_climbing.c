# ifndef CLK_TCK
# define CLK_TCK CLOCKS_PER_SEC
# endif

#include <cstdio>
#include <ctime>
#include <cmath>
#include <cstdlib>

#define TESRCASE 100000
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

double eight_digits_problem_time;
double eight_queens_problem_time;
int eight_digits_problem_failed_times;
int eight_queens_problem_failed_times;

inline void swap(int *a, int *b) {
    *a ^= *b ^= *a ^= *b;
}

inline bool solved(int *state) {
    for (int i = 0; i < 8; i++)
        if (state[i] != i + 1)
            return false;
    return true;
}

inline int manhattan_distance(int num, int position) {
    int dest_x = ceil(num / 3), dest_y = (num - 1) % 3 + 1, position_x = ceil(position / 3), position_y = (position - 1) % 3 + 1;
    return abs(dest_x - position_x) + abs(dest_y - position_y);
}

bool eight_digits_better(int *state, int position, int direction) {
    switch (direction) {
        case UP:
            if (position <= 3)
                return false;
            else
                return manhattan_distance(state[position - 4], position - 3) > manhattan_distance(state[position - 4], position);
        case DOWN:
            if (position >= 7)
                return false;
            else
                return manhattan_distance(state[position + 2], position + 3) > manhattan_distance(state[position + 2], position);
        case LEFT:
            if (position % 3 == 1)
                return false;
            else
                return manhattan_distance(state[position - 2], position - 1) > manhattan_distance(state[position - 2], position);
        case RIGHT:
            if (position % 3 == 0)
                return false;
            else
                return manhattan_distance(state[position], position + 1) > manhattan_distance(state[position], position);
    }
    return false;
}

void solve_one_case_of_8_digits_problem(int *state) {
    clock_t start_time = clock();
    int position, i;
    bool found;
    for (i = 0; i < 9; i++)
        if (state[i] == 0) {
            position = i + 1;
            break;
        }
    while (!solved(state)) {
        found = false;
        for (i = 0; i < 4; i++)
            if (eight_digits_better(state, position, i)) {
                found = true;
                switch (i) {
                    case UP:
                        swap(&state[position - 1], &state[position - 4]), position -= 3;
                        break;
                    case DOWN:
                        swap(&state[position - 1], &state[position + 2]), position += 3;
                        break;
                    case LEFT:
                        swap(&state[position - 1], &state[position - 2]), position--;
                        break;
                    case RIGHT:
                        swap(&state[position - 1], &state[position]), position++;
                        break;
                }
                break;
            }
        if (!found) {
            eight_digits_problem_failed_times++;
            return;
        }
    }
    eight_digits_problem_time += (double)(clock() - start_time) / CLK_TCK;
    printf("%lf\n", eight_digits_problem_time);
}

void solve_8_digits_problem() {
    FILE *fp = fopen("testcase_8_digits_problem", "r");
    int original_state[9];
    while (fscanf(fp, "%d %d %d %d %d %d %d %d %d", original_state, original_state + 1, original_state + 2, original_state + 3, original_state + 4, original_state + 5, original_state + 6, original_state + 7, original_state + 8) != EOF)
        solve_one_case_of_8_digits_problem(original_state);
    fclose(fp);
}

void solve_8_queens_problem() {
    return;
}

void print_result() {
    printf("eight digits problem average solved times: %lf\n", eight_digits_problem_time / (double)(TESRCASE - eight_digits_problem_failed_times));
    printf("eight digits problem solved rate: %lf\n", 1 - double(eight_digits_problem_failed_times) / TESRCASE);
}

int main() {
    eight_digits_problem_time = 0;
    eight_queens_problem_time = 0;
    eight_digits_problem_failed_times = 0;
    eight_queens_problem_failed_times = 0;
    solve_8_digits_problem();
    solve_8_queens_problem();
    print_result();
    return 0;
}


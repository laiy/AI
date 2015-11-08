# ifndef CLK_TCK
# define CLK_TCK CLOCKS_PER_SEC
# endif

#include <cstdio>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define TESRCASE 100000
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define EIGHT_DIGITS_ORIGINAL_TEMPERATURE 10
#define EIGHT_QUEENS_ORIGINAL_TEMPERATURE 50
#define MAX_TRIES_IN_ONE_TEMPERATURE 300
#define STOP_TEMPERATURE 0.000000001
#define COLD_DOWN_RATE 0.8

double eight_digits_problem_time;
double eight_queens_problem_time;
int eight_digits_problem_failed_times;
int eight_queens_problem_failed_times;
double temperature;

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
    int dis;
    switch (direction) {
        case UP:
            if (position <= 3)
                return false;
            else {
                dis = manhattan_distance(state[position - 4], position - 3) - manhattan_distance(state[position - 4], position);
                return (dis > 0) ? true : ((double)(rand() % 1000) / 1000) < exp(dis / temperature);
            }
        case DOWN:
            if (position >= 7)
                return false;
            else {
                dis = manhattan_distance(state[position + 2], position + 3) - manhattan_distance(state[position + 2], position);
                return (dis > 0) ? true : ((double)(rand() % 1000) / 1000) < exp(dis / temperature);
            }
        case LEFT:
            if (position % 3 == 1)
                return false;
            else {
                dis = manhattan_distance(state[position - 2], position - 1) - manhattan_distance(state[position - 2], position);
                return (dis > 0) ? true : ((double)(rand() % 1000) / 1000) < exp(dis / temperature);
            }
        case RIGHT:
            if (position % 3 == 0)
                return false;
            else {
                dis = manhattan_distance(state[position], position + 1) - manhattan_distance(state[position], position);
                return (dis > 0) ? true : ((double)(rand() % 1000) / 1000) < exp(dis / temperature);
            }
    }
    return false;
}

void solve_one_case_of_8_digits_problem(int *state) {
    clock_t start_time = clock();
    int position, i, tries_count;
    bool found;
    for (i = 0; i < 9; i++)
        if (state[i] == 0) {
            position = i + 1;
            break;
        }
    temperature = EIGHT_DIGITS_ORIGINAL_TEMPERATURE;
    tries_count = MAX_TRIES_IN_ONE_TEMPERATURE;
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
        if (--tries_count == 0)
            tries_count = MAX_TRIES_IN_ONE_TEMPERATURE, temperature *= COLD_DOWN_RATE;
        if (!found || temperature < STOP_TEMPERATURE) {
            eight_digits_problem_failed_times++;
            return;
        }
    }
    eight_digits_problem_time += (double)(clock() - start_time) / CLK_TCK;
}

void solve_8_digits_problem() {
    FILE *fp = fopen("testcase_8_digits_problem", "r");
    int original_state[9];
    while (fscanf(fp, "%d %d %d %d %d %d %d %d %d", original_state, original_state + 1, original_state + 2, original_state + 3, original_state + 4, original_state + 5, original_state + 6, original_state + 7, original_state + 8) != EOF)
        solve_one_case_of_8_digits_problem(original_state);
    fclose(fp);
}

int peers_of_attacking_queens(int *state) {
    int peers = 0, i, j, k;
    for (i = 0; i < 7; i++) {
        for (j = i + 1; j < 8; j++)
            if (state[j] == state[i])
                peers++;
        for (j = i + 1, k = state[i] + 1; j < 8 && k <= 8; j++, k++)
            if (state[j] == k)
                peers++;
        for (j = i + 1, k = state[i] - 1; j < 8 && k >= 1; j++, k--)
            if (state[j] == k)
                peers++;
    }
    return peers;
}

void solve_one_case_of_8_queens_problem(int *state) {
    clock_t start_time = clock();
    int h = peers_of_attacking_queens(state), i, j, best_i, best_j, temp, record, tries_count, temp_h;
    temperature = EIGHT_QUEENS_ORIGINAL_TEMPERATURE;
    tries_count = MAX_TRIES_IN_ONE_TEMPERATURE;
    while (h != 0) {
        temp_h = 28;
        for (i = 1; i <= 8; i++) {
            record = state[i - 1];
            for (j = 1; j <= 8; j++) {
                if (j != record) {
                    state[i - 1] = j;
                    temp = peers_of_attacking_queens(state);
                    if (temp < temp_h)
                        temp_h = temp, best_i = i, best_j = j;
                }
            }
            state[i - 1] = record;
        }
        if (--tries_count == 0)
            tries_count = MAX_TRIES_IN_ONE_TEMPERATURE, temperature *= COLD_DOWN_RATE;
        if (!(temperature < STOP_TEMPERATURE) && (temp_h < h || ((double)(rand() % 1000) / 1000) < exp(temp_h / temperature)))
            state[best_i - 1] = best_j, h = temp_h;
        else {
            eight_queens_problem_failed_times++;
            return;
        }
    }
    eight_queens_problem_time += (double)(clock() - start_time) / CLK_TCK;
    printf("%lf\n", eight_queens_problem_time);
}

void solve_8_queens_problem() {
    FILE *fp = fopen("testcase_8_queens_problem", "r");
    int original_state[8];
    while (fscanf(fp, "%d %d %d %d %d %d %d %d", original_state, original_state + 1, original_state + 2, original_state + 3, original_state + 4, original_state + 5, original_state + 6, original_state + 7) != EOF)
        solve_one_case_of_8_queens_problem(original_state);
    fclose(fp);
}

void print_result() {
    printf("eight digits problem average solved times: %lf\n", eight_digits_problem_time / (double)(TESRCASE - eight_digits_problem_failed_times));
    printf("eight digits problem solved rate: %lf\n", 1 - double(eight_digits_problem_failed_times) / TESRCASE);
    printf("eight queens problem average solved times: %lf\n", eight_queens_problem_time / (double)(TESRCASE - eight_queens_problem_failed_times));
    printf("eight queens problem solved rate: %lf\n", 1 - double(eight_queens_problem_failed_times) / TESRCASE);
}

int main() {
    eight_digits_problem_time = 0;
    eight_queens_problem_time = 0;
    eight_digits_problem_failed_times = 0;
    eight_queens_problem_failed_times = 0;
    /* solve_8_digits_problem(); */
    solve_8_queens_problem();
    print_result();
    return 0;
}


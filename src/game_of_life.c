#include <stdio.h>
#include <stdlib.h>
// for sleep
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void read_file(int **pArray, char *pattern, int **initial_matrix);
int update_cell(int **matrix, int line, int column);
void update_field(int **matrix, int time, int **initial_matrix);
void print_field(int **matrix);
void set_initial_pattern(int **matrix, int initial_pattern, int **initial_matrix);
void choose_initial_settings(int *initial_pattern, int *speed);

int main() {
    // allocate memory for field matrix
    int **pArray = (int **)malloc((HEIGHT) * sizeof(int *));

    for (int i = 0; i < HEIGHT; i++) {
        pArray[i] = (int *)malloc((WIDTH) * sizeof(int));
    }

    int **initial_matrix = (int **)malloc((HEIGHT) * sizeof(int *));

    for (int i = 0; i < HEIGHT; i++) {
        initial_matrix[i] = (int *)malloc((WIDTH) * sizeof(int));
    }

    int initial_pattern, speed, time;

    printf("\033c");
    choose_initial_settings(&initial_pattern, &speed);

    time = (6 - speed) * 100000 / speed;

    set_initial_pattern(pArray, initial_pattern, initial_matrix);

    // print first genertion
    print_field(pArray);

    usleep(time);

    update_field(pArray, time, initial_matrix);

    for (int i = 0; i < HEIGHT; i++) {
        free(pArray[i]);
        free(initial_matrix[i]);
    }
    free(pArray);
    free(initial_matrix);

    printf("The game is over!");
    sleep(2);

    return 0;
}

void read_file(int **pArray, char *pattern, int **initial_matrix) {
    FILE *file = fopen(pattern, "r");
    char c;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            c = fgetc(file);

            if (c == '\n') {
                c = fgetc(file);
            }

            if (c == '1') {
                pArray[i][j] = 1;
                initial_matrix[i][j] = 1;
            } else if (c == '0') {
                pArray[i][j] = 0;
                initial_matrix[i][j] = 0;
            }
        }
    }
    fclose(file);
}

// gets matrix and cell's coordinates
int update_cell(int **matrix, int line, int column) {
    // count neighbours
    int alive_neighbours_count = 0;
    int h, w;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            h = line + i;
            if (h < 0) {
                h += HEIGHT;
            } else if (h >= HEIGHT) {
                h -= HEIGHT;
            }

            w = column + j;
            if (w < 0) {
                w += WIDTH;
            } else if (w >= WIDTH) {
                w -= WIDTH;
            }

            alive_neighbours_count += matrix[h][w];
        }
    }

    // update cell status
    if (matrix[line][column] == 1 && (alive_neighbours_count < 2 || alive_neighbours_count > 3)) {
        return 0;
    } else if (matrix[line][column] == 0 && alive_neighbours_count == 3) {
        return 1;
    } else {
        return matrix[line][column];
    }
}

// updates cells one by one and return 0 if field remained the same, and 1 if it was updated
void update_field(int **matrix, int time, int **initial_matrix) {
    int updated_matrix[HEIGHT][WIDTH];

    int is_changed = 1;
    int is_different_from_initial = 1;

    while (is_changed && is_different_from_initial) {
        is_changed = 0;
        is_different_from_initial = 0;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                updated_matrix[i][j] = update_cell(matrix, i, j);
                if (updated_matrix[i][j] != matrix[i][j]) {
                    is_changed = 1;
                }
                if (updated_matrix[i][j] != initial_matrix[i][j]) {
                    is_different_from_initial = 1;
                }
            }
        }

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                matrix[i][j] = updated_matrix[i][j];
            }
        }
        printf("\033c");
        print_field(matrix);
        usleep(time);
    }
}

void print_field(int **matrix) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] == 0) {
                printf(" ");
            } else if (matrix[i][j] == 1) {
                printf("o");
            }
        }
        printf("\n");
    }

    printf("\n");
}

void set_initial_pattern(int **matrix, int initial_pattern, int **initial_matrix) {
    char filename[20];

    switch (initial_pattern) {
        case 1:
            sprintf(filename, "%s", "train.txt");
            break;

        case 2:
            sprintf(filename, "%s", "pulsar.txt");
            break;

        case 3:
            sprintf(filename, "%s", "r-pantamino.txt");
            break;

        case 4:
            sprintf(filename, "%s", "gun.txt");
            break;

        case 5:
            sprintf(filename, "%s", "galaxy_koka.txt");
            break;

        default:
            break;
    }

    read_file(matrix, filename, initial_matrix);
}

void choose_initial_settings(int *initial_pattern, int *speed) {
    printf("Please, choose initial pattern (from 1 to 5)\n");
    if (scanf("%d", initial_pattern) != 1 || *initial_pattern < 1 || *initial_pattern > 5) {
        printf("The game supports only 5 initial patterns\n");
        exit(1);
    }

    printf("Please, choose speed of the game from 1 (slow) to 5(fast)\n");

    if (scanf("%d", speed) != 1 || *speed < 1 || *speed > 5) {
        printf("Speed should be between 1 and 5\n");
        exit(1);
    }
}
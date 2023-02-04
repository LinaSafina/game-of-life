#include <stdio.h>
#include <stdlib.h>
// for sleep
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void start_game(int **pArray, char *pattern);
int update_cell(int **matrix, int line, int column);
void update_field(int **matrix, int time);
void print_field(int **matrix);
void read_file(int **matrix, int initial_pattern);
void choose_initial_settings(int *initial_pattern, int *speed);

int main() {
    // allocate memory for field matrix
    int **pArray = (int **)malloc((HEIGHT) * sizeof(int *));

    for (int i = 0; i < HEIGHT; i++) {
        pArray[i] = (int *)malloc((WIDTH) * sizeof(int));
    }

    int initial_pattern, speed, time;

    printf("\033c");
    choose_initial_settings(&initial_pattern, &speed);

    time = 6 - speed;

    read_file(pArray, initial_pattern);

    // print first genertion
    print_field(pArray);

    sleep(time);

    update_field(pArray, time);

    for (int i = 0; i < HEIGHT; i++) free(pArray[i]);
    free(pArray);

    return 0;
}

void start_game(int **pArray, char *pattern) {
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
            } else if (c == '0') {
                pArray[i][j] = 0;
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

    for (int i = line - 1; i <= line + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            if (i >= 0 && i < HEIGHT && j >= 0 && j < WIDTH) {
                h = i;
                w = j;
            } else {
                h = (i == -1) ? HEIGHT - 1 : 0;
                w = (j == -1) ? WIDTH - 1 : 0;
            }

            if (matrix[h][w] == 1 && !(i == line && j == column)) {
                alive_neighbours_count++;
            }
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
void update_field(int **matrix, int time) {
    int updated_matrix[HEIGHT][WIDTH];

    int unchanged = 1;

    while (unchanged) {
        unchanged = 0;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                updated_matrix[i][j] = update_cell(matrix, i, j);
                if (updated_matrix[i][j] != matrix[i][j]) {
                    unchanged = 1;
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
        sleep(time);
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

void read_file(int **matrix, int initial_pattern) {
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

    start_game(matrix, filename);
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
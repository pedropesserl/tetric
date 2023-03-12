#include <stdio.h>

#define ROWS 20
#define COLS 10
#define FROWS 22
#define FCOLS 12

#define EDGE -1
#define I 1
#define O 2
#define T 3
#define L 4
#define J 5
#define S 6
#define Z 7

#define initialize_board(grid) do {         \
    for (int i = 0; i < FROWS; i++)         \
        for (int j = 0; j < FCOLS; j++)     \
            grid[i][j] = EDGE;              \
                                            \
    for (int i = 1; i <= ROWS; i++)         \
        for (int j = 1; j <= COLS; j++)     \
            grid[i][j] = 0;                 \
                                            \
} while(0)

void print_board(int board[FROWS][FCOLS]) {
    for (int i = 0; i < FROWS; i++) {
        for (int j = 0; j < FCOLS; j++) {
            if (board[i][j] == EDGE)
                printf("#");
            if (board[i][j] == 0)
                printf(".");
        }
        printf("\n");
    }
}

int main() {
    int board[FROWS][FCOLS];
    initialize_board(board);

#ifdef DEBUG_FLAG
    print_board(board);
#endif

    return 0;
}

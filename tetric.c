#include <stdio.h>
#include <unistd.h>

#define ROWS 20
#define COLS 10
#define FROWS 22
#define FCOLS 12

#define EDGE -1
#define EMPTY 0
// different numbers for different colors
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

typedef struct {
    int matrix[4][4];
    float fall_delay;
    int posx, posy;
} Piece;

void print_board(int board[FROWS][FCOLS]) {
    for (int i = 0; i < FROWS; i++) {
        for (int j = 0; j < FCOLS; j++) {
            switch (board[i][j]) {
                case (EDGE):
                    printf("#"); break;
                case (EMPTY):
                    printf("."); break;
                case (I):
                    printf("I"); break;
                case (O):
                    printf("O"); break;
                case (T):
                    printf("T"); break;
                case (L):
                    printf("L"); break;
                case (J):
                    printf("J"); break;
                case (S):
                    printf("S"); break;
                case (Z):
                    printf("Z"); break;
            }
        }
        printf("\n");
    }
}

int main(void) {
    int board[FROWS][FCOLS];
    initialize_board(board);

    Piece piece_i = {
        .matrix = {
            {0, 0, 0, 0,},
            {I, I, I, I,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

    Piece piece_o = {
        .matrix = {
            {0, 0, 0, 0,},
            {0, O, O, 0,},
            {0, O, O, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = -1,
        .posy = 4,
    };

    Piece piece_t = {
        .matrix = {
            {0, T, 0, 0,},
            {T, T, T, 0,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

    Piece piece_l = {
        .matrix = {
            {0, 0, L, 0,},
            {L, L, L, 0,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

    Piece piece_j = {
        .matrix = {
            {J, 0, 0, 0,},
            {J, J, J, 0,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

    Piece piece_s = {
        .matrix = {
            {0, S, S, 0,},
            {S, S, 0, 0,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

    Piece piece_z = {
        .matrix = {
            {Z, Z, 0, 0,},
            {0, Z, Z, 0,},
            {0, 0, 0, 0,},
            {0, 0, 0, 0,},
        },
        .fall_delay = 0.8,
        .posx = 0,
        .posy = 4,
    };

#ifdef DEBUG_FLAG
    while (1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i + piece_z.posx][j + piece_z.posy] == EMPTY
                        && piece_z.matrix[i][j] != EMPTY)
                    board[i + piece_z.posx][j + piece_z.posy] = piece_z.matrix[i][j];
            }
        }

        print_board(board);

        usleep(piece_z.fall_delay * 1000 * 1000);

        piece_z.posx++;
    }

#endif

    return 0;
}

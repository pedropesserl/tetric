#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ROWS 20
#define COLS 10

#define EMPTY 0
// different numbers for different colors
#define I 1
#define O 2
#define T 3
#define L 4
#define J 5
#define S 6
#define Z 7

#define initialize_board(grid) do {        \
    for (int i = 0; i < ROWS; i++)         \
        for (int j = 0; j < COLS; j++)     \
            grid[i][j] = EMPTY;            \
                                           \
} while(0)

typedef struct Piece {
    char type;
    int matrix[4][4];
    float fall_time;
    int posv, posh; // position of matrix's upper-left corner relative to board
} Piece;

void print_board(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            switch (board[i][j]) {
                case (EMPTY): putchar('.'); break;
                case (I):     putchar('I'); break;
                case (O):     putchar('O'); break;
                case (T):     putchar('T'); break;
                case (L):     putchar('L'); break;
                case (J):     putchar('J'); break;
                case (S):     putchar('S'); break;
                case (Z):     putchar('Z'); break;
            }
        }
        printf("\n");
    }
}

int highest_row(Piece p) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (p.matrix[i][j] != EMPTY)
                return i;
    return -1;
}

int lowest_row(Piece p) {
    for (int i = 3; i >= 0; i--)
        for (int j = 0; j < 3; j++)
            if (p.matrix[i][j] != EMPTY)
                return i;
    return -1;
}

int leftmost_column(Piece p) {
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            if (p.matrix[i][j] != EMPTY)
                return i;
    return -1;
}

int rightmost_column(Piece p) {
    for (int j = 3; j >= 0; j--)
        for (int i = 0; i < 3; i++)
            if (p.matrix[i][j] != EMPTY)
                return i;
    return -1;
}

/* typedef struct Rectangle { */
/*     int posv, posh, szv, szh; */
/* } Rectangle; */

/* Rectangle find_bounding_box(Piece p) { */
/*     int posv = highest_row(p), posh = leftmost_column(p); */
/*     Rectangle bb = { */
/*         .posv = posv, */
/*         .posh = posh, */
/*         .szv = posv + lowest_row(p) + 1, */
/*         .szh = posh + rightmost_column(p) + 1, */
/*     }; */
/*     return bb; */
/* } */

int next_position_is_valid(int board[ROWS][COLS], Piece p) {
    int lowest = lowest_row(p);
    if (p.posv + lowest >= 19)
        return 0;     // end of board
    for (int j = 0; j < 4; j++)
        if (p.matrix[lowest][j] != EMPTY)
            if (board[p.posv + lowest + 1][j + p.posh] != EMPTY) 
                return 0; // blocks underneath
    return 1;
}

Piece new_piece(char type) {
    Piece p = {0};
    p.type = type;
    p.fall_time = 0.8;
    p.posv = 0;
    p.posh = 3;
    switch (type) {
        case 'I': {
            int m[4][4] = {
                {0, 0, 0, 0,},
                {I, I, I, I,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'O': {
            int m[4][4] = {
                {0, O, O, 0,},
                {0, O, O, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'T': {
            int m[4][4] = {
                {0, T, 0, 0,},
                {T, T, T, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'L': {
            int m[4][4] = {
                {0, 0, L, 0,},
                {L, L, L, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'J': {
            int m[4][4] = {
                {J, 0, 0, 0,},
                {J, J, J, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'S': {
            int m[4][4] = {
                {0, S, S, 0,},
                {S, S, 0, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
        case 'Z': {
            int m[4][4] = {
                {Z, Z, 0, 0,},
                {0, Z, Z, 0,},
                {0, 0, 0, 0,},
                {0, 0, 0, 0,},
            };
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    p.matrix[i][j] = m[i][j];
            break;
        }
    }
    return p;
}

int main(void) {
    int board[ROWS][COLS];
    initialize_board(board);

#ifdef DEBUG_FLAG
    Piece piece_z = new_piece('Z');
    // INICIALIZAR PEÇA
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (piece_z.matrix[i][j] != EMPTY)
                board[i][j + piece_z.posh] = piece_z.matrix[i][j];

    print_board(board);
    printf("-------------------------\n");
    usleep(piece_z.fall_time * 1000 * 1000);

    // ATUALIZAR PEÇA
    while (next_position_is_valid(board, piece_z)) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (piece_z.matrix[i][j] != EMPTY)
                    board[i + piece_z.posv][j + piece_z.posh] = EMPTY;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (piece_z.matrix[i][j] != EMPTY)
                    board[i + piece_z.posv + 1][j + piece_z.posh] = piece_z.matrix[i][j];

        piece_z.posv++;
        print_board(board);
        printf("-------------------------\n");
        usleep(piece_z.fall_time * 1000 * 1000);
    }
#endif

    return 0;
}

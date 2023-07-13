#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libtermio.h"
#include "matrix.h"
#include "piece.h"

#ifndef BDROWS
#define BDROWS 20
#endif
#ifndef BDCOLS
#define BDCOLS 10
#endif

#define INITIALIZE_BOARD(grid) do {        \
    for (int i = 0; i < BDROWS; i++)       \
        for (int j = 0; j < BDCOLS; j++)   \
            grid[i][j] = EMPTY;            \
                                           \
} while(0)

void print_board(int board[BDROWS][BDCOLS]) {
    for (int i = 0; i < BDROWS; i++) {
        for (int j = 0; j < BDCOLS; j++) {
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

int main(void) {
    int board[BDROWS][BDCOLS];
    INITIALIZE_BOARD(board);

    Piece piece_z = new_piece(Z, 0.8);
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

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
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
    printf("\033[48;5;234m"); // grey background
    printf("    ┏━━━━━━━━━━━━━━━━━━━━┓    \n");

    for (int i = 0; i < BDROWS; i++) {
        printf("    ┃");
        for (int j = 0; j < BDCOLS; j++) {
            switch (board[i][j]) {
                case (EMPTY):
                    printf("  ");
                    break;
                case (I):
                    printf("\033[48;5;51m  ");  // cyan
                    printf("\033[48;5;234m");
                    break;
                case (O):
                    printf("\033[48;5;220m  "); // yellow
                    printf("\033[48;5;234m");
                    break;
                case (T):
                    printf("\033[48;5;163m  "); // purple
                    printf("\033[48;5;234m");
                    break;
                case (L):
                    printf("\033[48;5;202m  "); // orange
                    printf("\033[48;5;234m");
                    break;
                case (J):
                    printf("\033[48;5;21m  ");  // blue
                    printf("\033[48;5;234m");
                    break;
                case (S):
                    printf("\033[48;5;40m  ");  // green
                    printf("\033[48;5;234m");
                    break;
                case (Z):
                    printf("\033[48;5;160m  "); // red
                    printf("\033[48;5;234m");
                    break;
            }
        }
        printf("┃    \n");
    }
    printf("    ┗━━━━━━━━━━━━━━━━━━━━┛    \n");
}

int quit = 0;

int main(void) {
    int board[BDROWS][BDCOLS];
    INITIALIZE_BOARD(board);
    srand(time(NULL));
    struct termios curr_config;
    disable_canonical_stdin(&curr_config);
    /* clock_t start, now; */

    while (!quit) {
        Piece piece = new_piece(rand() % 7 + 1, 0.1);
        // INICIALIZAR PEÇA
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (piece.matrix[i][j] != EMPTY)
                    board[i][j + piece.posh] = piece.matrix[i][j];

        print_board(board);
        cursor_up(22);
        cursor_left(29);
        usleep(piece.fall_time * 1000 * 1000);

        if (!next_position_is_valid(board, piece)) // encheu a tela
            quit = 1;

        // ATUALIZAR PEÇA
        while (!quit && next_position_is_valid(board, piece)) {
            /* start = clock(); */

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (piece.matrix[i][j] != EMPTY)
                        board[i+piece.posv][j+piece.posh] = EMPTY;

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    if (piece.matrix[i][j] != EMPTY)
                        board[i+piece.posv+1][j+piece.posh] = piece.matrix[i][j];

            piece.posv++;
            print_board(board);
            cursor_up(22);
            cursor_left(29);

            if (kb_hit()) {
                switch (fgetc(stdin)) {
                    case 'a': turn_left(&piece); break;
                    default: break;
                }
            }

            /* now = clock(); */
            /* if (((double)now - start)/CLOCKS_PER_SEC >= piece.fall_time) */
            /*     piece.posv++; */
            usleep(piece.fall_time * 1000 * 1000);
                
        }

    }

    set_stdin_flush(&curr_config);
    cursor_down(22);

    return 0;
}

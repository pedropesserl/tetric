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

void initialize_screen() {
    printf("\033[48;5;234m"); // grey background
    printf("    ┏━━━━━━━━━━━━━━━━━━━━┓    \n");
    for (int i = 0; i < BDROWS; i++) {
        printf("    ┃");
        for (int j = 0; j < BDCOLS; j++)
            printf("  ");
        printf("┃    \n");
    }
    printf("    ┗━━━━━━━━━━━━━━━━━━━━┛    \n");
}

void render_board(int board[BDROWS][BDCOLS]) {
    for (int i = 0; i < BDROWS; i++) {
        for (int j = 0; j < BDCOLS; j++)
            switch (board[i][j]) {
                case (EMPTY):
                    printf("\033[48;5;234m  ");
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
        cursor_down(1);
        cursor_left(BDCOLS*2);
    }
    cursor_up(BDROWS);
}

int quit = 0;

int main(void) {
    int board[BDROWS][BDCOLS];
    INITIALIZE_BOARD(board);
    srand(time(NULL));
    struct termios curr_config;
    disable_canonical_stdin(&curr_config);
    hide_cursor();
    clock_t start, now;

    initialize_screen();
    cursor_up(21);
    cursor_right(5);

    while (!quit) {
        Piece piece = new_piece(rand() % 7 + 1, 0.3);
        Piece piece_held = {0};
        // INICIALIZAR PEÇA
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (piece.matrix[i][j] != EMPTY)
                    board[i][j + piece.posh] = piece.matrix[i][j];

        render_board(board);

        if (!next_position_is_valid(board, piece)) // encheu a tela
            quit = 1;

        // ATUALIZAR PEÇA
        start = clock();
        while (next_position_is_valid(board, piece)) {
            now = clock();
            if (((double)now - start)/CLOCKS_PER_SEC >= piece.fall_time) {
                start = clock();
                now = clock();
                update_falling_piece(board, &piece);
                render_board(board);
            }
            if (kb_hit()) {
                switch (fgetc(stdin)) {
                    case 'a': turn_left(&piece);         break;
                    case 's': hold(&piece, &piece_held); break;
                    case 'd': turn_right(&piece);        break;
                    case 'f': turn_180(&piece);          break;
                    case ' ': hard_drop(&piece);         break;
                    case '\033': // arrow key
                        fgetc(stdin); // first character after \033 is [
                        switch (fgetc(stdin)) {
                            case 'D': move_left(&piece);  break;
                            case 'C': move_right(&piece); break;
                            case 'B': soft_drop(&piece);  break;
                        }
                        break;
                    default: break;
                }
            }
        }
    }

    set_stdin_flush(&curr_config);
    cursor_down(22);
    show_cursor();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "libtermio.h"
#include "matrix.h"
#include "piece.h"

#define FPS 30.0

#ifndef BDROWS
#define BDROWS 20
#endif
#ifndef BDCOLS
#define BDCOLS 10
#endif

void initialize_screen() {
    printf("\033[48;5;234m"); // grey background
    
    printf("                                            \n");
    printf("    ┏━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━━━━┓    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┗━━━━━━━━┛    \n");
    printf("    ┃                    ┃       HOLD       \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃    ┏━━━━━━━━┓    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┃        ┃    \n");
    printf("    ┃                    ┃    ┗━━━━━━━━┛    \n");
    printf("    ┃                    ┃       NEXT       \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┃                    ┃                  \n");
    printf("    ┗━━━━━━━━━━━━━━━━━━━━┛                  \n");
    printf("                                            \n");
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

int main(void) {
    struct termios curr_config;
    disable_canonical_stdin(&curr_config);
    hide_cursor();

    int board[BDROWS][BDCOLS] = {0};
    srand(time(NULL));
    clock_t fall_delay, fps, now;

    initialize_screen();
    cursor_up(22);
    cursor_right(5);

    for (;;) {
        fall_delay = clock();
        fps = clock();
        now = clock();
        if (((double)now - fps)/CLOCKS_PER_SEC >= 1.0/FPS) {
            fps = clock();
            render_board(board);
        }

        Piece piece = new_piece(rand() % 7 + 1, 0.8);
        Piece piece_held = {0};

        stamp_piece(board, piece);

        if (!down_is_valid(board, piece)) // encheu a tela
            goto quit;

        // ATUALIZAR PEÇA
        fall_delay = clock();
        while (down_is_valid(board, piece)) {
            now = clock();
            if (((double)now - fps)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps = clock();
                render_board(board);
            }
            if (((double)now - fall_delay)/CLOCKS_PER_SEC >= piece.fall_time) {
                fall_delay = clock();
                update_falling_piece(board, &piece);
            }
            if (kb_hit()) {
                switch (fgetc(stdin)) {
                    case '!': goto quit;                 break;
                    case 'a': turn_left(board, &piece);  break;
                    case 's': hold(&piece, &piece_held); break;
                    case 'd': turn_right(board, &piece); break;
                    case 'f': turn_180(board, &piece);   break;
                    case ' ': hard_drop(board, &piece);  break;
                    case '\033': // arrow key
                        fgetc(stdin); // first character after \033 is [
                        switch (fgetc(stdin)) {
                            case 'D': move_left(board, &piece);  break;
                            case 'C': move_right(board, &piece); break;
                            case 'B': soft_drop(&piece);         break;
                        }
                        break;
                    default: break;
                }
            }
        }
    }

quit:
    render_board(board);
    set_stdin_flush(&curr_config);
    cursor_down(22);
    cursor_left(30);
    show_cursor();

    return 0;
}

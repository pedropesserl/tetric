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

int process_keypress(char c, int board[BDROWS][BDCOLS], Piece *p, Piece *held) {
    switch (c) {
        case '!': return 0;             break;
        case 'a': turn_left(board, p);  break;
        case 's': hold(p, held);        break;
        case 'd': turn_right(board, p); break;
        case 'f': turn_180(board, p);   break;
        case ' ': hard_drop(board, p);  break;
        case '\033':      // arrow key
              fgetc(stdin); // first character after \033 is [
              switch (fgetc(stdin)) {
                  case 'D': move_left(board, p);  break;
                  case 'C': move_right(board, p); break;
                  case 'B': soft_drop(p);         break;
              }
              break;
        default: break;
    }
    return 1;
}

void quit(struct termios *term_config, int board[BDROWS][BDCOLS]) {
    render_board(board);
    set_stdin_flush(term_config);
    cursor_down(22);
    cursor_left(30);
    show_cursor();
    exit(0);
}

int main(void) {
    struct termios term_config;
    disable_canonical_stdin(&term_config);
    hide_cursor();

    int board[BDROWS][BDCOLS] = {0};
    srand(time(NULL));
    clock_t fall_control, fps_control;

    initialize_screen();
    cursor_up(22);
    cursor_right(5);

    for (;;) {
        fall_control = clock();
        fps_control = clock();
        if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
            fps_control = clock();
            render_board(board);
        }

        Piece piece = new_piece(rand() % 7 + 1, 0.4);
        Piece piece_held = {0};

        stamp_piece(board, piece);

        if (!down_is_valid(board, piece)) // filled screen
            quit(&term_config, board);

        // UPDATE PIECE
        fall_control = clock();
        while (down_is_valid(board, piece)) {
piece_falling:
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board);
            }
            if (((double)clock() - fall_control)/CLOCKS_PER_SEC >= piece.fall_time) {
                fall_control = clock();
                update_falling_piece(board, &piece);
            }
            if (kb_hit())
                if (process_keypress(fgetc(stdin), board, &piece, &piece_held) == 0)
                    quit(&term_config, board);
        }

        // wiggle room after touching ground
        fall_control = clock();
        while (((double)clock() - fall_control)/CLOCKS_PER_SEC < piece.fall_time) {
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board);
            }
            if (kb_hit())
                if (process_keypress(fgetc(stdin), board, &piece, &piece_held) == 0)
                    quit(&term_config, board);

            if (down_is_valid(board, piece)) // is now off of a ledge
                goto piece_falling;
        }
    }

    return 0;
}

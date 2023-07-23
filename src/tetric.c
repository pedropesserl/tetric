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

typedef struct Coord {
    int x, y;
} Coord;

/* int boardx = 0, boardy = 0, holdx = 0, holdy = 0, nextx = 0, nexty = 0; */

static void pad_line(const char *line, int left_pad, int right_pad) {
    for (int j = 0; j < left_pad; j++) printf(" ");
    printf("%s", line);
    for (int j = 0; j < right_pad; j++) printf(" ");
    printf("\n");
}

void initialize_screen(int term_rows, int term_cols,
                       int *boardx, int *boardy,
                       int *holdx, int *holdy,
                       int *nextx, int *nexty) {
    const int tall = 24, wide = 41;
    int lpad = (term_cols - wide)/2;
    int rpad = term_cols - wide - lpad;
    int tbpad = (term_rows - tall)/2;

    printf("\033[48;5;234m"); // grey background
    for (int i = 0; i < tbpad; i++) {
        for (int j = 0; j < term_cols; j++) printf(" ");
        printf("\n");
    }
    pad_line("                                         ", lpad, rpad);
    pad_line("   ┏━━━━━━━━━━━━━━━━━━━━┓   ┏━━━━━━━━┓   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┗━━━━━━━━┛   ", lpad, rpad);
    pad_line("   ┃                    ┃      HOLD      ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃   ┏━━━━━━━━┓   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┃        ┃   ", lpad, rpad);
    pad_line("   ┃                    ┃   ┗━━━━━━━━┛   ", lpad, rpad);
    pad_line("   ┃                    ┃      NEXT      ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┃                    ┃                ", lpad, rpad);
    pad_line("   ┗━━━━━━━━━━━━━━━━━━━━┛                ", lpad, rpad);
    pad_line("                                         ", lpad, rpad);
    for (int i = 0; i < tbpad; i++) {
        for (int j = 0; j < term_cols; j++) printf(" ");
        printf("\n");
    }

    // exporting coordinates
    *boardx = tbpad + 3;
    *boardy = lpad + 5;
    *holdx = tbpad + 3;
    *holdy = lpad + 30;
    *nextx = tbpad + 11;
    *nexty = lpad + 30;
}

void render_board(int board[BDROWS][BDCOLS], int boardx, int boardy) {
    cursor_to(boardx, boardy);
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

void render_piece_in_4x8_grid(Piece p) {
    /* printf("\033[48;5;234m        "); */
    /* cursor_down(1); */
    /* cursor_left(8); */
    /* printf("        "); */
    /* cursor_down(1); */
    /* cursor_left(8); */
    /* printf("        "); */
    /* cursor_down(1); */
    /* cursor_left(8); */
    /* printf("        "); */

    switch (p.type) {
        case (I):
            printf("\033[48;5;234m   ");
            printf("\033[48;5;51m  "); // cyan
            printf("\033[48;5;234m   ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   ");
            printf("\033[48;5;51m  "); // cyan
            printf("\033[48;5;234m   ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   ");
            printf("\033[48;5;51m  "); // cyan
            printf("\033[48;5;234m   ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   ");
            printf("\033[48;5;51m  "); // cyan
            printf("\033[48;5;234m   ");
            break;
        case (O):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m  ");
            printf("\033[48;5;220m    "); // yellow
            printf("\033[48;5;234m  ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m  ");
            printf("\033[48;5;220m    "); // yellow
            printf("\033[48;5;234m  ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
        case (T):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   ");
            printf("\033[48;5;163m  "); // purple
            printf("\033[48;5;234m   ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m ");
            printf("\033[48;5;163m      "); // purple
            printf("\033[48;5;234m ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
        case (L):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m     ");
            printf("\033[48;5;202m  "); // orange
            printf("\033[48;5;234m ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m "); 
            printf("\033[48;5;202m      "); // orange
            printf("\033[48;5;234m "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
        case (J):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m ");
            printf("\033[48;5;21m  "); // blue
            printf("\033[48;5;234m     ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m "); 
            printf("\033[48;5;21m      "); // blue
            printf("\033[48;5;234m "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
        case (S):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m "); 
            printf("\033[48;5;40m    "); // green
            printf("\033[48;5;234m   "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   "); 
            printf("\033[48;5;40m    "); // green
            printf("\033[48;5;234m "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
        case (Z):
            printf("\033[48;5;234m        ");
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m   "); 
            printf("\033[48;5;160m    "); // red
            printf("\033[48;5;234m "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m "); 
            printf("\033[48;5;160m    "); // red
            printf("\033[48;5;234m   "); 
            cursor_down(1);
            cursor_left(8);
            printf("\033[48;5;234m        ");
            break;
    }
}

void render_hold(Piece held, int holdx, int holdy) {
    cursor_to(holdx, holdy);
    render_piece_in_4x8_grid(held);
}

void render_next(Piece next, int nextx, int nexty) {
    cursor_to(nextx, nexty);
    render_piece_in_4x8_grid(next);
}

void quit(struct termios *term_config, int term_rows, int term_cols, int board[BDROWS][BDCOLS] /*, int boardx, int boardy*/) {
    /* render_board(board, boardx, boardy); */
    set_stdin_flush(term_config);
    cursor_down(term_rows);
    cursor_left(term_cols);
    show_cursor();
    exit(0);
}

int process_keypress(char c, struct termios *term_config, int term_rows, int term_cols,
                     int board[BDROWS][BDCOLS], Piece *p, Piece *held) {
    switch (c) {
        case '!': quit(term_config, term_rows, term_cols, board); break;
        case 'a': turn_left(board, p);      break;
        case 's': hold(p, held);            break;
        case 'd': turn_right(board, p);     break;
        case 'f': turn_180(board, p);       break;
        case ' ': hard_drop(board, p);      break;
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

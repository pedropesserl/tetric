#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "libtermio.h"
#include "matrix.h"
#include "piece.h"

#define FPS 30.0
/* #define WIGGLE_TIME 1.0 */

#ifndef BDROWS
#define BDROWS 20
#endif
#ifndef BDCOLS
#define BDCOLS 10
#endif

// colors:
#define BG     "\033[48;5;234m" // grey
#define CYAN   "\033[48;5;51m"
#define YELLOW "\033[48;5;220m"
#define PURPLE "\033[48;5;163m"
#define ORANGE "\033[48;5;202m"
#define BLUE   "\033[48;5;21m"
#define GREEN  "\033[48;5;40m"
#define RED    "\033[48;5;160m"

typedef struct Coord {
    int x, y;
} Coord;

static void pad_line(const char *line, int left_pad, int right_pad) {
    for (int j = 0; j < left_pad; j++) printf(" ");
    printf("%s", line);
    for (int j = 0; j < right_pad; j++) printf(" ");
    cursor_down(1);
    cursor_left(left_pad + 35 + right_pad);
}

void initialize_screen(int term_rows, int term_cols,
                       int *boardx, int *boardy,
                       int *holdx, int *holdy,
                       int *nextx, int *nexty) {
    const int tall = 22, wide = 35;
    int lpad = (term_cols - wide)/2;
    int rpad = term_cols - wide - lpad;
    int tbpad = (term_rows - tall)/2;

    cursor_to(1, 1);

    printf("%s", BG);
    for (int i = 0; i < tbpad - 1; i++) {
        for (int j = 0; j < term_cols; j++) printf(" ");
        cursor_down(1);
        cursor_left(term_cols);
    }
    for (int j = 0; j < term_cols; j++) printf(" ");
    cursor_down(1);
    cursor_left(term_cols);
    pad_line("┏━━━━━━━━━━━━━━━━━━━━┓   ┏━━━━━━━━┓", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┗━━━━━━━━┛", lpad, rpad);
    pad_line("┃                    ┃      HOLD   ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃   ┏━━━━━━━━┓", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┃        ┃", lpad, rpad);
    pad_line("┃                    ┃   ┗━━━━━━━━┛", lpad, rpad);
    pad_line("┃                    ┃      NEXT   ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┗━━━━━━━━━━━━━━━━━━━━┛             ", lpad, rpad);
    for (int i = 0; i < tbpad - 1; i++) {
        for (int j = 0; j < term_cols; j++) printf(" ");
        cursor_down(1);
        cursor_left(term_cols);
    }
    for (int j = 0; j < term_cols; j++) printf(" ");

    // exporting coordinates
    *boardx = tbpad + 2;
    *boardy = lpad + 2;
    *holdx = tbpad + 2;
    *holdy = lpad + 27;
    *nextx = tbpad + 10;
    *nexty = lpad + 27;
}

void render_board(int board[BDROWS][BDCOLS], int boardx, int boardy) {
    cursor_to(boardx, boardy);
    for (int i = 0; i < BDROWS; i++) {
        for (int j = 0; j < BDCOLS; j++)
            switch (board[i][j]) {
                case EMPTY: printf("%s  ", BG); break;
                case I: printf("%s  ", CYAN  ); break;
                case O: printf("%s  ", YELLOW); break;
                case T: printf("%s  ", PURPLE); break;
                case L: printf("%s  ", ORANGE); break;
                case J: printf("%s  ", BLUE  ); break;
                case S: printf("%s  ", GREEN ); break;
                case Z: printf("%s  ", RED   ); break;
            }
        cursor_down(1);
        cursor_left(BDCOLS*2);
    }
    cursor_up(BDROWS);
}

void render_piece_in_4x8_grid(Piece p) {
    switch (p.type) {
        case I:
            printf("%s   %s  %s   ", BG, CYAN, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s  %s   ", BG, CYAN, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s  %s   ", BG, CYAN, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s  %s   ", BG, CYAN, BG);
            break;
        case O:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s  %s    %s  ", BG, YELLOW, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s  %s    %s  ", BG, YELLOW, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
            break;
        case T:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s  %s   ", BG, PURPLE, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s %s      %s ", BG, PURPLE, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
            break;
        case L:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s     %s  %s ", BG, ORANGE, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s %s      %s ", BG, ORANGE, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
            break;
        case J:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s %s  %s     ", BG, BLUE, BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s %s      %s ", BG, BLUE, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
            break;
        case S:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s %s    %s   ", BG, GREEN, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s    %s ", BG, GREEN, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
            break;
        case Z:
            printf("%s        ", BG);
            cursor_down(1);
            cursor_left(8);
            printf("%s   %s    %s ", BG, RED, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s %s    %s   ", BG, RED, BG); 
            cursor_down(1);
            cursor_left(8);
            printf("%s        ", BG);
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

void clear_board(int board[BDROWS][BDCOLS]) {
    for (int i = 0; i < BDROWS; i++)
        for (int j = 0; j < BDCOLS; j++)
            board[i][j] = EMPTY;
}

void quit(struct termios *term_config, int term_rows, int term_cols) {
    set_stdin_flush(term_config);
    cursor_down(term_rows);
    cursor_left(term_cols);
    show_cursor();
    exit(0);
}

void reset(int term_rows, int term_cols,
           int board[BDROWS][BDCOLS], int *boardx, int *boardy,
           int *holdx, int *holdy, int *nextx, int *nexty,
           Piece *p, Piece *p_next, Piece *p_held,
           int *was_held, int *hard_dropped) {
    clear_board(board);
    initialize_screen(term_rows, term_cols, boardx, boardy, holdx, holdy, nextx, nexty);
    *hard_dropped = 0;
    *was_held = 0;
    memset(p, 0, sizeof(Piece));
    memset(p_held, 0, sizeof(Piece));
    *p_next = new_piece(rand() % 7 + 1, 0.8);
}

void process_keypress(char c, struct termios *term_config, int term_rows, int term_cols,
                      int board[BDROWS][BDCOLS], int *boardx, int *boardy,
                      int *holdx, int *holdy, int *nextx, int *nexty,
                      Piece *p, Piece *p_next, Piece *p_held,
                      int *was_held, int *hard_dropped) {
    switch (c) {
        case '!': quit(term_config, term_rows, term_cols);          break;
        case '1': reset(term_rows, term_cols, board, boardx,
                        boardy, holdx, holdy, nextx, nexty,
                        p, p_next, p_held, was_held, hard_dropped); break;
        case 'a': turn_left(board, p);                              break;
        case 's': hold(board, p, p_next, p_held, was_held);         break;
        case 'd': turn_right(board, p);                             break;
        case 'f': turn_180(board, p);                               break;
        case ' ': hard_drop(board, p, hard_dropped);                break;
        case '\033':      // arrow key
            fgetc(stdin); // first character after \033 is [
            switch (fgetc(stdin)) {
                case 'D': move_left(board, p);  break;
                case 'C': move_right(board, p); break;
                case 'B': soft_drop(board, p);  break;
            }
            break;
        default: break;
    }
}

void shift_above_rows(int board[BDROWS][BDCOLS], int curr_row) {
    for (int i = curr_row; i > 0; i--) {
        for (int j = 0; j < BDCOLS; j++) {
            board[i][j] = board[i-1][j];
        }
    }
    for (int j = 0; j < BDCOLS; j++)
        board[0][j] = EMPTY;
}

void clear_filled_rows(int board[BDROWS][BDCOLS], int *rows_count) {
    int filled_rows_cnt = 0;
    for (int i = 0; i < BDROWS; i++) {
        int empty_cells = 0;
        for (int j = 0; j < BDCOLS; j++) {
            if (board[i][j] == EMPTY) {
                empty_cells++;
                break;
            }
        }
        if (empty_cells == 0) { // filled_row
            for (int j = 0; j < BDCOLS; j++)
                board[i][j] = EMPTY;
            shift_above_rows(board, i);
            filled_rows_cnt++;
        }
    }

    *rows_count = filled_rows_cnt;
}

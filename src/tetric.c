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

// colors:
#define BG     "\033[48;5;234m" // grey
#define CYAN   "\033[48;5;51m"
#define YELLOW "\033[48;5;220m"
#define PURPLE "\033[48;5;163m"
#define ORANGE "\033[48;5;202m"
#define BLUE   "\033[48;5;21m"
#define GREEN  "\033[48;5;40m"
#define RED    "\033[48;5;160m"
#define LTGREY "\033[48;5;238m" // lighter grey

typedef struct ItemsXY_s {
    Coord board, hold, next, points, level;
} ItemsXY;

typedef struct GameState_s {
    int board[BDROWS][BDCOLS];
    int points, level, total_rows;
    float fall_time;
    Piece piece, p_held, p_next;
    Coord board_xy, hold_xy, next_xy, points_xy, level_xy;
    int hard_dropped, was_held;
} GameState;

static void pad_line(const char *line, int lpad, int rpad) {
    for (int j = 0; j < lpad; j++) printf(" ");
    printf("%s", line);
    for (int j = 0; j < rpad; j++) printf(" ");
    cursor_down(1);
    cursor_left(lpad + 35 + rpad);
}

void initialize_screen(int term_rows, int term_cols, GameState *gs) {
    const int tall = 22, wide = 35;
    int lpad = (term_cols - wide)/2;
    int rpad = term_cols - wide - lpad;
    int tbpad = (term_rows - tall)/2;

    cursor_to(1, 1);

    printf("%s", BG);
    for (int i = 0; i < tbpad-1; i++) {
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
    pad_line("┃                    ┃     POINTS  ", lpad, rpad);
    pad_line("┃                    ┃            0", lpad, rpad);
    pad_line("┃                    ┃             ", lpad, rpad);
    pad_line("┃                    ┃    LEVEL   0", lpad, rpad);
    pad_line("┗━━━━━━━━━━━━━━━━━━━━┛             ", lpad, rpad);
    for (int i = 0; i < tbpad; i++) {
        for (int j = 0; j < term_cols; j++) printf(" ");
        cursor_down(1);
        cursor_left(term_cols);
    }
    for (int j = 0; j < term_cols; j++) printf(" ");

    // exporting coordinates
    gs->board_xy.x = tbpad + 2;
    gs->board_xy.y = lpad + 2;
    gs->hold_xy.x = tbpad + 2;
    gs->hold_xy.y = lpad + 27;
    gs->next_xy.x = tbpad + 10;
    gs->next_xy.y = lpad + 27;
    gs->points_xy.x = tbpad + 19;
    gs->points_xy.y = lpad + 24;
    gs->level_xy.x = tbpad + 21;
    gs->level_xy.y = lpad + 33;
}

void render_board(int board[BDROWS][BDCOLS], Coord board_xy, Piece p) {
    Piece s = p;
    while (down_is_valid(board, s))
        s.pos.x++;

    cursor_to(board_xy.x, board_xy.y);
    for (int i = 0; i < BDROWS; i++) {
        for (int j = 0; j < BDCOLS; j++)
            switch (board[i][j]) {
                case I: printf("%s  ", CYAN  ); break;
                case O: printf("%s  ", YELLOW); break;
                case T: printf("%s  ", PURPLE); break;
                case L: printf("%s  ", ORANGE); break;
                case J: printf("%s  ", BLUE  ); break;
                case S: printf("%s  ", GREEN ); break;
                case Z: printf("%s  ", RED   ); break;
                case EMPTY:
                    if (down_is_valid(board, p) &&
                          s.pos.x <= i && i <= s.pos.x + 3 &&
                          s.pos.y <= j && j <= s.pos.y + 3 &&
                          s.matrix[i - s.pos.x][j - s.pos.y] != EMPTY)
                        printf("%s  ", LTGREY);
                    else
                        printf("%s  ", BG);
                    break;
            }
        cursor_down(1);
        cursor_left(BDCOLS*2);
    }
    cursor_up(BDROWS);
    printf("%s", BG);
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

void render_hold(Piece p_held, Coord hold) {
    cursor_to(hold.x, hold.y);
    render_piece_in_4x8_grid(p_held);
}

void render_next(Piece p_next, Coord next) {
    cursor_to(next.x, next.y);
    render_piece_in_4x8_grid(p_next);
}

void render_points(int npoints, Coord points) {
    cursor_to(points.x, points.y);
    printf("%12d", npoints);
}

void render_level(int nlevel, Coord level) {
    cursor_to(level.x, level.y);
    printf("%3d", nlevel);
}

void quit(struct termios *term_config, int term_rows, int term_cols) {
    set_stdin_flush(term_config);
    cursor_down(term_rows);
    cursor_left(term_cols);
    show_cursor();
    exit(0);
}

void reset(int term_rows, int term_cols, GameState *gs) {
    memset(&(gs->board), EMPTY, 10 * 20 * sizeof(int));
    memset(&(gs->piece), EMPTY, sizeof(Piece));
    memset(&(gs->p_held), EMPTY, sizeof(Piece));
    gs->p_next = new_piece(rand() % 7 + 1);
    initialize_screen(term_rows, term_cols, gs);
    gs->points = 0;
    gs->level = 0;
    gs->fall_time = 0.8;
    gs->total_rows = 0;
    gs->hard_dropped = 0;
    gs->was_held = 0;
}

void process_keypress(char c, struct termios *term_config,
                      int term_rows, int term_cols, GameState *gs) {
    switch (c) {
        case '!': quit(term_config, term_rows, term_cols);     break;
        case '1': reset(term_rows, term_cols, gs);             break;
        case 'a': turn_left(gs->board, &(gs->piece));          break;
        case 's': hold(gs->board, &(gs->piece), &(gs->p_next),
                       &(gs->p_held), &(gs->was_held));        break;
        case 'd': turn_right(gs->board, &(gs->piece));         break;
        case 'f': turn_180(gs->board, &(gs->piece));           break;
        case ' ': hard_drop(gs->board, &(gs->piece),
                            &(gs->hard_dropped));              break;
        case '\033':      // arrow key
            fgetc(stdin); // first character after \033 is [
            switch (fgetc(stdin)) {
                case 'D': move_left(gs->board, &(gs->piece));  break;
                case 'C': move_right(gs->board, &(gs->piece)); break;
                case 'B': soft_drop(gs->board, &(gs->piece));  break;
            }
            break;
        default: break;
    }
}

void shift_above_rows(int board[BDROWS][BDCOLS], int curr_row) {
    for (int i = curr_row; i > 0; i--)
        for (int j = 0; j < BDCOLS; j++)
            board[i][j] = board[i-1][j];

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
        if (empty_cells == 0) { // filled row
            for (int j = 0; j < BDCOLS; j++)
                board[i][j] = EMPTY;
            shift_above_rows(board, i);
            filled_rows_cnt++;
        }
    }

    *rows_count = filled_rows_cnt;
}

void update_fall_time(float *fall_time, int lv) {
    if ((1 <= lv && lv <= 10) || lv == 13 || lv == 16 || lv == 19 || lv == 29 || lv == 39)
        *fall_time *= 0.85;
}

void update_game_state(GameState *gs, int rows_cleared) {
    // score follows NEStris system
    switch (rows_cleared) {
        case 1:  gs->points +=   40 * (gs->level + 1); break; // single
        case 2:  gs->points +=  100 * (gs->level + 1); break; // double
        case 3:  gs->points +=  300 * (gs->level + 1); break; // triple
        default: gs->points += 1200 * (gs->level + 1); break; // tetris
    }
    if ((gs->total_rows + rows_cleared)/10 > gs->total_rows/10) {
        gs->level++;
        update_fall_time(&(gs->fall_time), gs->level);
    }
}

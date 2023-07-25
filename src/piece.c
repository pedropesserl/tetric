#include <stdlib.h>
#include "matrix.h"
#include "piece.h"

void clear_piece(int board[BDROWS][BDCOLS], Piece p) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p.matrix[i][j] != EMPTY)
                board[i + p.posv][j + p.posh] = EMPTY;
}

void stamp_piece(int board[BDROWS][BDCOLS], Piece p) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p.matrix[i][j] != EMPTY && i + p.posv >= 0)
                board[i + p.posv][j + p.posh] = p.matrix[i][j];
}

int boundaries_are_valid(Piece p) {
    if (p.posv + lowest_row(p.matrix) > 19)
        return 0;    // end of board
    if (p.posh + leftmost_column(p.matrix) < 0)
        return 0;    // left wall
    if (p.posh + rightmost_column(p.matrix) > 9)
        return 0;    // right wall
    return 1;
}

int piece_board_collision(int board[BDROWS][BDCOLS], Piece p) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p.matrix[i][j] != EMPTY && board[i + p.posv][j + p.posh] != EMPTY)
                return 1;
    return 0;
}

int down_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    if (p.posv + lowest_row(p.matrix) >= 19)
        return 0; // end of board
    for (int j = 0; j < 4; j++)
        for (int i = 3; i >= 0; i--)
            if (p.matrix[i][j] != EMPTY) {
                if (board[i+1 + p.posv][j + p.posh] != EMPTY)
                    return 0; // blocks underneath
                else
                    break;
            }
    return 1;
}

int left_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    if (p.posh + leftmost_column(p.matrix) <= 0)
        return 0; // left wall
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (p.matrix[i][j] != EMPTY) {
                if (board[i + p.posv][j-1 + p.posh] != EMPTY)
                    return 0; // blocks to the left
                else
                    break;
            }
    return 1;
}

int right_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    if (p.posh + rightmost_column(p.matrix) >= 9)
        return 0; // right wall
    for (int i = 0; i < 4; i++)
        for (int j = 3; j >= 0; j--)
            if (p.matrix[i][j] != EMPTY) {
                if (board[i + p.posv][j+1 + p.posh] != EMPTY)
                    return 0; // blocks to the left
                else
                    break;
            }
    return 1;
}

int turn_left_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    rotate_left(p.matrix, p.type == I ? 4 : 3);
    return boundaries_are_valid(p) && !piece_board_collision(board, p);
}

int turn_right_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    rotate_right(p.matrix, p.type == I ? 4 : 3);
    return boundaries_are_valid(p) && !piece_board_collision(board, p);
}

int turn_180_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    reflect_horizontal(p.matrix, p.type == I ? 4 : 3);
    reflect_vertical(p.matrix, p.type == I ? 4 : 3);
    return boundaries_are_valid(p) && !piece_board_collision(board, p);
}

Piece new_piece(int type, float fall_time) {
    Piece p = {0};
    p.type = type;
    p.fall_time = fall_time;
    p.posv = 0;
    p.posh = 3;
    switch (type) {
        case I: 
            // 0 0 0 0
            // I I I I
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[1][0] = p.matrix[1][1] = p.matrix[1][2] = p.matrix[1][3] = I;
            p.posv = -1;
            break;
        case O: 
            // 0 0 0 0
            // 0 O O 0
            // 0 O O 0
            // 0 0 0 0
            p.matrix[1][1] = p.matrix[1][2] = p.matrix[2][1] = p.matrix[2][2] = O;
            p.posv = -1;
            break;
        case T: 
            // 0 T 0 0
            // T T T 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][1] = p.matrix[1][0] = p.matrix[1][1] = p.matrix[1][2] = T;
            break;
        case L: 
            // 0 0 L 0
            // L L L 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][2] = p.matrix[1][0] = p.matrix[1][1] = p.matrix[1][2] = L;
            break;
        case J: 
            // J 0 0 0
            // J J J 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][0] = p.matrix[1][0] = p.matrix[1][1] = p.matrix[1][2] = J;
            break;
        case S: 
            // 0 S S 0
            // S S 0 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][1] = p.matrix[0][2] = p.matrix[1][0] = p.matrix[1][1] = S;
            break;
        case Z: 
            // Z Z 0 0
            // 0 Z Z 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][0] = p.matrix[0][1] = p.matrix[1][1] = p.matrix[1][2] = Z;
            break;
    }
    return p;
}

void update_falling_piece(int board[BDROWS][BDCOLS], Piece *p) {
    clear_piece(board, *p);
    p->posv++;
    stamp_piece(board, *p);
}

void turn_left(int board[BDROWS][BDCOLS], Piece *p) {
    if (p->type == O)
        return;

    clear_piece(board, *p);
    if (turn_left_is_valid(board, *p))
        rotate_left(p->matrix, p->type == I ? 4 : 3);
    stamp_piece(board, *p);
}

void turn_right(int board[BDROWS][BDCOLS], Piece *p) {
    if (p->type == O)
        return;

    clear_piece(board, *p);
    if (turn_right_is_valid(board, *p))
        rotate_right(p->matrix, p->type == I ? 4 : 3);
    stamp_piece(board, *p);
}

void turn_180(int board[BDROWS][BDCOLS], Piece *p) {
    if (p->type == O)
        return;

    clear_piece(board, *p);
    if (turn_180_is_valid(board, *p)) {
        reflect_horizontal(p->matrix, p->type == I ? 4 : 3);
        reflect_vertical(p->matrix, p->type == I ? 4 : 3);
    }
    stamp_piece(board, *p);
}

void hold(int board[BDROWS][BDCOLS], Piece *p, Piece *next, Piece *held, int *was_held) {
    if (*was_held)
        return;
    *was_held = 1;
    clear_piece(board, *p);
    if (held->type == 0) { // first hold
        *held = *next;
        *next = new_piece(rand() % 7 + 1, 0.8);
    }
    Piece temp = *p;
    *p = *held;
    p->posv = p->type == I || p->type == O ? -1: 0;
    p->posh = 3;
    *held = temp;
    held->posv = held->type == I || p->type == O ? -1: 0;
    held->posh = 3;
    stamp_piece(board, *p);
}

void move_left(int board[BDROWS][BDCOLS], Piece *p) {
    if (!left_is_valid(board, *p))
        return;
    
    clear_piece(board, *p);
    p->posh--;
    stamp_piece(board, *p);
}

void move_right(int board[BDROWS][BDCOLS], Piece *p) {
    if (!right_is_valid(board, *p))
        return;
    
    clear_piece(board, *p);
    p->posh++;
    stamp_piece(board, *p);
}

void soft_drop(int board[BDROWS][BDCOLS], Piece *p) {
    clear_piece(board, *p);
    while (down_is_valid(board, *p))
        p->posv++;
    stamp_piece(board, *p);
}

void hard_drop(int board[BDROWS][BDCOLS], Piece *p, int *hard_dropped) {
    *hard_dropped = 1;
    clear_piece(board, *p);
    while (down_is_valid(board, *p))
        p->posv++;
    stamp_piece(board, *p);
}

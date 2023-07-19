#ifndef PIECE_H_
#define PIECE_H_

#include "matrix.h"
#include "piece.h"

#define BDROWS 20
#define BDCOLS 10
#define EMPTY 0
#define I 1
#define O 2
#define T 3
#define L 4
#define J 5
#define S 6
#define Z 7

typedef struct Piece {
    int type;
    int matrix[MAX][MAX];
    float fall_time;
    int posv, posh; // position of matrix's upper-left corner relative to board
} Piece;

void clear_piece(int board[BDROWS][BDCOLS], Piece p);

void stamp_piece(int board[BDROWS][BDCOLS], Piece p);

int boundaries_are_valid(Piece p);

int piece_board_collision(int board[BDROWS][BDCOLS], Piece p);

int down_is_valid(int board[BDROWS][BDCOLS], Piece p);

int left_is_valid(int board[BDROWS][BDCOLS], Piece p);

int right_is_valid(int board[BDROWS][BDCOLS], Piece p);

int turn_left_is_valid(int board[BDROWS][BDCOLS], Piece p);

int turn_right_is_valid(int board[BDROWS][BDCOLS], Piece p);

Piece new_piece(int type, float fall_time);

void update_falling_piece(int board[BDROWS][BDCOLS], Piece *p);

void turn_left(int board[BDROWS][BDCOLS], Piece *p);

void turn_right(int board[BDROWS][BDCOLS], Piece *p);

void turn_180(int board[BDROWS][BDCOLS], Piece *p);

void hold(Piece *p, Piece *held);

void move_left(int board[BDROWS][BDCOLS], Piece *p);

void move_right(int board[BDROWS][BDCOLS], Piece *p);

void soft_drop(Piece *p);

void hard_drop(int board[BDROWS][BDCOLS], Piece *p);

#endif // PIECE_H_

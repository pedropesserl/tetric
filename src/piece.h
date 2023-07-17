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
    int matrix[4][4];
    float fall_time;
    int posv, posh; // position of matrix's upper-left corner relative to board
} Piece;

int next_position_is_valid(int board[BDROWS][BDCOLS], Piece p);

Piece new_piece(int type, float fall_time);

void turn_left(Piece *p);

void turn_right(Piece *p);

#endif // PIECE_H_

#include "matrix.h"
#include "piece.h"

int next_position_is_valid(int board[BDROWS][BDCOLS], Piece p) {
    int lowest = lowest_row(p.matrix);
    if (p.posv + lowest >= 19)
        return 0;     // end of board
    for (int j = 0; j < 4; j++)
        if (p.matrix[lowest][j] != EMPTY)
            if (board[lowest + 1 + p.posv][j + p.posh] != EMPTY) 
                return 0; // blocks underneath
    return 1;
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
            break;
        case O: 
            // 0 O O 0
            // 0 O O 0
            // 0 0 0 0
            // 0 0 0 0
            p.matrix[0][1] = p.matrix[0][2] = p.matrix[1][1] = p.matrix[1][2] = O;
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

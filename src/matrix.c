#include <stdio.h>
#include "matrix.h"

#ifndef EMPTY
#define EMPTY 0
#endif

void print_matrix(int m[SZ][SZ]) {
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}

int highest_row(int m[SZ][SZ]) {
    for (int i = 0; i < SZ; i++)
        for (int j = 0; j < SZ; j++)
            if (m[i][j] != EMPTY)
                return i;
    return -1;
}

int lowest_row(int m[SZ][SZ]) {
    for (int i = SZ-1; i >= 0; i--)
        for (int j = 0; j < SZ; j++)
            if (m[i][j] != EMPTY)
                return i;
    return -1;
}

int leftmost_column(int m[SZ][SZ]) {
    for (int j = 0; j < SZ; j++)
        for (int i = 0; i < SZ; i++)
            if (m[i][j] != EMPTY)
                return j;
    return -1;
}

int rightmost_column(int m[SZ][SZ]) {
    for (int j = SZ-1; j >= 0; j--)
        for (int i = 0; i < SZ; i++)
            if (m[i][j] != EMPTY)
                return j;
    return -1;
}



void rotate_left(int (*m)[SZ][SZ]) {
    (*m)[2][2] = 1;
    fprintf(stderr, "rotate_left: nao implementada (matrix.c)\n");
}

void rotate_right(int (*m)[SZ][SZ]) {
    fprintf(stderr, "rotate_right: nao implementada (matrix.c)\n");
}

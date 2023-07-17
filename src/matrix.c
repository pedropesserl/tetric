#include <stdio.h>
#include "matrix.h"

#ifndef EMPTY
#define EMPTY 0
#endif

void print_matrix(int m[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX-1; j++)
            printf("%d ", m[i][j]);
        printf("%d\n", m[i][MAX-1]);
    }
}

int highest_row(int m[MAX][MAX]) {
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            if (m[i][j] != EMPTY)
                return i;
    return -1;
}

int lowest_row(int m[MAX][MAX]) {
    for (int i = MAX-1; i >= 0; i--)
        for (int j = 0; j < MAX; j++)
            if (m[i][j] != EMPTY)
                return i;
    return -1;
}

int leftmost_column(int m[MAX][MAX]) {
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            if (m[i][j] != EMPTY)
                return j;
    return -1;
}

int rightmost_column(int m[MAX][MAX]) {
    for (int j = MAX-1; j >= 0; j--)
        for (int i = 0; i < MAX; i++)
            if (m[i][j] != EMPTY)
                return j;
    return -1;
}

static void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void rotate_left(int m[MAX][MAX], int sz) {
    for (int i = 0; i < sz/2; i++)
        for (int j = i; j < sz-1-i; j++) {
            swap(&(m[j][sz-1-i]), &(m[i][j]));
            swap(&(m[j][sz-1-i]), &(m[sz-1-j][i]));
            swap(&(m[j][sz-1-i]), &(m[sz-1-i][sz-1-j]));
        }
}

void rotate_right(int m[MAX][MAX], int sz) {
    for (int i = 0; i < sz/2; i++)
        for (int j = i; j < sz-1-i; j++) {
            swap(&(m[i][j]), &(m[j][sz-1-i]));
            swap(&(m[i][j]), &(m[sz-1-i][sz-1-j]));
            swap(&(m[i][j]), &(m[sz-1-j][i]));
        }
}

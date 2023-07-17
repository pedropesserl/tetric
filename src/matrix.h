#ifndef MATRIX_H_
#define MATRIX_H_

#define MAX 4

void print_matrix(int m[MAX][MAX]);

int highest_row(int m[MAX][MAX]);

int lowest_row(int m[MAX][MAX]);

int leftmost_column(int m[MAX][MAX]);

int rightmost_column(int m[MAX][MAX]);

void rotate_left(int m[MAX][MAX], int sz);

void rotate_right(int m[MAX][MAX], int sz);

#endif // MATRIX_H_

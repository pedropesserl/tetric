#ifndef MATRIX_H_
#define MATRIX_H_

#define SZ 4

void print_matrix(int m[SZ][SZ]);

int highest_row(int m[SZ][SZ]);

int lowest_row(int m[SZ][SZ]);

int leftmost_column(int m[SZ][SZ]);

int rightmost_column(int m[SZ][SZ]);

void rotate_left(int m[SZ][SZ]);

void rotate_right(int m[SZ][SZ]);

#endif // MATRIX_H_

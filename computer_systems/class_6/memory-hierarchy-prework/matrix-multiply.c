/*
Naive code for multiplying two matrices together.

There must be a better way!
*/

#include <stdio.h>
#include <stdlib.h>

/*
  A naive implementation of matrix multiplication.

  DO NOT MODIFY THIS FUNCTION, the tests assume it works correctly, which it
  currently does
*/
void matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  for (int i = 0; i < a_rows; i++) {
    for (int j = 0; j < b_cols; j++) {
      C[i][j] = 0;
      for (int k = 0; k < a_cols; k++)
        C[i][j] += A[i][k] * B[k][j];
    }
  }
}
void fast_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  double a_val;
  for (int x = 0; x < a_rows; x++) {
    for (int y = 0; y < b_cols; y++) {
      C[x][y] = 0;
    }
  }

  for (int i = 0; i < a_rows; i++) {
    double *c_ptr = *(C+i);
    int j;
    for (j = 0; j < b_cols; j++) {
        a_val = *(*(A+i)+j);
        int k;
        double *b_ptr = *(B+j);
        for (k = 0; k+3 < a_cols; k += 4) {
          *(c_ptr+k) += *b_ptr * a_val;
          *(c_ptr+k+1) += *(b_ptr+1) * a_val;
          *(c_ptr+k+2) += *(b_ptr+2) * a_val;
          *(c_ptr+k+3) += *(b_ptr+3) * a_val;
          b_ptr += 4;
        }
        for (; k < a_cols; k++) {
          C[i][k] += B[j][k] * A[i][j];
        }
    }
  }
}
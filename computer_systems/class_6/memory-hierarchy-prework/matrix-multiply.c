/*
Naive code for multiplying two matrices together.

There must be a better way!
*/

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include "csapp.c"

/*
  A naive implementation of matrix multiplication.

  DO NOT MODIFY THIS FUNCTION, the tests assume it works correctly, which it
  currently does
*/

void *thread(void *vargp);

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

typedef struct {
  int row;
  int col;
  double **A;
  double **B;
  double **C;
  int square_size;
} point;

#define MAXTHREADS 1
#define SQUARESIZE 1

long psum[MAXTHREADS];
long nelems_per_thread = SQUARESIZE / MAXTHREADS;
point my_point;

void fast_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  pthread_t tid[MAXTHREADS];
  long my_id[MAXTHREADS];
  my_point.A = A;
  my_point.B = B;
  my_point.C = C;
  my_point.square_size = a_cols;
  for (int i = 0; i < a_rows; i++) {
    my_point.row = i;
    for (int j = 0; j < b_cols; j++) {
      my_point.col = j;
      int k;
      C[i][j] = 0;
      for (k = 0; k < MAXTHREADS; k++){
        my_id[k] = k;
        Pthread_create(&tid[k], NULL, thread, &my_id[k]);
      }
      for (k = 0; k < MAXTHREADS; k++){
        Pthread_join(tid[k], NULL);
      }
      for (k=0; k < MAXTHREADS;k++){
        C[i][j] += psum[k];
      }
    }
  }
}

  void *thread(void *vargp) {
    int value = 0;
    long myid = *((long *)vargp);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;

    for (long k = start; k < end; k++) {
      value += my_point.A[my_point.row][k] * my_point.B[k][my_point.col];
    }
    psum[myid] = value;
    return NULL;
  }


















void old_fast_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
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
        for (k = 0; k+7 < a_cols; k += 8) {
          double *c_ptr_k = c_ptr+k;
          *(c_ptr_k) += *b_ptr * a_val;
          *(c_ptr_k+1) += *(b_ptr+1) * a_val;
          *(c_ptr_k+2) += *(b_ptr+2) * a_val;
          *(c_ptr_k+3) += *(b_ptr+3) * a_val;
          *(c_ptr_k+4) += *(b_ptr+4) * a_val;
          *(c_ptr_k+5) += *(b_ptr+5) * a_val;
          *(c_ptr_k+6) += *(b_ptr+6) * a_val;
          *(c_ptr_k+7) += *(b_ptr+7) * a_val;
          b_ptr += 8;
        }
        for (; k < a_cols; k++) {
          C[i][k] += B[j][k] * A[i][j];
        }
    }
  }
}
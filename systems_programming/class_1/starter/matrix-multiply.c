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
#define SQUARESIZE 512

long psum[MAXTHREADS];
long nelems_per_thread = SQUARESIZE / MAXTHREADS;
point my_point;
long value;

void parallel_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  pthread_t tid[MAXTHREADS];
  pthread_t one_tid;
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
        psum[k] = 0;
      }      
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
  my_point.C[my_point.row][my_point.col] = value;
  // psum[myid] = value;
  return NULL;
}

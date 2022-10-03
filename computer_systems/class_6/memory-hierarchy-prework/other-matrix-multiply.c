void nfast_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  double a_val;
  for (int x = 0; x < a_rows; x++) {
    for (int y = 0; y < b_cols; y++) {
      C[x][y] = 0;
    }
  }

  for (int i = 0; i < a_rows; i++) {
    for (int j = 0; j < b_cols; j++) {
        a_val = *(*(A+i)+j);
        int k;
        for (k = 0; k+3 < a_cols; k += 4) {
          double *b_ptr = *(B+j)+k;
          C[i][k] += *b_ptr * a_val;
          C[i][k+1] += *(b_ptr+1) * a_val;
          C[i][k+2] += *(b_ptr+2) * a_val;
          C[i][k+3] += *(b_ptr+3) * a_val;
        }
        for (; k < a_cols; k++) {
          double b_val = *(*(B+j)+k);
          C[i][k] += b_val * a_val;
        }
      }
  }
}





void old2_fast_matrix_multiply(double **C, double **A, double **B, int a_rows, int a_cols,
                     int b_cols) {
  double a_val;
  for (int x = 0; x < a_rows; x++) {
    for (int y = 0; y < b_cols; y++) {
      C[x][y] = 0;
    }
  }

  for (int i = 0; i < a_rows; i++) {
    for (int j = 0; j < b_cols; j++) {
        a_val = A[i][j];
        int k;
        for (k = 3; k < a_cols; k += 4) {
          C[i][k] += B[j][k] * a_val;
          C[i][k-1] += B[j][k-1] * a_val;
          C[i][k-2] += B[j][k-2] * a_val;
          C[i][k-3] += B[j][k-3] * a_val;
        }
        for (; k < a_cols; k++)
          C[i][k] += B[j][k] * a_val;
      }
  }
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
    for (int j = 0; j < b_cols; j++) {
        a_val = A[i][j];
        for (int k = 0; k < a_cols; k++)
          C[i][k] += B[j][k] * a_val;
      }
  }
}
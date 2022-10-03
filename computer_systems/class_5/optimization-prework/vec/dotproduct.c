#include "vec.h"
#include <time.h>
#include <stdio.h>


data_t dotproduct2(vec_ptr u, vec_ptr v) {
   data_t sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0, sum11 = 0;
   long i;
   long vec_len = u->len;
   for (i = 0; i+9 < u->len; i += 10) { 
      sum1 += u->data[i] * v->data[i];
      sum2 += u->data[i+1] * v->data[i+1];
      sum3 += u->data[i+2] * v->data[i+2];
      sum4 += u->data[i+3] * v->data[i+3];
      sum5 += u->data[i+4] * v->data[i+4];
      sum6 += u->data[i+5] * v->data[i+5];
      sum7 += u->data[i+6] * v->data[i+6];
      sum8 += u->data[i+7] * v->data[i+7];
      sum9 += u->data[i+8] * v->data[i+8];
      sum10 += u->data[i+9] * v->data[i+9];
   }   
   for (;i < vec_len;i++){
      sum11 += u->data[i] * v->data[i];
   }
   return sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8 + sum9 + sum10 + sum11;
}

data_t dotproduct1(vec_ptr u, vec_ptr v) {
   data_t sum = 0;
   long vec_len = u->len;
   long i;
   for (i = 0; i < vec_len; i += 1) { // we can assume both vectors are same length
        sum += (u->data[i] * v->data[i]);
   }  
   return sum;
}



data_t dotproduct(vec_ptr u, vec_ptr v) {
   data_t sum = 0, u_val, v_val;
   for (long i = 0; i < vec_length(u); i++) { // we can assume both vectors are same length
        get_vec_element(u, i, &u_val);
        get_vec_element(v, i, &v_val);
        sum += u_val * v_val;
   }   
   return sum;
}

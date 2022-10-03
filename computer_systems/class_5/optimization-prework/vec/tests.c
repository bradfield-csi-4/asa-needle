#include "vendor/unity.h"

#include "vec.h"

#include <stdlib.h>

extern data_t dotproduct(vec_ptr, vec_ptr);
extern data_t dotproduct1(vec_ptr, vec_ptr);
extern data_t dotproduct2(vec_ptr, vec_ptr);

void setUp(void) {
}

void tearDown(void) {
}

void test_empty(void) {
  vec_ptr u = new_vec(0);
  vec_ptr v = new_vec(0);

  TEST_ASSERT_EQUAL(0, dotproduct(u, v));

  free_vec(u);
  free_vec(v);
}

void test_basic(void) {
  vec_ptr u = new_vec(3);
  vec_ptr v = new_vec(3);

  set_vec_element(u, 0, 1);
  set_vec_element(u, 1, 2);
  set_vec_element(u, 2, 3);
  set_vec_element(v, 0, 4);
  set_vec_element(v, 1, 5);
  set_vec_element(v, 2, 6);

  TEST_ASSERT_EQUAL(32, dotproduct(u, v));

  free_vec(u);
  free_vec(v);
}

void test_longer(void) {
  long n = 1000000;
  vec_ptr u = new_vec(n);
  vec_ptr v = new_vec(n);

  for (long i = 0; i < n; i++) {
    set_vec_element(u, i, i + 1);
    set_vec_element(v, i, i + 1);
  }

  long expected = (2 * n * n * n + 3 * n * n + n) / 6; // formula for the sum of the squares of the first n natural numbers: 2n^3 + 3n^2 + n
  TEST_ASSERT_EQUAL(expected, dotproduct(u, v));

  free_vec(u);
  free_vec(v);
}

void test_random(void) {
  long n = 1000000;
  vec_ptr u;
  vec_ptr v;
  clock_t start, end;
  double clocks1, clocks2;
  double total_clock_dif = 0;
  long sum1, sum2;
  
  for (int count = 0; count < 100; count++){
    u = new_vec(n);
    v = new_vec(n);
    for (long i = 0; i < n; i++) {
      set_vec_element(u, i, rand());
      set_vec_element(v, i, rand());
    }

    start = clock();
    sum1 = dotproduct1(u, v);
    end = clock();
    clocks1 = end - start;
    start = clock();
    sum2 = dotproduct2(u, v);
    end = clock();
    clocks2 = end - start;
    TEST_ASSERT_EQUAL(sum1, sum2);
    total_clock_dif += (clocks1 - clocks2) / 100;
    free_vec(u);
    free_vec(v);
  }
  printf("%lf clocks faster, on average\n", total_clock_dif);
}


int main(void) {
    UNITY_BEGIN();

    // RUN_TEST(test_empty);
    // RUN_TEST(test_basic);
    // RUN_TEST(test_longer);
    RUN_TEST(test_random);
    return UNITY_END();
}

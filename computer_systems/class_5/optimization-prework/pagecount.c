#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define TEST_LOOPS 10000000

// division with r8: 24
// division with r16: 24
// division with r32: 40

uint64_t pagecount(uint64_t memory_size, uint64_t page_size) {
  int result = 1;
  while (page_size % 1024 == 0){
    page_size = page_size >> 10;
    memory_size = memory_size >> 10;
  }
  while (page_size % 2 == 0){
    page_size = page_size >> 1;
    memory_size = memory_size >> 1;
  }
  return memory_size;
}



// uint64_t pagecount(uint64_t memory_size, uint64_t page_size) {
//   int result = 1;
//   while (page_size % 16 == 0){
//     page_size = page_size >> 4;
//     memory_size = memory_size >> 4;
//   }
//   while (page_size % 2 == 0){
//     page_size = page_size >> 1;
//     memory_size = memory_size >> 1;
//   }
//   return memory_size;
// }

uint64_t pagecount2(uint64_t memory_size, uint64_t page_size) {
  return memory_size / page_size;
}


int main (int argc, char** argv) {
  clock_t baseline_start, baseline_end, test_start, test_end;
  uint64_t memory_size, page_size;
  double clocks_elapsed, time_elapsed;
  int i, ignore = 0;

  uint64_t msizes[] = {1L << 32, 1L << 40, 1L << 52};
  uint64_t psizes[] = {1L << 12, 1L << 16, 1L << 32};

  baseline_start = clock();
  for (i = 0; i < TEST_LOOPS; i++) {
    memory_size = msizes[i % 3];
    page_size = psizes[i % 3];
    ignore += 1 + memory_size +
              page_size; // so that this loop isn't just optimized away
  }
  baseline_end = clock();

  test_start = clock();
  for (i = 0; i < TEST_LOOPS; i++) {
    memory_size = msizes[i % 3];
    page_size = psizes[i % 3];
    ignore += pagecount(memory_size, page_size) + memory_size + page_size;
  }
  test_end = clock();

  clocks_elapsed = test_end - test_start - (baseline_end - baseline_start);
  time_elapsed = clocks_elapsed / CLOCKS_PER_SEC;

  printf("%.2fs to run %d tests (%.2fns per test)\n", time_elapsed, TEST_LOOPS,
         time_elapsed * 1e9 / TEST_LOOPS);
  return ignore;
}


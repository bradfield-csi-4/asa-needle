#include <stdint.h>
#include <stdio.h>
#include <time.h>

// division with r8: 24
// division with r16: 24
// division with r32: 40

// 0001 0000
// 0000 0010

uint64_t pagecount(uint64_t memory_size, uint64_t page_size) {
  int result = 1;
  while (page_size % 4 == 0){
    page_size = page_size >> 2;
    memory_size = memory_size >> 2;
  }
  if (page_size % 2 == 0){
    page_size = page_size >> 1;
    memory_size = memory_size >> 1;
  }
  return memory_size;
}

uint64_t pagecount2(uint64_t memory_size, uint64_t page_size) {
  return memory_size / page_size;
}

int main(){
    int result = pagecount(16, 4);
    int result2 = pagecount2(16, 4);
    printf("%d\n", result);
    printf("%d\n", result2);
    result = pagecount(3648, 64);
    result2 = pagecount2(3648, 64);
    printf("%d\n", result);
    printf("%d\n", result2);

    return 1;
}
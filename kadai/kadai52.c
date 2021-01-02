#include <stdio.h>

int main() {
  unsigned int i = 5;

  while(!(i == (0b0 - 0b1))) {
    printf("i=%d\n", i);
    --i;
  }

  return 0;
}

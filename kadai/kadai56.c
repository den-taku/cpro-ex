#include <stdio.h>

int main() {
  float x = 1;
  int i;
  for(i = 0; i < 10; ++i) {
    x = x - 1.0/3.0;
    if(x >= 0 && x <= 0.00000001) {
      break;
    }
    printf("%.20f\n", x);
  }
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int COUNT_SET_RAND = 0;

void print_oct(int m, int n, const float* x, const char * name) {
  int i, j;
  printf("%c = [ ", *name);
  for(i = 0; i < m; ++i){
    for(j = 0; j < n; ++j) {
      printf("%f ", x[i * n + j]);
    }
    printf("\n");
  }
  printf("];\n");
}

void srand_init(){
  if(COUNT_SET_RAND == 0){
    srand(time(NULL));
  }
  ++COUNT_SET_RAND;
}

void rand_init(int n, float* o) {
  srand_init();
  for(--n; n >= 0; --n) {
    o[n] = (float) rand() / (RAND_MAX / 2) - 1; 
  }
}

int main() {
  float y[6];
  print_oct(2, 3, y, "y");
  rand_init(6, y);
  print_oct(2, 3, y, "y");
  return 0;
}

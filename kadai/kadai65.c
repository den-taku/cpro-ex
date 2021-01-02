#include <stdio.h>

void print_oct(int m, int n, const float* x, const char * name) {
  int i;
  int j;
  printf("%c = [ ", *name);
  for(i = 0; i < m; ++i){
    for(j = 0; j < n; ++j) {
      printf("%f ", x[i * n + j]);
    }
    printf("\n");
  }
  printf("];\n");
}

void init(int n, float x, float* o) {
  for(--n; n >= 0; --n){
    o[n] = x;
  }
}

int main() {
  float y[6];
  print_oct(2, 3, y, "y");
  init(6, 7, y);
  print_oct(2, 3, y, "y");
  return 0;
}

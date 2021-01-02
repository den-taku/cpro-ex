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

int main() {
  float m[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  print_oct(3, 4, m, "m");
  return 0;
}

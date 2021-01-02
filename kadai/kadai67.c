#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void mul(int m, int n, const float* x, const float* A, float* o){
  int i, j;
  for(i = 0; i < m; ++i) { // i -> o[i] 
    int sum = 0;
    for(j = 0; j < n; ++j) { // j -> A[i+j], x[j]
      sum += A[i * n + j] * x[j];  
    }
    o[i] = sum; 
  }
}

int main() {
  float A[6] = {1, 2, 3, 4, 5, 6};
  float x[3] = {2, 3, 5};
  float o[2];
  mul(2, 3, x, A, o);
  print_oct(2, 3, A, "A");
  print_oct(3, 1, x, "x");
  print_oct(2, 1, o, "o");
  return 0;
}
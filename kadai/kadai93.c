#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int COUNT_SET_RAND = 0;

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

void print(int m, int n, const float* x) {
  int i;
  int j;
  for(i = 0; i < m; ++i){
    for(j = 0; j < n; ++j) {
      printf("%f ", x[i * n + j]);
    }
    printf("\n");
  }
}

void relu(int n, const float* x, float* y){
  int i;
  for(i = 0; i < n; ++i){
    if(x[i] < 0){
      y[i] = 0;
    }else{
      y[i] = x[i];
    }
  }
}

int main(int argc, char* argv[]){
  int n = 0;
  if(argc > 1){
    n = atoi(argv[1]);
  }
  float* x = (float*)malloc( sizeof(float) * n);
  float* y = (float*)malloc( sizeof(float) * n);
  rand_init(n, x);
  print(1, n, x);
  relu(n, x, y);
  print(1, n, y);
  free(x);
  free(y);
  return 0;
}

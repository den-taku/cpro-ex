#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void load(const char* filename, int m, int n, float* A, float* b){
  int i;
  FILE* fp = NULL;
  float* buf = (float*)malloc(sizeof(float) * (m + 1) * n);
  fp = fopen(filename, "r");
  if(!fp){
    printf("Err : %s cannot open\n", filename);
    exit(1);
  }
  fread(buf, sizeof(float), (m + 1) * n, fp);
  for(i = 0; i < m * n; ++i){
    A[i] = buf[i];
  }
  for(i = 0; i < n; ++i){
    b[i] = buf[(m * n) + i];
  }
  fclose(fp);
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

void init(int n, float x, float* y){
  int i;
  for(i = 0; i < n; ++i){
    y[i] = x;
  }
}

int main(int argc, char* argv[]){
  int m = 0;
  int n = 0;
  if(argc > 2){
    m = atoi(argv[1]);
    n = atoi(argv[2]);
  }
  float* A = (float*)malloc( sizeof(float) * m * n);
  float* b = (float*)malloc( sizeof(float) * n);
  load("test.dat", m, n, A, b);
  print(m, n, A);
  print(1, n, b);
  free(A);
  free(b);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void save(const char* filename, int m, int n, const float* A, const float* b){
  FILE *fp = NULL;
  fp = fopen(filename, "w");
  if(!fp){
    printf("Err : %s cannot open\n", filename);
    exit(1);
  }
  fwrite(A, sizeof(A[0]), m * n, fp);
  fwrite(b, sizeof(b[0]), n, fp); 
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
  init(m * n, 1, A);
  init(n, 2, b);
  print(m, n, A);
  print(1, n, b);
  save("test.dat", m, n, A, b);
  free(A);
  free(b);
  return 0;
}

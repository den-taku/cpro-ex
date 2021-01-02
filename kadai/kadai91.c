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

void change(float* l, int a, int b){
  float num = l[a];
  l[a] = l[b];
  l[b] = num;
}

void quick_sort_local(float* l, int large, int small) {
  float pivot = l[(large + small) / 2];
  int lag = large;
  int sml = small;

  while(true){
    while(l[lag] < pivot){
      ++lag;
    }
    while(l[sml] > pivot){
      --sml;
    }

    if(lag >= sml){
      break;
    }

    change(l, sml, lag);

    ++lag;
    --sml;
  }

  if(lag - large >= 2){
    quick_sort_local(l, large, lag - 1);
  }
  if(small - sml >= 2){
    quick_sort_local(l, sml + 1, small);
  }
}

void quick_sort(float* l, int length) {
  quick_sort_local(l, 0, length - 1);
}

void sort(float*y, int n){
  quick_sort(y, n);
}


int main(int argc, char* argv[]){
  int n = 0;
  if(argc > 1){
    n = atoi(argv[1]);
  }
  float* y = (float*)malloc( sizeof(float) * n);
  rand_init(n, y);
  print(1, n, y);
  sort(y, n);
  print(1, n, y);
  free(y);
  return 0;
}

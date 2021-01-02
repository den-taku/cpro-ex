#include <stdio.h>

void quick_sort(int* l, int length);
void quick_sort_local(int* l, int large, int small);
void change(int* l, int a, int b);

int main(){
  int x[5] = {4, 1, 8, 2, 9};
  int i;
  int range;

  for(i = 0; i < 5; i++){
    printf("x[%d] = %d\n", i, x[i]);
  }

  quick_sort(x, 5);
  range = x[4]; 

  printf("max = %d\n", range);
  return 0;
}

void quick_sort(int* l, int length) {
  quick_sort_local(l, 0, length - 1);
}

void quick_sort_local(int* l, int large, int small) {
  int pivot = l[(large + small) / 2];
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

void change(int* l, int a, int b){
  int num = l[a];
  l[a] = l[b];
  l[b] = num;
}
    


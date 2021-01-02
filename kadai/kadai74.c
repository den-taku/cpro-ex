#include <stdio.h>

#define ARRAY_SIZE 3

void minmax(int data[], int* min, int* max);
void quick_sort(int* l, int length);
void quick_sort_local(int* l, int large, int small);
void change(int* l, int a, int b);

int main(){
  int  data[ARRAY_SIZE];
  int max, min;
  minmax(data, &max, &min);
  printf("max : %d\n", max);
  printf("min : %d\n", min);
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
    
void minmax(int data[], int* min, int* max){
  int i;
  printf("Input %d numbers -> Show max and min\n", ARRAY_SIZE);
  for(i = 0; i < ARRAY_SIZE; ++i){
    scanf("%d", &data[i]);
  } 
  quick_sort(data, ARRAY_SIZE);
  *max = data[0];
  *min = data[ARRAY_SIZE - 1];
}

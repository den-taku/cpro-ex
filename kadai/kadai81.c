#include <stdio.h>

void ShowArray(int* array, int size, int count);

void BubbleSort(int* array, int size);

void SwapArray(int* array, int j);

int main(){
  int array[] = {
    64, 30, 8, 87, 45, 13
  };
  BubbleSort(array, 6);
  return 0;
}

void ShowArray(int* array, int size, int count){
  printf("loop%d: ", count);
  int i;
  for(i = 0; i < size; ++i){
    printf("%d ", array[i]);
  }
  printf("\n");
}

void BubbleSort(int* array, int size){
  int i, j;
  for(i = 0; i < size; ++i){
    for(j = 0; j < size - i; ++j){
      if(array[j] > array[j + 1]){
        SwapArray(array, j);
      }
    }
  ShowArray(array, size, i + 1);
  }
} 

void SwapArray(int* array, int j){
  array[j] += array[j + 1];
  array[j + 1] = array[j] - array[j + 1];
  array[j] -= array[j + 1];
}

#include <stdio.h>

void inc(int* a){
  ++(*a);
}

int main(){
  int n = 0;
  printf("input a digit: ");
  scanf("%d", &n);
  inc(&n);
  printf("output: %d\n", n);
  return 0;
}

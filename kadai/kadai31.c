#include <stdio.h>
#include <stdlib.h>

int CalculateFactorial(int n){
  if(n < 0){
    printf("out_of_range \n");
    exit(0);
  }else if(n == 0){
    return 1;
  }else{
    for(int i = n, n = 1 ; ; --i){
      n *= i;
      if(i == 1){
        return n;
      }
    }
  }
}

int main(){
  int a = 0;
  scanf("%d", &a);
  printf("%d! = %d \n", a, CalculateFactorial(a));
  return 0;
}

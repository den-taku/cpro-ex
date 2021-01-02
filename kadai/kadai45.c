#include <stdio.h>

void input(int& n){
  int i;

  do{
    printf("n = ");
    scanf("%d", &i);
    if(i >= 0){
      break;
    }
    printf("Invalid input\n");
  }while(1);

  n = i;
} 

int fact(int n){
  if(n == 0){
    return 1;
  }else{
    int i = n;
    n = 1;
    while(true){
      n *= i;
      if(i == 1){
       return n;
      } 
      --i;
    }
  }
}

int perm(int n, int r){
  return fact(n) / fact(n - r);
}

void print(int n){
  int i;
  for(i = 0; i <= n; ++i){
    printf("P(%d,%d) = %d\n", n, i, perm(n, i));
  }
} 

int main(){
  int n;
  input(n);
  print(n);
}

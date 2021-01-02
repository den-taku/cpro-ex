#include <stdio.h>

int Input();

void Show0b(int i);

int main() {
  int x = Input();

  Show0b(x);

  return 0;
}

int Input(){
  int i;
  printf("Enter integer : ");
  scanf("%d", &i);
  return i;
}

void Show0b(int i){
  int j;

  for(j = 32; j >= 0; --j) {
    printf("%d", ((i >> j) & 1));
  } 
  printf("\n");
} 

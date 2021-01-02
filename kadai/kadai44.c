#include <stdio.h>

#define M 3
#define N 4

int a[M][N];
 
void Input(){
  int i, j;
  for(i = 0; i < M; ++i){
    for(j = 0; j < N; ++j){ 
      a[i][j] = 10 * i + j;
    }
  }
}
 
void Show(){
  int i, j;
  for(i = 0; i < M; ++i){
    for(j = 0; j < N; ++j){ 
      if(i == 0){
        printf(" ");
      }
      printf("%d ", a[i][j]);
    }
    printf("\n");
  }

}

int main() {
  Input();
  Show();
}

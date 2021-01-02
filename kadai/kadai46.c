#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void hist(int n){
  int i;
  int count[10] = {0};
  srand(time(NULL));
  for(i = 0; i < n; ++i){
    switch(rand()%10){
      case 0:
        ++count[0];
        break;
      case 1:
        ++count[1];
        break;
      case 2:
        ++count[2];
        break;
      case 3:
        ++count[3];
        break;
      case 4:
        ++count[4];
        break;
      case 5:
        ++count[5];
        break;
      case 6:
        ++count[6];
        break;
      case 7:
        ++count[7];
        break;
      case 8:
        ++count[8];
        break;
      case 9:
        ++count[9];
        break;
    }
  }
  for(i = 0; i < 10; ++i){
    printf("%d : %.2f \n", i, (double)count[i] * 100 /n);
  }
  printf("\n");
}

int main(){
  hist(100);
  hist(1000);
  hist(10000);
  hist(100000);
  hist(1000000);
  return 0;
}

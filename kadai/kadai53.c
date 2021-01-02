#include <stdio.h>

int main() {
  char a = 'a';
  printf("%%c : %c\n", a);
  printf("%%d : %d\n", a);
  printf("%%x : %x\n", a);

  ++a;

  printf("%%c : %c\n", a);
  printf("%%d : %d\n", a);
  printf("%%x : %x\n", a);
 
  return 0;
} 

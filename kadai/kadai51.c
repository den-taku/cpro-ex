#include <stdio.h>

int fact(int n) {
  if(n == 1) {
    return 1;
  } else {
    return n  * fact(n - 1);
  }
}

int C(int n, int r) {
  if(n == r) {
    return 1;
  } else if(r == 1) {
    return n;
  } else {
    return C(n - 1, r - 1) + C(n - 1, r);
  }
}

int main(void) {
  int n, r;

  input:

    printf("Calculate C(n, r) : You enter 2 integer \n");

    printf("n : ");
    scanf("%d", &n);

    printf("r : ");
    scanf("%d", &r);
  if(n < r) {
    printf("Err : n >= r\n\n");
    goto input;
  }

  printf("C(%d, %d) = %d\n", n, r, C(n, r));
  return 0;
}

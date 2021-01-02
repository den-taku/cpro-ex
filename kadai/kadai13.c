#include <stdio.h>
#include <math.h>

float SolveQuadraticFunction1(float a, float b, float c) {
  return ( - b + sqrt(b * b - 4 * a * c) ) / ( 2 * a );
}

float SolveQuadraticFunction2(float a, float b, float c) {
  return ( - b - sqrt(b * b - 4 * a * c) ) / ( 2 * a );
}

int main() {
  float a, b, c;

  scanf("%f", &a);
  scanf("%f", &b);
  scanf("%f", &c);

  printf("%f\n", SolveQuadraticFunction1(a, b, c));
  printf("%f\n", SolveQuadraticFunction2(a, b, c));

  return 0;
}


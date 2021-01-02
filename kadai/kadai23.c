#include <stdio.h>
#include <math.h>

bool DiscriminantIsPositive(float a, float b, float c) {
  if(b * b - 4 * a * c >= 0) {
    return true;
  }else{
    return false;
  }
}

float SolveQuadraticFunction1(float a, float b, float c) {
  return ( - b + sqrt(b * b - 4 * a * c) ) / ( 2 * a );
}

float SolveQuadraticFunction2(float a, float b, float c) {
  return ( - b - sqrt(b * b - 4 * a * c) ) / ( 2 * a );
}

float SolveQuadraticFunctionReal(float a, float b, float c) {
  return ( - b ) / ( 2 * a );
}

float SolveQuadraticFunctionImaginal(float a, float b, float c) {
  return ( sqrt(- b * b + 4 * a * c) ) / ( 2 * a );
}

int main() {
  float a, b, c;

  scanf("%f", &a);
  scanf("%f", &b);
  scanf("%f", &c);

  if(DiscriminantIsPositive(a, b, c)){
    printf("%f\n", SolveQuadraticFunction1(a, b, c));
    printf("%f\n", SolveQuadraticFunction2(a, b, c));
  }else{
    printf("%f + %f i \n", SolveQuadraticFunctionReal(a, b, c),SolveQuadraticFunctionImaginal(a, b, c));
    printf("%f - %f i \n", SolveQuadraticFunctionReal(a, b, c),SolveQuadraticFunctionImaginal(a, b, c));
  }
  return 0;
}


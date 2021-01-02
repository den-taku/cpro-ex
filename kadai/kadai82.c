#include <stdio.h>
#include <math.h>

typedef struct {
  double x;
  double y;
} Vector2d;

double GetLength(Vector2d vec);

void Input(Vector2d* vec);

void Output(Vector2d vec);

int main(){
  Vector2d vector;
  Input(&vector);
  Output(vector);
  return 0;
}

double GetLength(Vector2d vec){
  return sqrt(vec.x * vec.x + vec.y * vec.y);
}

void Input(Vector2d* vec){
  printf("Input 2D Vector\n");
  printf("Input x: ");
  scanf("%lf", &vec->x);
  printf("Input y: ");
  scanf("%lf", &vec->y);
}

void Output(Vector2d vec){
  printf("Length: %.1lf\n", GetLength(vec));
} 

#include <stdio.h>
#include <math.h>

typedef struct {
  double x;
  double y;
} Vector2d;

double GetLength(Vector2d vec);

void Scale(Vector2d* vec);

void ScaleVector(Vector2d* v, double s);

void Input(Vector2d* vec);

void Output(Vector2d vec);

int main(){
  Vector2d vector;
  Input(&vector);
  Scale(&vector);
  Output(vector);
  return 0;
}

double GetLength(Vector2d vec){
  return sqrt(vec.x * vec.x + vec.y * vec.y);
}

void Scale(Vector2d* vec){
  double scale;
  printf("Input scale value: ");
  scanf("%lf", &scale);
  ScaleVector(vec, scale);
  printf("Result: %.1lf %.1lf\n", vec->x, vec->y);
}

void ScaleVector(Vector2d* v, double s){
  v->x *= s;
  v->y *= s;
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

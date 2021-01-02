#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STONE 0
#define SCISSORS 2
#define PAPER 5

void ManipulateInput(int* _input){
  while(1){
    printf("Your input (%d,%d,%d): ", STONE, SCISSORS, PAPER);
    scanf("%d", _input);
    printf("\n");
    if(*_input == STONE || *_input == SCISSORS || *_input == PAPER){
      break;
    }else{
      printf("Invalid input => Input again. \n\n");
    }
  }
}

void MakeHand(int* _hand){
  int x = rand() % 3;
  if(x == 0){
    *_hand = STONE; 
  }else if(x == 1){
    *_hand = SCISSORS;
  }else if(x == 2){
    *_hand = PAPER;
  }else{
    printf("err! \n");
    exit(1);
  }
}

void Compare(int _input, int _hand){
  printf("Comp:%d vs You:%d => ", _hand, _input);
  if(_hand == _input){
    printf("Try again. \n\n");
    return;
  }else if((_hand == STONE    && _input == SCISSORS) ||
           (_hand == SCISSORS && _input == PAPER)    ||
           (_hand == PAPER    && _input == STONE)      ){
    printf("You lose. \n");
    exit(0);
  }else if((_input == STONE    && _hand == SCISSORS) ||
           (_input == SCISSORS && _hand == PAPER)    ||
           (_input == PAPER    && _hand == STONE)      ){
    printf("You win. \n");
    exit(0);
  }
}

int main(){
  int input = 10;
  int hand = 10;
  srand(time(NULL));
  while(1){
    ManipulateInput(&input);
    MakeHand(&hand);
    Compare(input, hand);
  }
  return 0;
}

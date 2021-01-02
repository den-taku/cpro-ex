#include <iostream>
#include <stdlib.h>
using namespace std;

char com[2] = {'1', '1'};
char you[2] = {'1', '1'};

void ShowHands(){
  cout << endl
       << com[0] << "  " << com[1] << endl
                << " vs " << endl
       << you[0] << "  " << you[1] << endl
       << endl;          
}

void CheckEnd(){
  if(com[0] == com[1] == ' '){
    cout << "You win!" << endl
         << "Conglatulation!" << endl;
    exit(0);
  }else if(you[0] == you[1] == ' '){
    cout << "You lose!" << endl
         << "Think of the reason why you lose by tomorrow! :-)" << endl;
    exit(0);
  }
}

char Rule(char i, char o){
  switch(i){
    case '1':
      switch(o){
        case '1':
          return '2';
        case '2':
          return '3';
        case '3': 
          return '4';
        case '4': 
          return ' ';
        case ' ':
          return '1';
      }
      break;
    case '2':
      switch(o){
        case '1':
          return '3';
        case '2':
          return '4';
        case '3': 
          return ' ';
        case '4': 
          return '1';
        case ' ':
          return '2';
      }
    break;
    case '3':
       switch(o){
        case '1':
          return '4';
        case '2':
          return ' ';
        case '3': 
          return '1';
        case '4': 
          return '2';
        case ' ':
          return '3';
      }
    break;
    case '4':
      switch(o){
        case '1':
          return ' ';
        case '2':
          return '1';
        case '3': 
          return '2';
        case '4': 
          return '3';
        case ' ':
          return '4';
      }
    break;
    case ' ':
      switch(o){
        case '1':
          return '1';
        case '2':
          return '2';
        case '3': 
          return '3';
        case '4': 
          return '4';
        case ' ':
          return ' ';
      }
    break;  
  }
}

void Write(bool b, int y, int c){
  if(b){
    com[c] = Rule(you[y],com[c]);
  }else{
    you[y] = Rule(com[c], you[y]);
  }
}

void Calculate(bool your_turn, char y, char c){
  if(your_turn){
    if(y == 'r'){
      if(c == 'r'){
        Write(true, 1, 1);
      }else{
        Write(true, 1, 0);
      }
    }else{
      if(c == 'r'){
        Write(true, 0, 1);
      }else{
        Write(true, 0, 0);
      }
    }
  }else{
    if(c == 'r'){
      if(y == 'r'){
        Write(false, 1, 1);
      }else{
        Write(false, 0, 1);
      }
    }else{
      if(y == 'r'){
        Write(false, 1, 0);
      }else{
        Write(false, 0, 0);
      }
    }
  }    
}



  
void InputY(char& x){
  cout  << "Your hand : " << flush;
  cin >> x;
} 

void InputC(char& x){
  cout << "Opponent's hand : " << flush;
  cin >> x;  
}

bool CheckInput(char x){
  if(x == 'r' || x == 'l'){
    return false;
  }else{
    cout << "Invalid input : Try again." << endl;
    return true;
  }
}


void YouChoice(){
  char y, c;
  cout << "Left(l) or Right(r) -> Left(l) or Right(r)" << endl;
  do{
    InputY(y);
  }while(CheckInput(y));
  do{
    InputC(c);
  }while(CheckInput(c));
  Calculate(true, y, c);
}

void ComChoice(){
}

int main(){
  cout << "Game Start!" << endl;
  while(1){
    ShowHands();
    YouChoice();
    CheckEnd();
    ShowHands();
    ComChoice();
    CheckEnd();
  }
} 

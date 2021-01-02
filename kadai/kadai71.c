#include <stdio.h>

int main(){
  int i;
  char str[128];

  printf("Input a word : ");
  scanf("%s", str);

  if(!str[0]){
    printf("Err\n");
    return 1;
  }
 
  for(i = 0; str[i]; ++i){
  if(str[i] == 'Z'){
   str[i] = 'A';
    continue;
  }
  if(str[i] == 'z'){
   str[i] = 'a';
    continue;
  }
    ++(str[i]);
  }
  printf("%s\n", str);
}
     
  

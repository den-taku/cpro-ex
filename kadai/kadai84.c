#include <stdio.h>

#define ORIGINAL "test.txt"
#define BACKUP   "test.bak"

int main(){
  FILE *fp1 = NULL, *fp2 = NULL;
  char str[128];
  printf("Copy : %s -> %s\n", ORIGINAL, BACKUP);
  fp1 = fopen(ORIGINAL, "r");
  if(!fp1){
    printf("File %s cannot open\n", ORIGINAL);
    return 0;
  }
  fp2 = fopen("test.bak", "w");
  if(!fp2){
    printf("File %s cannot open\n", BACKUP);
    fclose(fp1);
    return 0;
  }
  while(fgets(str, 128, fp1)){
    fprintf(fp2, "%s", str);
  }
  printf("Finished : %s -> %s\n", ORIGINAL, BACKUP);
}

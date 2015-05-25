#include <stdio.h>

const char *hw = "Hello World";
unsigned y = 100;

int main() {
  int x = 0;
  printf("Enter a number: ");
  scanf("%d", &x);
  printf("You entered: %d\n", x);

  if(x>y)
    printf("%s %d %u\n", hw, x, y);
  else
    printf("%d %s %u\n", x, hw, y);

  int j=0;
  for(int i=0; i<x; i++) {
    j += x;
  }

  printf("j: %d \n", j);
  
  return 0;
}


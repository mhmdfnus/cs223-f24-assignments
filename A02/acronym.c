/*
Author: Mohammad Fanous
Date: 09/13/2024
acronym.c asks user for a phrase and makes an acronym of first
(capital) letters.
*/

#include <stdio.h>

int main() {
  char input[1024];
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", input);
  int i = 0;
  int j = 0;
  char finalStr[1024];
  while (input[i] != '\0'){
    if (input[i] >= 'A' && input[i] <= 'Z'){
      finalStr[j] = input[i];
      j++;
    }
    i++;
  }
  finalStr[j] = '\0';
  printf("Your acronym is %s", finalStr);
  return 0;
}

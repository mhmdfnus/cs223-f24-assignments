/*
  Author: Mohammad Fanous
  Date: 09/13/2024
  exlaim.c asks the user for a word and prints it modified replacing
  lowercase characters with characters from {'@', '!', '#', '*'}.
 */

#include <stdio.h>
#include <stdlib.h>

int main() {

char buff[32];
 printf("Enter a word: ");
 scanf("%s", buff);
 char set[] = {'@', '!', '#', '*'};
 
 int i = 0;
 while (buff[i] != '\0'){
   if (buff[i] >= 'a' && buff[i] <= 'z'){
     int newCharIndex = rand() % 4;
     char newChar = set[newCharIndex];
     buff[i] = newChar;
   }
   i++;
 }

 printf("OMG! %s",buff);

  return 0;
}

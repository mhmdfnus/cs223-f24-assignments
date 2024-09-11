/***************************************************
 * mathgame.c
 * Author: Mohammad Fanous
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>


int main() {
    printf("Welcome to Math Game! ");
    printf("How many rounds do you want to play? ");

    int response = 0;
    scanf("%d", &response);
    int correct = 0;
    for (int i = 0; i < response; i++){
      int number1 = rand() % 10;
      int number2 = rand() % 10;
      int response2;
      printf("%d + %d = ? ",number1, number2);
      scanf("%d", &response2);
      if (response2 != number1 + number2){
	printf("Incorrect :(\n");
	}
      else{
	printf("Correct!\n");
	  correct ++;
      }
        
    }
    printf("You answered %d/%d correctly.",correct,response);
    
    return 0;
}

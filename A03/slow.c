/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 09/20/2024
 * Description
 this program asks for a phrase and a pause length
 and outputs an ent-phrase (every char separated by '.')
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int pause;
  char buff[32];
  char* final;

  printf("Pause length: ");
  scanf("%d", &pause);

  printf("Text: ");
  scanf("%s", buff);

  int lenBuff = strlen(buff);
  final = malloc(sizeof(char) * (lenBuff + lenBuff * pause) + 1);
  for (int i = 0; i < lenBuff; i++){
    int finalIndex = i + i * pause;
    final[finalIndex] = buff[i];
    for (int j = 0; j < pause; j++){
      final[finalIndex + j + 1] = '.';
    }
  }
  
  printf("%s", final);

  free(final);
  
  return 0;
}

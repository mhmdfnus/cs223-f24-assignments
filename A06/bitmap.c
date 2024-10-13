/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/13/2024
 * Description
 turns a 64-bit ul into an 8x8 1bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  unsigned long currentMask = 0x1ul;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here

  for (int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col ++){
      int bit_index = 63 - (row * 8 + col);
      unsigned long mask = 0x1ul << bit_index;
      if (img & mask){
	printf("@");
      }
      else{
	printf(" ");
      }
    }
    printf("\n");
  }
  return 0;
}

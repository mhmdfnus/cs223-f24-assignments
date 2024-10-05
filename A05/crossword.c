/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/04/2024
 * Description
 Simple Crossword puzzle
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to free the crossword memory
void free_crossword(char **crossword, int rows) {
  for (int i = 0; i < rows; i++) {
    free(crossword[i]);
  }
  free(crossword);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    return 1;
  }

  char *word1 = argv[1];
  char *word2 = argv[2];
  int len1 = strlen(word1);  // Length of word1 will be the number of rows
  int len2 = strlen(word2);  // Length of word2 will be the number of columns

  // Find the common letter and its index in both words
  int idx1 = -1, idx2 = -1;
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      if (word1[i] == word2[j]) {
	idx1 = i;  // Index in word1
	idx2 = j;  // Index in word2
	break;
      }
    }
    if (idx1 != -1) break;  // Stop if a common letter is found
  }

  if (idx1 == -1 || idx2 == -1) {
    printf("No common letter!\n");
    return 1;
  }

  // Dynamically allocate the crossword (2D array)
  //with len1 rows and len2 columns
  char **crossword = (char **)malloc(len1 * sizeof(char *));
  for (int i = 0; i < len1; i++) {
    crossword[i] = (char *)malloc(len2 * sizeof(char));
    // Initialize the grid with dots
    for (int j = 0; j < len2; j++) {
      crossword[i][j] = '.';
    }
  }

  // Place word1 vertically at the column corresponding to
  //the common letter in word2
  for (int i = 0; i < len1; i++) {
    crossword[i][idx2] = word1[i];
  }

  // Place word2 horizontally at the row corresponding to
  //the common letter in word1
  for (int j = 0; j < len2; j++) {
    crossword[idx1][j] = word2[j];
  }

  // Print the crossword
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      printf("%c ", crossword[i][j]);
    }
    printf("\n");
  }

  // Free the dynamically allocated memory
  free_crossword(crossword, len1);

  return 0;
}


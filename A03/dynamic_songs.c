/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 09/20/2024
 * Description: Read songs from a CSV and print them
 * in an attractive table
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the song struct
typedef struct song {
  char artist[128];
  char title[128];
  int duration; 
  float danceability;
  float energy;
  float tempo;
  float valence;
} song;

int main() {
  FILE *infile;
  int numSongs = 0;
  char buff[128];
  
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }

  // Reading in number of songs from the first line
  if (fgets(buff, sizeof(buff), infile) != NULL) {
    numSongs = atoi(strtok(buff, ","));  // Convert the first token to int (numSongs)
  } else {
    printf("Error: unable to read the number of songs from the file\n");
    fclose(infile);
    exit(1);
  }

  // Skipping header line
  fgets(buff, sizeof(buff), infile);

  // Dynamically allocate memory for the songs array
  song *songs = malloc(sizeof(song) * numSongs);
  if (songs == NULL) {
    printf("Memory allocation failed!\n");
    fclose(infile);
    exit(1);
  }

  // Reading song data
  for (int i = 0; i < numSongs; i++) {
    if (fgets(buff, sizeof(buff), infile) != NULL) {
      strcpy(songs[i].title, strtok(buff, ","));
      strcpy(songs[i].artist, strtok(NULL, ","));
      songs[i].duration = atoi(strtok(NULL, ","));
      songs[i].danceability = atof(strtok(NULL, ","));
      songs[i].energy = atof(strtok(NULL, ","));
      songs[i].tempo = atof(strtok(NULL, ","));
      songs[i].valence = atof(strtok(NULL, ","));
    }
  }

  // Print the songs
  printf("\nWelcome to Dynamic Mohammad's Danceability Directory.\n");
  for (int i = 0; i < numSongs; i++) {
    int duration_min = songs[i].duration / 60000; // convert ms to minutes
    int duration_sec = (songs[i].duration % 60000) / 1000; // remaining seconds
    printf("%2d) %-30s artist: %-20s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n",  
       i, songs[i].title, songs[i].artist, 
       duration_min, duration_sec, 
       songs[i].danceability, songs[i].energy, 
       songs[i].tempo, songs[i].valence);
  }

  // Free the dynamically allocated memory and close the file
  free(songs);
  fclose(infile);

  return 0;
}

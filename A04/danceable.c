/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 09/27/2024
 * Description: Danceability Song Directory Using Linked Lists
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

// Define the node struct for the linked list
struct node {
  song* song;
  struct node* next;
};

// Function to create a new song node
struct node* create_node(song* song) {
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  n->song = song;
  n->next = NULL;
  return n;
}

// Function to insert a song at the front of the linked list
struct node* insert_front(song* song, struct node* head) {
  struct node* n = create_node(song);
  n->next = head;
  return n;
}

// Function to print the song list
void print_songs(struct node* head) {
  struct node* current = head;

  for (int i = 0; current != NULL; i++) {
    int duration_min = current->song->duration / 60000; // convert ms to minutes
    int duration_sec = (current->song->duration % 60000) / 1000; // remaining seconds
    printf("%2d) %-30s artist: %-20s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n",
	   i, current->song->title, current->song->artist, 
	   duration_min, duration_sec, 
	   current->song->danceability, current->song->energy, 
	   current->song->tempo, current->song->valence);
    current = current->next;
  }
}

// Function to count the number of songs in the linked list
int count_songs(struct node* head) {
  struct node* current = head;
  int count = 0;

  while (current != NULL) {
    count++;
    current = current->next;
  }

  return count; // Return the number of songs counted
}

// Function to remove and return the most danceable song
song* remove_most_danceable(struct node** head) {
  if (*head == NULL) return NULL; // Return NULL if the list is empty

  struct node* current = *head;
  struct node* most_danceable_node = current;
    
  // Find the most danceable song
  while (current != NULL) {
    if (current->song->danceability > most_danceable_node->song->danceability) {
      most_danceable_node = current;
    }
    current = current->next;
  }

  // Remove the most danceable song from the list
  if (most_danceable_node == *head) {
    *head = most_danceable_node->next; // Update head if it's the first node
  } else {
    current = *head;
    while (current->next != most_danceable_node) {
      current = current->next;
    }
    current->next = most_danceable_node->next; // Bypass the most danceable node
  }

  song* most_danceable_song = most_danceable_node->song;
  free(most_danceable_node); // Free the node
  return most_danceable_song; // Return the song
}

// Function to clear the linked list
void clear_list(struct node* head) {
  struct node* current = head;
  while (current != NULL) {
    struct node* next = current->next;
    free(current->song); // Free the song
    free(current); // Free the node
    current = next;
  }
}

int main() {
  FILE* infile;
  char buff[256]; // Increased buffer size for longer lines
  struct node* head = NULL;

  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }

  // Skipping header line
  fgets(buff, sizeof(buff), infile);

  // Reading song data
  while (fgets(buff, sizeof(buff), infile) != NULL) {
    song* new_song = malloc(sizeof(song));
    if (new_song == NULL) {
      printf("ERROR: Out of space!\n");
      exit(1);
    }
        
    strcpy(new_song->title, strtok(buff, ","));
    strcpy(new_song->artist, strtok(NULL, ","));
    new_song->duration = atoi(strtok(NULL, ","));
    new_song->danceability = atof(strtok(NULL, ","));
    new_song->energy = atof(strtok(NULL, ","));
    new_song->tempo = atof(strtok(NULL, ","));
    new_song->valence = atof(strtok(NULL, ","));
        
    head = insert_front(new_song, head);
  }
  fclose(infile); // Close the file after reading

  char choice;
  printf("\nWelcome to Dynamic Mohammad's Danceability Directory.\n");

  // Main loop for user interaction
  do {
    print_songs(head); // Print songs directly
    printf("=======================\n");

    int numSongs = count_songs(head); // Get the number of songs
    if (numSongs == 0) {
      printf("Dataset contains 0 songs\n");
    } else {
      printf("Dataset contains %d songs\n", numSongs); // Print number of songs
    }

    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %c", &choice); // Use scanf for input

    if (choice == 'd') {
      song* most_danceable_song = remove_most_danceable(&head);
      if (most_danceable_song != NULL) {
	printf("---------------------------------------- Most danceable ------------------------------------\n");
	int duration_min = most_danceable_song->duration / 60000; // convert ms to minutes
	int duration_sec = (most_danceable_song->duration % 60000) / 1000; // remaining seconds
	printf("%-30s artist: %-20s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n",
	       most_danceable_song->title, most_danceable_song->artist,
	       duration_min, duration_sec, 
	       most_danceable_song->danceability, most_danceable_song->energy, 
	       most_danceable_song->tempo, most_danceable_song->valence);
	printf("--------------------------------------------------------------------------------------------\n");
	free(most_danceable_song); // Free the most danceable song
      }
    }
  } while (choice == 'd');

  // Clean up remaining memory
  clear_list(head);
  return 0;
}

/*
Author: Mohammad Fanous
Date: 09/13/2024
Songs.c displays 3 songs with attributes of title, artist, duration,
and danceability and offers the user the option  to edit an attribute
of one of these songs.
*/

#include <stdio.h>
#include <string.h>

// Define the song struct
typedef struct song {
    int durationMinutes;
    int durationSeconds;
    float danceability;
    char artist[32];
    char title[32];
} song;

// Function to print the song table
void printSongTable(song songArray[], int songCount) {
    printf("\n");
    for (int i = 0; i < songCount; i++) {
        printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f\n", 
               i, songArray[i].title, songArray[i].artist,
               songArray[i].durationMinutes, songArray[i].durationSeconds,
               songArray[i].danceability);
    }
    printf("\n=======================\n");
}

// Function to edit the song details
void editSong(song songArray[], int songID) {
    char attribute[32];
    
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf(" %[^\n]%*c", attribute);

    if (strcmp(attribute, "artist") == 0) {
        printf("Enter new artist: ");
        scanf(" %[^\n]%*c", songArray[songID].artist);
    } else if (strcmp(attribute, "title") == 0) {
        printf("Enter new title: ");
        scanf(" %[^\n]%*c", songArray[songID].title);
    } else if (strcmp(attribute, "duration") == 0) {
        printf("Enter new duration (minutes): ");
        scanf("%d", &songArray[songID].durationMinutes);
        printf("Enter new duration (seconds): ");
        scanf("%d", &songArray[songID].durationSeconds);
    } else if (strcmp(attribute, "danceability") == 0) {
        printf("Enter new danceability: ");
        scanf("%f", &songArray[songID].danceability);
    } else {
        printf("Invalid attribute!\n");
    }
}

int main() {
    // Initialize songs
    song song0, song1, song2;

    strcpy(song0.title, "Shout");
    strcpy(song1.title, "As it was");
    strcpy(song2.title, "Wish you were here");

    strcpy(song0.artist, "Tears for Fears");
    strcpy(song1.artist, "Harry Styles");
    strcpy(song2.artist, "Pink Floyd");

    song0.durationMinutes = 4;
    song1.durationMinutes = 2;
    song2.durationMinutes = 5;

    song0.durationSeconds = 11;
    song1.durationSeconds = 47;
    song2.durationSeconds = 34;

    song0.danceability = 0.50;
    song1.danceability = 0.70;
    song2.danceability = 0.30;
    
    song songArray[3] = {song0, song1, song2};

    printf("Welcome to Steven Struct's Song List.\n");
    printSongTable(songArray, 3);
    
    int songID;
    printf("Enter a song id to edit [0,1,2]: ");
    scanf("%d", &songID);
    
    if (songID < 0 || songID > 2) {
        printf("Invalid choice!\n");
        return 0;
    }

    editSong(songArray, songID);
    
    // Print the updated song list as a table
    printSongTable(songArray, 3);
    
    return 0;
}

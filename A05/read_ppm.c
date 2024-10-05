/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/4/2024
 * Description: 
 * Reads a binary PPM (P6) image file and returns 
 * the pixel data as a 1D array of ppm_pixel structs.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_ppm.h"

// Helper function to check if a line contains only whitespace
int is_whitespace_only(const char* line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace((unsigned char)line[i])) {
            return 0;  // The line contains a non-whitespace character
        }
    }
    return 1;  // The line contains only whitespace
}

struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Cannot open file '%s'\n", filename);
        return NULL;
    }

    // Read the magic number (P6)
    char magic_number[3];
    if (!fgets(magic_number, sizeof(magic_number), fp)) {
        printf("Error: Cannot read magic number\n");
        fclose(fp);
        return NULL;
    }
    if (strncmp(magic_number, "P6", 2) != 0) {
        printf("Error: Invalid PPM format (must be P6)\n");
        fclose(fp);
        return NULL;
    }

    // Skip comments and whitespace-only lines
    char line[1024];
    do {
        if (!fgets(line, sizeof(line), fp)) {
            printf("Error: Cannot read file header\n");
            fclose(fp);
            return NULL;
        }
    } while (line[0] == '#' || is_whitespace_only(line));  // Skip comment and whitespace-only lines

    // Read width and height from the header
    if (sscanf(line, "%d %d", width, height) != 2) {
        printf("Error: Invalid width/height in header\n");
        fclose(fp);
        return NULL;
    }

    // Read the maximum color value (assume it’s less than 256)
    do {
        if (!fgets(line, sizeof(line), fp)) {
            printf("Error: Cannot read max color value\n");
            fclose(fp);
            return NULL;
        }
    } while (line[0] == '#' || is_whitespace_only(line));  // Skip comment and whitespace-only lines

    int max_val;
    if (sscanf(line, "%d", &max_val) != 1 || max_val != 255) {
        printf("Error: Invalid max color value\n");
        fclose(fp);
        return NULL;
    }

    // Allocate memory for the image data (width * height pixels)
    struct ppm_pixel* pixels = (struct ppm_pixel*)malloc((*width) * (*height) * sizeof(struct ppm_pixel));
    if (!pixels) {
        printf("Error: Cannot allocate memory for pixel data\n");
        fclose(fp);
        return NULL;
    }

    // Read pixel data from the file
    size_t pixel_count = (*width) * (*height);
    if (fread(pixels, sizeof(struct ppm_pixel), pixel_count, fp) != pixel_count) {
        printf("Error: Cannot read pixel data\n");
        free(pixels);
        fclose(fp);
        return NULL;
    }

    // Close the file and return the pixel data
    fclose(fp);
    return pixels;
}

// 2D array version (unimplemented)
struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}

/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/04/2024
 * Description:
 * Reads a PPM file and prints the pixel grid in the required format.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
    int w, h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

    if (!pixels) {
        printf("Error: Failed to read PPM file\n");
        return 1;
    }

    // Print the initial "Testing file" line in the correct format
    printf("Testing file feep-raw.ppm: %d %d\n", w, h);

    // Print out the grid of pixels without extra spaces between pixel triplets
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // Access the pixel at row i, column j
            struct ppm_pixel p = pixels[i * w + j];  // Use flat array indexing

            // Print the pixel values in the format "(R,G,B)" without trailing space at the end of each row
            printf("(%d,%d,%d)", p.red, p.green, p.blue);
            
            // Only print a space if it's not the last pixel in the row
            if (j < w - 1) {
                printf(" ");
            }
        }
        printf("\n"); // Move to the next row
    }

    free(pixels); // Free allocated memory
    return 0;
}

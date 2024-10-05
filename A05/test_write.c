/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/05/2024
 * Description:
 * Reads a PPM file, writes it to another file, and prints the pixel grid.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    int w, h;

    // Step 1: Read the original PPM file
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
    if (!pixels) {
        printf("Error: Failed to read PPM file\n");
        return 1;
    }

    // Step 2: Write the pixel data to a new file called "test.ppm"
    write_ppm("test.ppm", pixels, w, h);

    // Step 3: Read the newly created "test.ppm" file to verify the contents
    struct ppm_pixel* new_pixels = read_ppm("test.ppm", &w, &h);
    if (!new_pixels) {
        printf("Error: Failed to read written PPM file\n");
        free(pixels); // Free original pixel data
        return 1;
    }

    // Step 4: Print the contents of the newly created file (same format as before)
    printf("Testing file test.ppm: %d %d\n", w, h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            struct ppm_pixel p = new_pixels[i * w + j];
            printf("(%d,%d,%d)", p.red, p.green, p.blue);
            if (j < w - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }

    // Clean up
    free(pixels);      // Free original pixel data
    free(new_pixels);  // Free newly read pixel data

    return 0;
}

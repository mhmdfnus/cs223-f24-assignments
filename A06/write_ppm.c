/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/05/2024
 * Description: 
 * Writes a binary PPM (P6) image file from a 1D array 
 * of ppm_pixel structs.
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
  FILE *fp = fopen(filename, "wb");  // Open the file in binary write mode
  if (!fp) {
    printf("Error: Cannot open file '%s' for writing\n", filename);
    return;
  }

  // Write the PPM header
  fprintf(fp, "P6\n");             // Magic number
  fprintf(fp, "%d %d\n", w, h);    // Width and height
  fprintf(fp, "255\n");            // Max color value (255)

  // Write the pixel data in binary format
  size_t pixel_count = w * h;
  fwrite(pixels, sizeof(struct ppm_pixel), pixel_count, fp);

  // Close the file
  fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}

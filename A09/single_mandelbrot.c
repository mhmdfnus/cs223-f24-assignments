/*
Author: Mohammad Fanous
Date: 11/8/2024
this program is a single process that outputs a PPM image of the mandelbrot set
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // Generate palette
    struct ppm_pixel palette[1000];
    srand(time(0));
    for (int i = 0; i < maxIterations; i++) {
        palette[i].red = rand() % 256;
        palette[i].green = rand() % 256;
        palette[i].blue = rand() % 256;
    }

    // Allocate memory for the image
    struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
    if (!pixels) {
        fprintf(stderr, "Error: Cannot allocate memory for image\n");
        return 1;
    }

    // Measure start time
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    // Compute Mandelbrot set
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            float x0 = xmin + col * (xmax - xmin) / size;
            float y0 = ymin + row * (ymax - ymin) / size;

            float x = 0.0, y = 0.0;
            int iter = 0;
            while (x*x + y*y <= 4.0 && iter < maxIterations) {
                float xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iter++;
            }

            if (iter < maxIterations) {
                pixels[row * size + col] = palette[iter];
            } else {
                pixels[row * size + col] = (struct ppm_pixel){0, 0, 0};
            }
        }
    }

    // Measure end time and calculate duration
    gettimeofday(&tend, NULL);
    double computation_time = (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec) / 1e6;
    printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, computation_time);

    // Write the image to a PPM file
    char filename[64];
    snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
    printf("Writing file: %s\n", filename);
    write_ppm(filename, pixels, size, size);

    // Free memory
    free(pixels);
    return 0;
}

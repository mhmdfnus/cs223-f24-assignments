/*
Author: Mohammad Fanous
Date: 11/8/2024
this program is a multi-threaded version of the single process program to output a PPM image of the mandelbrot set where each of the 4 threads is responsible for computing a quadrant of the final image
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

// Structure to pass data to each thread
struct ThreadData {
    int size;
    int maxIterations;
    float xmin, xmax, ymin, ymax;
    int col_start, col_end, row_start, row_end;
    struct ppm_pixel *pixels;
    struct ppm_pixel *palette;
};

// Function to generate a random color palette
void generate_palette(struct ppm_pixel *palette, int maxIterations) {
    srand(time(0));
    for (int i = 0; i < maxIterations; i++) {
        palette[i].red = rand() % 256;
        palette[i].green = rand() % 256;
        palette[i].blue = rand() % 256;
    }
}

// Thread function to compute the Mandelbrot set for the assigned region
void *compute_mandelbrot(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;

    printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
           (unsigned long)pthread_self(), data->col_start, data->col_end, data->row_start, data->row_end);

    for (int row = data->row_start; row < data->row_end; row++) {
        for (int col = data->col_start; col < data->col_end; col++) {
            float x0 = data->xmin + col * (data->xmax - data->xmin) / data->size;
            float y0 = data->ymin + row * (data->ymax - data->ymin) / data->size;

            float x = 0.0, y = 0.0;
            int iter = 0;
            while (x*x + y*y <= 4.0 && iter < data->maxIterations) {
                float xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iter++;
            }

            if (iter < data->maxIterations) {
                data->pixels[row * data->size + col] = data->palette[iter];
            } else {
                data->pixels[row * data->size + col] = (struct ppm_pixel){0, 0, 0};
            }
        }
    }

    printf("Thread %lu) finished\n", (unsigned long)pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;
    int numProcesses = 4;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); return 1;
        }
    }
    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    // Generate color palette
    struct ppm_pixel *palette = malloc(maxIterations * sizeof(struct ppm_pixel));
    generate_palette(palette, maxIterations);

    // Allocate memory for the image
    struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
    if (!pixels) {
        fprintf(stderr, "Error: Cannot allocate memory for image\n");
        free(palette);
        return 1;
    }

    // Start timing
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    // Dynamically allocate memory for threads and thread data
    pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
    struct ThreadData *thread_data = malloc(numProcesses * sizeof(struct ThreadData));
    int half_size = size / 2;

    // Create threads
    for (int i = 0; i < numProcesses; i++) {
        thread_data[i].size = size;
        thread_data[i].maxIterations = maxIterations;
        thread_data[i].xmin = xmin;
        thread_data[i].xmax = xmax;
        thread_data[i].ymin = ymin;
        thread_data[i].ymax = ymax;
        thread_data[i].pixels = pixels;
        thread_data[i].palette = palette;

        // Set the column and row ranges based on the quadrant
        if (i == 0) {  // Top-left
            thread_data[i].col_start = 0;
            thread_data[i].col_end = half_size;
            thread_data[i].row_start = 0;
            thread_data[i].row_end = half_size;
        } else if (i == 1) {  // Top-right
            thread_data[i].col_start = half_size;
            thread_data[i].col_end = size;
            thread_data[i].row_start = 0;
            thread_data[i].row_end = half_size;
        } else if (i == 2) {  // Bottom-left
            thread_data[i].col_start = 0;
            thread_data[i].col_end = half_size;
            thread_data[i].row_start = half_size;
            thread_data[i].row_end = size;
        } else if (i == 3) {  // Bottom-right
            thread_data[i].col_start = half_size;
            thread_data[i].col_end = size;
            thread_data[i].row_start = half_size;
            thread_data[i].row_end = size;
        }

        pthread_create(&threads[i], NULL, compute_mandelbrot, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < numProcesses; i++) {
        pthread_join(threads[i], NULL);
    }

    // End timing
    gettimeofday(&tend, NULL);
    double computation_time = (tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec) / 1e6;
    printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, computation_time);

    // Write the image to a PPM file
    char filename[64];
    snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
    printf("Writing file: %s\n", filename);
    write_ppm(filename, pixels, size, size);

    // Free allocated memory
    free(pixels);
    free(palette);
    free(threads);
    free(thread_data);

    return 0;
}

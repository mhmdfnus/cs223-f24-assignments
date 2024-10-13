/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/13/2024
 * Description:
 * Decodes hidden messages within image colors!
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: decode <file.ppm>\n");
        return 1;
    }

    const char* filename = argv[1];

    // Variables to hold image width, height
    int width, height;

    // Call the read_ppm function to read the PPM file
    struct ppm_pixel* pixels = read_ppm(filename, &width, &height);
    if (pixels == NULL) {
        fprintf(stderr, "Error: Could not read PPM file '%s'\n", filename);
        return 1;
    }

    printf("Reading %s with width %d and height %d\n", filename, width, height);

    // Calculate the maximum number of characters that can be stored
    int max_chars = (width * height * 3) / 8;
    printf("Max number of characters in the image: %d\n", max_chars);

    // Allocate memory to store the decoded message
    char* message = malloc(max_chars + 1);  // +1 for the null terminator
    if (message == NULL) {
        perror("Error allocating memory for message");
        free(pixels);  // Free allocated pixel data
        return 1;
    }

    int bit_count = 0;
    int char_index = 0;
    unsigned char current_char = 0;

    // Iterate over each byte in the pixel data to extract the LSBs
    unsigned char* pixel_data = (unsigned char*)pixels;  // Treat pixel data as bytes
    for (int i = 0; i < width * height * 3; i++) {
        unsigned char lsb = pixel_data[i] & 1;  // Extract the LSB
        current_char = (current_char << 1) | lsb;  // Shift and accumulate bits
        bit_count++;

        // If we have accumulated 8 bits, store the character
        if (bit_count == 8) {
            if (current_char == '\0') {  // Stop at null terminator
                break;
            }
            message[char_index++] = current_char;
            current_char = 0;  // Reset for the next character
            bit_count = 0;
        }
    }

    message[char_index] = '\0';  // Null-terminate the message

    // Print the decoded message
    printf("%s", message);

    // Free allocated memory
    free(message);
    free(pixels);

    return 0;
}

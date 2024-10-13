/*----------------------------------------------
 * Author: Mohammad Fanous
 * Date: 10/13/2024
 * Description:
 * Encodes a hidden message into a PPM image's pixel data.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

// Function to generate the output filename by adding "-encoded"
void generate_encoded_filename(const char* input_filename, char* output_filename) {
    strcpy(output_filename, input_filename);  // Copy input filename

    // Insert "-encoded" before the file extension
    char* extension = strrchr(output_filename, '.');
    if (extension) {
        strcpy(extension, "-encoded.ppm");
    } else {
        strcat(output_filename, "-encoded.ppm");  // Append if no extension
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: encode <file.ppm>\n");
        return 1;
    }

    const char* filename = argv[1];

    // Variables to hold the image's width, height, and pixel data
    int width, height;

    // Read the PPM file
    struct ppm_pixel* pixels = read_ppm(filename, &width, &height);
    if (pixels == NULL) {
        fprintf(stderr, "Error: Could not read PPM file '%s'\n", filename);
        return 1;
    }

    printf("Reading %s with width %d and height %d\n", filename, width, height);

    // Calculate the maximum number of characters that can be stored
    int max_chars = (width * height * 3) / 8;
    printf("Max number of characters in the image: %d\n", max_chars);

    // Allocate memory dynamically for the message
    char* message = (char*)malloc(max_chars + 1);  // +1 for the null terminator
    if (message == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for the message\n");
        free(pixels);
        return 1;
    }

    // Prompt the user for a message to encode
    printf("Enter a phrase: ");
    fgets(message, max_chars + 1, stdin);  // Limit input to max_chars

    // Ensure the message length does not exceed the allowed limit
    int message_len = strlen(message);
    if (message_len > max_chars) {
        fprintf(stderr, "Error: Message is too long! Max length is %d characters.\n", max_chars);
        free(message);
        free(pixels);
        return 1;
    }

    // Iterate through the pixel data and embed the message
    unsigned char* pixel_data = (unsigned char*)pixels;  // Treat pixel data as bytes
    int bit_index = 0;

    for (int i = 0; i < message_len + 1; i++) {  // Include the null terminator
        unsigned char c = message[i];

        // Embed each bit of the character into the LSB of the pixel data
        for (int bit = 7; bit >= 0; bit--) {
            unsigned char bit_value = (c >> bit) & 1;  // Extract the bit

            // Clear the LSB of the current pixel byte and set it to bit_value
            pixel_data[bit_index] = (pixel_data[bit_index] & ~1) | bit_value;

            bit_index++;
        }
    }

    // Generate the encoded filename
    char output_filename[256];
    generate_encoded_filename(filename, output_filename);

    // Write the encoded PPM image using `write_ppm()`
    write_ppm(output_filename, pixels, width, height);
    printf("Writing file %s\n", output_filename);

    // Free allocated memory
    free(message);
    free(pixels);

    return 0;
}

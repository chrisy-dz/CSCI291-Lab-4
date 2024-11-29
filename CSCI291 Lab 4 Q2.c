#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 512
#define HEIGHT 512

// Skip comments and blank lines in PGM file
void skipline(FILE *file) {
    int character;
    while ((character = fgetc(file)) != EOF) {
        if (character == '#') {
            // Ignore the entire comment line
            while ((character = fgetc(file)) != EOF && character != '\n');
        } else if (character != '\n' && character != ' ') {
            ungetc(character, file); // Put back valid data
            break;
        }
    }
}

// Read a PGM file in text format
int readPGMText(const char *filename, unsigned char *pixels) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'.\n", filename);
        return -1;
    }

    char header[3];
    int width, height, maxGray;

    // Read header and validate PGM format
    if (fscanf(file, "%2s", header) != 1 || strcmp(header, "P2") != 0) {
        fprintf(stderr, "Error: Invalid PGM file '%s'.\n", filename);
        fclose(file);
        return -1;
    }

    skipline(file); // Skip comments

    // Read dimensions and max gray value
    if (fscanf(file, "%d %d %d", &width, &height, &maxGray) != 3 || width != WIDTH || height != HEIGHT) {
        fprintf(stderr, "Error: Invalid dimensions in '%s'.\n", filename);
        fclose(file);
        return -1;
    }

    // Read pixel values
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        int pixelValue;
        if (fscanf(file, "%d", &pixelValue) != 1) {
            fprintf(stderr, "Error: Invalid pixel data in '%s'.\n", filename);
            fclose(file);
            return -1;
        }
        pixels[i] = (unsigned char)pixelValue;
    }

    fclose(file);
    return 0;
}

// Write a PGM file in text format
int writePGMText(const char *filename, unsigned char *pixels) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing.\n", filename);
        return -1;
    }

    fprintf(file, "P2\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fprintf(file, "%d ", pixels[i]);
        if ((i + 1) % WIDTH == 0) fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

// Write a PGM file in binary format
int writePGMBinary(const char *filename, unsigned char *pixels) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing.\n", filename);
        return -1;
    }

    fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(pixels, sizeof(unsigned char), WIDTH * HEIGHT, file);

    fclose(file);
    return 0;
}

// Embed secret image into cover image using 4-bit LSB steganography
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        unsigned char secretMSB = (secretPixels[i] & 0xF0) >> 4; // Extract 4 MSB
        coverPixels[i] = (coverPixels[i] & 0xF0) | secretMSB;    // Embed into LSB
    }
}

// Extract secret image from stego image
void extractLSB(unsigned char *stegoPixels, unsigned char *outputPixels) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        outputPixels[i] = (stegoPixels[i] & 0x0F) << 4; // Extract LSB and shift
    }
}

int main() {
    // File names
    const char *coverFile = "baboon.pgm";
    const char *secretFile = "farm.pgm";
    const char *stegoFile = "stego.pgm";
    const char *extractedFile = "extracted.pgm";

    // Allocate memory for images
    unsigned char *coverPixels = malloc(WIDTH * HEIGHT);
    unsigned char *secretPixels = malloc(WIDTH * HEIGHT);
    unsigned char *outputPixels = malloc(WIDTH * HEIGHT);

    if (!coverPixels || !secretPixels || !outputPixels) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Read cover and secret images
    if (readPGMText(coverFile, coverPixels) != 0 || readPGMText(secretFile, secretPixels) != 0) {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Embed secret image
    embedLSB(coverPixels, secretPixels);

    // Save stego image in binary format
    if (writePGMBinary(stegoFile, coverPixels) != 0) {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Extract and save the secret image
    extractLSB(coverPixels, outputPixels);
    if (writePGMText(extractedFile, outputPixels) != 0) {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Clean up
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    printf("Steganography completed successfully.\n");
    return 0;
}

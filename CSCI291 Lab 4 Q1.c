#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
int isValidFloat(const char *str);
void processFile(const char *inputFile, const char *outputFile);

int main() {
    const char *inputFile = "data.txt";
    const char *outputFile = "valid_data.txt";

    processFile(inputFile, outputFile);

    return 0;
}

// Function to check if a string represents a valid float
int isValidFloat(const char *str) {
    char *endPtr;
    strtod(str, &endPtr); // Attempt to parse the string as a float
    return (*str != '\0' && *endPtr == '\0'); // Ensure entire string is parsed
}

// Function to process the file
void processFile(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");
    char buffer[100];
    int invalidCount = 0;

    // Verify if the file was opened successfully
    if (input == NULL || output == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Process the file
    while (fscanf(input, "%s", buffer) != EOF) {
        if (isValidFloat(buffer)) {
            fprintf(output, "%s\n", buffer); // Write valid floats to the output file
        } else {
            invalidCount++; // Increment invalid count for corrupted entries
        }
    }

    // Verify no hardware failure occurred
    if (ferror(input)) {
        perror("Error reading from input file");
        fclose(input);
        fclose(output);
        exit(EXIT_FAILURE);
    }

    // Close the files
    fclose(input);
    fclose(output);

    // Display the number of invalid float values
    printf("Number of invalid float values: %d\n", invalidCount);
}
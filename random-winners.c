#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#define MAX_LINE_LENGTH 256
#define INITIAL_CAPACITY 100

// Function to read all lines from file into an array
int read_lines(const char *filename, char ***lines, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    int capacity = INITIAL_CAPACITY;
    *lines = malloc(capacity * sizeof(char *));
    if (!*lines) {
        fclose(file);
        return -2;
    }

    *count = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        // Remove trailing newline and carriage return
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
            len--;
        }

        // Skip empty lines
        if (len == 0) {
            continue;
        }

        // Expand array if needed
        if (*count >= capacity) {
            capacity *= 2;
            char **temp = realloc(*lines, capacity * sizeof(char *));
            if (!temp) {
                fclose(file);
                return -2;
            }
            *lines = temp;
        }

        // Allocate and copy the line
        (*lines)[*count] = malloc((len + 1) * sizeof(char));
        if (!(*lines)[*count]) {
            fclose(file);
            return -2;
        }
        strcpy((*lines)[*count], buffer);
        (*count)++;
    }

    fclose(file);
    return 0;
}

// Fisher-Yates shuffle to randomize the array
void shuffle(char **array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() % (n - i);
        char *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Free allocated memory
void free_lines(char **lines, int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

void print_usage(const char *program_name) {
    printf("Usage: %s <input_file> <number_of_winners>\n", program_name);
    printf("\n");
    printf("Arguments:\n");
    printf("  input_file        Path to text file with one nickname per line\n");
    printf("  number_of_winners Number of random winners to select\n");
    printf("\n");
    printf("Example:\n");
    printf("  %s participants.txt 3\n", program_name);
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int num_winners = atoi(argv[2]);

    // Validate number of winners
    if (num_winners <= 0) {
        fprintf(stderr, "Error: Number of winners must be a positive integer.\n");
        return 1;
    }

    // Read all lines from file
    char **lines = NULL;
    int line_count = 0;
    int result = read_lines(filename, &lines, &line_count);

    if (result == -1) {
        fprintf(stderr, "Error: Could not open file '%s'. Please check the file path.\n", filename);
        return 1;
    }

    if (result == -2) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        if (lines) free_lines(lines, line_count);
        return 1;
    }

    if (line_count == 0) {
        fprintf(stderr, "Error: The file '%s' contains no valid nicknames.\n", filename);
        free(lines);
        return 1;
    }

    // Check if we have enough participants
    if (num_winners > line_count) {
        fprintf(stderr, "Error: Cannot select %d winners from only %d participants.\n", 
                num_winners, line_count);
        fprintf(stderr, "Please reduce the number of winners or add more participants to the file.\n");
        free_lines(lines, line_count);
        return 1;
    }

    // Seed random number generator
    srand(time(NULL));

    // Debug: print all loaded participants
    #ifdef DEBUG
    printf("DEBUG: Loaded %d participants:\n", line_count);
    for (int i = 0; i < line_count; i++) {
        printf("  [%d] '%s'\n", i, lines[i]);
    }
    printf("\n");
    #endif

    // Debug: print all loaded participants
    #ifdef DEBUG
    printf("DEBUG: Loaded %d participants:\n", line_count);
    for (int i = 0; i < line_count; i++) {
        printf("  [%d] '%s'\n", i, lines[i]);
    }
    printf("\n");
    #endif

    // Shuffle the array and pick the first num_winners
    shuffle(lines, line_count);

    // Output the winners
    printf("🎉 Randomly selected %d winner(s) from %d participants:\n\n", num_winners, line_count);
    for (int i = 0; i < num_winners; i++) {
        printf("  %d. %s\n", i + 1, lines[i]);
    }
    printf("\n");

    // Clean up
    free_lines(lines, line_count);

    return 0;
}

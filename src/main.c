#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

#define MAX_INPUT_LENGTH 32

void show_menu(void) {
    printf("\n");
    printf("Metric Trainer - Metric Conversion Practice\n");
    printf("==========================================\n\n");
    printf("Select categories:\n");
    printf("  a) Distance     (miles ↔ km, inches ↔ cm)\n");
    printf("  b) Weight       (pounds ↔ kg)\n");
    printf("  c) Temperature  (Celsius ↔ Fahrenheit)\n");
    printf("  d) Volume       (gallons ↔ liters)\n");
    printf("  all) All categories\n\n");
    printf("Enter choice (e.g., \"b\", \"all\", or \"ac\"): ");
    fflush(stdout);
}

char* get_user_input(void) {
    static char input[MAX_INPUT_LENGTH];
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Remove newline if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        return input;
    }
    return NULL;
}

void trim_whitespace(char *str) {
    // Remove leading whitespace
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    
    // Move trimmed string to beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    // Remove trailing whitespace
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[--len] = '\0';
    }
}

int main(void) {
    char *user_input;
    
    printf("Welcome to Metric Trainer!\n");
    
    while (1) {
        show_menu();
        
        user_input = get_user_input();
        if (user_input == NULL) {
            printf("Error reading input.\n");
            break;
        }
        
        trim_whitespace(user_input);
        
        // Convert to lowercase for easier comparison
        for (int i = 0; user_input[i]; i++) {
            if (user_input[i] >= 'A' && user_input[i] <= 'Z') {
                user_input[i] = user_input[i] + ('a' - 'A');
            }
        }
        
        printf("You entered: '%s'\n", user_input);
        
        // Basic validation - check if input contains only valid characters
        bool valid_input = true;
        for (int i = 0; user_input[i]; i++) {
            if (user_input[i] != 'a' && user_input[i] != 'b' && 
                user_input[i] != 'c' && user_input[i] != 'd' &&
                strcmp(user_input, "all") != 0) {
                if (strlen(user_input) > 3 || strcmp(user_input, "all") != 0) {
                    valid_input = false;
                    break;
                }
            }
        }
        
        // Handle special cases
        if (strcmp(user_input, "quit") == 0 || strcmp(user_input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(user_input, "all") == 0) {
            printf("Selected: All categories\n");
            break;  // For now, just exit after selection
        } else if (strlen(user_input) == 1 && strchr("abcd", user_input[0])) {
            printf("Selected: Category %c\n", user_input[0]);
            break;  // For now, just exit after selection
        } else if (valid_input && strlen(user_input) > 1) {
            printf("Selected multiple categories: %s\n", user_input);
            break;  // For now, just exit after selection
        } else {
            printf("Invalid input. Please enter 'a', 'b', 'c', 'd', 'all', or combinations like 'ac'.\n");
            printf("You can also type 'quit' to exit.\n\n");
        }
    }
    
    return 0;
}

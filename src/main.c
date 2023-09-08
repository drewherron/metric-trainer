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
        
        // Handle special cases first
        if (strcmp(user_input, "quit") == 0 || strcmp(user_input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        // Try to parse the category selection
        category_selection_t selection;
        if (parse_category_input(user_input, &selection)) {
            printf("Selected categories:\n");
            
            if (selection.active[CATEGORY_DISTANCE]) {
                printf("  ✓ Distance (miles ↔ km, inches ↔ cm)\n");
            }
            if (selection.active[CATEGORY_WEIGHT]) {
                printf("  ✓ Weight (pounds ↔ kg)\n");
            }
            if (selection.active[CATEGORY_TEMPERATURE]) {
                printf("  ✓ Temperature (Celsius ↔ Fahrenheit)\n");
            }
            if (selection.active[CATEGORY_VOLUME]) {
                printf("  ✓ Volume (gallons ↔ liters)\n");
            }
            
            printf("\nTotal: %d categories selected\n", selection.num_active);
            printf("(Practice session will start here in future steps)\n");
            break;  // For now, just exit after successful selection
        } else {
            printf("Invalid input: '%s'\n", user_input);
            printf("Please enter 'a', 'b', 'c', 'd', 'all', or combinations like 'ac'.\n");
            printf("You can also type 'quit' to exit.\n\n");
        }
    }
    
    return 0;
}

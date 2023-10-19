#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

#define MAX_INPUT_LENGTH 32

// Function prototypes
void run_practice_session(const category_selection_t *selection);

void show_menu(void) {
    printf("\n");
    printf("Metric Trainer - Metric Conversion Practice\n");
    printf("==========================================\n\n");
    printf("Select categories:\n");
    printf("  a) Distance     (miles ↔ km, feet ↔ m, inches ↔ cm)\n");
    printf("  b) Weight       (pounds ↔ kg, ounces ↔ grams)\n");
    printf("  c) Temperature  (Celsius ↔ Fahrenheit)\n");
    printf("  d) Volume       (gallons ↔ liters)\n");
    printf("  all) All categories\n\n");
    printf("Enter choice (e.g., \"b\", \"all\", \"ac\", or \"help\"): ");
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
        
        // Handle input that was too long (no newline found and buffer full)
        if (len == MAX_INPUT_LENGTH - 1 && input[len - 1] != '\0') {
            // Clear the rest of the input line
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long. Maximum length is %d characters.\n", MAX_INPUT_LENGTH - 1);
            return NULL;
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

void run_practice_session(const category_selection_t *selection) {
    session_stats_t stats = {0}; // Initialize statistics
    float user_answer;
    bool continue_session = true;
    
    printf("Practice Session Started!\n");
    printf("─────────────────────────\n");
    printf("• Enter a number to answer questions\n");
    printf("• Type 'skip' to skip a question\n");
    printf("• Type 'quit' or 'exit' to end the session\n\n");
    
    while (continue_session) {
        // Generate a new question
        question_t question = generate_question(selection);
        
        // Check if question generation failed
        if (strstr(question.question_text, "Error:") != NULL) {
            printf("%s\n", question.question_text);
            break;
        }
        
        // Display the question
        printf("%s", question.question_text);
        
        // Get user's answer
        if (get_numeric_answer(&user_answer)) {
            // Check the answer and provide feedback
            bool correct = check_answer(&question, user_answer);
            
            // Update statistics
            update_stats(&stats, &question, correct);
            
            printf("\n");
        } else {
            // User chose to quit, skip, or there was an error
            // get_numeric_answer() already handled the appropriate messages
            // Check if user wants to quit (based on stdin EOF)
            if (feof(stdin)) {
                printf("\nSession ended.\n");
                continue_session = false;
            }
            // For skip or other cases, just continue with next question
            printf("\n");
        }
    }
    
    // Print session summary
    print_session_summary(&stats);
}

int main(void) {
    char *user_input;
    
    // Initialize random number generator
    init_random_seed();
    
    printf("Welcome to Metric Trainer!\n");
    
    while (1) {
        show_menu();
        
        user_input = get_user_input();
        if (user_input == NULL) {
            // get_user_input() already printed error message if needed
            // For EOF, exit gracefully
            if (feof(stdin)) {
                printf("\nGoodbye!\n");
                break;
            }
            // For other errors or too-long input, continue the loop
            continue;
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
        } else if (strcmp(user_input, "help") == 0 || strcmp(user_input, "h") == 0 || strcmp(user_input, "?") == 0) {
            printf("\nHelp - Valid Input Options:\n");
            printf("─────────────────────────────\n");
            printf("Categories (select one or more):\n");
            printf("  a = Distance     (miles ↔ km, feet ↔ m, inches ↔ cm)\n");
            printf("  b = Weight       (pounds ↔ kg, ounces ↔ grams)\n");
            printf("  c = Temperature  (Celsius ↔ Fahrenheit)\n");
            printf("  d = Volume       (gallons ↔ liters)\n\n");
            printf("Special options:\n");
            printf("  all   = Select all categories\n");
            printf("  help  = Show this help message\n");
            printf("  quit  = Exit the program\n\n");
            printf("Examples:\n");
            printf("  a     = Distance only\n");
            printf("  ac    = Distance and Temperature\n");
            printf("  abcd  = All categories (same as 'all')\n\n");
            continue;
        }
        
        // Try to parse the category selection
        category_selection_t selection;
        if (parse_category_input(user_input, &selection)) {
            printf("Selected categories:\n");
            
            if (selection.active[CATEGORY_DISTANCE]) {
                printf("  ✓ Distance (miles ↔ km, feet ↔ m, inches ↔ cm)\n");
            }
            if (selection.active[CATEGORY_WEIGHT]) {
                printf("  ✓ Weight (pounds ↔ kg, ounces ↔ grams)\n");
            }
            if (selection.active[CATEGORY_TEMPERATURE]) {
                printf("  ✓ Temperature (Celsius ↔ Fahrenheit)\n");
            }
            if (selection.active[CATEGORY_VOLUME]) {
                printf("  ✓ Volume (gallons ↔ liters)\n");
            }
            
            printf("\nTotal: %d categories selected\n", selection.num_active);
            printf("Starting practice session...\n\n");
            run_practice_session(&selection);
            break;  // Exit after practice session
        } else {
            printf("Invalid input: '%s'\n", user_input);
            printf("Valid options:\n");
            printf("  • Single categories: 'a', 'b', 'c', 'd'\n");
            printf("  • Combinations: 'ac', 'bd', 'abc', etc.\n");
            printf("  • All categories: 'all'\n");
            printf("  • Exit: 'quit' or 'exit'\n\n");
        }
    }
    
    return 0;
}

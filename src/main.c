/*
 * main.c - Metric Trainer Main Program
 * 
 * Interactive terminal-based metric conversion practice program.
 * Provides a menu system for category selection, session length
 * configuration, and manages the main practice loop.
 * 
 * Features:
 * - Multi-category selection (Distance, Weight, Temperature, Volume)
 * - Configurable session lengths (5, 10, 20, or unlimited questions)
 * - Comprehensive help system with detailed usage instructions
 * - Enhanced error handling and user guidance
 * - Session statistics and performance tracking
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

#define MAX_INPUT_LENGTH 32

/* ========== Function Prototypes ========== */
void run_practice_session(const category_selection_t *selection);

/**
 * Display the main menu with category options and usage instructions
 */
void show_menu(void) {
    printf("\n");
    printf("Metric Trainer - Metric Conversion Practice\n");
    printf("==========================================\n\n");
    printf("Select categories:\n");
    printf("  a) Distance     (miles ↔ km, feet ↔ m, inches ↔ cm)\n");
    printf("  b) Weight       (pounds ↔ kg, ounces ↔ grams)\n");
    printf("  c) Temperature  (Celsius ↔ Fahrenheit, Celsius ↔ Kelvin)\n");
    printf("  d) Volume       (gallons ↔ liters, cups ↔ ml, fl oz conversions)\n");
    printf("  all) All categories\n\n");
    printf("Enter choice (e.g., \"b\", \"all\", \"ac\", or \"help\"): ");
    fflush(stdout);
}

/**
 * Get user input from stdin with length validation and error handling
 * @return Pointer to static input buffer, or NULL on error/EOF
 */
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
            printf("⚠️  Input too long (max %d characters).\n", MAX_INPUT_LENGTH - 1);
            printf("💡 Try shorter commands like 'a', 'help', or 'all'\n");
            return NULL;
        }
        
        return input;
    }
    return NULL;
}

/**
 * Remove leading and trailing whitespace from a string in-place
 * @param str String to trim (modified in place)
 */
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


/**
 * Run the main practice session with question generation and user interaction
 * @param selection Pointer to active category selection for question generation
 */
void run_practice_session(const category_selection_t *selection) {
    session_stats_t stats = {0}; // Initialize statistics
    float user_answer;
    bool continue_session = true;
    int questions_asked = 0;
    
    printf("Practice Session Started!\n");
    printf("─────────────────────────\n");
    printf("• Enter a number to answer questions\n");
    printf("• Type 'skip' to skip a question\n");
    printf("• Type 'quit' or 'exit' to return to main menu\n\n");
    
    while (continue_session) {
        // Generate a new question
        question_t question = generate_question(selection);
        
        // Check if question generation failed
        if (strstr(question.question_text, "Error:") != NULL) {
            printf("%s\n", question.question_text);
            break;
        }
        
        // Display the question with improved formatting
        questions_asked++;
        printf("\n[Question %d] %s\n", questions_asked, question.question_text);
        printf("═══════════════════════════════════════\n");
        
        // Get user's answer
        int answer_result = get_numeric_answer(&user_answer);
        if (answer_result == 1) {
            // Valid number entered - check the answer and provide feedback
            bool correct = check_answer(&question, user_answer);
            
            // Update statistics
            update_stats(&stats, &question, correct);
            
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        } else if (answer_result == -1) {
            // User wants to quit/exit - end the session
            continue_session = false;
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        } else {
            // Skip, empty input, or other cases - continue with next question
            // get_numeric_answer() already handled the appropriate messages
            if (feof(stdin)) {
                printf("\nSession ended.\n");
                continue_session = false;
            }
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
        }
    }
    
    // Print session summary
    print_session_summary(&stats);
}

/**
 * Main program entry point - handles menu system and application flow
 * @return Exit status (0 for successful completion)
 */
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
                printf("\n👋 Goodbye!\n");
                break;
            }
            // For other errors or too-long input, show menu again
            printf("Please try again.\n\n");
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
            printf("\n🎓 Metric Trainer - Complete Help Guide\n");
            printf("═══════════════════════════════════════════════════════════\n");
            
            printf("\n📚 CATEGORY SELECTION\n");
            printf("─────────────────────\n");
            printf("Choose conversion categories for practice:\n");
            printf("  📏 a = Distance     (miles ↔ km, feet ↔ m, inches ↔ cm)\n");
            printf("  ⚖️  b = Weight       (pounds ↔ kg, ounces ↔ grams)\n");
            printf("  🌡️  c = Temperature  (Celsius ↔ Fahrenheit, Celsius ↔ Kelvin)\n");
            printf("  🥤 d = Volume       (gallons ↔ liters, cups ↔ ml, fl oz conversions)\n");
            
            printf("\n🎯 INPUT OPTIONS\n");
            printf("────────────────\n");
            printf("  • Single category:     'a', 'b', 'c', or 'd'\n");
            printf("  • Multiple categories: 'ac', 'bd', 'abc'\n");
            printf("  • All categories:      'all' or 'abcd'\n");
            printf("  • Get this help:       'help', 'h', or '?'\n");
            printf("  • Exit program:        'quit' or 'exit'\n");
            
            printf("\n🎮 PRACTICE SESSION\n");
            printf("───────────────────\n");
            printf("After selecting categories, you'll choose session length:\n");
            printf("  • Quick (5 questions)    • Standard (10 questions)\n");
            printf("  • Extended (20 questions) • Unlimited (until you quit)\n");
            
            printf("\nDuring practice:\n");
            printf("  • Enter numbers (decimals OK): 5.2, 100, -3.14\n");
            printf("  • Skip difficult questions:    'skip'\n");
            printf("  • End session early:           'quit' or 'exit'\n");
            
            printf("\n📊 FEATURES\n");
            printf("───────────\n");
            printf("  ✓ Realistic conversion ranges and tolerances\n");
            printf("  ✓ Educational feedback and hints for wrong answers\n");
            printf("  ✓ Session statistics with category breakdowns\n");
            printf("  ✓ Progress tracking within sessions\n");
            
            printf("\n💡 EXAMPLES\n");
            printf("───────────\n");
            printf("  'a'    → Practice distance conversions only\n");
            printf("  'cd'   → Practice temperature and volume together\n");
            printf("  'all'  → Practice all conversion types\n");
            
            printf("\n═══════════════════════════════════════════════════════════\n");
            printf("Ready to start? Enter your category choice above! 🚀\n\n");
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
                printf("  ✓ Temperature (Celsius ↔ Fahrenheit, Celsius ↔ Kelvin)\n");
            }
            if (selection.active[CATEGORY_VOLUME]) {
                printf("  ✓ Volume (gallons ↔ liters, cups ↔ ml, fl oz conversions)\n");
            }
            
            printf("\nTotal: %d categories selected\n", selection.num_active);
            printf("Starting practice session...\n\n");
            run_practice_session(&selection);
            // Session ended - continue to show menu again
        } else {
            printf("\n❌ Invalid input: '%s'\n", user_input);
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            printf("📖 Quick Reference:\n");
            printf("  ✓ Single categories: 'a', 'b', 'c', 'd'\n");
            printf("  ✓ Multiple categories: 'ac', 'bd', 'abc'\n");
            printf("  ✓ All categories: 'all'\n");
            printf("  ✓ Get help: 'help' or '?'\n");
            printf("  ✓ Exit program: 'quit' or 'exit'\n");
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            printf("💡 Tip: Try 'help' for detailed explanations\n\n");
        }
    }
    
    return 0;
}

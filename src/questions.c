#include "questions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void init_categories(category_selection_t *selection) {
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        selection->active[i] = false;
    }
    selection->num_active = 0;
}

bool parse_category_input(const char *input, category_selection_t *selection) {
    init_categories(selection);
    
    // Handle empty input
    if (input == NULL || input[0] == '\0') {
        return false;
    }
    
    if (strcmp(input, "all") == 0) {
        // Enable all categories
        for (int i = 0; i < CATEGORY_COUNT; i++) {
            selection->active[i] = true;
        }
        selection->num_active = CATEGORY_COUNT;
        return true;
    }
    
    // Parse individual characters in the input string
    // This allows combinations like "ac", "bd", "abcd", etc.
    for (int i = 0; input[i] != '\0'; i++) {
        switch (input[i]) {
            case 'a':
                if (!selection->active[CATEGORY_DISTANCE]) {
                    selection->active[CATEGORY_DISTANCE] = true;
                    selection->num_active++;
                }
                break;
            case 'b':
                if (!selection->active[CATEGORY_WEIGHT]) {
                    selection->active[CATEGORY_WEIGHT] = true;
                    selection->num_active++;
                }
                break;
            case 'c':
                if (!selection->active[CATEGORY_TEMPERATURE]) {
                    selection->active[CATEGORY_TEMPERATURE] = true;
                    selection->num_active++;
                }
                break;
            case 'd':
                if (!selection->active[CATEGORY_VOLUME]) {
                    selection->active[CATEGORY_VOLUME] = true;
                    selection->num_active++;
                }
                break;
            default:
                // Invalid character found - reject the entire input
                return false;
        }
    }
    
    // Must have selected at least one category
    return selection->num_active > 0;
}

question_t generate_question(const category_selection_t *selection) {
    question_t q = {0};
    // TODO: Implement question generation
    printf("DEBUG: generate_question called\n");
    strcpy(q.question_text, "Placeholder question");
    return q;
}

bool check_answer(const question_t *question, float user_answer) {
    // TODO: Implement answer checking with tolerance
    printf("DEBUG: check_answer called with %.2f\n", user_answer);
    return false;
}

void update_stats(session_stats_t *stats, const question_t *question, bool correct) {
    // TODO: Implement statistics tracking
    printf("DEBUG: update_stats called, correct=%s\n", correct ? "true" : "false");
}

void print_session_summary(const session_stats_t *stats) {
    // TODO: Implement session summary display
    printf("DEBUG: print_session_summary called\n");
    printf("Session complete! (Summary functionality coming soon)\n");
}

void init_random_seed(void) {
    static bool initialized = false;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = true;
    }
}

float generate_random_value(float min, float max) {
    if (min >= max) {
        return min;
    }
    
    // Generate a random float between min and max
    float range = max - min;
    float random_fraction = (float)rand() / (float)RAND_MAX;
    return min + (random_fraction * range);
}

float round_to_precision(float value, int decimal_places) {
    float multiplier = powf(10.0f, (float)decimal_places);
    return roundf(value * multiplier) / multiplier;
}

// Basic conversion functions (placeholders for now)
float miles_to_km(float miles) {
    return miles * 1.609344f;
}

float km_to_miles(float km) {
    return km / 1.609344f;
}

float inches_to_cm(float inches) {
    return inches * 2.54f;
}

float cm_to_inches(float cm) {
    return cm / 2.54f;
}

float pounds_to_kg(float pounds) {
    return pounds * 0.453592f;
}

float kg_to_pounds(float kg) {
    return kg / 0.453592f;
}

float fahrenheit_to_celsius(float fahrenheit) {
    return (fahrenheit - 32.0f) * 5.0f / 9.0f;
}

float celsius_to_fahrenheit(float celsius) {
    return celsius * 9.0f / 5.0f + 32.0f;
}

float gallons_to_liters(float gallons) {
    return gallons * 3.78541f;
}

float liters_to_gallons(float liters) {
    return liters / 3.78541f;
}
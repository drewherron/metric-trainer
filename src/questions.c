#include "questions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

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
    
    // Ensure we have at least one active category
    if (selection->num_active == 0) {
        strcpy(q.question_text, "Error: No categories selected");
        return q;
    }
    
    // Pick a random active category
    category_t chosen_category = pick_random_category(selection);
    
    // Get available conversions for this category  
    int conversion_count = 0;
    const conversion_info_t *conversions = get_conversions_for_category(chosen_category, &conversion_count);
    
    if (conversion_count == 0) {
        strcpy(q.question_text, "Error: No conversions available");
        return q;
    }
    
    // Pick a random conversion from this category
    int conversion_index = rand() % conversion_count;
    const conversion_info_t *conv = &conversions[conversion_index];
    
    // Generate a random value within the conversion's range
    float value = generate_random_value(conv->min_value, conv->max_value);
    value = round_to_precision(value, 1); // Round to 1 decimal place for cleaner questions
    
    // Calculate the correct answer
    float answer = conv->convert_func(value);
    answer = round_to_precision(answer, 2); // Allow more precision in answers
    
    // Calculate tolerance for this question
    float tolerance = answer * (conv->tolerance_percent / 100.0f);
    if (tolerance < 0.1f) tolerance = 0.1f; // Minimum tolerance
    
    // Fill in the question structure
    q.category = chosen_category;
    q.value = value;
    q.correct_answer = answer;
    q.tolerance = tolerance;
    strcpy(q.from_unit, conv->from_unit);
    strcpy(q.to_unit, conv->to_unit);
    
    // Format the question text
    snprintf(q.question_text, MAX_QUESTION_TEXT,
             "Convert %.1f %s to %s: ",
             value, conv->from_unit, conv->to_unit);
    
    return q;
}

bool check_answer(const question_t *question, float user_answer) {
    float difference = fabsf(user_answer - question->correct_answer);
    bool is_correct = difference <= question->tolerance;
    
    printf("Your answer: %.2f, Correct answer: %.2f (tolerance: ±%.2f)\n", 
           user_answer, question->correct_answer, question->tolerance);
    
    if (is_correct) {
        printf("✓ Correct!\n");
    } else {
        printf("✗ Incorrect. The correct answer is %.2f %s\n", 
               question->correct_answer, question->to_unit);
    }
    
    return is_correct;
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

// Distance conversion functions
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

float feet_to_meters(float feet) {
    return feet * 0.3048f;
}

float meters_to_feet(float meters) {
    return meters / 0.3048f;
}

float pounds_to_kg(float pounds) {
    return pounds * 0.453592f;
}

float kg_to_pounds(float kg) {
    return kg / 0.453592f;
}

float ounces_to_grams(float ounces) {
    return ounces * 28.3495f;
}

float grams_to_ounces(float grams) {
    return grams / 28.3495f;
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

// Helper function to pick a random active category
category_t pick_random_category(const category_selection_t *selection) {
    if (selection->num_active == 0) {
        return CATEGORY_DISTANCE; // Fallback
    }
    
    // Create array of active category indices
    category_t active_categories[CATEGORY_COUNT];
    int count = 0;
    
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        if (selection->active[i]) {
            active_categories[count++] = (category_t)i;
        }
    }
    
    // Pick a random one
    int random_index = rand() % count;
    return active_categories[random_index];
}

// Complete distance conversion data with realistic ranges
static const conversion_info_t distance_conversions[] = {
    {
        "miles", "mi", "kilometers", "km", 
        miles_to_km, 1.0f, 100.0f, 2.0f
    },
    {
        "kilometers", "km", "miles", "mi", 
        km_to_miles, 1.0f, 160.0f, 2.0f
    },
    {
        "inches", "in", "centimeters", "cm", 
        inches_to_cm, 1.0f, 36.0f, 1.5f
    },
    {
        "centimeters", "cm", "inches", "in", 
        cm_to_inches, 1.0f, 90.0f, 1.5f
    },
    {
        "feet", "ft", "meters", "m", 
        feet_to_meters, 1.0f, 50.0f, 2.0f
    },
    {
        "meters", "m", "feet", "ft", 
        meters_to_feet, 1.0f, 15.0f, 2.0f
    }
};

static const conversion_info_t weight_conversions[] = {
    {
        "pounds", "lb", "kilograms", "kg", 
        pounds_to_kg, 1.0f, 200.0f, 2.0f
    },
    {
        "kilograms", "kg", "pounds", "lb", 
        kg_to_pounds, 1.0f, 90.0f, 2.0f
    },
    {
        "ounces", "oz", "grams", "g", 
        ounces_to_grams, 1.0f, 32.0f, 1.5f
    },
    {
        "grams", "g", "ounces", "oz", 
        grams_to_ounces, 1.0f, 900.0f, 1.5f
    }
};

static const conversion_info_t temperature_conversions[] = {
    {
        "degrees Fahrenheit", "°F", "degrees Celsius", "°C", 
        fahrenheit_to_celsius, -20.0f, 120.0f, 2.0f
    },
    {
        "degrees Celsius", "°C", "degrees Fahrenheit", "°F", 
        celsius_to_fahrenheit, -30.0f, 50.0f, 2.0f
    }
};

static const conversion_info_t volume_conversions[] = {
    {
        "gallons", "gal", "liters", "L", 
        gallons_to_liters, 1.0f, 20.0f, 2.0f
    },
    {
        "liters", "L", "gallons", "gal", 
        liters_to_gallons, 1.0f, 50.0f, 2.0f
    }
};

const conversion_info_t* get_conversions_for_category(category_t category, int *count) {
    switch (category) {
        case CATEGORY_DISTANCE:
            *count = sizeof(distance_conversions) / sizeof(distance_conversions[0]);
            return distance_conversions;
            
        case CATEGORY_WEIGHT:
            *count = sizeof(weight_conversions) / sizeof(weight_conversions[0]);
            return weight_conversions;
            
        case CATEGORY_TEMPERATURE:
            *count = sizeof(temperature_conversions) / sizeof(temperature_conversions[0]);
            return temperature_conversions;
            
        case CATEGORY_VOLUME:
            *count = sizeof(volume_conversions) / sizeof(volume_conversions[0]);
            return volume_conversions;
            
        default:
            *count = 0;
            return NULL;
    }
}

bool is_valid_number(const char *input) {
    if (input == NULL || *input == '\0') {
        return false;
    }
    
    // Skip leading whitespace
    const char *start = input;
    while (*start && isspace(*start)) {
        start++;
    }
    
    // Check if only whitespace
    if (*start == '\0') {
        return false;
    }
    
    char *endptr;
    strtof(start, &endptr);
    
    // Check if the entire non-whitespace string was consumed
    while (*endptr != '\0') {
        if (!isspace(*endptr)) {
            return false;
        }
        endptr++;
    }
    
    return true;
}

bool get_numeric_answer(float *answer) {
    static char input[64];
    
    printf("Your answer: ");
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Remove newline if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        // Handle too-long input
        if (len == sizeof(input) - 1 && input[len - 1] != '\0') {
            // Clear remaining input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long. Please enter a number.\n");
            return false;
        }
        
        // Trim whitespace
        char *start = input;
        while (*start && isspace(*start)) {
            start++;
        }
        
        char *end = start + strlen(start) - 1;
        while (end > start && isspace(*end)) {
            *end = '\0';
            end--;
        }
        
        // Check for empty input
        if (*start == '\0') {
            printf("Please enter a number.\n");
            return false;
        }
        
        // Check for special commands
        if (strcmp(start, "quit") == 0 || strcmp(start, "exit") == 0) {
            printf("Exiting practice session...\n");
            return false;
        }
        
        if (strcmp(start, "skip") == 0) {
            printf("Skipping question...\n");
            return false;
        }
        
        // Validate and parse number
        if (!is_valid_number(start)) {
            printf("Invalid input. Please enter a valid number (e.g., 5.2, -3.14, 0.5).\n");
            return false;
        }
        
        *answer = strtof(start, NULL);
        return true;
    }
    
    // EOF or read error
    if (feof(stdin)) {
        printf("\nExiting...\n");
    } else {
        printf("Error reading input.\n");
    }
    return false;
}
/* 
 * questions.h - Question Generation and Conversion System
 * 
 * Data Structure Design:
 * 
 * 1. conversion_info_t: Defines a single conversion type with:
 *    - Unit names and abbreviations (e.g., "miles"/"mi" -> "kilometers"/"km")
 *    - Conversion function pointer for the mathematical transformation
 *    - Practical value ranges for generating realistic questions
 *    - Tolerance percentage for accepting "close enough" answers
 * 
 * 2. question_t: Represents a single generated question with:
 *    - Category and direction information  
 *    - Specific value to convert and correct answer
 *    - Human-readable question text
 *    - Calculated tolerance for this specific question
 * 
 * 3. session_stats_t: Tracks user performance across categories
 *    - Overall and per-category statistics
 *    - Used for identifying weak areas and progress tracking
 * 
 * The system supports bidirectional conversions (metric ↔ imperial) with
 * realistic value ranges and flexible answer tolerance.
 */

#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <stdbool.h>

/* ========== Global Variables ========== */
extern bool g_whole_numbers_mode;  // Flag for whole numbers only mode
extern bool g_easy_mode;           // Flag for easy mode (increments of 5)

#define MAX_UNIT_NAME 32
#define MAX_QUESTION_TEXT 128
#define MAX_CONVERSIONS_PER_CATEGORY 8

typedef enum {
    CATEGORY_DISTANCE = 0,
    CATEGORY_WEIGHT,
    CATEGORY_TEMPERATURE, 
    CATEGORY_VOLUME,
    CATEGORY_COUNT
} category_t;

typedef enum {
    DIRECTION_TO_METRIC = 0,    // Imperial/US → Metric
    DIRECTION_TO_IMPERIAL,      // Metric → Imperial/US
    DIRECTION_BOTH              // Either direction
} conversion_direction_t;

typedef struct {
    bool active[CATEGORY_COUNT];
    int num_active;
} category_selection_t;

typedef struct {
    char from_unit[MAX_UNIT_NAME];      // e.g., "miles", "F"
    char from_abbrev[8];                // e.g., "mi", "F" 
    char to_unit[MAX_UNIT_NAME];        // e.g., "kilometers", "C"
    char to_abbrev[8];                  // e.g., "km", "C"
    float (*convert_func)(float);       // Function pointer for conversion
    float min_value;                    // Minimum practical value to generate
    float max_value;                    // Maximum practical value to generate
    float tolerance_percent;            // Acceptable error percentage (e.g., 1.0 for 1%)
} conversion_info_t;

typedef struct {
    category_t category;
    conversion_direction_t direction;   // Which direction this conversion goes
    char from_unit[MAX_UNIT_NAME];
    char to_unit[MAX_UNIT_NAME];
    float value;                        // The value to convert
    float correct_answer;               // The correct converted value
    char question_text[MAX_QUESTION_TEXT];
    float tolerance;                    // Acceptable tolerance for this specific question
} question_t;

typedef struct {
    int total_questions;
    int correct_answers;
    int category_totals[CATEGORY_COUNT];
    int category_correct[CATEGORY_COUNT];
} session_stats_t;

/* ========== Conversion Functions ========== */
/* Mathematical conversion functions for unit transformations */

float miles_to_km(float miles);               /* Convert miles to kilometers */
float km_to_miles(float km);                  /* Convert kilometers to miles */
float inches_to_cm(float inches);             /* Convert inches to centimeters */
float cm_to_inches(float cm);                 /* Convert centimeters to inches */
float feet_to_m(float feet);                  /* Convert feet to meters */
float m_to_feet(float m);                     /* Convert meters to feet */
float pounds_to_kg(float pounds);             /* Convert pounds to kilograms */
float kg_to_pounds(float kg);                 /* Convert kilograms to pounds */
float ounces_to_grams(float ounces);          /* Convert ounces to grams */
float grams_to_ounces(float grams);           /* Convert grams to ounces */
float fahrenheit_to_celsius(float fahrenheit); /* Convert Fahrenheit to Celsius */
float celsius_to_fahrenheit(float celsius);   /* Convert Celsius to Fahrenheit */
float celsius_to_kelvin(float celsius);       /* Convert Celsius to Kelvin */
float kelvin_to_celsius(float kelvin);        /* Convert Kelvin to Celsius */
float gallons_to_liters(float gallons);       /* Convert gallons to liters */
float liters_to_gallons(float liters);        /* Convert liters to gallons */
float cups_to_ml(float cups);                 /* Convert cups to milliliters */
float ml_to_cups(float ml);                   /* Convert milliliters to cups */
float liters_to_fl_oz(float liters);          /* Convert liters to fluid ounces */
float fl_oz_to_liters(float fl_oz);           /* Convert fluid ounces to liters */
float ml_to_fl_oz(float ml);                  /* Convert milliliters to fluid ounces */
float fl_oz_to_ml(float fl_oz);               /* Convert fluid ounces to milliliters */

/* ========== Core Functions ========== */
/* Primary system functionality for question generation and management */

/**
 * Initialize category selection structure with all categories disabled
 * @param selection Pointer to category_selection_t to initialize
 */
void init_categories(category_selection_t *selection);

/**
 * Parse user input string into category selection flags
 * @param input User input string (e.g., "ac", "all", "b")
 * @param selection Pointer to category_selection_t to populate
 * @return true if input is valid and parsed successfully
 */
bool parse_category_input(const char *input, category_selection_t *selection);

/**
 * Generate a random conversion question from selected categories
 * @param selection Pointer to active category selection
 * @return Generated question_t structure with all fields populated
 */
question_t generate_question(const category_selection_t *selection);

/**
 * Check if user's answer is within acceptable tolerance
 * @param question Pointer to the question being answered
 * @param user_answer The user's numeric answer
 * @return true if answer is correct (within tolerance)
 */
bool check_answer(const question_t *question, float user_answer);

/**
 * Update session statistics with question result
 * @param stats Pointer to session_stats_t to update
 * @param question Pointer to the question that was answered
 * @param correct Whether the answer was correct
 */
void update_stats(session_stats_t *stats, const question_t *question, bool correct);

/**
 * Print comprehensive session summary with statistics
 * @param stats Pointer to session_stats_t containing results
 */
void print_session_summary(const session_stats_t *stats);

/* ========== Input Functions ========== */
/* Functions for handling user input and validation */

/**
 * Get and validate numeric answer from user input
 * @param answer Pointer to float to store the parsed answer
 * @return 1 if valid number entered, -1 for quit/exit, 0 for skip/error
 */
int get_numeric_answer(float *answer);

/**
 * Validate that a string represents a valid number
 * @param input String to validate
 * @return true if string is a valid numeric representation
 */
bool is_valid_number(const char *input);

/* ========== Utility Functions ========== */
/* Helper functions for internal system operations */

/**
 * Get array of conversion info for a specific category
 * @param category The category to retrieve conversions for
 * @param count Pointer to int to store the number of conversions
 * @return Pointer to array of conversion_info_t structures
 */
const conversion_info_t* get_conversions_for_category(category_t category, int *count);

/**
 * Randomly select an active category from user selection
 * @param selection Pointer to category selection with active flags
 * @return Randomly chosen active category
 */
category_t pick_random_category(const category_selection_t *selection);

/**
 * Generate random floating point value within specified range
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random float between min and max
 */
float generate_random_value(float min, float max);

/**
 * Round value to specified number of decimal places
 * @param value The value to round
 * @param decimal_places Number of decimal places to keep
 * @return Rounded value
 */
float round_to_precision(float value, int decimal_places);

/**
 * Initialize random number generator with current time seed
 */
void init_random_seed(void);

#endif
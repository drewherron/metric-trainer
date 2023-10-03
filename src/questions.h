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
    char from_unit[MAX_UNIT_NAME];      // e.g., "miles", "°F"
    char from_abbrev[8];                // e.g., "mi", "°F" 
    char to_unit[MAX_UNIT_NAME];        // e.g., "kilometers", "°C"
    char to_abbrev[8];                  // e.g., "km", "°C"
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

// Conversion functions (to be implemented)
float miles_to_km(float miles);
float km_to_miles(float km);
float inches_to_cm(float inches);
float cm_to_inches(float cm);
float pounds_to_kg(float pounds);
float kg_to_pounds(float kg);
float fahrenheit_to_celsius(float fahrenheit);
float celsius_to_fahrenheit(float celsius);
float gallons_to_liters(float gallons);
float liters_to_gallons(float liters);

// Core functions
void init_categories(category_selection_t *selection);
bool parse_category_input(const char *input, category_selection_t *selection);
question_t generate_question(const category_selection_t *selection);
bool check_answer(const question_t *question, float user_answer);
void update_stats(session_stats_t *stats, const question_t *question, bool correct);
void print_session_summary(const session_stats_t *stats);

// Utility functions
const conversion_info_t* get_conversions_for_category(category_t category, int *count);
category_t pick_random_category(const category_selection_t *selection);
float generate_random_value(float min, float max);
float round_to_precision(float value, int decimal_places);
void init_random_seed(void);

#endif
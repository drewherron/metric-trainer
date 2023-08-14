#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <stdbool.h>

#define MAX_UNIT_NAME 32
#define MAX_QUESTION_TEXT 128

typedef enum {
    CATEGORY_DISTANCE = 0,
    CATEGORY_WEIGHT,
    CATEGORY_TEMPERATURE, 
    CATEGORY_VOLUME,
    CATEGORY_COUNT
} category_t;

typedef struct {
    bool active[CATEGORY_COUNT];
    int num_active;
} category_selection_t;

typedef struct {
    category_t category;
    char from_unit[MAX_UNIT_NAME];
    char to_unit[MAX_UNIT_NAME];
    float value;
    float correct_answer;
    char question_text[MAX_QUESTION_TEXT];
} question_t;

typedef struct {
    int total_questions;
    int correct_answers;
    int category_totals[CATEGORY_COUNT];
    int category_correct[CATEGORY_COUNT];
} session_stats_t;

void init_categories(category_selection_t *selection);
bool parse_category_input(const char *input, category_selection_t *selection);
question_t generate_question(const category_selection_t *selection);
bool check_answer(const question_t *question, float user_answer);
void update_stats(session_stats_t *stats, const question_t *question, bool correct);
void print_session_summary(const session_stats_t *stats);

#endif
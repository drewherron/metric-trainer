#include "questions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void init_categories(category_selection_t *selection) {
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        selection->active[i] = false;
    }
    selection->num_active = 0;
}

bool parse_category_input(const char *input, category_selection_t *selection) {
    init_categories(selection);
    
    if (strcmp(input, "all") == 0) {
        // Enable all categories
        for (int i = 0; i < CATEGORY_COUNT; i++) {
            selection->active[i] = true;
        }
        selection->num_active = CATEGORY_COUNT;
        return true;
    }
    
    // Parse individual characters
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
                // Invalid character
                return false;
        }
    }
    
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
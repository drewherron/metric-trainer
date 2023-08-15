#include <stdio.h>
#include <stdlib.h>
#include "questions.h"

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

int main(void) {
    printf("Main!\n\n");

    show_menu();

    return 0;
}

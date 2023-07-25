#include <stdio.h>
#include <stdlib.h>

void show_menu(void) {
    printf("Metric Trainer - Metric Conversion Practice\n");
    printf("==========================================\n\n");
    printf("Select categories:\n");
    printf("a) Distance\n");
    printf("b) Weight\n");
    printf("c) Temperature\n");
    printf("d) Volume\n");
    printf("all) All categories\n\n");
    printf("Enter choice (e.g., \"b\", \"all\", or \"ac\"): ");
}

int main(void) {
    printf("Main!\n\n");

    show_menu();

    return 0;
}

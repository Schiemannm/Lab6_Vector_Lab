/*
 * @file main.c
    * @brief Main function for vector operations calculator
    * @author Matthew Schiemann
    * Makefile Implemented
 */

#include <stdio.h>
#include <string.h>
#include "ui.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // Ui Help Function for command list
        if (strcmp(argv[1], "-h") == 0) {
            ui_help();
            return 0;
        }
        fprintf(stderr, "Unknown option: %s (try -h)\n", argv[1]);
        return 1;
    }
    return ui_run();
}



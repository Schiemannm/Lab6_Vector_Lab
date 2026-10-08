/*
 * @file storage.c
 * @brief Used to store Vectors into a array for later retrieval!
 */
#include <string.h>
#include "storage.h"

static vect store[MAX_VECTS];
static int used[MAX_VECTS]; // Boolean 1/0 Yes/No if the slot is used

int addvect(vect v) {
    int empty = -1;
    for (int i = 0; i < MAX_VECTS; i++) {
        if (used[i] && strcmp(store[i].name, v.name) == 0) {            // If it is used, check if the name is the same, if so replace it
            store[i] = v;              
            return 0;

        }
        if (!used[i] && empty < 0) {                                    // If it is not used, and we haven't found an empty slot yet, mark this as the first empty slot
            empty = i;
        }
    }
    if (empty < 0) {
        // Extra Random comment to test git diff, this should not be here, but I want to see if it shows up in the diff.
        return -1;
    }          

    
    store[empty] = v;
    used[empty] = 1;
    return 0;
}

int findvect(const char *name, vect *out) {
    for (int i = 0; i < MAX_VECTS; i++) {
        if (used[i] && strcmp(store[i].name, name) == 0) {
            *out = store[i];
            return 0;
        }
    }
    return -1;
}

int vect_at(int index, vect *out) {

    if (index < 0 || index >= MAX_VECTS || !used[index]) {
        return -1; 
    }
    *out = store[index];
    return 0;
}

void clearvects(void) {
    // Looked up on google how to set all of the values in an array to 0, and found memset.
    memset(used, 0, sizeof used);
}
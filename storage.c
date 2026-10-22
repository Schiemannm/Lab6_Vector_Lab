/*
 * @file storage.c
 * @brief Used to store Vectors into a array for later retrieval!
 */
#include <string.h>
#include "storage.h"
#include <stdlib.h>
#include <stdio.h>

static vect *store;
static int *used; // 1 or 0 if it is used or not
int quantity = 0; // Number of Vectors currently stored
int currentMaxCapacity = MAX_VECTS;

int initializeMemory(void) {
    currentMaxCapacity = MAX_VECTS;
    store = malloc(sizeof(vect) * currentMaxCapacity);  // Allocate memory for the store array
    used = malloc(sizeof(int) * currentMaxCapacity);    // Allocate memory for the used array
    if (store == NULL || used == NULL) { // Check if the allocation was successful
        return -1; // Return -1 if the allocation failed
    }
    memset(used, 0, sizeof(int) * currentMaxCapacity); // Initialize the used array to 0 (all slots are free)
    quantity = 0; // Initialize the quantity of Vectors stored to 0
    return 0; // Return 0 to indicate success
}
int addvect(vect v) {
    int empty = -1;
    if (quantity >= currentMaxCapacity - 3) {
        store = realloc(store, sizeof(vect) * (currentMaxCapacity + 5)); // Increase the size of the store array by 5 if we are running out of space
        used = realloc(used, sizeof(int) * (currentMaxCapacity + 5)); // Increase the size of the used array by 5 to match the new size of the store array
        memset(&used[currentMaxCapacity], 0, sizeof(int) * 5);
        memset(&store[currentMaxCapacity], 0, sizeof(vect) * 5);
        currentMaxCapacity += 5; // Increase the MAX_VECTS by 5 to reflect the new size of the store array
        
    }
    for (int i = 0; i < currentMaxCapacity; i++) {
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
    quantity++;
    used[empty] = 1;
    //DEBUG printf("successfull USED Array Assigned!"); //DEBUG
    return 0;
}

int findvect(const char *name, vect *out) {
    for (int i = 0; i < currentMaxCapacity; i++) {
        if (used[i] && strcmp(store[i].name, name) == 0) {
            *out = store[i];
            return 0;
        }
    }
    return -1;
}

int vect_at(int index, vect *out) {

    //DEBUG printf("%d", currentMaxCapacity);
    //DEBUG printf("\n");
    //DEBUG printf("%d", used[index]);
    if (index < 0 || index >= currentMaxCapacity || !used[index]) {

        return -1; 
    }
    *out = store[index];

    //DEBUG printf("store index vals %s, %f, %f, %f", out->name,out->x,out->y,out->z);
    return 0;
}

void clearvects(void) {
    // Looked up on google how to set all of the values in an array to 0, and found memset.
    memset(used, 0, sizeof(int) * currentMaxCapacity);
    quantity = 0;

}

void freeMemory(void) {
    free(store); // Free the memory allocated for the store array
    free(used); // Free the memory allocated for the used array
}
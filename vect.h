/*
 * @file vect.h
 * @brief Vector type and math operations (inner layer)
 */
#ifndef VECT_H
#define VECT_H

#define NAME_LEN 10

/* 
 * Struct for a 3d vector and a name.
 * used as kind of an "object" so we can do our
 * vector calculations better
 */
typedef struct {
    char name[NAME_LEN];
    double x, y, z;
} vect;

// Function prototypes for vector math operations
vect add(vect a, vect b);
vect sub(vect a, vect b);
vect scale(vect a, double k);
double dot(vect a, vect b);
vect cross(vect a, vect b);

#endif
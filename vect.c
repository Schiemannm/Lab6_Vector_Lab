/*
 * @file vect.c
 * @brief Vector math. 
 */
#include "vect.h"

/* Add two vectors */
vect add(vect a, vect b) {
    vect r;
    r.name[0] = '\0';
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;

}

/* Subtract two vectors */
vect sub(vect a, vect b) {
    vect r;
    r.name[0] = '\0';
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;

}

/* Scale a vector by a scalar */
vect scale(vect a, double k) {
    vect r;
    
    r.name[0] = '\0';
    r.x = a.x * k;
    r.y = a.y * k;
    r.z = a.z * k;
    return r;
}

/* Compute the dot product of two vectors */
double dot(vect a, vect b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Compute the cross product of two vectors */
vect cross(vect a, vect b) {
    vect r;
    r.name[0] = '\0';
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
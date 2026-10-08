/**
 * @file storage.h
 * @brief Fixed-size vector storage (middle layer)
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "vect.h"

#define MAX_VECTS 10

int addvect(vect v);                       // 0 ok (add or replace), -1 full
int findvect(const char *name, vect *out); // 0 found (copy in *out), -1 not found
int vect_at(int index, vect *out);         // 0 if slot in use, -1 if empty/out of range
void clearvects(void);

#endif
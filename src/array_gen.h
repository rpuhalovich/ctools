#ifndef ARRAY_H
#define ARRAY_H

#include <yeet.h>

typedef struct {
    int* items;
    int length;
    int capacity;
} intArray;
intArray* newintArray();
void freeintArray(int_Array* a);
typedef struct {
    float* items;
    int length;
    int capacity;
} floatArray;
floatArray* newfloatArray();
void freefloatArray(float_Array* a);
typedef struct {
    Line* items;
    int length;
    int capacity;
} LineArray;
LineArray* newLineArray();
void freeLineArray(Line_Array* a);

#endif // ARRAY_H

// dynamic_array.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

// Define the StringArray structure
typedef struct {
    char **data;
    int size;
    int capacity;
} StringArray;

// Declare functions (without implementations)
void init(StringArray *arr);
void append(StringArray *arr, const char *str);
void freeArray(StringArray *arr);
void printArray(const StringArray *arr);

#endif // DYNAMIC_ARRAY_H

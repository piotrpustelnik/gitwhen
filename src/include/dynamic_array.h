#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

// Define the dynamic array structure
typedef struct {
    char **data;      // Array of string pointers (array of char* pointers)
    int size;         // Number of strings in the array
    int capacity;     // Total capacity of the array
} StringArray;

// Function declarations
void init(StringArray *arr);
void append(StringArray *arr, const char *str);
void freeArray(StringArray *arr);
void printArray(const StringArray *arr);

#endif // STRING_ARRAY_H

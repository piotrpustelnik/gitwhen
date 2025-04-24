#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the dynamic array structure
typedef struct {
    char **data;      // Array of string pointers (array of char* pointers)
    int size;         // Number of strings in the array
    int capacity;     // Total capacity of the array
} StringArray;

// Initialize the dynamic string array
void init(StringArray *arr) {
    arr->capacity = 2;           // Initial capacity
    arr->size = 0;               // Initially no strings
    arr->data = (char **)malloc(arr->capacity * sizeof(char *));  // Allocate memory for pointers
}

// Append a string to the array
void append(StringArray *arr, const char *str) {
    // If array is full, resize it
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = (char **)realloc(arr->data, arr->capacity * sizeof(char *));
    }

    // Allocate memory for the new string and copy the string into the array
    arr->data[arr->size] = (char *)malloc(strlen(str) + 1); // +1 for null terminator
    strcpy(arr->data[arr->size], str);  // Copy the string into the allocated space
    arr->size++;
}

// Free memory used by the string array
void freeArray(StringArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        free(arr->data[i]);  // Free each string
    }
    free(arr->data);        // Free the array of pointers
    arr->data = NULL;       // Avoid dangling pointer
    arr->size = 0;
    arr->capacity = 0;
}

// Print all strings in the array
void printArray(const StringArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        printf("%s\n", arr->data[i]);
    }
}


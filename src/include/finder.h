#ifndef FINDER_H
#define FINDER_H

#include "dynamic_array.h"
#include <stddef.h> // For size_t

// Function to find all Git repositories in a given directory.
// Returns an array of strings representing the paths of Git repositories found.
// The caller is responsible for freeing the memory.
int find_repos(StringArray *arr, const char *dir_path);

#endif // FINDER_H

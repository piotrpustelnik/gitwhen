#ifndef FINDER_H
#define FINDER_H

#include <stddef.h>  // For size_t

// Function to find all Git repositories in a given directory.
// Returns an array of strings representing the paths of Git repositories found.
// The caller is responsible for freeing the memory.
// The size of the returned array is stored in the `num_repos` parameter.
int find_repos(char **repos, const char *dir_path, size_t *num_repos);

#endif // FINDER_H

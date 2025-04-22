#ifndef FINDER_H
#define FINDER_H

#include <stddef.h>  // For size_t

// Function to find all Git repositories in a given directory.
// Returns an array of strings representing the paths of Git repositories found.
// The caller is responsible for freeing the memory.
// The size of the returned array is stored in the `num_repos` parameter.
char **find_repos(const char *dir_path, size_t *num_repos);

// Frees the memory allocated for the list of repository paths returned by `find_repos`.
void free_repo_list(char **repo_list, size_t num_repos);

#endif // FINDER_H

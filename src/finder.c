#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "finder.h"

// Function to check if a directory is a Git repository
int is_git_repo(const char *path) {
    struct stat statbuf;
    char git_path[4096];

    // Build the path to the ".git" directory
    snprintf(git_path, sizeof(git_path), "%s/.git", path);

    // Check if the ".git" directory exists
    if (stat(git_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        return 1;  // It's a Git repository
    }

    return 0;  // Not a Git repository
}

// Function to recursively find Git repositories in the given directory
char **find_repos(const char *dir_path, size_t *num_repos) {
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    char subdir_path[4096];
    char **repo_list = NULL;
    size_t repo_count = 0;

    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    // Traverse the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Build the path to the current directory
        snprintf(subdir_path, sizeof(subdir_path), "%s/%s", dir_path, entry->d_name);

        // Check if it's a directory
        struct stat statbuf;
        if (stat(subdir_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // If it's a Git repository, add it to the list
            if (is_git_repo(subdir_path)) {
                // Resize the repository list and add the new repo
                repo_list = realloc(repo_list, (repo_count + 1) * sizeof(char *));
                if (repo_list == NULL) {
                    perror("realloc");
                    closedir(dir);
                    return NULL;
                }

                // Allocate memory for the new repo path and copy it
                repo_list[repo_count] = strdup(subdir_path);
                if (repo_list[repo_count] == NULL) {
                    perror("strdup");
                    closedir(dir);
                    return NULL;
                }

                repo_count++;
            }

            // Recursively search subdirectories
            char **subdir_repos = find_repos(subdir_path, num_repos);
            if (subdir_repos != NULL) {
                // Resize the list to accommodate the found repositories
                repo_list = realloc(repo_list, (repo_count + *num_repos) * sizeof(char *));
                if (repo_list == NULL) {
                    perror("realloc");
                    closedir(dir);
                    return NULL;
                }

                // Add the subdirectory repositories to the list
                for (size_t i = 0; i < *num_repos; i++) {
                    repo_list[repo_count + i] = subdir_repos[i];
                }
                repo_count += *num_repos;

                // Free the subdir repository list
                free(subdir_repos);
            }
        }
    }

    closedir(dir);

    *num_repos = repo_count;  // Set the number of repositories found
    return repo_list;
}

// Function to free the memory allocated for the list of repository paths
void free_repo_list(char **repo_list, size_t num_repos) {
    if (repo_list != NULL) {
        for (size_t i = 0; i < num_repos; i++) {
            free(repo_list[i]);
        }
        free(repo_list);
    }
}

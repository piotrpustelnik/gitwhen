#include "dynamic_array.h"
#include "finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH_LEN 4096
#define MAX_REPOS 100
#define MAX_DEPTH 3


// Copy string to heap and get a new pointer
char *copy_string(const char *c)
{
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}


// Check if path ends with '.git'
bool is_git_path(const char *path) {
    // Check if the path ends with ".git"
    size_t len = strlen(path);
    if (len >= 5 && strcmp(path + len - 4, ".git") == 0) {
        // Check if the .git directory exists
        struct stat st;
        return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
    }
    
    // Path does not end with ".git"
    return false;
}

// Get parent path
// First we find a path that contains '.git'
// Then we extract the parent folder path
char *get_parent_path(const char *path) {
    // Find the last occurrence of '/'
    const char *last_slash = strrchr(path, '/');
    
    if (!last_slash) {
        return copy_string("");  // No slash found, return an empty string
    }

    size_t len = last_slash - path;
    char *parent_path = copy_string(path);  // Copy the full path first
    if (parent_path) {
        parent_path[len] = '\0';  // Null-terminate after the last slash
    }
    
    return parent_path;
}


    
int find_repos(StringArray *arr, const char *dir_path) {
    

    if (strlen(dir_path) >= MAX_PATH_LEN) {
        fprintf(stderr, "Error: Path too long\n");
        return -1;
    }


    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir failed");
        return -1;
    }

    struct dirent *entry;
    char new_dir[MAX_PATH_LEN];

    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..'
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Build the full path for the current entry
        snprintf(new_dir, sizeof(new_dir), "%s/%s", dir_path, entry->d_name);

        printf("Checking directory: %s\n", dir_path);

        struct stat statbuf;
        if (stat(new_dir, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // Check if it's a Git repo
            if (is_git_path(new_dir)) {
                printf("Found git directory: %s\n", new_dir);
                append(arr, get_parent_path(new_dir));
            }

            // Recursively search this directory
            find_repos(arr, new_dir);
        }
    }

    closedir(dir);
    return 0;
    
}

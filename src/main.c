#include "finder.h"
#include "gitwrap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For getcwd

void print_usage(const char *program_name) {
    printf("Usage: %s [--directory <path>] <start_date> [end_date]\n", program_name);
}

int main(int argc, char *argv[]) {
    // Default directory path to NULL
    const char *dir_path = NULL;
    const char *start_date = NULL;
    const char *end_date = NULL;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--directory") == 0) {
            if (i + 1 < argc) {
                dir_path = argv[i + 1];
                i++; // Skip the directory argument
            } else {
                fprintf(stderr, "Error: Missing directory path after --directory\n");
                return 1;
            }
        } else if (start_date == NULL) {
            start_date = argv[i];
        } else if (end_date == NULL) {
            end_date = argv[i];
        } else {
            fprintf(stderr, "Error: Too many arguments\n");
            print_usage(argv[0]);
            return 1;
        }
    }

     // If no directory path provided, set it to the current working directory
     if (dir_path == NULL) {
        // Get the current working directory using getcwd
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            return 1;
        }
        dir_path = cwd;  // Set dir_path to the current directory
    }

    // If only one date is provided, use that as the end date (for a single day search)
    // if (end_date == NULL) {
    //     end_date = start_date;
    // }

    // Find all Git repositories in the specified directory
    size_t num_repos = 0;
    
    // TODO: figure out how to do dynamically
    char** repos = malloc(100 * sizeof(char *)); // Array of 100 string pointers;

    // TODO: add if malloc fails
    
    find_repos(repos, dir_path, &num_repos);

    if (repos == NULL) {
        fprintf(stderr, "Error: Failed to find Git repositories in directory '%s'\n", dir_path);
        return 1;
    }

    if (num_repos == 0) {
        printf("No Git repositories found in the specified directory.\n");
    } else {
        // Delete git.txt file if it exists
        if (access("git.txt", F_OK) == 0) {
            if (remove("git.txt") != 0) {
            perror("Error deleting git.txt");
            return 1;
            }
        }
        // For each repository found, print commits between the start and end date
        for (size_t i = 0; i < num_repos; i++) {
            printf("Processing repository: %s\n", repos[i]);
            run_git_log(repos[i], start_date, end_date);
        }
    }

    // Free the allocated memory for the repository paths
    free(repos);

    return 0;
}

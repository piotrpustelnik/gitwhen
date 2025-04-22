
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

// Helper function to free the repo list
void free_repo_list(char **repo_list, size_t count) {
    if (repo_list != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(repo_list[i]);
        }
        free(repo_list);
    }
}

// Check if a directory is a Git repo (using stat instead of opendir)
bool is_git_repo(const char *path) {
    char git_path[4096];
    snprintf(git_path, sizeof(git_path), "%s/.git", path);
    struct stat st;
    return (stat(git_path, &st) == 0 && S_ISDIR(st.st_mode));
}

int find_repos(char **repo_list, const char *dir_path, size_t *num_repos) {
    
    // Input validation
    if (!repo_list || !dir_path || !num_repos) {
        fprintf(stderr, "Error: NULL parameter(s)\n");
        return -1;
    }
    
    if (strlen(dir_path) >= 4096) {
        fprintf(stderr, "Error: Path too long\n");
        return -1;
    }
    
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir failed");
        return -1;
    }
    
    *num_repos = 0;  // Initialize count
    
    if (is_git_repo(dir_path)) {
        repo_list[0] = strdup(dir_path);
        if (!repo_list[0]) {
            perror("strdup failed");
            closedir(dir);
            return -1;
        }
        *num_repos = 1;
    }
    
    closedir(dir);
    return 0;  // Success
}


int main(){

    const char* dir_path = "/home/piotr/projects/gitwhen";
    int num_repos = 0;
    char **repos = malloc(100 * sizeof(char *)); // Array of 100 string pointers;

    find_repos(repos, dir_path, &num_repos);

    return 0;

}
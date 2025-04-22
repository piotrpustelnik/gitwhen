#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "gitwrap.h"

void run_git_log(const char *repo_path, const char *start_date, const char *end_date) {
    // Prepare the base git log command
    char command[1024];
    snprintf(command, sizeof(command), "git -C %s log --pretty=format:\"%%h %%ad %%s\" --date=short", repo_path);

    // If start_date is provided, filter commits from that date onwards
    if (start_date != NULL) {
        snprintf(command + strlen(command), sizeof(command) - strlen(command),
                 " --since=\"%s\"", start_date);
    }

    // If end_date is provided, filter commits up to that date
    if (end_date != NULL) {
        snprintf(command + strlen(command), sizeof(command) - strlen(command),
                 " --until=\"%s\"", end_date);
    }

    // Execute the git log command and print the results
    printf("Running command: %s\n", command);
    FILE *git_log_output = popen(command, "r");
    if (git_log_output == NULL) {
        perror("Failed to run git log");
        return;
    }

    // Read and print the output of the git log command
    char line[256];
    while (fgets(line, sizeof(line), git_log_output)) {
        printf("%s", line);
    }

    fclose(git_log_output);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void run_git_log(const char *repo_path, const char *start_date, const char *end_date) {
    char command[1024];

    // Build the base command for git log
    int written = snprintf(command, sizeof(command),
        "git -C \"%s\" log --pretty=format:\"%%h %%ad %%s\" --date=short",
        repo_path);
    if (written < 0 || written >= sizeof(command)) {
        fprintf(stderr, "Command buffer overflow\n");
        return;
    }

    // Append optional date filters
    if (start_date) {
        written += snprintf(command + written, sizeof(command) - written,
                            " --since=\"%s\"", start_date);
    }

    if (end_date) {
        written += snprintf(command + written, sizeof(command) - written,
                            " --until=\"%s\"", end_date);
    }

    // Open the command as a pipe
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run git log");
        return;
    }

    // Read all output into a dynamic buffer
    char buffer[1024];
    char *output = NULL;
    size_t total_length = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        size_t len = strlen(buffer);
        char *new_output = realloc(output, total_length + len + 1);
        if (!new_output) {
            perror("Failed to allocate memory");
            free(output);
            pclose(fp);
            return;
        }
        output = new_output;
        memcpy(output + total_length, buffer, len);
        total_length += len;
        output[total_length] = '\0';
    }

    pclose(fp);

    // Only print if there was actual output
    if (output && total_length > 0) {
        printf("==================== GIT LOG ====================\n");
        printf("Repository: %s\n\n", repo_path);
        printf("%-10s %-12s %-50s\n", "Commit", "Date", "Message");
        printf("--------------------------------------------------\n");
        printf("%s", output);
        printf("\n====================================================\n\n");
    }

    free(output);
}

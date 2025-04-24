#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ANSI escape codes for colors
#define RESET_COLOR      "\x1b[0m"
#define BOLD_BLUE        "\x1b[1;34m"
#define GREEN            "\x1b[32m"
#define YELLOW           "\x1b[33m"
#define RED              "\x1b[31m"
#define BOLD_WHITE       "\x1b[1;37m"
#define BOLD_CYAN        "\x1b[1;36m"

// Function to run git log and print it nicely formatted
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
        // Printing headers with color
        printf(BOLD_CYAN "==================== GIT LOG ====================\n" RESET_COLOR);
        printf(GREEN "Repository: " BOLD_WHITE "%s\n\n", repo_path);
        printf(BOLD_BLUE "%-10s %-12s %-50s\n", "Commit", "Date", "Message");
        printf(RESET_COLOR "--------------------------------------------------\n");

        // Print the output with a color for each line
        printf("%s", output);

        // Printing footer with color
        printf(RESET_COLOR "\n====================================================\n\n");
    }

    free(output);
}

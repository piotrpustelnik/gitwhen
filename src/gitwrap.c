#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void run_git_log(const char *repo_path, const char *start_date, const char *end_date) {
    char command[1024];

    // Create the git.txt file and write the repository path at the beginning
    FILE *file = fopen("git.txt", "w");
    if (file == NULL) {
        perror("Failed to open git.txt for writing");
        return;
    }

    // Write the repo path into the file with header
    fprintf(file, "==================== GIT LOG ====================\n");
    fprintf(file, "Repository: %s\n\n", repo_path);

    // Create a header for the commit logs
    fprintf(file, "%-10s %-12s %-50s\n", "Commit", "Date", "Message");
    fprintf(file, "--------------------------------------------------\n");
    fclose(file);

    // Build the base command for git log with escaped % symbols
    int written = snprintf(command, sizeof(command),
        "git -C \"%s\" log --pretty=format:\"%%h %%ad %%s\" --date=short",
        repo_path);
    if (written < 0 || written >= sizeof(command)) {
        fprintf(stderr, "Command buffer overflow\n");
        return;
    }

    // Append optional dates
    if (start_date) {
        written += snprintf(command + written, sizeof(command) - written,
                            " --since=\"%s\"", start_date);
    }

    if (end_date) {
        written += snprintf(command + written, sizeof(command) - written,
                            " --until=\"%s\"", end_date);
    }

    // Append file redirection to the command to append output to git.txt
    snprintf(command + strlen(command), sizeof(command) - strlen(command), 
             " >> git.txt");

    // Print the final command to ensure it's correct
    printf("Running command: %s\n", command);

    // Execute the git log command and append the output to git.txt
    int status = system(command);
    if (status == -1) {
        perror("Failed to run command");
        return;
    }


    // Reopen the file to append the commit logs
    file = fopen("git.txt", "a");
    if (file == NULL) {
        perror("Failed to open git.txt for appending");
        return;
    }

    // Add space for clarity
    fprintf(file, "\n====================================================\n");

    fclose(file);
}

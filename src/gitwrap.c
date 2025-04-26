#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RESET "\x1b[0m"
#define COLOR_HEADER "\x1b[1;36m"
#define COLOR_REPO "\x1b[1;33m"
#define COLOR_COMMIT "\x1b[1;32m"
#define COLOR_DATE "\x1b[1;35m"
#define COLOR_MESSAGE "\x1b[1;37m"

// Function to get the Git user name
char *get_git_user_name()
{
    const char USER_NAME_BUFFER_SIZE = 100;
    FILE *fp = popen("git config user.name", "r");
    if (fp == NULL)
    {
        perror("Failed to run git config user.name");
        return NULL;
    }

    // Allocate memory for the user name (buffer size)
    char *user_name = malloc(USER_NAME_BUFFER_SIZE);
    if (user_name == NULL)
    {
        perror("Failed to allocate memory for user name");
        pclose(fp);
        return NULL;
    }

    // Read the output into the user_name buffer
    if (fgets(user_name, USER_NAME_BUFFER_SIZE, fp) == NULL)
    {
        fprintf(stderr, "Failed to get git user name\n");
        free(user_name);
        pclose(fp);
        return NULL;
    }

    // Remove any trailing newline character
    user_name[strcspn(user_name, "\n")] = '\0';

    pclose(fp);
    return user_name;
}

void run_git_log(const char *repo_path, const char *git_author, const char *start_date,
                 const char *end_date)
{
    static int header_printed = 0;
    char command[1024];

    int written = snprintf(command, sizeof(command),
                           "git -C \"%s\" log --all --pretty=format:\"%%h %%ad %%s\" --date=short "
                           "--author=\"%s\" 2>/dev/null",
                           repo_path, git_author);

    if (written < 0 || written >= sizeof(command))
    {
        fprintf(stderr, "Command buffer overflow\n");
        return;
    }

    if (start_date)
    {
        written +=
            snprintf(command + written, sizeof(command) - written, " --since=\"%s\"", start_date);
    }

    if (end_date)
    {
        written +=
            snprintf(command + written, sizeof(command) - written, " --until=\"%s\"", end_date);
    }

    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed to run git log");
        return;
    }

    char buffer[1024];
    char *output = NULL;
    size_t total_length = 0;

    while (fgets(buffer, sizeof(buffer), fp))
    {
        size_t len = strlen(buffer);
        char *new_output = realloc(output, total_length + len + 1);
        if (!new_output)
        {
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

    if (output && total_length > 0)
    {
        const char *hline =
            "┌────────────────────────────────────────────────────────────────────────────┐";
        const char *mline =
            "├────────────────────────────────────────────────────────────────────────────┤";
        const char *bline =
            "└────────────────────────────────────────────────────────────────────────────┘";

        if (!header_printed)
        {
            printf("%s\n", hline);
            printf("│ %s%-74s%s │\n", COLOR_HEADER, "GIT LOG", COLOR_RESET);
            printf("%s\n", mline);
            header_printed = 1;
        }

        printf("│ %sRepository:%s %-62s │\n", COLOR_REPO, COLOR_RESET, repo_path);
        printf("│ %-10s %-12s %-50s │\n", "Commit", "Date", "Message");
        printf("│ %-10s %-12s %-50s │\n", "------", "----------",
               "--------------------------------------------------");

        char *line = strtok(output, "\n");
        while (line != NULL)
        {
            char commit[16], date[16], message[1024];
            if (sscanf(line, "%15s %15s %[^\n]", commit, date, message) == 3)
            {
                size_t msg_len = strlen(message);
                size_t offset = 0;
                int first_line = 1;
                while (offset < msg_len)
                {
                    char chunk[51] = {0}; // 50 chars + null terminator
                    size_t chunk_len = (msg_len - offset >= 50) ? 50 : msg_len - offset;
                    strncpy(chunk, message + offset, chunk_len);
                    chunk[chunk_len] = '\0';

                    // Pad shorter lines manually
                    char padded[51];
                    snprintf(padded, sizeof(padded), "%-50s", chunk);

                    if (first_line)
                    {
                        printf("│ %s%-10s%s %s%-12s%s %s%s%s │\n", COLOR_COMMIT, commit,
                               COLOR_RESET, COLOR_DATE, date, COLOR_RESET, COLOR_MESSAGE, padded,
                               COLOR_RESET);
                        first_line = 0;
                    }
                    else
                    {
                        printf("│ %-10s %-12s %s%s%s │\n", "", "", COLOR_MESSAGE, padded,
                               COLOR_RESET);
                    }

                    offset += chunk_len;
                }
            }
            else
            {
                printf("│ %s%-76s%s │\n", COLOR_MESSAGE, line, COLOR_RESET);
            }
            line = strtok(NULL, "\n");
        }

        printf("%s\n\n", bline);
    }

    free(output);
}

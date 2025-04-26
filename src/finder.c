#include "finder.h"
#include "dynamic_array.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*
Find all git directories (parent path of .git folder)
using unix find command.

Append each result to string array.
*/
int find_repos(StringArray *arr, const char *dir_path)
{

    if (strlen(dir_path) >= MAX_PATH_LEN)
    {
        fprintf(stderr, "Error: Path too long\n");
        return -1;
    }

    // Command buffer
    char command[1024];
    // Redirect stderr (2) to /dev/null
    // This is because we are parsing output line by line and append
    // paths to string array
    snprintf(command, sizeof(command), "find %s -type d -name .git -printf '%%h\\n' 2>/dev/null",
             dir_path);

    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("popen failed");
        return 1;
    }

    char buffer[MAX_PATH_LEN];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Add the cleaned path to the array
        append(arr, buffer);
    }

    int status = pclose(fp);
    if (status == -1)
    {
        perror("pclose failed");
        return 1;
    }

    return 0;
}

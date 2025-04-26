#include "dynamic_array.h"
#include "finder.h"
#include "gitwrap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // For getcwd

void print_usage(const char *program_name)
{
    printf("Usage: %s <start_date> [end_date]\n", program_name);
    printf("Dates must be in YYYY-MM-DD format.\n");
}

// Return pointer to ISO8601 formatted string date beginning at midnight
// NULL pointer if parsing failed.
// Caller needs to free from heap.
char *format_iso_date_midnight(const char *input)
{
    struct tm tm = {0};
    if (sscanf(input, "%4d-%2d-%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3)
    {
        fprintf(stderr, "Date parsing failed\n");
        return NULL;
    }

    tm.tm_year -= 1900; // Adjust year to tm_year format
    tm.tm_mon -= 1;     // Adjust month to tm_mon format (0-11)

    // Allocate memory on the heap for iso_date_time
    char *iso_date_time = (char *)malloc(20 * sizeof(char)); // Format: YYYY-MM-DDTHH:MM:SS

    if (iso_date_time == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Create the ISO 8601 formatted string (UTC time for simplicity)
    strftime(iso_date_time, 20, "%Y-%m-%dT%H:%M:%S", &tm);

    return iso_date_time; // Return pointer to the formatted string
}

// Return pointer to ISO8601 formatted input date + one day
// NULL pointer if parsing failed.
// Caller needs to free from heap.
char *format_iso_date_midnight_plus_one_day(const char *input)
{
    struct tm tm = {0};
    if (sscanf(input, "%4d-%2d-%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3)
    {
        fprintf(stderr, "Date parsing failed\n");
        return NULL;
    }

    tm.tm_year -= 1900; // tm_year is years since 1900
    tm.tm_mon -= 1;     // tm_mon is 0-based

    // Add one day
    tm.tm_mday += 1;

    // Normalize the date
    if (mktime(&tm) == -1)
    {
        fprintf(stderr, "Date normalization failed\n");
        return NULL;
    }

    // Allocate space for ISO 8601 date-time string
    char *iso_date_time = malloc(20); // "YYYY-MM-DDTHH:MM:SS"
    if (!iso_date_time)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Format to ISO 8601 (assuming midnight UTC for simplicity)
    strftime(iso_date_time, 20, "%Y-%m-%dT%H:%M:%S", &tm);

    return iso_date_time;
}

int main(int argc, char *argv[])
{
    const char *start_date = NULL;
    const char *end_date = NULL;

    // Parse command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (start_date == NULL)
        {
            start_date = argv[i];
        }
        else if (end_date == NULL)
        {
            end_date = argv[i];
        }
        else
        {
            fprintf(stderr, "Error: Too many arguments\n");
            print_usage(argv[0]);
            return 1;
        }
    }

    // Check if start_date is provided
    if (start_date == NULL)
    {
        fprintf(stderr, "Error: Missing start date\n");
        print_usage(argv[0]);
        return 1;
    }

    start_date = format_iso_date_midnight(start_date);
    if (start_date == NULL)
    {
        fprintf(stderr, "Error: Invalid start date format. Expected YYYY-MM-DD.\n");
        return 1;
    }

    // If end_date is not provided, use start_date as end_date
    if (end_date == NULL)
    {
        end_date = format_iso_date_midnight_plus_one_day(start_date);
    }
    else
    {
        end_date = format_iso_date_midnight_plus_one_day(end_date);
        if (end_date == NULL)
        {
            fprintf(stderr, "Error: Invalid end date format. Expected YYYY-MM-DD.\n");
            return 1;
        }
    }

    // Get current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    // Print values for testing
    printf("Start Date: %s\n", start_date);
    printf("End Date: %s\n", end_date);
    printf("Directory: %s\n", cwd);

    // Find all Git repositories in the specified directory

    // // Initialise dynamic array for repo strings
    StringArray *repos_arr = (StringArray *)malloc(sizeof(StringArray));
    init(repos_arr);

    int err = find_repos(repos_arr, cwd);

    if (err)
    {
        fprintf(stderr, "find_repos: %d", err);
        freeArray(repos_arr);
        return 1;
    }

    if (repos_arr->size == 0)
    {
        fprintf(stderr, "Error: Failed to find Git repositories in directory '%s'\n", cwd);
        freeArray(repos_arr);
        return 1;
    }

    // Delete git.txt file if it exists
    if (access("git.txt", F_OK) == 0)
    {
        if (remove("git.txt") != 0)
        {
            perror("Error deleting git.txt");
            return 1;
        }
    }

    // For each repository found, print commits between the start and end date
    for (size_t i = 0; i < repos_arr->size; i++)
    {
        // printf("Processing repository: %s\n", repos_arr->data[i]);
        run_git_log(repos_arr->data[i], start_date, end_date);
    }

    // Free allocated memory for dynamic array
    freeArray(repos_arr);
    return 0;
}

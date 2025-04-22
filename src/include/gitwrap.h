#ifndef GITWRAP_H
#define GITWRAP_H

// Runs `git log` for a given repo path and dates.
// Outputs formatted commits to stdout.
void run_git_log(const char *repo_path, const char *start_date, const char *end_date);

#endif // GITWRAP_H
#ifndef GITWRAP_H
#define GITWRAP_H

// Return git user name
// as configured in git.config
char *get_git_user_name();

// Runs `git log` for a given repo path and dates.
// Outputs formatted commits to stdout.
void run_git_log(const char *repo_path, const char *git_author, const char *start_date,
                 const char *end_date);

#endif // GITWRAP_H
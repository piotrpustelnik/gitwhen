#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



int main() {
    const char *input = "2025-04-24";

    // Call the function to format the date
    char *iso_date_time = format_iso_date(input);

    if (iso_date_time != NULL) {
        // Print the ISO 8601 formatted date
        printf("ISO 8601 Date-Time: %s\n", iso_date_time);

        // Remember: you need to free the memory later
        // free(iso_date_time);
    }

    return 0;
}

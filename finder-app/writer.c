// Akshay Nandi (akshaynandi2762)

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    // Open syslog with LOG_USER facility
    openlog("writer", LOG_PID, LOG_USER);

    // Check the number of arguments passed
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments. Expected 2, got %d", argc - 1);
        fprintf(stderr, "Usage: %s <write_file> <write_string>\n", argv[0]);
        closelog();
        return 1;
    }

    const char *write_file = argv[1];
    const char *write_string = argv[2];

    // Log a message that we are writing write_string to the write_file
    syslog(LOG_DEBUG, "Writing %s to %s", write_string, write_file);

    // Open the file for writing (in overwrite mode)
    FILE *fp = fopen(write_file, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "Error opening file %s", write_file);
        perror("Error opening file");
        closelog();
        return 1;
    }

    // Write the write_string to the file
    if (fputs(write_string, fp) == EOF) {
        syslog(LOG_ERR, "Error writing to file %s", write_file);
        perror("Error writing to file");
        fclose(fp);
        closelog();
        return 1;
    }

    // Clean up
    fclose(fp);
    closelog();
    return 0;
}


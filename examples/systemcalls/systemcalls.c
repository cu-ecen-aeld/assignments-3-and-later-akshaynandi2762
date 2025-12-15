#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdbool.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    if (cmd == NULL)
        return false;

    int command = system(cmd);

    if (command == -1)
        return false;

    if (WIFEXITED(command) && WEXITSTATUS(command) == 0)
        return true;

    return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/
bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];

    pid_t fork_command = fork();
    if (fork_command < 0)
    {
        return false;
    }

    if (fork_command == 0)
    {
        // Child
        execv(command[0], command);
        // execv only returns on error
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent
        int parent_status;
        if (waitpid(fork_command, &parent_status, 0) < 0)
            return false;

        if (WIFEXITED(parent_status) && WEXITSTATUS(parent_status) == 0)
            return true;

        return false;
    }
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    for (int i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];

    pid_t fork_command = fork();
    if (fork_command < 0)
    {
        return false;
    }

    if (fork_command == 0)
    {
        // Child

        int file_desc = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_desc < 0)
        {
            perror("file open failed");
            return false;
        }

        // Redirect stdout to file
        if (dup2(file_desc, STDOUT_FILENO) < 0)
        {
            close(file_desc);
            return false;
        }

        close(file_desc);

        execv(command[0], command);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent
        int parent_status;
        if (waitpid(fork_command, &parent_status, 0) < 0)
            return false;

        if (WIFEXITED(parent_status) && WEXITSTATUS(parent_status) == 0)
            return true;

        return false;
    }
}

#include "systemcalls.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int command_ret_status = system(cmd);
    if(command_ret_status == 0)
    {
        printf("Command was executed successfully!");
        return true;
    }
    else
    {
        printf("Command failed to execute or to be invoked..");
        return false;
    }
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
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];
    
    pid_t fork_command = fork();

    if(fork_command < 0)
    {
        // If fork() failed
        printf("Fork command failed!");
        return false;
    }
    else if(fork_command == 0)
    {
        // If this is the child, it runs a new process
        printf("Child is running with the PID: %d", getpid());
        if(execv("echo", command) == -1)
        {
            printf("execv return with -1");
            return false;
        }
    }
    else
    {
        // Parent has forked a child process and is waiting now..
        int status;
        if (waitpid(fork_command, &status, 0) != -1)
        { 
            if(WIFEXITED(status))
            { 
                int returned = WEXITSTATUS(status);
                printf("Exited normally with status %d, with WIFEXITED(status) info: %d\n", returned, WIFEXITED(status));
            }
            else if(WIFSIGNALED(status))
            { 
                int signum = WTERMSIG(status);
                printf("Exited due to receiving signal %d\n", signum);
            }
            else if(WIFSTOPPED(status))
            { 
                int signum = WSTOPSIG(status);
                printf("Stopped due to receiving signal %d\n", signum);
            }
            else
            {
                printf("Abnormally exited due to unknown reason..\n");
            }
        }

        else
        {
            printf("waitpid() failed!");
            return false;
        }
    }

    return true;
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

    char * command[count + 1];
    int i;
    for(i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork failed");
        return false;
    }

    if(pid == 0)
    {
        // Child process

        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd < 0)
        {
            perror("open failed");
        }

        // Redirect stdout to file
        if(dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2 failed");
            close(fd);

        }

        close(fd);
        execv(command[0], command);

        // execv only returns on error
        perror("execv failed");
    }
    else
    {
        // Parent process
        int status;
        if(waitpid(pid, &status, 0) < 0)
        {
            perror("waitpid failed");
            return false;
        }

        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            return true;
        }

        return false;
    }

    return true;
}


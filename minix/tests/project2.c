
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#define MAX_PROCS 100       // Max concurrent processes
#define MAX_FILEPATH 64     // Max string length to store created file path

/*
 * Name: write_then_read
 *
 * Description:
 *     Given a path to write to, makes the system calls necessary to create
 *     a new file, write 5 kilobytes to it, then read them back out.
 */
int write_then_read(char* file_path)
{
    FILE *fptr = NULL;

    clock_t write_time;
    clock_t read_time;
    double write_time_value;
    double read_time_value;

    pid_t this_pid = getpid();

    char read_buffer[10240]; // 10 KB, call me superstitious

    int file_index = 0;

    int retcode = 0;
    int lines_read = 0;

    // Perform the writing load
    write_time = clock();
    errno = 0;
    fptr = fopen(file_path, "w+");
    if (fptr == NULL)
    {
        printf("!\tError creating file %s for pid %u. Errno: %d\n", file_path, this_pid, errno);
        return -1;
    }

    printf("Starting write for process with pid %u\n", this_pid);
    // 1024 * 5 characters == 5 KB :)
    for (; file_index < 1024; file_index++)
    {
        retcode = fprintf(fptr, "%04d\n", file_index);
        if (retcode < 0)
        {
            printf("!\tWrite to file %s for pid %u failed!", file_path, this_pid);
            fclose(fptr);
            return -1;
        }
    }
    write_time = clock() - write_time;
    printf("Write complete for pid %u\n", this_pid);

    // Rewind file pointer to beginning of file
    rewind(fptr);

    // Perform the reading load
    read_time = clock();
    printf("Starting read for process with pid %u\n", this_pid);
    // fscanf returns 1 on successful conversion to specified format string
    while (fscanf(fptr, "%s", read_buffer) == 1)
    {
        ++lines_read;
    }

    // If fscanf fails, either it reached the end of the file or there was
    // a read error. Check that condition here.
    if (feof(fptr))
    {
        read_time = clock() - read_time;
        printf("Read complete for pid %u\n", this_pid);
    }
    else
    {
        printf("!\tError while reading file %s for pid %u\n", file_path, this_pid);
        fclose(fptr);
        return -1;
    }

    // Append summary info to the end for test & performance scripts
    write_time_value = ((double)write_time/CLOCKS_PER_SEC);
    read_time_value = ((double)read_time/CLOCKS_PER_SEC);
    retcode = fprintf(fptr, "Lines read: %04d \t Write time: %f \t Read time: %f\n", lines_read, write_time_value, read_time_value);
    if (retcode < 0)
    {
        printf("!\tAppend to file %s for pid %u failed!", file_path, this_pid);
        fclose(fptr);
        return -1;
    }

    fclose(fptr);
    return 0;
}

/*
 * Name: main
 *
 * Description:
 *     The main entry point of the program. Handles spawning and joining
 *     the processes that will do the actual reading and writing.
 */
int main(int argc, char *argv[])
{
    int i = 0;

    int max_procs = 0;

    char file_path[MAX_FILEPATH];
    char* endptr;

    pid_t child_pid = 0;

    // Optionally, the user can specify a lower limit to the max number of
    // processes to fork. Check for that parameter here.
    if (argc > 1)
    {
        // strtoimax converts a string-represented number to an integer,
        // if possible. Sets errno to ERANGE if conversion fails.
        errno = 0;
        max_procs = strtoimax(argv[1], &endptr, 10);

        if (errno == ERANGE || max_procs == 0 || max_procs > MAX_PROCS)
        {
            printf("!\tInvalid input for max processes! Use an integer value <= %u.\n", MAX_PROCS);
            return -1;
        }
        else
        {
            printf("Running with a user-selected process maximum of %u\n", max_procs);
        }
    }
    else
    {
        max_procs = MAX_PROCS;
        printf("Running with default process maximum of %u\n", MAX_PROCS);
    }

    // Create the 100 processes by forking
    for (i = 0; i < max_procs; i++)
    {
	if ((child_pid = fork()) == 0)
        {
            sprintf(file_path, "./file_%03u", i);
            write_then_read(file_path);

            printf("Child process with pid %u finished for index %u\n", getpid(), i);

            // Return to end child process (AND TO AVOID FORK BOMBING!)
            return 0;
        }
        else if (child_pid > 0)
        {
            printf("Process with pid %u successfully created for index %u\n", child_pid, i);
        }
        else // error while forking
        {
            printf("!\tFailed to fork new process at index %u: [%s]\n", i, strerror(errno));
        }
    }

    // Reap child processes
    for (i = 0; i < max_procs; i++)
    {
        if ((child_pid = wait(NULL)) > 0)
        {
            printf("Successfully reaped child process with pid %u\n", child_pid);
        }
        else // == -1, which indicates an error
        {
            printf("!\tFailed to reap child process: [%s]\n", strerror(errno));
        }
    }

    return 0;
}

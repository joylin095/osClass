#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct timeval start, end;
    pid_t pid;
    int status;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        close(pipefd[0]);

        gettimeofday(&start, NULL);

        if (write(pipefd[1], &start, sizeof(start)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);

        execvp(argv[1], &argv[1]);

        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(&status);

        if (!WIFEXITED(status))
        {
            fprintf(stderr, "Child process did not exit normally\n");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);

        if (read(pipefd[0], &start, sizeof(start)) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        gettimeofday(&end, NULL);

        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double elapsed = seconds + microseconds / 1e6;

        printf("Elapsed time: %.6f seconds\n", elapsed);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

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

    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
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
        void *sharedMemory = shmat(shmid, NULL, 0);
        if (sharedMemory == (void *)-1)
        {
            perror("shmat");
            exit(EXIT_FAILURE);
        }

        gettimeofday(&start, NULL);

        *(struct timeval *)sharedMemory = start;

        execvp(argv[1], &argv[1]);

        shmdt(sharedMemory);

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

        void *sharedMemory = shmat(shmid, NULL, 0);
        if (sharedMemory == (void *)-1)
        {
            perror("shmat");
            exit(EXIT_FAILURE);
        }

        gettimeofday(&end, NULL);

        struct timeval *start_ptr = (struct timeval *)sharedMemory;
        struct timeval start = *start_ptr;

        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double elapsed = seconds + microseconds / 1e6;

        shmdt(sharedMemory);

        shmctl(shmid, IPC_RMID, NULL);

        printf("Elapsed time: %.6f seconds\n", elapsed);
    }
    return 0;
}
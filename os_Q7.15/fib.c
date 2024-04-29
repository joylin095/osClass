#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SEQUENCE_LENGTH 100

int fibSequence[MAX_SEQUENCE_LENGTH];
int n;
int fibComputed = 0;

pthread_mutex_t fibMutex;
pthread_cond_t fibCond;

void *GenerateFibonacci(void *arg)
{
    int *sequence = (int *)arg;
    sequence[0] = 0;
    if (n > 0)
    {
        sequence[1] = 1;
        for (int i = 2; i < n; i++)
        {
            pthread_mutex_lock(&fibMutex);
            fibComputed = i;
            sequence[i] = sequence[i - 1] + sequence[i - 2];
            pthread_cond_signal(&fibCond);
            pthread_mutex_unlock(&fibMutex);
        }
    }
    fibComputed = n;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number of Fibonacci numbers>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0 || n > MAX_SEQUENCE_LENGTH)
    {
        printf("Number of Fibonacci numbers must be greater than 0 and less than or equal to %d\n", MAX_SEQUENCE_LENGTH);
        return 1;
    }

    pthread_t thread;
    pthread_attr_t attr;
    int threadError;

    pthread_attr_init(&attr);
    pthread_mutex_init(&fibMutex, NULL);
    pthread_cond_init(&fibCond, NULL);

    threadError = pthread_create(&thread, &attr, GenerateFibonacci, (void *)fibSequence);

    if (threadError)
    {
        printf("Error - pthread_create() return code: %d\n", threadError);
        return 1;
    }

    printf("Generated Fibonacci sequence:\n");
    for (int i = 0; i < n; i++)
    {
        pthread_mutex_lock(&fibMutex);
        while (fibComputed <= i)
        {
            pthread_cond_wait(&fibCond, &fibMutex);
        }
        printf("%d ", fibSequence[i]);
        pthread_mutex_unlock(&fibMutex);
    }
    printf("\n");

    pthread_mutex_destroy(&fibMutex);
    pthread_cond_destroy(&fibCond);
    pthread_exit(NULL);
    return 0;
}
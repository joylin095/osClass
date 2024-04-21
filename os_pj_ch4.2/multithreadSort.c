#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

void *Sort(void *arg);
void *Merge(void *arg);

int numbers[SIZE] = {4, 2, 7, 1, 9, 3, 6, 8, 5, 0};
int sortedNumbers[SIZE];

typedef struct
{
    int start;
    int end;
} ThreadArgs;

int main()
{
    printf("Initial arrays:\n");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    pthread_t thread1, thread2, thread3;
    ThreadArgs args1 = {0, SIZE / 2};
    ThreadArgs args2 = {SIZE / 2, SIZE};

    pthread_create(&thread1, NULL, Sort, (void *)&args1);
    pthread_create(&thread2, NULL, Sort, (void *)&args2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_create(&thread3, NULL, Merge, NULL);
    pthread_join(thread3, NULL);

    printf("Sorted Array: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", sortedNumbers[i]);
    }
    printf("\n");

    return 0;
}

void *Sort(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int start = args->start;
    int end = args->end;

    for (int i = start; i < end; i++)
    {
        for (int j = start; j < end - i + start - 1; j++)
        {
            if (numbers[j] > numbers[j + 1])
            {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

void *Merge(void *args)
{
    int i = 0, j = SIZE / 2, k = 0;

    while (i < SIZE / 2 && j < SIZE)
    {
        if (numbers[i] < numbers[j])
        {
            sortedNumbers[k++] = numbers[i++];
        }
        else
        {
            sortedNumbers[k++] = numbers[j++];
        }
    }
    while (i < SIZE / 2)
    {
        sortedNumbers[k++] = numbers[i++];
    }
    while (j < SIZE)
    {
        sortedNumbers[k++] = numbers[j++];
    }
    pthread_exit(NULL);
}
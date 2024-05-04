#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void InsertItem(buffer_item item);
void RemoveItem();
void *Producer(void *param);
void *Consumer(void *param);

int main(int argc, char *argv[])
{
    int sleepTime, producerNum, consumerNum;

    if (argc != 4)
    {
        printf("Usage: %s <sleep time> <producer threads> <consumer threads>\n", argv[0]);
        return 0;
    }

    sleepTime = atoi(argv[1]);
    producerNum = atoi(argv[2]);
    consumerNum = atoi(argv[3]);

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    srand(time(0));

    for (int i = 0; i < producerNum; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, Producer, NULL);
    }

    for (int i = 0; i < consumerNum; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, Consumer, NULL);
    }

    sleep(sleepTime);
    return 0;
}

void *Producer(void *param)
{
    buffer_item item;
    int r;

    r = rand() % 5;
    sleep(r);
    item = rand();

    InsertItem(item);
}

void *Consumer(void *param)
{
    int r;

    r = rand() % 5;
    sleep(r);

    RemoveItem();
}

void InsertItem(buffer_item item)
{
    sem_wait(&empty);

    pthread_mutex_lock(&mutex);

    buffer[insertPointer++] = item;
    insertPointer %= 5;
    printf("Producer produced %d \n", item);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void RemoveItem()
{
    sem_wait(&full);

    pthread_mutex_lock(&mutex);

    printf("Consumer consumed %d \n", buffer[removePointer]);
    buffer[removePointer++] = -1;
    removePointer %= 5;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}

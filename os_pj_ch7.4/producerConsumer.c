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

void InsertItem(buffer_item item, int pid);
void RemoveItem(int cid);
void *Producer(void *producerNo);
void *Consumer(void *consumerNo);

int main(int argc, char *argv[])
{
    pthread_t producers[5], consumers[5];
    int sleepTime, producerNum, consumerNum;

    if (argc != 4)
    {
        printf("Usage: %s <sleep time> <producer threads> <consumer threads>\n", argv[0]);
        return 0;
    }

    sleepTime = atoi(argv[1]);
    producerNum = atoi(argv[2]);
    consumerNum = atoi(argv[3]);

    if (producerNum > 5 || consumerNum > 5)
    {
        printf("producer and consumer max 5\n");
        return 0;
    }
    else if (consumerNum > producerNum)
    {
        printf("consumer cannot be larger than producer\n");
        return 0;
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    srand(time(0));

    int id[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < producerNum; i++)
    {
        pthread_create(&producers[i], NULL, Producer, (void *)&id[i]);
    }

    for (int i = 0; i < consumerNum; i++)
    {
        pthread_create(&consumers[i], NULL, Consumer, (void *)&id[i]);
    }

    for (int i = 0; i < producerNum; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < consumerNum; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    sleep(sleepTime);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

void *Producer(void *producerNo)
{
    buffer_item item;
    int r;

    r = rand() % 5;
    sleep(r);
    item = rand();

    InsertItem(item, *((int *)producerNo));
}

void *Consumer(void *consumerNo)
{
    int r;

    r = rand() % 5;
    sleep(r);

    RemoveItem(*((int *)consumerNo));
}

void InsertItem(buffer_item item, int producerNo)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[insertPointer] = item;

    printf("Producer #%d inserted item %d at buffer position %d\n", producerNo, item, insertPointer);

    insertPointer = (insertPointer + 1) % 5;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void RemoveItem(int consumerNo)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    buffer_item item = buffer[removePointer];

    printf("Consumer #%d removed item %d at buffer position %d\n", consumerNo, item, removePointer);

    removePointer = (removePointer + 1) % 5;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}

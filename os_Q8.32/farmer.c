#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_FARMERS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t crossCond = PTHREAD_COND_INITIALIZER;

int on_bridge = 0;

void *Northbound_farmer(void *arg)
{
    pthread_mutex_lock(&mutex);

    while (on_bridge)
    {
        pthread_cond_wait(&crossCond, &mutex);
    }

    on_bridge = 1;
    printf("Northbound farmer is crossing the bridge.\n");
    pthread_mutex_unlock(&mutex);

    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);
    on_bridge = 0;
    pthread_cond_signal(&crossCond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *Southbound_farmer(void *arg)
{
    pthread_mutex_lock(&mutex);

    while (on_bridge)
    {
        pthread_cond_wait(&crossCond, &mutex);
    }

    on_bridge = 1;
    printf("Southbound farmer is crossing the bridge.\n");
    pthread_mutex_unlock(&mutex);

    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);
    on_bridge = 0;
    pthread_cond_signal(&crossCond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t north_thread[MAX_FARMERS], south_thread[MAX_FARMERS];

    for (int i = 0; i < MAX_FARMERS; i++)
    {
        pthread_create(&north_thread[i], NULL, Northbound_farmer, NULL);
        pthread_create(&south_thread[i], NULL, Southbound_farmer, NULL);
    }

    for (int i = 0; i < MAX_FARMERS; i++)
    {
        pthread_join(north_thread[i], NULL);
        pthread_join(south_thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
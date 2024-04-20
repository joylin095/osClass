#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_RESOURCES 5;

int available_resources = MAX_RESOURCES;
pthread_mutex_t resource_lock;

int decrease_count(int count)
{
    pthread_mutex_lock(&resource_lock);
    if (count > available_resources)
    {
        pthread_mutex_unlock(&resource_lock);
        return -1;
    }
    else
    {
        available_resources -= count;
        pthread_mutex_unlock(&resource_lock);
        return 0;
    }
}

int increase_count(int count)
{
    pthread_mutex_lock(&resource_lock);
    available_resources += count;
    pthread_mutex_unlock(&resource_lock);
    return 0;
}

int main()
{
    if (pthread_mutex_init(&resource_lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }

    // Destroy the mutex lock
    pthread_mutex_destroy(&resource_lock);

    return 0;
}
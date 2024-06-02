#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CYLINDERS 5000
#define NUM_REQUESTS 1000

// Function prototypes
int fcfs(int requests[], int numRequests, int initialPosition);
int scan(int requests[], int numRequests, int initialPosition);
int c_scan(int requests[], int numRequests, int initialPosition);
void generate_requests(int requests[], int numRequests);
int compare(const void *a, const void *b);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <initial_position>\n", argv[0]);
        return 1;
    }

    int initialPosition = atoi(argv[1]);
    if (initialPosition < 0 || initialPosition >= NUM_CYLINDERS)
    {
        printf("Error: Initial position must be between 0 and %d\n", NUM_CYLINDERS - 1);
        return 1;
    }

    int requests[NUM_REQUESTS];
    generate_requests(requests, NUM_REQUESTS);

    printf("Initial position: %d\n", initialPosition);

    int fcfs_movement = fcfs(requests, NUM_REQUESTS, initialPosition);
    int scan_movement = scan(requests, NUM_REQUESTS, initialPosition);
    int c_scan_movement = c_scan(requests, NUM_REQUESTS, initialPosition);

    printf("Total head movement for FCFS: %d\n", fcfs_movement);
    printf("Total head movement for SCAN: %d\n", scan_movement);
    printf("Total head movement for C-SCAN: %d\n", c_scan_movement);

    return 0;
}

void generate_requests(int requests[], int numRequests)
{
    srand(time(NULL));
    for (int i = 0; i < numRequests; i++)
    {
        requests[i] = rand() % NUM_CYLINDERS;
    }
}

int fcfs(int requests[], int numRequests, int initialPosition)
{
    int headMovement = 0;
    int currenPosition = initialPosition;

    for (int i = 0; i < numRequests; i++)
    {
        headMovement += abs(requests[i] - currenPosition);
        currenPosition = requests[i];
    }

    return headMovement;
}

int scan(int requests[], int numRequests, int initialPosition)
{
    int headMovement = 0;
    int currenPosition = initialPosition;

    qsort(requests, numRequests, sizeof(int), compare);

    int i;
    for (i = 0; i < numRequests; i++)
    {
        if (requests[i] > currenPosition)
        {
            break;
        }
    }

    for (int j = i; j < numRequests; j++)
    {
        headMovement += abs(requests[j] - currenPosition);
        currenPosition = requests[j];
    }

    if (i > 0)
    {
        headMovement += abs((NUM_CYLINDERS - 1) - currenPosition);
        currenPosition = NUM_CYLINDERS - 1;

        for (int j = i - 1; j >= 0; j--)
        {
            headMovement += abs(requests[j] - currenPosition);
            currenPosition = requests[j];
        }
    }

    return headMovement;
}

int c_scan(int requests[], int numRequests, int initialPosition)
{
    int headMovement = 0;
    int currenPosition = initialPosition;

    qsort(requests, numRequests, sizeof(int), compare);

    int i;
    for (i = 0; i < numRequests; i++)
    {
        if (requests[i] > currenPosition)
        {
            break;
        }
    }

    for (int j = i; j < numRequests; j++)
    {
        headMovement += abs(requests[j] - currenPosition);
        currenPosition = requests[j];
    }

    if (i > 0)
    {
        headMovement += abs((NUM_CYLINDERS - 1) - currenPosition);
        currenPosition = 0;
        headMovement += (NUM_CYLINDERS - 1);

        for (int j = 0; j < i; j++)
        {
            headMovement += abs(requests[j] - currenPosition);
            currenPosition = requests[j];
        }
    }

    return headMovement;
}

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    int burstTime;
    int priority;
} Process;

void FCFS(Process processes[], int n);
void SJF(Process processes[], int n);
void PriorityScheduling(Process processes[], int n);
void RoundRobin(Process processes[], int n, int quantum);
void PriorityWithRoundRobin(Process processes[], int n, int quantum);

int main()
{
    Process processes[] = {
        {1, 4, 3},
        {2, 5, 2},
        {3, 8, 2},
        {4, 7, 1},
        {5, 3, 3}};

    int n = sizeof(processes) / sizeof(processes[0]);

    Process processesCopyFCFS[n];
    Process processesCopySJF[n];
    Process processesCopyPriority[n];
    Process processesCopyRR[n];
    Process processesCopyPriorityRR[n];
    memcpy(processesCopyFCFS, processes, sizeof(processes));
    memcpy(processesCopySJF, processes, sizeof(processes));
    memcpy(processesCopyPriority, processes, sizeof(processes));
    memcpy(processesCopyRR, processes, sizeof(processes));
    memcpy(processesCopyPriorityRR, processes, sizeof(processes));

    printf("Original Process order: \n");
    printf("process ID, BurstTime, Priority\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d, %d, %d\n", processes[i].id, processes[i].burstTime, processes[i].priority);
    }
    printf("\n");

    printf("FCFS Scheduling:\n");
    FCFS(processesCopyFCFS, n);

    printf("\nShortest Job First (SJF) Scheduling:\n");
    SJF(processesCopySJF, n);

    printf("\nPriority-Based Scheduling:\n");
    PriorityScheduling(processesCopyPriority, n);

    printf("\nRound-Robin Scheduling:\n");
    RoundRobin(processesCopyRR, n, 2);

    printf("\nPriority with Round-Robin Scheduling:\n");
    PriorityWithRoundRobin(processesCopyPriorityRR, n, 2);

    return 0;
}

void FCFS(Process processes[], int n)
{
    printf("Process execution order: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", processes[i].id);
    }
    printf("\n");
}

void SJF(Process processes[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].burstTime > processes[j + 1].burstTime)
            {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    FCFS(processes, n);
}

void PriorityScheduling(Process processes[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    FCFS(processes, n);
}

void RoundRobin(Process processes[], int n, int quantum)
{
    printf("Process execution order: ");
    int done = 0;

    while (!done)
    {
        done = 1;

        for (int i = 0; i < n; i++)
        {
            if (processes[i].burstTime > 0)
            {
                done = 0;
                if (processes[i].burstTime > quantum)
                {
                    processes[i].burstTime -= quantum;
                    printf("%d ", processes[i].id);
                }
                else
                {
                    processes[i].burstTime = 0;
                    printf("%d ", processes[i].id);
                }
            }
        }
        if (done)
        {
            break;
        }
    }
    printf("\n");
}

void PriorityWithRoundRobin(Process processes[], int n, int quantum)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    printf("Process execution order: ");

    for (int i = 0; i < n; i++)
    {
        if (i != n - 1)
        {
            if (processes[i].priority == processes[i + 1].priority)
            {
                int j = i, sameLength;
                while (j < n && processes[j].priority == processes[i].priority)
                {
                    j += 1;
                }
                j -= 1;
                sameLength = j - i + 1;
                while (sameLength)
                {
                    for (int k = i; k <= j; k++)
                    {
                        if (sameLength == 1)
                        {
                            printf("%d ", processes[k].id);
                            sameLength -= 1;
                            i = j;
                            break;
                        }
                        if (processes[k].burstTime > quantum)
                        {
                            processes[k].burstTime -= quantum;
                        }
                        else
                        {
                            processes[k].burstTime = 0;
                            sameLength -= 1;
                        }
                        printf("%d ", processes[k].id);
                    }
                }
            }
            else
            {
                printf("%d ", processes[i].id);
            }
        }
        else
        {
            printf("%d ", processes[i].id);
        }
    }
    printf("\n");
}
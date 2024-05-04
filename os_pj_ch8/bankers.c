#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

int n; // customers number
int m; // resources

int avaliable[MAX_SIZE];
int maximum[MAX_SIZE][MAX_SIZE];
int allocation[MAX_SIZE][MAX_SIZE];
int need[MAX_SIZE][MAX_SIZE];

int RequestResources(int customerNum, int request[]);
void ReleaseResources(int customerNum, int release[]);

int main(int argc, char *argv[])
{
    m = argc - 1;
    for (int i = 1; i < argc; i++)
    {
        avaliable[i - 1] = atoi(argv[i]);
    }

    printf("Enter the number of customers: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d,", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }
    }

    while (1)
    {
        char buf[MAX_SIZE];
        char c;
        int i = 0;

        while ((c = getchar()) != (int)'\n')
        {
            buf[i++] = c;
        }

        if (buf[0] == 'R' && buf[1] == 'Q')
        {
            int customerIndex;
            int require[m];
            char *pointer = (buf + 3);
            sscanf(pointer, "%d", &customerIndex);

            pointer += 2;
            for (int i = 0; i < m; i++)
            {
                sscanf(pointer, "%d", &require[i]);
                pointer += 2;
            }

            if (RequestResources(customerIndex, require) == 0)
            {
                printf("This situation is safe, accepted.\n");
                continue;
            }
            else
            {
                printf("This request is unsafe, denied.\n");
                continue;
            }
        }
        else if (buf[0] == 'R' && buf[1] == 'L')
        {
            int customerIndex;
            int release[m];
            char *pointer = (buf + 3);
            sscanf(pointer, "%d", &customerIndex);

            pointer += 2;
            for (int i = 0; i < m; i++)
            {
                sscanf(pointer, "%d", &release[i]);
                pointer += 2;
            }

            ReleaseResources(customerIndex, release);
        }
        else if (buf[0] == '*')
        {
            printf("The avaliable vector is:\n");
            for (int i = 0; i < m; i++)
            {
                printf("%d ", avaliable[i]);
            }
            printf("\n");

            printf("The maximum matrix is:\n");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    printf("%d ", maximum[i][j]);
                }
                printf("\n");
            }

            printf("The allocation matrix is:\n");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    printf("%d ", allocation[i][j]);
                }
                printf("\n");
            }

            printf("The need matrix is:\n");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    printf("%d ", need[i][j]);
                }
                printf("\n");
            }
        }
    }
}

int RequestState(int customerNum, int request[])
{
    int isRequestMoreThanNeed = 0;
    int isRequestMoreThanAvaliable = 0;

    for (int i = 0; i < m; i++)
    {
        if (request[i] > need[customerNum][i])
        {
            isRequestMoreThanNeed = 1;
            break;
        }
    }
    if (isRequestMoreThanNeed)
    {
        return 0;
    }

    for (int i = 0; i < m; i++)
    {
        if (request[i] > avaliable[i])
        {
            isRequestMoreThanAvaliable = 1;
            break;
        }
    }
    if (isRequestMoreThanAvaliable)
    {
        return 0;
    }

    return 1;
}

int SafeState()
{
    int finish[n], work[m];
    int canDoFlag = 1, canWork = 0;
    for (int i = 0; i < n; i++)
    {
        finish[i] = 0;
    }
    for (int i = 0; i < m; i++)
    {
        work[i] = avaliable[i];
    }

    while (canDoFlag)
    {
        canDoFlag = 0;
        for (int i = 0; i < n; i++)
        {
            canWork = 1;
            for (int j = 0; j < m; j++)
            {
                if (need[i][j] > work[j])
                {
                    canWork = 0;
                    break;
                }
            }
            if (canWork && !finish[i])
            {
                for (int j = 0; j < m; j++)
                {
                    work[j] += allocation[i][j];
                }
                finish[i] = 1;
                canDoFlag = 1;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (!finish[i])
        {
            return 0;
        }
    }

    return 1;
}

int RequestResources(int customerNum, int request[])
{
    if (RequestState(customerNum, request))
    {
        for (int i = 0; i < m; i++)
        {
            avaliable[i] -= request[i];
            allocation[customerNum][i] += request[i];
            need[customerNum][i] -= request[i];
        }

        if (SafeState())
        {
            int addResouceFlag = 1;
            for (int i = 0; i < m; i++)
            {
                if (need[i] != 0)
                {
                    addResouceFlag = 0;
                    break;
                }
            }
            if (addResouceFlag)
            {
                for (int i = 0; i < m; i++)
                {
                    avaliable[i] += allocation[customerNum][i];
                    allocation[customerNum][i] = 0;
                    need[customerNum][i] = maximum[customerNum][i];
                }
            }
            return 0;
        }
        else
        {
            for (int i = 0; i < m; i++)
            {
                avaliable[i] += request[i];
                allocation[customerNum][i] -= request[i];
                need[customerNum][i] += request[i];
            }
        }
    }

    return -1;
}

void ReleaseResources(int customerNum, int release[])
{
    for (int i = 0; i < m; i++)
    {
        if (allocation[customerNum][i] < release[i])
        {
            printf("This release request is invaild! denied.\n");
        }
        return;
    }

    for (int i = 0; i < m; i++)
    {
        avaliable[i] += release[i];
        allocation[customerNum][i] -= release[i];
        need[customerNum][i] += release[i];
    }
}
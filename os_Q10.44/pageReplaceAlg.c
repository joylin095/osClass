#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGESIZE 12
void PrintPage(int page[], int frameSize);

int main()
{
    srand(time(NULL));
    int page[PAGESIZE];
    int frameSize;
    for (int i = 0; i < PAGESIZE; i++)
    {
        page[i] = rand() % 10;
    }
    PrintPage(page, PAGESIZE);

    printf("please enter page frame 1 to 7: ");
    scanf("%d", &frameSize);
    if (frameSize < 1 || frameSize > 7)
    {
        printf("page frame 1 to 7\n");
        return 1;
    }

    int frameFIFO[frameSize];

    //========FIFO==========
    for (int i = 0; i < frameSize; i++)
    {
        frameFIFO[i] = -1;
    }
    int FIFOfaults = 0;
    int FIFOorder = 0;
    for (int i = 0; i < PAGESIZE; i++)
    {
        if (i < frameSize)
        {
            frameFIFO[FIFOorder] = page[i];
            FIFOorder += 1;
            FIFOfaults += 1;
        }
        for (int j = 0; j < frameSize; j++)
        {
            if (page[i] == frameFIFO[j])
            {
                break;
            }
            if (j == (frameSize - 1))
            {
                FIFOfaults += 1;
                frameFIFO[FIFOorder] = page[i];
                FIFOorder += 1;
            }
        }
        PrintPage(frameFIFO, frameSize);
        if (FIFOorder == frameSize)
        {
            FIFOorder = 0;
        }
    }
    printf("FIFO, number of page faults: %d\n", FIFOfaults);
    printf("==========\n");

    //========LRU==========
    int frameLRU[frameSize];
    int frameTime[frameSize];
    int LRUindex = 0, LRUtime = 0;
    int LRUfaults = 0;
    for (int i = 0; i < frameSize; i++)
    {
        frameLRU[i] = -1;
        frameTime[i] = -1;
    }

    for (int i = 0; i < PAGESIZE; i++)
    {
        if (i < frameSize)
        {
            frameLRU[i] = page[i];
            frameTime[i] = i;
            LRUfaults += 1;
            PrintPage(frameLRU, frameSize);
            continue;
        }
        for (int j = 0; j < frameSize; j++)
        {
            if (page[i] == frameLRU[j])
            {
                frameTime[j] = i;
                break;
            }
            if (j == (frameSize - 1))
            {
                frameLRU[LRUindex] = page[i];
                frameTime[LRUindex] = i;
                LRUfaults += 1;
            }
        }

        LRUtime = 99999999;
        for (int j = 0; j < frameSize; j++)
        {
            if (LRUtime > frameTime[j])
            {
                LRUtime = frameTime[j];
                LRUindex = j;
            }
        }

        PrintPage(frameLRU, frameSize);
    }

    printf("LRU, number of page faults: %d\n", LRUfaults);
    printf("==========\n");

    //========optimal==========
    int frameOPT[frameSize];
    int LongTimeUsedindex = 0, recordTime = 0, LongTimeUsedTime = -1;
    int OPTfaults = 0;
    for (int i = 0; i < frameSize; i++)
    {
        frameOPT[i] = -1;
        frameTime[i] = -1;
    }

    for (int i = 0; i < PAGESIZE; i++)
    {
        if (i < frameSize)
        {
            frameOPT[i] = page[i];
            OPTfaults += 1;
        }
        for (int j = 0; j < frameSize; j++)
        {
            if (page[i] == frameOPT[j])
            {
                break;
            }
            if (j == (frameSize - 1))
            {
                frameOPT[LongTimeUsedindex] = page[i];
                OPTfaults += 1;
            }
        }

        for (int j = 0; j < frameSize; j++)
        {
            frameTime[j] = -1;
        }
        recordTime = 0;
        for (int j = i + 1; j < PAGESIZE; j++)
        {
            if (recordTime == frameSize)
            {
                break;
            }
            for (int k = 0; k < frameSize; k++)
            {
                if (page[j] == frameOPT[k])
                {
                    if (frameTime[k] == -1)
                    {
                        frameTime[k] = j;
                        recordTime += 1;
                        continue;
                    }
                }
            }
        }
        if (recordTime != frameSize)
        {
            for (int j = 0; j < frameSize; j++)
            {
                if (frameTime[j] == -1)
                {
                    LongTimeUsedindex = j;
                    break;
                }
            }
        }
        else
        {
            LongTimeUsedTime = -1;
            for (int j = 0; j < frameSize; j++)
            {
                if (frameTime[j] > LongTimeUsedTime)
                {
                    LongTimeUsedTime = frameTime[j];
                    LongTimeUsedindex = j;
                }
            }
        }

        PrintPage(frameOPT, frameSize);
    }

    printf("OPT, number of page faults: %d\n", OPTfaults);
    printf("==========\n");
}

void PrintPage(int page[], int frameSize)
{
    for (int i = 0; i < frameSize; i++)
    {
        printf("%d ", page[i]);
    }
    printf("\n");
}
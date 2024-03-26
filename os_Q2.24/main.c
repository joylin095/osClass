#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int OpenSourceFile(const char *fileName);
int OpenDestFile(const char *fileName);
void CopyFile(int sourceFile, int destFile);

int main()
{
    char sourceFileName[256];
    char destFileName[256];
    int sourceFile, destFile;

    printf("Enter the name of the source file: ");
    scanf("%s", sourceFileName);

    sourceFile = OpenSourceFile(sourceFileName);
    if (sourceFile == -1)
    {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    printf("Enter the name of the destination file: ");
    scanf("%s", destFileName);

    destFile = OpenDestFile(destFileName);
    if (destFile == -1)
    {
        perror("Error opening destination file");
        exit(EXIT_FAILURE);
    }

    CopyFile(sourceFile, destFile);

    close(sourceFile);
    close(destFile);
}

int OpenSourceFile(const char *fileName)
{
    return open(fileName, O_RDONLY);
}

int OpenDestFile(const char *fileName)
{
    return open(fileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
}

void CopyFile(int sourceFile, int destFile)
{
    ssize_t byteRead, byteWrite;
    char buffer[BUFFER_SIZE], test[BUFFER_SIZE];

    while ((byteRead = read(sourceFile, buffer, BUFFER_SIZE)) > 0)
    {
        byteWrite = write(destFile, buffer, byteRead);

        if (byteRead != byteWrite)
        {
            perror("Error writing to destination file");
            close(sourceFile);
            close(destFile);
            exit(EXIT_FAILURE);
        }

        if (byteRead == -1)
        {
            perror("Error reading from source file");
            close(sourceFile);
            close(destFile);
            exit(EXIT_FAILURE);
        }
    }
}
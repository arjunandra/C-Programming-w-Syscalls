#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFValue 1048576

char bufferString[BUFValue + 100];
char reverseBufferString[BUFValue + 100];

void main(int argc, char *argv[])
{
    long long int file1, file2;
    long long fileSize, count = 0;

    char* fileName = argv[1];
    // printf("The file name is %s\n", fileName);

    // Creating destination path
    char destination[] = "./Assignment/";
    strcat(destination, fileName);

    file1 = open(fileName, O_RDONLY);
    // Error handling for opening file
    perror("Opening Input File");

    // Making directory
    mkdir("Assignment", 0700);
    // Error handling for opening files
    perror("Creating New Directory");

    file2 = open(destination, O_WRONLY | O_CREAT, 0600);
    // Error handling for opening file
    perror("Opening Output File");

    long long int stringOffset = lseek(file1, -BUFValue, SEEK_END), counter = 0;
    // printf("offset got");
    fileSize = lseek(file1, 0, SEEK_END);
    // printf("%lld", fileSize);

    long long int i = fileSize;
    while (i > BUFValue) {
        i = lseek(file1, i - BUFValue, SEEK_SET);
        read(file1, bufferString, BUFValue);

        for (long long int j = 0; j < BUFValue; j++) {
            reverseBufferString[j] = bufferString[BUFValue - j - 1];
        }

        write(file2, reverseBufferString, BUFValue);

         // Calculating Progress Value
        float progressValue = (float) ((fileSize - i) * 100) / fileSize;
    
        // Printing out value to progress bar
        char outputString[100];
        sprintf(outputString, sizeof(outputString), "%.2f%%\r", progressValue);
        write(1, outputString, sizeof(outputString));
    } 

    // Resetting Buffer To Start
    lseek(file1, 0, SEEK_SET);  
    read(file1, bufferString, i);

    for(long long int j = 0; j < i; j++){
        reverseBufferString[j] = bufferString[i - j - 1];
    }

    // Printing out end result
    write(file2, reverseBufferString, i);
    write(1, "\r100%\n", sizeof("\r100%\n"));

    close (file1);
    close (file1);
    exit(EXIT_SUCCESS);

    return;
}


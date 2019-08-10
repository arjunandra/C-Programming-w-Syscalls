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

void permissionsPrinting (struct stat buffer, char* fileType){

    write(1, "\n", sizeof("\n"));

    write(1, "User has read permissions on ", sizeof("User has read permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IRUSR) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "User has write permissions on ", sizeof("User has write permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IWUSR) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "User has execute permissions on ", sizeof("User has execute permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IXUSR) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "\n", sizeof("\n"));

    write(1, "Group has read permissions on ", sizeof("Group has read permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IXUSR) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "Group has write permissions on ", sizeof("Group has write permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IWGRP) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "Group has execute permissions on ", sizeof("Group has execute permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IXGRP) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "\n", sizeof("\n"));

    write(1, "Other has read permissions on ", sizeof("Other has read permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IROTH) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "Other has write permissions on ", sizeof("Other has write permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IWOTH) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "Other has execute permissions on ", sizeof("Other has execute permissions on ")); 
    write(1, fileType, sizeof(fileType));
    write(1, ": ", sizeof(": "));

    if (buffer.st_mode & S_IXOTH) {
        write(1, "Yes\n", sizeof("Yes\n"));
    }

    else {
        write(1, "No\n", sizeof("No\n"));
    }

    write(1, "\n", sizeof("\n"));

    return;

}


int main(int argc, char *argv[]){

    struct stat inputFileBuffer, outputFileBuffer, directoryBuffer;

    stat(argv[1], &inputFileBuffer);
    stat(argv[2], &outputFileBuffer);
    stat(argv[3], &directoryBuffer);
    if(S_ISREG(inputFileBuffer.st_mode) && S_ISREG(outputFileBuffer.st_mode) && S_ISDIR(directoryBuffer.st_mode)){

        write(1, "Directory is created: ", sizeof("Directory is created: "));
        
        if (directoryBuffer.st_mode & S_IRUSR && directoryBuffer.st_mode & S_IWUSR && directoryBuffer.st_mode & S_IXUSR) {
            write(1, "Yes\n", sizeof("Yes\n"));
        }

        else {
            write(1, "No\n", sizeof("No\n"));
        }

        int file1 = open(argv[1], O_RDONLY);
        // Error handling for opening file
        perror("Opening Input File");

        int file2 = open(argv[2], O_RDONLY);
        // Error handling for opening file
        perror("Opening Output File");

        // Calculating File Sizes
        long long int inputFileSize = lseek(file1, 0, SEEK_END);
        long long int outputFileSize = lseek(file2, 0, SEEK_END);

        int flag = 0;

        char filesNotReversed[100] = "Whether file contents are reversed in newfile: No\n";
        char filesReversed[100] = "Whether file contents are reversed in newfile: Yes\n";

        // Comparing file sizes
        if (inputFileSize != outputFileSize) {
            write(1, filesNotReversed, sizeof(filesNotReversed));
            exit(EXIT_FAILURE);
        }

        long long int inputOffset = inputFileSize;
        long long int outputOffset = lseek(file2, 0, SEEK_SET);
        
        while (inputOffset > BUFValue) {

            inputOffset = lseek(file1, inputOffset-BUFValue, SEEK_SET);

            read(file1, bufferString, BUFValue);
            read(file2, reverseBufferString, BUFValue);

            if (strcmp(bufferString, reverseBufferString) == 0) {
                flag = 1;
                break;
            }

            outputOffset = lseek(file2, outputOffset + BUFValue, SEEK_SET);
        }

        if (flag == 1) {
            write(1, filesNotReversed, sizeof(filesNotReversed));
        }

        else {

            // Resetting Buffer To Start
            lseek(file1, 0, SEEK_SET);

            // Reading Buffer and Reverse Buffer Strings
            read(file1, bufferString, inputOffset);
            read(file2, reverseBufferString, inputOffset);

            if (strcmp(bufferString, reverseBufferString) == 0) {
                write(1, filesNotReversed, sizeof(filesNotReversed));
            }
            else{
                write(1, filesReversed, sizeof(filesReversed));
            }

        }

        // Printing out all permissions features 
        permissionsPrinting(inputFileBuffer, "oldfile");
        permissionsPrinting(outputFileBuffer, "newfile");
        permissionsPrinting(directoryBuffer, "directory");

    }
    else{
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
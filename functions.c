/* 
    File: functions.c
    Description: function bodies
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//contains function declarations
#include "functions.h"

//global variables
//command line argument flags
extern int numWordsBool;
extern int outFileBool;
extern int infilePathBool;
//file paths
extern char *infilePath;
extern char *outFilePath;
//files
extern int infile;
extern int outfile;
//amount of words to print/append - default of 10 words
extern int wordsToPrint;

///This function opens a file in read only mode
//One input param of file which is the files name/absolute path
//Returns file descriptor
int openFileRead(char *filePath) {
    int fd;
    char *errorMsg = "Error: source file can't be opened\n";
    
    if ((fd = open(filePath, O_RDONLY)) < 0) {
        writeStdErr(errorMsg, strlen(errorMsg));
        exit(1);
    }   
    
    return fd;
}

//This function opens a file in write only mode for appending, if file does not exist it will be created
//One input param of file which is the files name/absolute path
//Returns file descriptor
int openFileWriteAppend(char *filePath) {
    int fd;
    char *errorMsg = "Error: destination file can't be opened\n";

    if ((fd = open(filePath, O_WRONLY | O_APPEND | O_CREAT, 0664)) < 0) {
        writeStdErr(errorMsg, strlen(errorMsg));
        close(infile);
        exit(1);
    }

    return fd;  
}

//This function seeks to the start of a file
//One input param of fd whch is a file descriptor
//Returns the offset location in file
int fileSeekStart(int fd) {
    int bytesRead;
    
    bytesRead = lseek(fd, 0, SEEK_SET);

    return bytesRead;
}

//This function seeks to the end of a file
//One input param of fd which is a file descriptor
//Retuns the offset location in file
int fileSeekEnd(int fd) {
    int bytesRead;

    bytesRead = lseek(fd, 0, SEEK_END);

    return bytesRead;
}

//This function writes to standard output
//Two input params of buffer, content to be written, and size, amount of bytes to be written
//Returns the amount of bytes written to standard output
int writeStdOut(char *buffer, int size) {
    int bytesWritten;

    bytesWritten = write(1, buffer, size);

    return bytesWritten;
}

//This function writes to standard error output
//Two input params of buffer, content to be written, and size, amount of bytes to be written
//Returns the amount of bytes written to standard error output
int writeStdErr(char *buffer, int size) {
    int bytesWritten;
    
    bytesWritten = write(2, buffer, size);

    return bytesWritten;
}

//This function checks if a given string is an abosolute path
//One input param of string which contains a string literal
//Returns an integer of either 1 or 0, 1 meaning true, 0 meaning false
int isAbsolutePath(char *string) {
    int isAbsolutePath = 0;
    
    //compares the first character of the input string to "/" if it is not equal it is not an absolute path
    if (strncmp(string, "/", 1) == 0) {
        isAbsolutePath = 1;
    }

    return isAbsolutePath;
}

//This function checks if a command line argument is a number
//Two input params of argIndex, which is an index of the argument, and argv, which contains the command line arguments
//Returns an integer of either 1 or 0, 1 meaning truem 0 meaning false
int isNum(int argIndex, char *argv[]) {
    int intBool = 1; //represents if argv[i] is a number, set to true by deafault

    //for loop iterates through each char in the string store in the argv[] index provided, isdigit() function checks if the
    //char is a number, if it isnt intBool is set to 0, thus the argument is not a number
    for (int i = 0, n = strlen(argv[argIndex]); i < n; i++) {
        if (!isdigit(argv[argIndex][i])) {
            intBool = 0;
        }
    }
    
    return intBool;
}

//This function processes command line arguments and identifies which are present, which are not, and if there are too many or invalid arguments
//Two input params of argc, argv and index, which is the position in argv processing should begin at
//No return value
void processCommandLineArgs(int argc, char *argv[], int index) {
    for (int i = index; i < argc; i++) {
        //Only if the string in argv[i] is equal to -a, there is space for another argument after -a, the string in the next position argb[i+1] is an absolute path
        //and that the previous argument argv[i - 1] is not equal to -a will the argument be accepted.
        if (strcmp(argv[i], "-a") == 0 && i + 1 <= argc - 1 && isAbsolutePath(argv[i + 1]) == 1 && outFileBool != 1) {
            outFilePath = argv[i + 1];
            outFileBool = 1;
            i++;
        //Only if the string in argv[i] is equal to -n, there is space for another argument after -n the string in the next position argb[i+1] is a number and that 
        //the previous argument argv[i - 1] is not equal to -n will the argument be accepted.
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 <= argc - 1 && numWordsBool != 1 && isNum(i + 1, argv) == 1) {
            numWordsBool = 1;
            i++;
            wordsToPrint = atoi(argv[i]);
        //If the arguments don't match any of the above if statements then we know that the argument is invalid and program will be exited       
        } else {
            char *errorMsg = "Error: invalid arguments\nUsage: ./fileutil [input file path] [-a output file path] [-n no. words] (When using [input file path] with other arguments, it must be the first argument)\n";
            writeStdErr(errorMsg, strlen(errorMsg));
            exit(1);
        }
    }
}
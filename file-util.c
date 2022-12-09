/*  
    File: fileutil.c
    Name: Yash Joshi
    Start date: 18/08/2022
    Last modified: 26/08/2022
    Program Description: This program, by default, reads and outputs the first 10 words of a file called sample.txt in the current directory. 
    There are three optional command line arguments that can be used to alter the operation of the program. To read words from a specific file
    provide the absolute path of the file as the first command line argument when executing the program, if provided with other arguments this
    must alawys be the first argument. To change the amount of words read and outputted from a file use the -n flag followed by the number of words.
    Instead of outputting 10 or n words to the command line an output file can be specified using the -a flag followed by the files absolute path.
    Either 10 or n words will then be appended to the end of the file, If the file does not exist it will be created.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//Contains function declarations
#include "functions.h"

//Global variables
//Command line argument flags
int numWordsBool = 0;
int outFileBool = 0;
int infilePathBool = 0;
//File paths
char *infilePath;
char *outFilePath;
//Files
int infile;
int outfile;
//Amount of words to print/append - default of 10 words
int wordsToPrint = 10;

int main(int argc, char *argv[]) {
    int infileSize;
    int totalWords = 0;
    int wordCount = 0;
    int endWordIndex;

    //if command line arguments are provided then enter and start processing
    if (argc > 1) {
        //if 7 or more command line arguments are provided then exit program with error
        if (argc < 7) {
            //if less than 7 arguments then identify if input file path has been provided
            //if provided then set corresponding flag to 1 and set infilepath variable to path and process arguments from position 2
            //if not provided then start processing arguments from position 1
            if (isAbsolutePath(argv[1]) == 1) {
                infilePathBool = 1;
                infilePath = argv[1];
                processCommandLineArgs(argc, argv, 2);
            } else {
                processCommandLineArgs(argc, argv, 1);
            } 
        } else {
            char *errorMsg = "Error: maximum number of arguments exceeded\n";
            //writes error message to standard error output and exits program cleanly with status of 1
            writeStdErr(errorMsg, strlen(errorMsg));
            exit (1);
        }
    }
    
    //if input file path provided then open file for read only
    //if input ile is not provided then open default file of sample.txt in current directory
    if (infilePathBool == 1) {
        infile = openFileRead(infilePath);
    } else {
        infile = openFileRead("sample.txt");
    }

    //Seek to end of input file to get size then reset seek position to start of file
    infileSize = fileSeekEnd(infile) + 1;
    fileSeekStart(infile);

    //if output file flag and path are provided indicated by the outFileBool variable set to 1 then open the file for writing in append mode
    if (outFileBool == 1) {
        outfile = openFileWriteAppend(outFilePath);
    }

    //initalise array to hold file contents with file size read from line 162
    char fileCont[infileSize];

    //reads input file content into array and adds null terminator
    read(infile, &fileCont, infileSize);
    fileCont[infileSize - 1] = '\0';

    //iterates through array to identify the amount of words stored in the file, number of words stored in variable totalWords
    for (int i = 0; i < infileSize; i++) {
        //variable isDelim tells us if the previous character read was a delimeter, 1 if true 0 if false
        int isDelim;
        //checks if character i in array fileCont is equal to word delimeter, which can be a space, null terminator, tab or new line if not a delimeter then set isDelim to 0 and read next character
        if (fileCont[i] == ' ' || fileCont[i] == '\0' || fileCont[i] == '\t' || fileCont[i] == '\n') {
            //if character is equal to a delimeter then first check if previous character read was delimeter by checking variable isDelim
            if (isDelim == 0) {
                //if isDelim is false then increment totalWords variable and set isDelim variable to 1 as current character is a delimeter
                totalWords++;
                isDelim = 1;
            }
        } else {
            isDelim = 0;
        }
    }

    //once we know how many words there are in the file we can print them to output or append them to the output file 
    //by default the global variable wordsToPrint is 10, if the user specified the amount of words to print, such as 50, this wordsToPrint is updated
    //first check if the total words in the file is less than the amount we need to print or append, if yes then simply write all the contents of the 
    //array fileCont to standard output or to the output file
    if (totalWords < wordsToPrint) {
        //if output file provided then append all array contents to end of file and close output file
        // if no output file then write all array contents to standard output
        if (outFileBool == 1) {
            write(outfile, &fileCont, infileSize);
            close(outfile);
            char *successMsg = "Append successful\n";
            writeStdOut(successMsg, strlen(successMsg));
        } else {
            writeStdOut(fileCont, infileSize);
        }
    } else {
        //if total words exceeds amount we need to print then we identift the index of the last word that needs to be printed/appended
        //iterates through array till the nth word and saves its index in endWordIndex
        int i = 0;
        while (wordCount < wordsToPrint) {
            //variable isDelim tells us if the previous character read was a delimeter, 1 if true 0 if false
            int isDelim;
            //checks if character i in array fileCont is equal to word delimeter, which can be a space, null terminator, 
            //tab or new line if not a delimeter then set isDelim to 0 and read next character
            if (fileCont[i] == ' ' || fileCont[i] == '\0' || fileCont[i] == '\t' || fileCont[i] == '\n') {
                //if character is equal to a delimeter then first check if previous character read was delimeter by checking variable isDelim
                if (isDelim == 0) {
                    //if isDelim is false then increment wordCount variable and set isDelim variable to 1 as current character is a delimeter
                    wordCount++;
                    endWordIndex = i;
                    isDelim = 1;
                }                
            } else {
                isDelim = 0;
            }
            i++;
        }
        //Once the index of the last word is found either write n words to standard output or if output file is provided at runtime append n words to that file
        if (outFileBool == 1) {
            write(outfile, &fileCont, endWordIndex);
            close(outfile);
            char *successMsg = "Append successful\n";
            writeStdOut(successMsg, strlen(successMsg));
        } else {
            writeStdOut(fileCont, endWordIndex);
            writeStdOut("\n", 1);
        }
    }
    
    //once all operations have completed, close input file and exit program with status of 0
    close(infile);
    exit(0);
}

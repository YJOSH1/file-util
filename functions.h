/* 
    File: functions.h
    Description: function declarations
*/

//This function opens a file in read only mode
//One input param of file which is the files name/absolute path
//Returns file descriptor
int openFileRead(char *filePath);

//This function opens a file in write only mode for appending, if file does not exist it will be created
//One input param of file which is the files name/absolute path
//Returns file descriptor
int openFileWriteAppend(char *filePath);

//This function seeks to the start of a file
//One input param of fd whch is a file descriptor
//Returns the offset location in file
int fileSeekStart(int fd);

//This function seeks to the end of a file
//One input param of fd which is a file descriptor
//Retuns the offset location in file
int fileSeekEnd(int fd);

//This function writes to standard output
//Two input params of buffer, content to be written, and size, amount of bytes to be written
//Returns the amount of bytes written to standard output
int writeStdOut(char *buffer, int size);

//This function writes to standard error output
//Two input params of buffer, content to be written, and size, amount of bytes to be written
//Returns the amount of bytes written to standard error output
int writeStdErr(char *buffer, int size);

//This function checks if a given string is an abosolute path
//One input param of string which contains a string literal
//Returns an integer of either 1 or 0, 1 meaning true, 0 meaning false
int isAbsolutePath(char *string);

//This function checks if a command line argument is a number
//Two input params of argIndex, which is an index of the argument, and argv, which contains the command line arguments
//Returns an integer of either 1 or 0, 1 meaning truem 0 meaning false
int isNum(int argIndex, char *argv[]);

//This function processes command line arguments and identifies which are present, which are not, and if there are too many or invalid arguments
//Two input params of argc, argv and index, which is the position in argv processing should begin at
//No return value
void processCommandLineArgs(int argc, char *argv[], int index);







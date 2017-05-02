#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

//160*3=480
#define MAXSIZE 480
int main(int argc, char *argv[]) {
    char buffer[MAXSIZE], *mainFolder, *inputFile, *outputFile;
    DIR *currentFolder;
    char error[] = "Error! System call failed.";
    const char s[2] = "\n";
    int readSize;
    int fd = open(argv[1], O_RDONLY, S_IREAD);
    if(fd == -1){
        write(2,error,strlen(error));
        return -1;
    }
    //receive and save the information
    readSize = read(fd, buffer, MAXSIZE);
    if(readSize<0){
        write(2,error,strlen(error));
        return -1;
    }
    mainFolder = strtok(buffer,s);
    inputFile = strtok(NULL,s);
    outputFile = strtok(NULL,s);

    currentFolder = opendir(mainFolder);
    while(currentFolder){
        //do something
        //return to main folder
        currentFolder = opendir(mainFolder);
    }
}

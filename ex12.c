#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

//160*3=480
#define MAXSIZE 480

int main(int argc, char *argv[]) {
    struct stat stat_p;
    char buffer[MAXSIZE], *mainFolder, *inputFile, *outputFile;
    DIR *currentFolder;
    char strCurrFolder[MAXSIZE];
    char error[] = "Error! System call failed.";
    const char n[2] = "\n";
    const char s[2] = "/";
    char* execArgs[3] = {};
    int readSize;
    int fdCsv = open("results.csv", O_WRONLY | O_CREAT);
    int fd = open(argv[1], O_RDONLY, S_IREAD);
    int fdInput,fdOutput,fdExecOutput;
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
    mainFolder = strtok(buffer,n);
    inputFile = strtok(NULL,n);
    outputFile = strtok(NULL,n);
    fdInput = open(inputFile, O_RDONLY, S_IREAD);
    fdOutput = open(outputFile, O_WRONLY, S_IWRITE);
    //open main folder
    currentFolder = opendir(mainFolder);
    struct dirent* folder = readdir(currentFolder);
    //while there are no folders in the main folder
    while(folder){
        //save folder name = student name
        write(fdCsv,folder->d_name,sizeof(folder->d_name));
        strcpy(strCurrFolder,mainFolder);
        strcat(strCurrFolder,"/");
        strcat(strCurrFolder,folder->d_name);
        //open student folder
        currentFolder = opendir(strCurrFolder);
        //if not found - look inside folder
        //if no folders - no file
        //find c file
        //run the c file
        dup2(fdInput,0);
        dup2(fdExecOutput,1);
        execArgs[0] = "./a.out";
        execArgs[1] = NULL;
        execArgs[2] = NULL;
        execvp(strCurrFolder,execArgs);
        //compare to output file
        int pid =fork();
        if(pid==0){

        }
        else{
            pid = waitpid(pid,&status,0);
        }
        //calculate score
        //write score

        //open the next folder
        folder = readdir(currentFolder);
    }
}
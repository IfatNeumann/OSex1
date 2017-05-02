#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define SIZE 10
int checkIdentical(int fd1, int fd2,int* read1, int* read2, char* buffer1,char* buffer2, int* index);
int compareBuffers(char* buffer1,char* buffer2, int length);
int checkSimilar(int fd1, int fd2,int* read1, int* read2, char* buffer1,char* buffer2, int* index);
int checkIfSpaces(int* readSize,int fd, char* buffer, int index);

int main(int argc, char *argv[]) {
    //1-identical,2-similar,3-different
    char buffer1[SIZE], buffer2[SIZE];
    char error[] = "Error!,System call failed.";
    int read1,read2,index, result;
    int fd1 = open(argv[1], O_RDONLY, S_IREAD);
    int fd2 = open(argv[2], O_RDONLY, S_IREAD);
    if (fd1 == -1||fd2 == -1) {
        // handle case where couldn't open file.
        if(fd1 == -1)
            close(fd2);
        if(fd2 == -1)
            close(fd1);
        write(2,error,strlen(error));
        return -1;
    }
    //check if identical
    result = checkIdentical(fd1,fd2,&read1,&read2,buffer1,buffer2,&index);
    if(result==1){
        //close
        close(fd1);
        close(fd2);
        return 1;
    }
    else if(result==-1){
        //close
        close(fd1);
        close(fd2);
        // handle case where couldn't read file.
        write(2,error,strlen(error));
        return -1;
    }
    /* at this point they are similar/different (not equal).
     * we have an index of the first different character */
    result = checkSimilar(fd1,fd2,&read1,&read2,buffer1,buffer2,&index);

    //close
    close(fd1);
    close(fd2);

    if(result==1) {
        return 2;
    }
    if(result==-1){
        // handle case where couldn't read file.
        write(2,error,strlen(error));
        return -1;
    }
    //they are different
    return 3;
}

//return 1 if identical, 0 if not, -1 if failed reading
int checkIdentical(int fd1, int fd2,int* read1, int* read2, char* buffer1,char* buffer2, int* index){
    *read1 = read(fd1, buffer1, SIZE);
    *read2 = read(fd2, buffer2, SIZE);
    do {
        //if one of the reads failed
        if (*read1 < 0 || *read2 < 0) {
            return -1;
        }
        //not identical
        if (*read1!=*read2){
            //initialize *index
            *index=0;
            return 0;
        }
        *index = compareBuffers(buffer1,buffer2,*read1);
        //if they are not identical
        if (*index != -1)
            return *index;
        // if not identical:
        *read1 = read(fd1, buffer1, SIZE);
        *read2 = read(fd2, buffer2, SIZE);
        //until one of them reach the end of file
    }while (*read1!=0&&*read2!=0);
    //if both of them reached the end of file
    if(*read1==*read2)
        return 1;
    //not in the same length
    return 0;
}

//return -1 if equal, index of first different bite if not
int compareBuffers(char* buffer1,char* buffer2, int length){
    for (int i=0;i<length;++i){
        if(buffer1[i]!=buffer2[i])
            return i;
    }
    return -1;
}

//return 1 if similar, 0 if not, -1 if failed reading
int checkSimilar(int fd1, int fd2,int* read1, int* read2, char* buffer1,char* buffer2, int* index){
    int index1=*index, index2=*index;
    //if we reached the end of the buffer
    if(*index==SIZE){
        *read1 = read(fd1, buffer1, SIZE);
        *read2 = read(fd2, buffer2, SIZE);
    }
    do {
        //if one of the reads failed
        if (*read1 < 0 || *read2 < 0) {
            return -1;
        }
        while(index1<*read1 && index2<*read2){
            while((buffer1[index1] == ' ' || buffer1[index1] == '\n')&&(index1<*read1))
                index1++;
            buffer1[index1] = tolower(buffer1[index1]);
            while((buffer2[index2] == ' ' || buffer2[index2] == '\n')&&(index2<*read2))
                index2++;
            buffer2[index2] = tolower(buffer2[index2]);
            if(buffer1[index1]!=buffer2[index2])
                return 0;
            index1++;
            index2++;
        }
        // if they are identical so far - continue reading (if we reached the end of the buffer)
        if(index1==*read1){
            *read1 = read(fd1, buffer1, SIZE);
            index1=0;
        }
        if(index2==*read2){
            *read2 = read(fd2, buffer2, SIZE);
            index2=0;
        }
        //until one of them reach the end of file (one of them equals 0)
    }while (*read1!=0&&*read2!=0);
    if(*read1==0){
        return (checkIfSpaces(read2,fd2,buffer2,index2));
    }
    else{
        return (checkIfSpaces(read1,fd1,buffer1,index1));
    }
}

//return 0 if found a char different then ' ' or '\n', 1 if not, -1 if read failed
int checkIfSpaces(int* readSize,int fd, char* buffer, int index){
    while(*readSize!=0) {
        while ((buffer[index] == ' ' || buffer[index] == '\n') && (index < *readSize))
            index++;
        //reached the end of the buffer
        if (index == *readSize) {
            *readSize = read(fd, buffer, SIZE);
            if(*readSize<0){
                return -1;
            }
            index = 0;
        }
        else
            return 0;
    }
    return 1;
}
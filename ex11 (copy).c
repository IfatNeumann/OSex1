#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>

#define SIZE 100
int main1(int argc, char *argv[]) {
    char buffer1[SIZE+1], buffer2[SIZE+1];
    char fixedBuffer1[SIZE+1], fixedBuffer2[SIZE+1],fixedCompare[SIZE+1];
    //buffers' counters
    int i1=0,i2=0;
    //fixedBuffers' counters
    int j1 = 0, j2 = 0;
    int result,ret=0;
    //1-identical,2-similar,3-different
    int read1=1, read2=1;
    bool identical = false,diff = false,doneRead1=true,doneRead2=true;
    int fd1 = open(argv[1], O_RDONLY, S_IREAD);
    if (fd1 == -1) {
        /* Handle case where couldn't open file. */
    }
    int fd2 = open(argv[2], O_RDONLY, S_IREAD);
    if (fd2 == -1) {
        /* Handle case where couldn't open file. */
    }
    //until the end of the files
    do{
        //read
        //if there is more to read in the first file
        //+ we're done transferring the last read
        if(read1!=0&&doneRead1){
            read1 = read(fd1, buffer1, SIZE);
        }
        //if there is more to read in the second file
        //+ we're done transferring the last read
        if(read2!=0&&doneRead2){
            read2 = read(fd2, buffer2, SIZE);
        }
        //if one of the reads failed
        if (read1 < 0 || read2 < 0) {
            /* Handle case where couldn't read file. */
        }
        //end the reading for the strcmp function
        buffer1[read1] = '\0';
        buffer2[read2] = '\0';
        //check if they are identical - works only for the first round
        result = strcmp(buffer1, buffer2);
        if (result == 0) {
            //they are not identical
            identical=true;
        }
        if(result==0&&read1<SIZE&&identical){
            break;
        }
        //fix
        for (; i1 < read1||strlen(fixedBuffer1)<SIZE; i1++) {
            if (buffer1[i1] != ' ' && buffer1[i1] != '\n') {
                fixedBuffer1[j1] = tolower(buffer1[i1]);
                j1++;
            }
        }
        for (; i2 < read2||strlen(fixedBuffer2)<SIZE; i2++) {
            if (buffer2[i2] != ' ' && buffer2[i2] != '\n') {
                fixedBuffer2[j2] = tolower(buffer2[i2]);
                j2++;
            }
        }
        fixedBuffer1[j1] = '\0';
        fixedBuffer2[j2] = '\0';

        // compare after every read so we won't work
        // after we know they are not equal
        if(strlen(fixedBuffer1)<strlen(buffer2)){
            strcpy(fixedCompare, fixedBuffer2);
            fixedCompare[strlen(fixedBuffer1)+1] = '\0';
            result = strcmp(fixedBuffer1, fixedCompare);
        }
        else if(strlen(fixedBuffer1)>strlen(buffer2)){
            strcpy(fixedCompare, fixedBuffer1);
            fixedCompare[strlen(fixedBuffer2)+1] = '\0';
            result = strcmp(fixedCompare, fixedBuffer2);
        }
        else{
            result = strcmp(fixedBuffer1, fixedBuffer2);
        }
        //they are different - we can stop reading
        if (result != 0) {
            diff=true;
            break;
        }

        //1
        //we didn't finish transferring from buffer1 to fixedBuffer1
        if(strlen(fixedBuffer1)>=SIZE&&i1 <= read1) {
            doneRead1 = false;
        }
        //done reading from buffer2 and fixedBuffer2 is not full
        else if(strlen(fixedBuffer1)<SIZE&&i1 == read1){
            doneRead1=true;
            i1=0;
        }
        //done reading from buffer2 and fixedBuffer2 is full
        else{
            doneRead1=true;
            i1=0;
            j1=0;
        }

        //2
        //we didn't finish transferring from buffer2 to fixedBuffer2
        if(strlen(fixedBuffer2)>=SIZE&&i2 <= read2){
            doneRead2=false;
        }
        //done reading from buffer2 and fixedBuffer2 is not full
        else if(strlen(fixedBuffer2)<SIZE&&i2 == read2){
            doneRead2=true;
            i2=0;
        }
            //done reading from buffer2 and fixedBuffer2 is full
        else{
            doneRead2=true;
            i2=0;
            j2=0;
        }
    }while(read1>0&&read2>0);

    //close
    close(fd1);
    close(fd2);

    if(diff)
        return 3;
    if(identical)
        return 1;
    //they are similar
    return 2;
}


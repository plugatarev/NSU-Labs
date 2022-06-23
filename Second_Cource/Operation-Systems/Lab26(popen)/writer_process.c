#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE_SIZE 256
#define READ_ERROR -1
#define ERROR 1
#define SUCCESS 0
#define WRITE_ERROR -1
#define TRUE 1
#define EMPTY 0
int main(){
    char message[MESSAGE_SIZE];
    while (TRUE){
        int read_sesult = read(STDIN_FILENO, message, MESSAGE_SIZE);
        if (read_sesult == READ_ERROR){
            perror("read");
            return ERROR;
        }
        int write_result = write(STDOUT_FILENO, message, read_sesult);
        if (write_result == WRITE_ERROR){
            perror("writing to pipe");
            return ERROR;
        }
        if (read_sesult == EMPTY) break;
    }
    return SUCCESS;
}
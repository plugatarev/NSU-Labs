#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

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
        int message_length = read(STDIN_FILENO, message, MESSAGE_SIZE);
        if (message_length == READ_ERROR){
            perror("read");
            return ERROR;
        }
        if (message_length == EMPTY) break;
        for (int i = 0; i < message_length; ++i){
            message[i] = toupper(message[i]);
        }
        int write_result = write(STDOUT_FILENO, message, message_length);
        if (write_result == WRITE_ERROR){
            perror("writing upper message");
            return ERROR;
        }
    }
    return SUCCESS;
}
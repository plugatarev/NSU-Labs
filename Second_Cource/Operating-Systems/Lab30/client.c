#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define DEFAULT 0
#define TRUE 1
#define SOCKET_ERROR -1
#define CLOSE_ERROR -1
#define CONNECT_ERROR -1
#define READ_ERROR -1
#define END_OF_FILE 0
#define ERROR 1
#define SUCCESS 0
#define MESSAGE_LENGTH 256
#define SOCKET_NAME "Server_socket"

void catchsignal(int sig){}

int write_message(int socket_descriptor){
    char message[MESSAGE_LENGTH];
    while(TRUE){
        int read_result = read(STDIN_FILENO, message, MESSAGE_LENGTH);
        if (read_result == READ_ERROR){
            perror("read");
            return ERROR;
        }
        if (read_result == END_OF_FILE) break;
        int write_result = write(socket_descriptor, message, read_result);
        if (write_result == SOCKET_ERROR){
            perror("write to socket");
            return ERROR;
        }
    }
    return SUCCESS;
}

int close_socket(int socket_descriptor){
    int close_result = close(socket_descriptor);
    if (close_result == CLOSE_ERROR){
        perror("close");
        return ERROR;
    }
    return SUCCESS;
}

int main(){
    void* sigset_result = sigset(SIGPIPE, catchsignal);
    if (sigset_result == SIG_ERR){
        perror("working sigset");
        return ERROR;
    }
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    int socket_descriptor = socket(AF_UNIX, SOCK_STREAM, DEFAULT);
    if (socket_descriptor == SOCKET_ERROR) {
        perror("socket");
        return ERROR;
    }
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);
    int error = connect(socket_descriptor, (struct sockaddr*) &address, sizeof(address));
    if (error == CONNECT_ERROR) {
        perror("connect with server");
        close_socket(socket_descriptor);
        return ERROR;
    }
    error = write_message(socket_descriptor);
    if (error == ERROR){
        close_socket(socket_descriptor);
        return ERROR;
    }
    int close_result = close_socket(socket_descriptor);
    if (close_result == ERROR) return ERROR;
    return SUCCESS;
}
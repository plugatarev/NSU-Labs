#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define END_OF_FILE 0
#define DEFAULT 0
#define READ_ERROR -1
#define BIND_ERROR -1
#define ERROR 1
#define UNLINK_ERROR -1
#define QUEUE_SIZE 0
#define SUCCESS 0
#define MESSAGE_LENGTH 256
#define TRUE 1
#define WRITE_ERROR -1
#define CLOSE_ERROR -1
#define ACCEPT_ERROR -1
#define LISTEN_ERROR -1
#define SOCKET_ERROR -1
#define SOCKET_NAME "Server_socket"

int read_message(int client_socket_descriptor){
    char message[MESSAGE_LENGTH];
    while(TRUE){
        int read_result = read(client_socket_descriptor, message, MESSAGE_LENGTH);
        if (read_result == READ_ERROR){
            perror("read");
            return ERROR;
        }
        if (read_result == END_OF_FILE) break;
        for (int i = 0; i < read_result; i++){
            message[i] = toupper(message[i]);
        }
        int write_result = write(STDOUT_FILENO, message, read_result);
        if (write_result == WRITE_ERROR){
            perror("write to standard output");
            return ERROR;
        }
    }
    return SUCCESS;
}

int contact_the_client(int socket_descriptor, struct sockaddr_un* address){
    int bind_result = bind(socket_descriptor, (const struct sockaddr *) address, sizeof(*address));
    if (bind_result == BIND_ERROR) {
        perror("bind");
        return ERROR;
    }

    int listen_error = listen(socket_descriptor, QUEUE_SIZE);
    if (listen_error == LISTEN_ERROR){
        perror("listen");
        return ERROR;
    }

    int client_socket_descriptor = accept(socket_descriptor, NULL, NULL);
    if (client_socket_descriptor == ACCEPT_ERROR) {
        perror("accept");
        return ERROR;
    }
    return client_socket_descriptor;
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
    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));
    int socket_descriptor = socket(AF_UNIX, SOCK_STREAM, DEFAULT);
    if (socket_descriptor == SOCKET_ERROR) {
        perror("socket");
        return ERROR;
    }
    unlink(SOCKET_NAME);
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_NAME, sizeof(address.sun_path) - 1);
    int client_socket_descriptor = contact_the_client(socket_descriptor, &address);
    if (client_socket_descriptor == ERROR) return ERROR;
    int read_result = read_message(client_socket_descriptor);
    if (read_result == ERROR){
        close_socket(client_socket_descriptor);  
        close_socket(socket_descriptor);   
        return ERROR;
    }
    int close_result = close_socket(client_socket_descriptor);
    if (close_result == ERROR) return ERROR;
    int unlink_result = unlink(SOCKET_NAME);
    if (unlink_result == UNLINK_ERROR){
        perror("unlink");
        return ERROR;
    }
    return SUCCESS;
}

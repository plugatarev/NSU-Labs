#include "header.h"
#include "connection.h"

void freeBufferConnection(connection* conn) {
    if (conn->allSize > 0) {
        free(conn->buffer);
        conn->buffer = NULL;
        conn->allSize = 0;
        conn->handleSize = 0;
    }
}

void dropConnection(connection* conn) {
    freeBufferConnection(conn);

    int error_code = close(conn->clientSocket);
    if (error_code == ERROR) {
        perror("close client socket");
    }

    if (conn->serverSocket != EMPTY) {
        error_code = close(conn->serverSocket);
        if (error_code == ERROR) {
            perror("close server socket");
        }
    }
    conn->handleSize = 0;
    conn->handleCacheSize = 0;
    conn->cacheIndex = EMPTY;
    printf("dropped connection\n");
}

struct connection* initConnection(int clientSocket) {
    connection* conn = (struct connection*)malloc(sizeof(struct connection));
    if (conn == NULL) {
        perror("initConnection");
        return NULL;
    }
    conn->clientSocket = clientSocket;
    conn->serverSocket = EMPTY;
    conn->cacheIndex = EMPTY;
    conn->handleSize = 0;
    conn->buffer = NULL;
    conn->allSize = 0;
    conn->status = READ_FROM_CLIENT;
    conn->handleCacheSize = 0;
    return conn;
}
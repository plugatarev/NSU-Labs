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
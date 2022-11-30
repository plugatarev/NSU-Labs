#ifndef LAB32_CONNECTION_H
#define LAB32_CONNECTION_H

#include "stdio.h"

enum connectionStatus {
    READ_FROM_CLIENT,
    WRITE_TO_SERVER,
    READ_FROM_SERVER,
    READ_FROM_CACHE_WRITE_CLIENT,
    DROP
};

struct connection {
    int clientSocket;
    int serverSocket;

    enum connectionStatus status;
    char* buffer;
    size_t allSize;
    size_t handleSize;

    int cacheIndex;
    ssize_t handleCacheSize;
} typedef connection;

void freeBufferConnection(connection* conn);
void dropConnection(connection* conn);

#endif

#include "header.h"
#include "parser.h"
#include "network.h"
#include "pthreadSync.h"
#include "cache.h"
#include "connection.h"

void nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, O_NONBLOCK);
    if (flags == ERROR) {
        perror("fcntl");
        return;
    }
    flags |= O_NONBLOCK;
    int res = fcntl(fd, F_SETFL, flags);
    if (res == ERROR) {
        perror("fcntl");
    }
}

struct cacheInfo cache[MAX_CACHE_SIZE];

void writeToClient(connection* conn) {
    lockMutex(&cache[conn->cacheIndex].mutex);
    printf("WRITE TO CLIENT\n");
    if (cache[conn->cacheIndex].status == VALID || cache[conn->cacheIndex].status == DOWNLOADING) {
        char* message = cache[conn->cacheIndex].data + conn->handleSize;
        size_t size = cache[conn->cacheIndex].allSize - conn->handleSize;
        ssize_t writeCount = send(conn->clientSocket, message, size, NO_FLAGS);
        if (writeCount == ERROR) {
            perror("send to client");
            makeCacheInvalid(cache, conn->cacheIndex);
            conn->status = DROP;
            unlockMutex(&cache[conn->cacheIndex].mutex);
            return;
        }
        conn->handleSize += writeCount;
        if (writeCount == 0) {
            printf("SEND MESSAGE TO CLIENT\n");
            conn->status = DROP;
            unlockMutex(&cache[conn->cacheIndex].mutex);
            return;
        }
    }
    unlockMutex(&cache[conn->cacheIndex].mutex);
}

char* appendNewData(char* dstBuffer, size_t* oldSize, char* srcBuffer, size_t srcSize) {
    if (*oldSize == 0) {
        *oldSize = srcSize;
        dstBuffer = (char*)malloc(*oldSize);
        memcpy(dstBuffer, srcBuffer, srcSize);
        return dstBuffer;
    }

    size_t old = *oldSize;
    *oldSize += srcSize;
    dstBuffer = (char*)realloc(dstBuffer, *oldSize);
    if (dstBuffer == NULL) {
        *oldSize = 0;
        perror("allocation memory in appendNewData");
        return NULL;
    }
    memcpy(dstBuffer + old, srcBuffer, srcSize);

    return dstBuffer;
}

void writeToServer(connection* conn) {
    printf("WRITE TO SERVER\n");
    if (conn->handleSize < conn->allSize) {
        ssize_t res = send(conn->serverSocket, conn->buffer + conn->handleSize,
                           conn->allSize, NO_FLAGS);
        conn->handleSize += res;
        if (res == ERROR) {
            makeCacheInvalid(cache, conn->cacheIndex);
            conn->status = DROP;
            return;
        }
    }
    if (conn->handleSize == conn->allSize) {
        freeBufferConnection(conn);
        conn->status = READ_FROM_SERVER;
    }
}

void readFromClient(connection* conn) {
    printf("READ FROM CLIENT\n");
    char buf[BUFFER_SIZE];
    ssize_t readCount;
    for (int i = 0; i < 2; i++) {
        readCount = recv(conn->clientSocket, buf, BUFFER_SIZE, NO_FLAGS);
        if (readCount == ERROR) {
            if (errno == EAGAIN) {
                readCount = EOF;
                break;
            }
            perror("receive buffer from client");
            conn->status = DROP;
            return;
        }
//        nonblock(conn->clientSocket);
        conn->buffer = appendNewData(conn->buffer, &conn->allSize, buf, readCount);
        if (conn->buffer == NULL) {
            conn->status = DROP;
            return;
        }
    }
    if (conn->buffer != NULL && readCount == EOF) parseClientMessage(cache, conn);
    else if (conn->buffer == NULL && readCount == EOF) conn->status = DROP;
}

void writeFromCacheToClient(connection* conn) {
    printf("WRITE FROM CACHE TO CLIENT\n");
    int localCacheStat;
    localCacheStat = cache[conn->cacheIndex].status;
    lockMutex(&cache[conn->cacheIndex].mutex);
    if (localCacheStat == VALID || localCacheStat == DOWNLOADING) {
        if (conn->handleCacheSize < cache[conn->cacheIndex].allSize) {
            ssize_t writtenSize = send(
                    conn->clientSocket,
                    cache[conn->cacheIndex].data + conn->handleCacheSize,
                    cache[conn->cacheIndex].allSize - conn->handleCacheSize,
                    NO_FLAGS);
            if (writtenSize == ERROR) {
                perror("send to client from cache");
                conn->handleCacheSize = 0;
                conn->status = DROP;
                unlockMutex(&cache[conn->cacheIndex].mutex);
                return;
            }
            conn->handleCacheSize += writtenSize;
        }
        else if (conn->handleCacheSize == cache[conn->cacheIndex].allSize && localCacheStat == VALID) {
            conn->handleCacheSize = 0;
            printf("SEND MESSAGE FROM CACHE TO CLIENT\n");
            conn->status = DROP;
        }
        unlockMutex(&cache[conn->cacheIndex].mutex);
    }
    else {
        conn->status = DROP;
        return;
    }
}

void putToCache(char* message, connection* conn, ssize_t readCount) {
    lockMutex(&cache[conn->cacheIndex].mutex);
    if (cache[conn->cacheIndex].status == DOWNLOADING && (cache[conn->cacheIndex].allSize == 0)) {
        int body = getBodyIndex(message, readCount);
        if (body == ERROR) {
            unlockMutex(&cache[conn->cacheIndex].mutex);
            return;
        }

        ssize_t statusCode = getStatusCodeAnswer(message);
        if (statusCode != HTTP_STATUS_OK) {
            printf("statusCode: %zd\n", statusCode);
            makeCacheInvalid(cache, conn->cacheIndex);
            printf("DO NOT NEED TO BE CACHED\n");
            conn->status = DROP;
            unlockMutex(&cache[conn->cacheIndex].mutex);
            return;
        }
        ssize_t size = readCount - body;
        cache[conn->cacheIndex].data = appendNewData(cache[conn->cacheIndex].data,
                                                     &cache[conn->cacheIndex].allSize,
                                                     message + body,size);
        if (cache[conn->cacheIndex].data == NULL) {
            dropConnection(conn);
            printf("CACHE IS FULL");
        }
        cache[conn->cacheIndex].status = DOWNLOADING;
        cache[conn->cacheIndex].readers--;
    }
    else if (readCount == 0) {
        cache[conn->cacheIndex].status = VALID;
        printf("READ MESSAGE FROM SERVER\n");
    }
    else {
        cache[conn->cacheIndex].data = appendNewData(cache[conn->cacheIndex].data,&cache[conn->cacheIndex].allSize,
                                                     message, readCount);
        if (cache[conn->cacheIndex].data == NULL) {
            conn->status = DROP;
            printf("Cache is full");
        }
    }
    unlockMutex(&cache[conn->cacheIndex].mutex);
}
void readFromServer(connection* conn) {
    printf("READ FROM SERVER\n");
    char message[BUFFER_SIZE];
    ssize_t readCount = recv(conn->serverSocket, message, BUFFER_SIZE, NO_FLAGS);
    printf("%ld\n", readCount);
    if (readCount == ERROR) {
        perror("receive from server");
        makeCacheInvalid(cache, conn->cacheIndex);
        conn->status = DROP;
        return;
    }
    putToCache(message, conn, readCount);
    if (conn->status == DROP) return;
    writeToClient(conn);
}

int handleEvent(connection* conn) {
    switch (conn->status) {
        case READ_FROM_CLIENT: {
            readFromClient(conn);
            break;
        }
        case WRITE_TO_SERVER: {
            writeToServer(conn);
            break;
        }
        case READ_FROM_SERVER: {
            readFromServer(conn);
            break;
        }
        case READ_FROM_CACHE_WRITE_CLIENT: {
            writeFromCacheToClient(conn);
            break;
        }
        case DROP:
            break;
    }
    return conn->status != DROP;
}

void* run(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return ERROR_CODE;
    }
    connection* conn = (connection*) param;
    int isOpen = TRUE;
    do {
        isOpen = handleEvent(conn);
    } while (isOpen);
    dropConnection(conn);
    free(conn);
    return SUCCESS_CODE;
}

int isValidPort(int port) {
    return (port >= MIN_PORT && port <= MAX_PORT);
}

int main(int argc, char* argv[]) {
    if (argc != COMMAND_ARGUMENT_COUNT){
        printf("Usage: <program> <proxy_port>");
        return ERROR;
    }

    int error_code = initCache(cache);
    if (error_code == ERROR) return ERROR;
    int proxySocketPort = atoi(argv[PORT_ARGUMENT_NUMBER]);
    if (!isValidPort(proxySocketPort)) {
        printf("Invalid proxy port");
        return ERROR;
    }

    int proxySocket = getProxySocket(proxySocketPort);
    if (proxySocket == ERROR) return ERROR;
    __sighandler_t sig = signal(SIGPIPE, SIG_IGN);
    if (sig == SIG_ERR) {
        perror("signal");
        return ERROR;
    }
    pthread_attr_t attrDet;
    error_code = pthread_attr_init(&attrDet);
    if (error_code != SUCCESS) {
        perror("pthread_attr_init");
        return ERROR;
    }
    error_code = pthread_attr_setdetachstate(&attrDet, PTHREAD_CREATE_DETACHED);
    if (error_code != SUCCESS) {
        perror("pthread_attr_setdetachstate");
        return ERROR;
    }
    while (TRUE) {
        int client = accept(proxySocket, (struct sockaddr*) NULL, NULL);
        if (client == ERROR) continue;
        connection* conn = (struct connection*)malloc(sizeof(struct connection));
        conn->clientSocket = client;
        conn->serverSocket = EMPTY;
        conn->cacheIndex = EMPTY;
        conn->status = READ_FROM_CLIENT;

        pthread_t thread;
        error_code = pthread_create(&thread, &attrDet, run, (void*)conn);
        if (error_code != SUCCESS) {
            perror("pthread_create");
            free(conn);
            continue;
        }
        else {
            printf("ACCEPTED NEW CONNECTION\n");
        }
    }
}
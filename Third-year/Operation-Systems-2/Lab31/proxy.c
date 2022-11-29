#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include "errno.h"

#define MAX_PORT 65535
#define MIN_PORT 1025
#define SO_REUSEADDR_EN 1
#define PORT_ARGUMENT_NUMBER 1
#define NO_FLAGS 0
#define MAX_CONNECTIONS 100
#define MAX_CACHE_SIZE 1024
#define BUFFER_SIZE (512 * 1024)
#define MAX_CLIENTS_COUNT 20
#define DECIMAL 10
#define ERROR_CODE (char*)1
#define ERROR (-1)
#define HTTP_PORT 80
#define DEFAULT_PROTOCOL 0
#define POLLNONE 0
#define TRUE 1
#define TIMEOUT_INFINITE (-1)
#define COMMAND_ARGUMENT_COUNT 2
#define SHUTDOWN (-1)
#define LISTENING 0
#define EMPTY (-1)
#define HTTP_STATUS_OK 200

enum connectionStatus {
    READ_FROM_CLIENT,
    WRITE_TO_SERVER,
    READ_FROM_SERVER,
    READ_FROM_CACHE_WRITE_CLIENT,
    NOT_ACTIVE
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

enum cacheStatus {
    DOWNLOADING,
    VALID,
    INVALID
};

enum cacheSearchStatus {
    FIND_CACHE,
    FREE_CACHE,
    NOT_USING_CACHE
};

struct cacheInfo {
    size_t allSize;
    size_t readers;

    char* data;
    char* url;
    enum cacheStatus status;
};

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

//url = "http://fit.ippolitov.me/CN_2/2021/2.html"
char* createGet(char* url, size_t* len) {
    char* afterProtocol = strstr(url, "://"); //"afterProtocol = ://fit.ippolitov.me/CN_2/2021/2.html"
    if (afterProtocol == NULL) {
        fprintf(stderr, "Incorrect input.\n");
        return ERROR_CODE;
    }

    char* afterHost = strchr(afterProtocol + 3, '/'); //"/CN_2/2021/2.html"
    int hostLength = afterHost == NULL ? strlen(afterProtocol + 3) : afterHost - (afterProtocol + 3);

    char hostName[hostLength + 1];
    strncpy(hostName, afterProtocol + 3, hostLength);
    hostName[hostLength] = 0;

    char request[] = "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n";

    char* buffer;
    if (afterHost == NULL) {
        *len = sizeof(request) + sizeof(hostName) + 1;
        buffer = (char*) malloc(sizeof(char) * (*len));
        if (buffer == NULL) {
            perror("malloc");
            return ERROR_CODE;
        }
        sprintf(buffer, request, "/", hostName);
    }
    else {
        *len = strlen(request) - 4 + strlen(afterHost) + strlen(hostName);
        buffer = (char*) malloc(sizeof(char) * (*len));
        if (buffer == NULL) {
            perror("malloc");
            return ERROR_CODE;
        }
        sprintf(buffer, request, afterHost, hostName);
    }
    return buffer;
}


int isGetRequest(char* httpData) {
    return strncmp(httpData, "GET ", 4) == 0;
}

// "GET http://parallels.nsu.ru/ HTTP/1.1\r\nHost: parallels.nsu.ru\r\nProxy-Connection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\nCookie: _ga=GA1.2.1254981895.1667230970\r\n\r\n"
char* getUrlFromData(char* httpData) {
    char* protocol = httpData + 4;
    char* version = strchr(protocol, ' ');
    if (version == NULL) return NULL;

    char* url = (char*) malloc(version - protocol + 1);
    if (url == NULL) {
        perror("malloc for getUrlFromData");
        return NULL;
    }

    memcpy(url, protocol, (size_t)(version - protocol));
    url[version - protocol] = '\0';

    return url;
}

char* getHostFromUrl(char* url) {
    // http://
    char* startHost = url + 7;
    char* endHost = strchr(startHost, '/');
    if (endHost == NULL) return NULL;

    char* result = (char*)malloc(sizeof(char) * (endHost - startHost + 1));
    if (result == NULL) {
        perror("malloc for getHostFromUrl");
        return NULL;
    }

    memcpy(result, startHost, (size_t)(endHost - startHost));
    result[endHost - startHost] = '\0';

    printf("Host:%s\n", result);
    return result;
}

ssize_t getStatusCodeAnswer(char* httpData) {
    char* afterHTTP = httpData + sizeof("HTTP/1.1");// ignore "HTTP/1.1 "

    char* strAfterCode;
    ssize_t statusCode = (int)strtol(afterHTTP, &strAfterCode, DECIMAL);
    if (strAfterCode == afterHTTP || statusCode <= 0) {
        return ERROR;
    }
    return statusCode;
}

int getBodyIndex(const char* request, size_t len) {
    for (size_t j = 0; j < len - 3; j++) {
        if (request[j] == '\r' && request[j + 1] == '\n' && request[j + 2] == '\r' && request[j + 3] == '\n') {
            return (int)j + 4;
        }
    }
    return ERROR;
}

int getProxySocket(int port) {
    struct sockaddr_in listenAddress;
    listenAddress.sin_addr.s_addr = htonl(INADDR_ANY); //Можем слушать на всех интерфейсах
    listenAddress.sin_family = AF_INET;
    listenAddress.sin_port = htons(port);

    int proxySocket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (proxySocket == ERROR) {
        perror("proxySocket");
        return ERROR;
    }
    int sock_opt = SO_REUSEADDR_EN; //Для повторного перезапуска
    int errorCode = setsockopt(proxySocket, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt));
    if (errorCode == ERROR) {
        perror("setsockopt");
        return ERROR;
    }

    errorCode = bind(proxySocket, (struct sockaddr*) &listenAddress, sizeof(listenAddress));
    if (errorCode == ERROR) {
        perror("bind proxySocket");
        return ERROR;
    }

    nonblock(proxySocket);
    errorCode = listen(proxySocket, MAX_CLIENTS_COUNT);
    if (errorCode == ERROR) {
        perror("listen proxySocket");
        return ERROR;
    }
    return proxySocket;
}

int getServerSocket(char* url) {
    char* host = getHostFromUrl(url);
    struct hostent* hostInfo = gethostbyname(host);
    if (hostInfo == NULL) {
        perror("Cannot get host by name");
        return ERROR;
    }

    struct sockaddr_in destinationAddress;

    destinationAddress.sin_family = AF_INET;
    destinationAddress.sin_port = htons(HTTP_PORT);
    memcpy(&destinationAddress.sin_addr, hostInfo->h_addr, hostInfo->h_length);

    int serverSocket = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (serverSocket == ERROR) {
        perror("Cannot create socket");
        return ERROR;
    }
    int res = connect(serverSocket, (struct sockaddr*) &destinationAddress, sizeof(destinationAddress));
    if (res == ERROR) {
        perror("Cannot connect");
        return ERROR;
    }
    nonblock(serverSocket);

    return serverSocket;
}

void freeBufferConnection(connection* conn) {
    if (conn->allSize > 0) {
        free(conn->buffer);
        conn->buffer = NULL;
        conn->allSize = 0;
        conn->handleSize = 0;
    }
}

void dropConnection(connection *conn, int connIndex, int *activefdsCount) {

    freeBufferConnection(&conn[connIndex]);

    int error_code = close(conn[connIndex].clientSocket);
    if (error_code == ERROR) {
        perror("close client socket");
    }

    if (conn[connIndex].serverSocket != EMPTY) {
        error_code = close(conn[connIndex].serverSocket);
        if (error_code == ERROR) {
            perror("close server socket");
        }
    }

    conn[connIndex] = conn[*activefdsCount - 1];
    conn[*activefdsCount - 1].buffer = NULL;
    conn[*activefdsCount - 1].allSize = 0;
    conn[*activefdsCount - 1].handleSize = 0;
    conn[*activefdsCount - 1].cacheIndex = EMPTY;
    conn[*activefdsCount - 1].serverSocket = EMPTY;
    conn[*activefdsCount - 1].clientSocket = EMPTY;
    conn[*activefdsCount - 1].status = NOT_ACTIVE;
    (*activefdsCount)--;
    printf("dropped connection\n");
}

void wrongMethod(connection* conn, int index, int* activeClients) {
    char errorMessage[] = "405 Method Not Allowed\r\n";
    write(conn[index].clientSocket, errorMessage, strlen(errorMessage));
}

void cannotResolve(connection* conn, int index, int* activeClients) {
    char errorMessage[] = "523 Origin Is Unreachable\r\n";
    write(conn->clientSocket, errorMessage, strlen(errorMessage));
}

struct cacheInfo cache[MAX_CACHE_SIZE];

void writeToClient(connection *conn, int index, struct pollfd *fds, int *activeClientsCount);

int searchCache(char* url, connection* conn) {
    for (int j = 0; j < MAX_CACHE_SIZE; j++) { //try find url in cache
        if (cache[j].url != NULL && strcmp(cache[j].url, url) == 0) {
            if (cache[j].status == VALID || cache[j].status == DOWNLOADING) {
                cache[j].readers++;
                freeBufferConnection(conn);
                conn->allSize = 0;
                conn->handleSize = 0;
                conn->cacheIndex = j;
                conn->status = READ_FROM_CACHE_WRITE_CLIENT;
                return FIND_CACHE;
            }
        }
    }
    for (int j = 1; j < MAX_CACHE_SIZE; j++) { //no url in cache, try find free cache
        if (cache[j].url == NULL) {
            cache[j].readers = 1;
            cache[j].status = DOWNLOADING;
            conn->cacheIndex = j;
            cache[j].data = NULL;
            cache[j].allSize = 0;
            conn->status = WRITE_TO_SERVER;

            cache[j].url = (char*)malloc(strlen(url) + 1);
            if (cache[j].url == NULL) {
                perror("malloc");
                return ERROR;
            }
            memcpy(cache[j].url, url, strlen(url) + 1);
            return FREE_CACHE;
        }
    }
    for (int j = 0; j < MAX_CACHE_SIZE; j++) {// no url in cache, try find not using cache
        if (cache[j].readers == 0 || cache[j].status == INVALID) {
            cache[j].readers = 1;
            cache[j].status = DOWNLOADING;
            conn->cacheIndex = j;
            cache[j].allSize = 0;

            free(cache[j].data);
            cache[j].data = NULL;
            conn->status = WRITE_TO_SERVER;

            free(cache[j].url);
            cache[j].url = (char *) malloc(sizeof(url));
            if (cache[j].url == NULL) {
                perror("malloc");
                return ERROR;
            }
            memcpy(cache[j].url, url, strlen(url));
            return NOT_USING_CACHE;
        }
    }
    printf("ERROR: cache is full\n");
    return ERROR;
}

void makeCacheInvalid(int id) {
    cache[id].status = INVALID;
}

void initCache() {
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        cache[i].allSize = 0;
        cache[i].readers = 0;
        cache[i].data = NULL;
        cache[i].url = NULL;
    }
}

void destroyCache() {
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        free(cache[i].data);
        free(cache[i].url);
    }
    printf("destroy cache\n");
}

//CONNECT
void updatePoll(connection* conn, struct pollfd* fds, int activeClientsCount, int proxySocket) {

    if (activeClientsCount == MAX_CONNECTIONS) fds[LISTENING].fd = SHUTDOWN;
    else fds[LISTENING].fd = proxySocket;

    for (int i = 0; i < activeClientsCount; ++i) {
        int clifdIndex = i * 2 + 1;
        int srvfdIndex = i * 2 + 2;
        fds[clifdIndex].fd = conn[i].clientSocket;
        fds[srvfdIndex].fd = conn[i].serverSocket;
        switch (conn[i].status) {
            case READ_FROM_CLIENT:
                fds[clifdIndex].events = POLLIN;
                fds[srvfdIndex].events = POLLNONE;
                break;
            case WRITE_TO_SERVER:
                fds[clifdIndex].events = POLLNONE;
                fds[srvfdIndex].events = POLLOUT;
                break;
            case READ_FROM_SERVER:
                fds[clifdIndex].events = POLLOUT;
                fds[srvfdIndex].events = POLLIN;
                break;
            case READ_FROM_CACHE_WRITE_CLIENT:
                fds[clifdIndex].events = POLLOUT;
                fds[srvfdIndex].events = POLLNONE;
                break;
            case NOT_ACTIVE:
                break;
        }
    }
}

char* appendNewData(char* dstBuffer, size_t* oldSize, char* srcBuffer, size_t srcSize) {
    if (*oldSize == 0) {
        *oldSize = srcSize;
        dstBuffer = (char*)malloc(*oldSize);
        if (dstBuffer == NULL) {
            *oldSize = 0;
            perror("allocation memory in appendNewData");
            return NULL;
        }
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

void initNewConnection(connection* conn, int connIndex, int clientfd) {
    conn[connIndex].clientSocket = clientfd;
    conn[connIndex].serverSocket = EMPTY;
    conn[connIndex].allSize = 0;
    conn[connIndex].handleSize = 0;
    conn[connIndex].buffer = NULL;
    conn[connIndex].cacheIndex = EMPTY;
    conn[connIndex].status = READ_FROM_CLIENT;
}

void writeToServer(connection* conn, int connIndex, struct pollfd* fds, int* localCount) {
    if (fds[connIndex * 2 + 2].revents & POLLOUT) {
        if (conn[connIndex].handleSize < conn[connIndex].allSize) {
            ssize_t res = send(conn[connIndex].serverSocket, conn[connIndex].buffer + conn[connIndex].handleSize,
                               conn[connIndex].allSize, NO_FLAGS);

            conn[connIndex].handleSize += res;
            if (res == ERROR) {
                makeCacheInvalid(conn[connIndex].cacheIndex);
                dropConnection(conn, connIndex, localCount);
                return;
            }
        }
        if (conn[connIndex].handleSize == conn[connIndex].allSize) {
            freeBufferConnection(&conn[connIndex]);
            conn[connIndex].status = READ_FROM_SERVER;
        }
    }
}

void parseClientMessage(connection* conn, int connIndex, int* activeClientsCount) {
    if (conn[connIndex].allSize > 3) {
        char* url = getUrlFromData(conn[connIndex].buffer);
        if (url != NULL) {
            if (!isGetRequest(conn[connIndex].buffer)) {
                wrongMethod(conn, connIndex, activeClientsCount);
                printf("Not get request\n");
                printf("request = %s", conn[connIndex].buffer);
                dropConnection(conn, connIndex, activeClientsCount);
                free(url);
            }
            else {
                int foundInCache = searchCache(url, &conn[connIndex]);
                if (foundInCache == ERROR) {
                    destroyCache();
                    printf("ERROR: Cache is full");
                    exit(EXIT_FAILURE);
                }
                if (foundInCache == FREE_CACHE || foundInCache == NOT_USING_CACHE) { // if url wasn't in cache
                    conn[connIndex].serverSocket = getServerSocket(url);
                    free(conn[connIndex].buffer);
                    //"GET /wiki/Publikacii/ HTTP/1.0\r\nHost: parallels.nsu.ru\r\n\r\n"
                    conn[connIndex].buffer = createGet(url, &conn[connIndex].allSize);

                    if (conn[connIndex].serverSocket == ERROR) {
                        cannotResolve(conn, connIndex, activeClientsCount);
                        printf("Can't create server socket\n");
                        dropConnection(conn, connIndex, activeClientsCount);
                        free(url);
                        return;
                    }
                    printf("GET REQUEST FROM CLIENT\n");
                }
            }
        }
        else {
            printf("Invalid url\n");
            dropConnection(conn, connIndex, activeClientsCount);
        }
    }
    else {
        printf("Invalid client message\n");
        dropConnection(conn, connIndex, activeClientsCount);
    }
}

void readFromClient(connection* conn, int connIndex, struct pollfd* fds, int* activeClientsCount) {
    char buf[BUFFER_SIZE];
    int clientfdIndex = connIndex * 2 + 1;
    if ((fds[clientfdIndex].revents & POLLHUP) || (fds[clientfdIndex].revents & POLLERR)) { //disconnected
        dropConnection(conn, connIndex, activeClientsCount);
        return;
    }
    ssize_t readCount;
    for (int i = 0; i < 2; i++) {
        if (fds[clientfdIndex].revents & POLLIN) {
            readCount = recv(conn[connIndex].clientSocket, buf, BUFFER_SIZE, NO_FLAGS);
            if (readCount == ERROR) {
                if (errno == EAGAIN) {
                    readCount = EOF;
                    break;
                }
                perror("receive buffer from client");
                dropConnection(conn, connIndex, activeClientsCount);
                return;
            }
            conn[connIndex].buffer = appendNewData(conn[connIndex].buffer, &conn[connIndex].allSize,
                                                   buf, readCount);
            if (conn[connIndex].buffer == NULL) {
                dropConnection(conn, connIndex, activeClientsCount);
                return;
            }
        }
    }
    if (conn[connIndex].buffer != NULL && readCount == EOF) parseClientMessage(conn, connIndex, activeClientsCount);
}

void writeFromCacheToClient(connection* conn, int connIndex, struct pollfd* fds, int* activefdsCount) {
    int clientfdIndex = connIndex * 2 + 1;
    if (fds[clientfdIndex].revents & POLLOUT) {
        int localCacheStat;
        localCacheStat = cache[conn[connIndex].cacheIndex].status;
        if (localCacheStat == VALID || localCacheStat == DOWNLOADING) {
            if (conn[connIndex].handleCacheSize < cache[conn[connIndex].cacheIndex].allSize) {
                ssize_t writtenSize = send(
                        conn[connIndex].clientSocket,
                        cache[conn[connIndex].cacheIndex].data + conn[connIndex].handleCacheSize,
                        cache[conn[connIndex].cacheIndex].allSize - conn[connIndex].handleCacheSize,
                        NO_FLAGS);
                if (writtenSize == ERROR) {
                    perror("send to client from cache");
                    conn[connIndex].handleCacheSize = 0;
                    dropConnection(conn, connIndex, activefdsCount);
                    return;
                }
                conn[connIndex].handleCacheSize += writtenSize;
            }
            else if (conn[connIndex].handleCacheSize == cache[conn[connIndex].cacheIndex].allSize && localCacheStat == VALID) {
                conn[connIndex].handleCacheSize = 0;
                printf("SEND MESSAGE FROM CACHE TO CLIENT\n");
                dropConnection(conn, connIndex, activefdsCount);
            }
        }
        else {
            dropConnection(conn, connIndex, activefdsCount);
            return;
        }
    }
}

void putToCache(char* message, connection* conn, int connIndex, ssize_t readCount, int* activefdsCount) {
    if (cache[conn[connIndex].cacheIndex].status == DOWNLOADING && (cache[conn[connIndex].cacheIndex].allSize == 0)) {
        int body = getBodyIndex(message, readCount);
        if (body == ERROR) return;

        ssize_t statusCode = getStatusCodeAnswer(message);
        if (statusCode != HTTP_STATUS_OK) {
            printf("statusCode: %zd", statusCode);
            makeCacheInvalid(conn[connIndex].cacheIndex);
            printf("DO NOT NEED TO BE CACHED\n");
            return;
        }
        ssize_t size = readCount - body;
        cache[conn[connIndex].cacheIndex].data = appendNewData(cache[conn[connIndex].cacheIndex].data,
                                                               &cache[conn[connIndex].cacheIndex].allSize,
                                                               message + body,size);
        if (cache[conn[connIndex].cacheIndex].data == NULL) {
            dropConnection(conn, connIndex, activefdsCount);
            printf("CACHE IS FULL");
        }
        cache[conn[connIndex].cacheIndex].status = DOWNLOADING;
        cache[conn[connIndex].cacheIndex].readers--;
    }
    else if (readCount == 0) {
        cache[conn[connIndex].cacheIndex].status = VALID;
        printf("READ MESSAGE FROM SERVER\n");
    }
    else {
        cache[conn[connIndex].cacheIndex].data = appendNewData(cache[conn[connIndex].cacheIndex].data,&cache[conn[connIndex].cacheIndex].allSize,
                                                               message, readCount);
        if (cache[conn[connIndex].cacheIndex].data == NULL) {
            dropConnection(conn, connIndex, activefdsCount);
            printf("Cache is full");
        }
    }
}

void readFromServer(connection* conn, int connIndex, struct pollfd* fds, int* activefdsCount) {
    char message[BUFFER_SIZE];
    int serverfdIndex = connIndex * 2 + 2;
    if (fds[serverfdIndex].revents & POLLIN) {
        ssize_t readCount = recv(conn[connIndex].serverSocket, message, BUFFER_SIZE, NO_FLAGS);
        if (readCount == ERROR) {
            perror("receive from server");
            makeCacheInvalid(conn[connIndex].cacheIndex);
            dropConnection(conn, connIndex, activefdsCount);
            return;
        }
        putToCache(message, conn, connIndex, readCount, activefdsCount);
        writeToClient(conn, connIndex, fds, activefdsCount);
    }
}

void writeToClient(connection* conn, int clientIndex, struct pollfd* fds, int* activeClientsCount) {
    int clientfdIndex = clientIndex * 2 + 1;
    if (fds[clientfdIndex].revents & POLLOUT &&
        (cache[conn[clientIndex].cacheIndex].status == VALID || cache[conn[clientIndex].cacheIndex].status == DOWNLOADING)) {
        char* message = cache[conn[clientIndex].cacheIndex].data + conn[clientIndex].handleSize;
        size_t size = cache[conn[clientIndex].cacheIndex].allSize - conn[clientIndex].handleSize;
        ssize_t writeCount = send(conn[clientIndex].clientSocket, message, size, NO_FLAGS);
        if (writeCount == ERROR) {
            perror("send to client");
            makeCacheInvalid(conn[clientIndex].cacheIndex);
            dropConnection(conn, clientIndex, activeClientsCount);
            return;
        }
        conn[clientIndex].handleSize += writeCount;
        if (writeCount == 0) {
            printf("SEND MESSAGE TO CLIENT\n");
            dropConnection(conn, clientIndex, activeClientsCount);
            return;
        }
    }
}

void handleEvent(connection* conn, int clientIndex, struct pollfd* fds, int* activeClientsCount) {
    switch (conn[clientIndex].status) {
        case READ_FROM_CLIENT: {
            readFromClient(conn, clientIndex, fds, activeClientsCount);
            break;
        }
        case WRITE_TO_SERVER: {
            writeToServer(conn, clientIndex, fds, activeClientsCount);
            break;
        }
        case READ_FROM_SERVER: {
            readFromServer(conn, clientIndex, fds, activeClientsCount);
            break;
        }
        case READ_FROM_CACHE_WRITE_CLIENT: {
            writeFromCacheToClient(conn, clientIndex, fds, activeClientsCount);
            break;
        }
        case NOT_ACTIVE:
            break;
    }
}

void tryAcceptNewClient(struct pollfd* fds, int* activeClientsCount, connection* conn, int proxySocket) {
    if (fds[LISTENING].fd != SHUTDOWN && fds[LISTENING].revents & POLLIN) {
        int newClientSocket = accept(proxySocket, NULL, NULL);
        nonblock(newClientSocket);
        if (newClientSocket != ERROR) {
            initNewConnection(conn, *activeClientsCount, newClientSocket);
            printf("New client [%d] accepted\n", *activeClientsCount + 1);
            (*activeClientsCount)++;
        }
    }
}

void run(int proxySocket) {
    int activeClientsCount = 0;
    struct pollfd fds[2 * MAX_CONNECTIONS + 1];
    connection conn[MAX_CONNECTIONS + 1];

    fds[LISTENING].fd = proxySocket;
    fds[LISTENING].events = POLLIN;

    while (TRUE) {
        int nfds = activeClientsCount * 2 + 1;
        int polled = poll(fds, nfds, TIMEOUT_INFINITE); //wait for event
            if (polled == ERROR) {
            perror("poll");
        }

        tryAcceptNewClient(fds, &activeClientsCount, conn, proxySocket);

        for (int i = 0; i < activeClientsCount; i++) {
            handleEvent(conn, i, fds, &activeClientsCount);
        }
        updatePoll(conn, fds, activeClientsCount, proxySocket);
    }
}

int isValidPort(int port) {
    return (port >= MIN_PORT && port <= MAX_PORT);
}

int main(int argc, char* argv[]) {
    if (argc != COMMAND_ARGUMENT_COUNT){
        printf("Usage: <program> <proxy_port>");
        return ERROR;
    }

    initCache();
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
    run(proxySocket);
    destroyCache();
    return EXIT_SUCCESS;
}
#include "header.h"
#include "cache.h"
#include "network.h"
#include "connection.h"

//url = "http://fit.ippolitov.me/CN_2/2021/2.html"
char* createGetRequest(char* url, size_t* len) {
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
        *len = strlen(request) - 4 + strlen(afterHost) + strlen(hostName) + 1;
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

void parseClientMessage(struct cacheInfo* cache, connection* conn) {
    if (conn->allSize > 3) {
        char* url = getUrlFromData(conn->buffer);
        if (url != NULL) {
            if (!isGetRequest(conn->buffer)) {
                wrongMethod(conn);
                printf("Not get request\n");
                printf("request = %s", conn->buffer);
                conn->status = DROP;
                free(url);
            }
            else {
                int foundInCache = searchCache(cache, url, conn);
                if (foundInCache == ERROR) {
                    destroyCache(cache);
                    printf("ERROR: Cache is full");
                    exit(EXIT_FAILURE);
                }
                if (foundInCache == FREE_CACHE || foundInCache == NOT_USING_CACHE) { // if url wasn't in cache
                    conn->serverSocket = getServerSocket(url);
                    free(conn->buffer);
                    //"GET /wiki/Publikacii/ HTTP/1.0\r\nHost: parallels.nsu.ru\r\n\r\n"
                    conn->buffer = createGetRequest(url, &conn->allSize);

                    if (conn->serverSocket == ERROR) {
                        cannotResolve(conn);
                        printf("Can't create server socket\n");
                        conn->status = DROP;
                        free(url);
                        return;
                    }
                    printf("GET REQUEST FROM CLIENT\n");
                }
            }
        }
        else {
            printf("Invalid url\n");
            conn->status = DROP;
        }
    }
    else {
        printf("Invalid client message\n");
        conn->status = DROP;
    }
}
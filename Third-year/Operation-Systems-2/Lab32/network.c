#include "header.h"
#include "parser.h"

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
    return serverSocket;
}

void wrongMethod(connection* conn) {
    char errorMessage[] = "405 Method Not Allowed\r\n";
    write(conn->clientSocket, errorMessage, strlen(errorMessage));
}

void cannotResolve(connection* conn) {
    char errorMessage[] = "523 Origin Is Unreachable\r\n";
    write(conn->clientSocket, errorMessage, strlen(errorMessage));
}
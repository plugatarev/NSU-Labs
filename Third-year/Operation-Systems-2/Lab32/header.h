#ifndef LAB32_HEADER_H
#define LAB32_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include "errno.h"
#include <pthread.h>
#include <fcntl.h>

#define SUCCESS 0
#define MAX_PORT 65535
#define MIN_PORT 1025
#define SO_REUSEADDR_EN 1
#define PORT_ARGUMENT_NUMBER 1
#define NO_FLAGS 0
#define MAX_CACHE_SIZE 1024
#define BUFFER_SIZE (512 * 1024)
#define MAX_CLIENTS_COUNT 20
#define DECIMAL 10
#define ERROR_CODE (char*)1
#define SUCCESS_CODE (char*)0
#define ERROR (-1)
#define HTTP_PORT 80
#define DEFAULT_PROTOCOL 0
#define TRUE 1
#define COMMAND_ARGUMENT_COUNT 2
#define EMPTY (-1)
#define HTTP_STATUS_OK 200

#endif

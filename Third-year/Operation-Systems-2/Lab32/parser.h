#ifndef LAB32_PARSER_H
#define LAB32_PARSER_H

#include "cache.h"
#include "connection.h"

char* createGetRequest(char* url, size_t* len);
int isGetRequest(char* httpData);
char* getUrlFromData(char* httpData);
char* getHostFromUrl(char* url);
ssize_t getStatusCodeAnswer(char* httpData);
int getBodyIndex(const char* request, size_t len);
void parseClientMessage(struct cacheInfo* cache, connection* conn);

#endif

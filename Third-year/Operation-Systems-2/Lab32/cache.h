#ifndef LAB32_CACHE_H
#define LAB32_CACHE_H

#include <stdlib.h>
#include "connection.h"

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
    pthread_mutex_t mutex;
};

int searchCache(struct cacheInfo* cache, char* url, connection* conn);
void makeCacheInvalid(struct cacheInfo* cache, int id);
int initCache(struct cacheInfo* cache);
void destroyCache(struct cacheInfo* cache);

#endif

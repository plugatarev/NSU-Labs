#include "header.h"
#include "connection.h"
#include "pthreadSync.h"
#include "cache.h"

int searchCache(struct cacheInfo* cache, char* url, connection* conn) {
    for (int j = 0; j < MAX_CACHE_SIZE; j++) { //try find url in cache
        lockMutex(&cache[j].mutex);
        if (cache[j].url != NULL && strcmp(cache[j].url, url) == 0) {
            if (cache[j].status == VALID || cache[j].status == DOWNLOADING) {
                cache[j].readers++;
                freeBufferConnection(conn);
                conn->allSize = 0;
                conn->handleSize = 0;
                conn->cacheIndex = j;
                conn->status = READ_FROM_CACHE_WRITE_CLIENT;
                unlockMutex(&cache[j].mutex);
                return FIND_CACHE;
            }
        }
        unlockMutex(&cache[j].mutex);
    }
    for (int j = 0; j < MAX_CACHE_SIZE; j++) { //no url in cache, try find free cache
        lockMutex(&cache[j].mutex);
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
                unlockMutex(&cache[j].mutex);
                return ERROR;
            }
            memcpy(cache[j].url, url, strlen(url) + 1);
            unlockMutex(&cache[j].mutex);
            return FREE_CACHE;
        }
        unlockMutex(&cache[j].mutex);
    }
    for (int j = 0; j < MAX_CACHE_SIZE; j++) {// no url in cache, try find not using cache
        lockMutex(&cache[j].mutex);
        if (cache[j].readers == 0 || cache[j].status == INVALID) {
            cache[j].readers = 1;
            cache[j].status = DOWNLOADING;
            conn->cacheIndex = j;
            cache[j].allSize = 0;

            free(cache[j].data);
            cache[j].data = NULL;
            conn->status = WRITE_TO_SERVER;

            free(cache[j].url);
            cache[j].url = (char*) malloc(sizeof(url));
            if (cache[j].url == NULL) {
                perror("malloc");
                unlockMutex(&cache[j].mutex);
                return ERROR;
            }
            memcpy(cache[j].url, url, strlen(url));
            unlockMutex(&cache[j].mutex);
            return NOT_USING_CACHE;
        }
        unlockMutex(&cache[j].mutex);
    }
    printf("ERROR: cache is full\n");
    return ERROR;
}

void makeCacheInvalid(struct cacheInfo* cache, int id) {
    cache[id].status = INVALID;
}

int initCache(struct cacheInfo* cache) {
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        cache[i].allSize = 0;
        cache[i].readers = 0;
        cache[i].data = NULL;
        cache[i].url = NULL;
        int error_code = initMutex(&cache[i].mutex);
        if (error_code == ERROR) return ERROR;
    }
    return SUCCESS;
}

void destroyCache(struct cacheInfo* cache) {
    for (int i = 0; i < MAX_CACHE_SIZE; i++) {
        free(cache[i].data);
        free(cache[i].url);
    }
    printf("destroy cache\n");
}
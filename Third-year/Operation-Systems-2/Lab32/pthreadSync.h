#ifndef LAB32_PTHREADSYNC_H
#define LAB32_PTHREADSYNC_H

#include "header.h"

int initMutex(pthread_mutex_t* mutex);
int lockMutex(pthread_mutex_t* mutex);
int unlockMutex(pthread_mutex_t* mutex);

#endif

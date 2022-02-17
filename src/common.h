#ifndef __COMMON__
#define __COMMON__

#include <queue>
#include <string>
#include <pthread.h>
#include <sys/time.h>

// This queue stores all the works producer creates
extern std::queue<std::string>* works;

// All the mutex definitions
extern pthread_mutex_t workMutex;
extern pthread_mutex_t waitWorkMutex;
extern pthread_mutex_t waitQueueMutex;
extern pthread_mutex_t threadCountMutex;
extern pthread_mutex_t logMutex;

// All condition definitions
extern pthread_cond_t hasWorkCond;
extern pthread_cond_t hasSpaceCond;

// Other global variables
extern bool allWorkProduced;
extern int activeThreads;
extern int totalThreads;
extern struct timeval startTime;
extern struct timeval currentTime;

// functions related to time processing
float getCurrentTime();



#endif
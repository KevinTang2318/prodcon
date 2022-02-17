#include "common.h"

using namespace std;


// global variables
queue<string> *works = new queue<string>();

// global mutexes
pthread_mutex_t workMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitWorkMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitQueueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t threadCountMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;

// global conditions
pthread_cond_t hasWorkCond = PTHREAD_COND_INITIALIZER; 
pthread_cond_t hasSpaceCond = PTHREAD_COND_INITIALIZER;

// other variables
bool allWorkProduced = false;
int activeThreads = 0;
int totalThreads = 0;
struct timeval startTime;

float getCurrentTime() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    float seconds = (float) (currentTime.tv_sec - startTime.tv_sec) + (float) (currentTime.tv_usec - startTime.tv_usec) / 1000000;
    return seconds;
}
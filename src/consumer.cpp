/* 
    The code for consumer function
*/

#include "consumer.h"
#include "common.h"
#include "tands.h"
#include "log.h"

using namespace std;

void* consumer(void* arg) {
    string cmd;
    int* id = (int *) arg; // Get the id of the consumer


    while (true) {
        
        /*
            Check whether it's time to exit since producer has finished all works and consumer has no work to do
            This deals with the case that when consumer has just finished a work and returned, the queue is empty
        */
        if (allWorkProduced && works->empty()) {
            break;
        }

        // Write log to ask for work
        writeEntry(getCurrentTime(), *id, -1, "Ask", -1);

        // If the queue is empty, and producer hasn't finished producing yet, the consumer waits for the hasWork condition
        if (works->empty()) {
            pthread_mutex_lock(&waitWorkMutex);
            pthread_cond_wait(&hasWorkCond, &waitWorkMutex);
            pthread_mutex_unlock(&waitWorkMutex);
        }

        /*
            This deals with the case that when getting freed from waiting for the condition
            the producer has finished all the work and no work left to do in the queue
        */
        if (allWorkProduced && works->empty()) {
            break;
        }

        // Enter the critical section
        pthread_mutex_lock(&workMutex);

        // This checks if after entering the critical section, all the works are done by other threads
        // If this is the case, we just continue
        // Otherwise, do the work
        if (!works->empty()) {
            cmd = works->front();
            works->pop();
            // Write to log file for receiving a task
            writeEntry(getCurrentTime(), *id, works->size(), "Receive", atoi(cmd.substr(1).c_str()));
            pthread_cond_signal(&hasSpaceCond);
        }
        else {
            pthread_mutex_unlock(&workMutex);
            continue;
        }

        pthread_mutex_unlock(&workMutex);

        // Do the task
        string paramStr = cmd.substr(1);
        int param = atoi(paramStr.c_str());
        Trans(param);

        // Write to log file for completing a task
        writeEntry(getCurrentTime(), *id, -1, "Complete", param);
    }

    pthread_mutex_lock(&threadCountMutex);
    activeThreads--;
    pthread_mutex_unlock(&threadCountMutex);

    return (void*) NULL;
}
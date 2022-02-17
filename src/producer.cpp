/* 
    The code for producer function
*/

#include <stdio.h>
#include <iostream>
#include <string>

#include "producer.h"
#include "common.h"
#include "tands.h"
#include "log.h"

using namespace std;

void* producer(void* arg) {
    string cmd; // used to store the command read from either file or user input
    allWorkProduced = false;  // A flag set to let the consumer know that the producer has finished producing all tasks
    int* id = (int*) arg;

    while (!cin.eof()) {
        cin >> cmd;
        
        // First process when there is empty emput
        if (cmd == "") {
            printf("Producer terminating!\n");
            break;
        }

        // When the queue is full wait until there are space in the queue to put more works in
        if (works->size() == totalThreads * 2) {
            pthread_mutex_lock(&waitQueueMutex);
            pthread_cond_wait(&hasSpaceCond, &waitQueueMutex);
            pthread_mutex_unlock(&waitQueueMutex);
        }

        // Parse command and put command in queue
        if (cmd[0] == 'T') {
            pthread_mutex_lock(&workMutex);
            works->push(cmd);

            // Write log for Receive to file
            writeEntry(getCurrentTime(), *id, works->size(), "Work", atoi(cmd.substr(1).c_str()));

            // When works are put into a queue, producer will notify the consumer that there are items in the queue now
            pthread_cond_signal(&hasWorkCond);

            pthread_mutex_unlock(&workMutex);
        }
        else if (cmd[0] == 'S') {
            string sleepTimeStr = cmd.substr(1);
            int sleepTime = atoi(sleepTimeStr.c_str());
            // Write log
            writeEntry(getCurrentTime(), *id, -1, "Sleep", sleepTime);
            Sleep(sleepTime);
        }
    }

    // When all works are processed, set this flag to true
    allWorkProduced = true;

    // Write log
    writeEntry(getCurrentTime(), *id, -1, "End", -1);

    // It is possible that a consumer is still waiting for the hasWorkCond while producer has finished all works
    // In this case, we send additional hasWorkCond to free them from waiting
    // The consumer will determin whether to terminate on their side.
    while(activeThreads != 0) {
        pthread_cond_signal(&hasWorkCond);
    }

    return (void*) NULL;
}

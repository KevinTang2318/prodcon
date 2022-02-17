/*
  This is the main function of prodcon program
  This function takes the input, 
  and generates the corresponding producer and consumer threads
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

#include "src/producer.h"
#include "src/consumer.h"
#include "src/common.h"
#include "src/log.h"

using namespace std;

int main(int argc, char* argv[]) {
    int nthreads;
    int id = NULL;

    // Check whether the input arguments is valid, and process the input
    if (argc == 2) {
        nthreads = atol(argv[1]);
    }
    else if (argc == 3) {
        nthreads = atol(argv[1]);
        id = atol(argv[2]);
    }
    else {
        printf("Wrong usage!\n");
        return -1;
    }

    // Initialize variables
    activeThreads = nthreads;
    totalThreads = nthreads;
    int ids[nthreads];
    int producerId = 0;

    // Open file to read
    string logFileName;
    if (id != NULL) {
        logFileName = "prodcon." + to_string(id) + ".log";
    }
    else {
        logFileName = "prodcon.log";
    }
    createLog(logFileName, nthreads);

    // Get start time
    gettimeofday(&startTime, NULL);

    pthread_t producerThread;
    pthread_t consumerThreads[nthreads];


    //Create producer thread
    int ret = pthread_create(&producerThread, NULL, producer, &producerId);
    if (ret != 0) {
        perror("Create producer thread failed!");
        exit(-1);
    }

    //Create all consumer threads
    for (int i = 0; i < nthreads; i++) {
        ids[i] = i+1; // Generate the id for each consumer
        ret = pthread_create(&consumerThreads[i], NULL, consumer, &ids[i]);
        if (ret != 0) {
            perror("Create consumer thread failed!");
            exit(-1);
        }
    }

    // Join all threads
    pthread_join(producerThread, NULL);
    for (int i = 0; i < nthreads; i++) {
        pthread_join(consumerThreads[i], NULL);
    }
    
    closeLog();

    return 0;
}
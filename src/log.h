#ifndef __LOGIO__
#define __LOGIO__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

void createLog(std::string fileName, int nthreads);
void writeEntry(float second, int id, int queueSize = -1, std::string cmd = "", int num = -1);
void closeLog();



#endif
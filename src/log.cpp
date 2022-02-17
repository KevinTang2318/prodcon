#include "log.h"
#include "common.h"

using namespace std;

FILE* fp;

int work = 0;
int receive = 0;
int ask = 0;
int complete = 0;
int sleep = 0;
vector<int> threadStat;
double lastTranSecond;

void createLog(string fileName, int nthreads) {
    fp = fopen(fileName.c_str(), "w+");
    threadStat.resize(nthreads);
    for (int i = 0; i < nthreads; i++) {
        threadStat[i] = 0;
    }
}

// This function prints the given entry in a formatted style to the log file
void writeEntry(float second, int id, int queueSize, string cmd, int num) {
    pthread_mutex_lock(&logMutex);
    if (cmd == "Ask") {
        fprintf(fp, "%4.3f ID=%2d      %s\n", second, id, cmd.c_str());
        ask++;
    }
    else if (cmd == "Sleep") {
        fprintf(fp, "%4.3f ID=%2d      %-12s %d\n", second, id, cmd.c_str(), num);
        sleep++;
    }
    else if (cmd == "Complete") {
        fprintf(fp, "%4.3f ID=%2d      %-12s %d\n", second, id, cmd.c_str(), num);
        complete++;
        threadStat[id-1]++;
        lastTranSecond = second;
    }
    else if (cmd == "Work") {
        fprintf(fp, "%4.3f ID=%2d Q=%2d %-12s %d\n", second, id, queueSize, cmd.c_str(), num);
        work++;
    }
    else if (cmd == "Receive") {
        fprintf(fp, "%4.3f ID=%2d Q=%2d %-12s %d\n", second, id, queueSize, cmd.c_str(), num);
        receive++;
    }
    else if (cmd == "End") {
        fprintf(fp, "%4.3f ID=%2d      %s\n", second, id, cmd.c_str());
    }
    pthread_mutex_unlock(&logMutex);
}

// Print summary and close the log file
void closeLog() {

    fprintf(fp, "Summary: \n");
    fprintf(fp, "    Work %10d\n", work);
    fprintf(fp, "    Ask  %10d\n", ask);
    fprintf(fp, "    Receive %7d\n", receive);
    fprintf(fp, "    Complete %6d\n", complete);
    fprintf(fp, "    Sleep %9d\n", sleep);
    
    for (int i = 0; i < threadStat.size(); i++) {
        fprintf(fp, "    Thread %2d %5d\n", i+1, threadStat[i]);
    }

    fprintf(fp, "Transactions per second: %.2f\n", 1/(lastTranSecond / complete));

    fclose(fp);
}
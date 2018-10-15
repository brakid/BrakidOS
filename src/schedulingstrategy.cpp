#include "schedulingstrategy.h"
#include "types.h"
#include "process.h"
#include "io.h"
#include "list.h"

int selectedSchedulingStrategy = 0;

Process* roundRobin(Process* currentProcess) {
    List* processes = getProcessList();
    int currentIndex = find(processes, currentProcess);
    //start at process after the current one
    int runningIndex = currentIndex + 1;
    while (runningIndex != currentIndex) {
        runningIndex = runningIndex % size(processes);
        Process* process = (Process*)get(processes, runningIndex);
        if (process->processState == READY) {
            return process;
        }
        runningIndex++;
    }
    if (currentProcess->processState == READY) {
        return currentProcess;
    } else {
        println("No process available");
        for(;;);
    }
}

SchedulingStrategy schedulingStrategies[1] = {roundRobin};

SchedulingStrategy getSelectedSchedulingStrategy() {
    return schedulingStrategies[selectedSchedulingStrategy];
}
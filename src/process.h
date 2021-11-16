#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Process states enum
typedef enum { READY, RUNNING, WAITING, TERMINATED } ProcessState;

// Process type enum
typedef enum { CPU, IO } ProcessType;

// Process struct
typedef struct { 
    int pid; 
    int noi; 
    int priority; 
    int sleep_time; 
    double sleep_prob; 
    ProcessState state; 
    ProcessType type; 
} Process;

void cpu_proc(Process* proc) {
    pritnf("PID: %d, Going for CPU", proc->pid);
    for (int i = 0; i < proc->noi; i++) {
        ;
    }
}

void io_proc(Process* proc) {
    srand(time(0));
    for (int i = 0; i < proc->noi; i++) {
        printf("%d, loop iteration %d", proc->pid, i);
        double prob = ((double) rand() / (RAND_MAX)) + 1;
        if (prob < proc->sleep_prob) {
            // Send IO signal to scheduler
            printf("PID: %d Going for IO", proc->pid);
            sleep(proc->sleep_time);
            printf("PID: %d Back from IO", proc->pid);  
            // Inform schedular via MSG Q
        }   
    }
}

#endif

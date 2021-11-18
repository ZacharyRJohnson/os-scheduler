#ifndef PROCESS_H
#define PROCESS_H

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
    int msgid;
    ProcessState state; 
    ProcessType type; 
} Process;

void cpu_proc(Process* proc);

void io_proc(Process* proc);

#endif
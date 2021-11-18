#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>

void generate() {    
    int num_procs = 4;
    int cpu_priority = 2;  
    int cpu_noi = 10000;
    int cpu_sleep_time = 1;
    double cpu_sleep_prob = 0.3;
    int io_priority = 1;
    int io_noi = 4000;
    int io_sleep_time = 3;
    double io_sleep_prob = 0.7;
    int t = 1;

    Process processes[num_procs];
    for (int i = 0; i < num_procs; i++) {
        sleep(1);
        int pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error while forking process: %s", strerror(errno));
            exit(1);
        }
        else if (pid == 0) {
            // If child process, run the process code
            int child_id = getpid();
            if (i % 2 == 0) {
                Process proc = {
                    child_id,
                    cpu_noi,
                    cpu_priority,
                    cpu_sleep_time,
                    cpu_sleep_prob,
                    READY,
                    CPU
                };
                cpu_proc(&proc);
                exit(0);
            }
            else {
                Process proc = {
                    child_id,
                    io_noi,
                    io_priority,
                    io_sleep_time,
                    io_sleep_prob,
                    READY,
                    IO
                };
                io_proc(&proc);
                exit(0);
            }
        }
        else {
            if (i % 2 == 0) {
                Process proc = {
                    pid,
                    cpu_noi,
                    cpu_priority,
                    cpu_sleep_time,
                    cpu_sleep_prob,
                    READY,
                    CPU
                };
                processes[i] = proc;
            }
            else {
                Process proc = {
                    pid,
                    io_noi,
                    io_priority,
                    io_sleep_time,
                    io_sleep_prob,
                    READY,
                    IO
                };
                processes[i] = proc;
            }
        }
    }

    int status = 0;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0);
}


struct mesg_buffer {
    long priority;
    pid_t pid;
} message;

int main() {
    generate();

    key_t key;
    int msgid;
    key = ftok("./temp.txt", 65);
    if (key == -1) {
        printf("ERROR");
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    message.priority = 1;
  
    // display the message
    printf("All processes completed\n");
}

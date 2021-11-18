#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include <string.h>
#include <errno.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void generate(int num_procs, int t, int nois[num_procs], int priorities[num_procs], double sleep_probs[num_procs], int sleep_times[num_procs]) {
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
                    nois[i],
                    priorities[i],
                    sleep_times[i],
                    sleep_probs[i],
                    READY,
                    CPU
                };
                cpu_proc(&proc);
                exit(0);
            }
            else {
                Process proc = {
                    child_id,
                    nois[i],
                    priorities[i],
                    sleep_times[i],
                    sleep_probs[i],
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
                    nois[i],
                    priorities[i],
                    sleep_times[i],
                    sleep_probs[i],
                    READY,
                    CPU
                };
                processes[i] = proc;
            }
            else {
                Process proc = {
                    pid,
                    nois[i],
                    priorities[i],
                    sleep_times[i],
                    sleep_probs[i],
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
    int nois[4] = {5, 18, 9, 24};
    int priorities[4] = {1,2,3,4};
    double sleep_probs[4] = {.5, .1, .8, .65};
    int sleep_times[4] = {3, 2, 5, 1};
    generate(4, 5, nois, priorities, sleep_probs, sleep_times);

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
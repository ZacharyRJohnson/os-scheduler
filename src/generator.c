#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include <string.h>
#include <errno.h>
#include <math.h>

void generate(int num_procs, int t, char* nois[num_procs], char* priorities[num_procs], char* sleep_probs[num_procs], char* sleep_times[num_procs]) {
    Process processes[num_procs];
    for (int i = 0; i < num_procs; i++) {
        int pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error while forking process: %s", strerror(errno));
            exit(1);
        }
        else if (pid == 0) {
            
            char *proc_args[] = {
                "./process",
                nois[i],
                priorities[i],
                sleep_probs[i],
                sleep_times[i]
            };
            execv(proc_args[0], proc_args);
        }
        else {
            
        }
    }
}

int main() {

}
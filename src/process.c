#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "process.h"
#include <sys/ipc.h>
#include <sys/msg.h>

struct mesg_buffer {
    long priority;
    pid_t pid;
};

int send_message(struct mesg_buffer thing) {
	key_t key;
    int msgid;
	key = ftok("./temp.txt", 65);
    if (key == -1) {
        printf("ERROR");
    }

    msgid = msgget(key, 0666 | IPC_CREAT);

	int a = msgsnd(msgid, &thing, sizeof(thing), 0);
	return a;
}

void cpu_proc(Process* proc) {
	struct mesg_buffer thing;
	thing.pid = proc->pid;
	thing.priority = proc->priority;
	int a = send_message(thing);

	printf("CPU BOUND %d %d \n", proc->priority, a);

	for (int i = 0; i < proc->noi; i++) {
		;
	}

	// pause();
	// a = msgsnd(msgid, &thing, sizeof(thing), 0);
	// printf("CPU BOUND %d %d \n", proc->pid, a);
}

void io_proc(Process* proc) {
	srand(time(0));
	for (int i = 0; i < proc->noi; i++) {
		// printf("%d, loop iteration %d\n", proc->pid, i);
		double prob = ((double) rand() / (RAND_MAX)) + 1;
		if (prob < proc->sleep_prob) {
			// Send IO signal to scheduler
			// printf("PID: %d Going for IO\n", proc->pid);
			sleep(proc->sleep_time);
			// printf("PID: %d Back from IO\n", proc->pid);	
			// Inform schedular via MSG Q
		}	
	}
}
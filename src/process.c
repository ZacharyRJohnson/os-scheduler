#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
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

Process* current;

void cpu_handler() {
	printf("Working on Process CPU:  %d %d \n", current->pid, current->noi);
	current->noi -= 3;
	sleep(2);

	struct mesg_buffer thing;
	thing.pid = current->pid;
	thing.priority = current->priority;
	if (current->noi > 0) {
		send_message(thing);
	}
}

void cpu_proc(Process* proc) {
	current = proc;
	signal(SIGCONT, cpu_handler);

	struct mesg_buffer thing;
	thing.pid = proc->pid;
	thing.priority = proc->priority;
	printf("CPU BOUND %d %d \n", current->pid, current->noi);
	int a = send_message(thing);

	while (current->noi > 0) {
		pause();
	}

	printf("Completed %d\n", current->pid);
}




void io_handler() {
	printf("Working on Process IO:  %d %d \n", current->pid, current->noi);
	

	struct mesg_buffer thing;
	thing.pid = current->pid;
	thing.priority = current->priority;

	// printf("%d, \n", proc->pid);
	srand(time(0));
	for (int i = 0; i < 3 && 0 < current->noi; i++) {
		double prob = (double)rand() / (double)RAND_MAX;
		if (prob < current->sleep_prob) {
			// Send IO signal to scheduler
			// Stops progress and sleeps for a time
			printf("PID: %d Going for IO\n", current->pid);
			sleep(current->sleep_time);
			break;
		}
		current->noi -= 1;
	}

	// Alert scheduler we want to work
	if (current->noi > 0) {
	sleep(2);
		send_message(thing);
	}
}


void io_proc(Process* proc) {
	current = proc;
	signal(SIGCONT, io_handler);

	struct mesg_buffer thing;
	thing.pid = proc->pid;
	thing.priority = proc->priority;
	printf("IO BOUND %d %d \n", current->pid, current->noi);
	int a = send_message(thing);

	while (current->noi > 0) {
		pause();
	}

	printf("Completed %d\n", current->pid);
}
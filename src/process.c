#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "process.h"

void cpu_proc(Process* proc) {
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

int main(int argc, char* argv[]) {
	
}
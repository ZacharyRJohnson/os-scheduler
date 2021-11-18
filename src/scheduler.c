#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
  
// structure for message queue
struct mesg_buffer {
    long priority;
    pid_t pid;
} message;


void round_robin(int msgid) {
    // Schedule infinitly
    while (1) {
        msgrcv(msgid, &message, sizeof(message), 0, 0);
        printf("Waking up: %d \n", 
                        message.pid);

        kill( message.pid, SIGCONT);
        sleep(2);
    }
}



void priority_round_robin(int msgid) {
    // Schedule infinitly
    while (1) {
        msgrcv(msgid, &message, sizeof(message), 0, 0);
        printf("Waking up: %d \n", 
                        message.pid);

        kill( message.pid, SIGCONT);
        sleep(2);
    }
}


int main(int argc, char* argv[]) {
    key_t key;
    int msgid;
    key = ftok("./temp.txt", 65);
    if (key == -1) {
        printf("ERROR");
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (argc == 2) {
        if (strcmp(argv[1], "PR") == 0) {
            priority_round_robin(msgid);
        }
        else if (strcmp(argv[1], "RR") == 0){
            round_robin(msgid);
        }
        else {
            fprintf(stderr, "Argument not recognized. Input 'PR' or 'RR' to specify scheduling algorithm, or nothing for round robin.\n");
            return 1;
        }
    }
    else if (argc == 1) {
        round_robin(msgid);
    }
    else {
        fprintf(stderr, "Argument not recognized. Input 'PR' or 'RR' to specify scheduling algorithm, or nothing for round robin.\n");
        return 1;
    }

    msgctl(msgid, IPC_RMID, NULL);
}
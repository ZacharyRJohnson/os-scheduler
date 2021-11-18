#include <signal.h>
#include <stdio.h>
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


int main() {
    key_t key;
    int msgid;
    key = ftok("./temp.txt", 65);
    if (key == -1) {
        printf("ERROR");
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    round_robin(msgid);

    msgctl(msgid, IPC_RMID, NULL);
}
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


int main() {
    key_t key;
    int msgid;
    key = ftok("./temp.txt", 65);
    if (key == -1) {
        printf("ERROR");
    }

    msgid = msgget(key, 0666 | IPC_CREAT);

    while (1) {
        msgrcv(msgid, &message, sizeof(message), 3, 0);
        printf("Data Received is : %d \n", 
                        message.pid);
        // sleep(1);
        // kill( message.pid, SIGUSR1);
    }

    msgctl(msgid, IPC_RMID, NULL);
    // kill( 14185, SIGUSR1);
    // kill( 14187, SIGUSR1);
}
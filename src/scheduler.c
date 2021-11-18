#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
  
// structure for message queue
struct mesg_buffer {
    long priority;
    pid_t pid;
} message;


typedef struct node {
    struct node* next;
    struct mesg_buffer* val;
} node;

void round_robin(int msgid) {
    struct node* root = NULL;
    struct node* last = NULL;

    // Schedule infinitly
    while (1) {
        struct msqid_ds buf;
        int num_messages = 0;

        do {
            msgctl(msgid, IPC_STAT, &buf);
            num_messages = buf.msg_qnum;
            if (num_messages > 0) {
                msgrcv(msgid, &message, sizeof(message), 0, 0);
                printf("LOADED %d \n", message.pid);
                struct mesg_buffer* val = malloc(sizeof(struct mesg_buffer));
                val->pid = message.pid;
                val->priority = message.priority;
                struct node* just = malloc(sizeof(struct node));
                just->val = val;
                just->next = NULL;
                if (last != NULL) {
                    last->next = just;
                    last = just;
                } else {
                    root = just;
                    last = just;
                }
            }
        } while (num_messages - 1 > 0);


        if (root != NULL) {
            printf("Waking up: %d \n", 
                            root->val->pid);

            struct node* temp = root;
            if (root == last) {
                last = NULL;
            }

            int c_pid = root->val->pid;
            root = root->next;

            free(temp->val);
            free(temp);
            
            kill( c_pid, SIGCONT);
            
            sleep(1);
        }
        sleep(1);
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
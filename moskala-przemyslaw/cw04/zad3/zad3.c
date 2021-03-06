//
// Created by przemek on 04.04.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <signal.h>
#include <string.h>

int PPID;


int recieved_from_child=0;
int recieved_from_parent=0;
int sent_to_parent=0;

void hdl(int sig, siginfo_t *siginfo, void *context){

    int s = siginfo->si_signo;
        if(siginfo->si_signo==SIGRTMIN+1) printf("aaaassd");
            //s=SIGUSR1;
        if(siginfo->si_signo==SIGRTMIN+2) printf("adasdfa");
            //s=SIGUSR2;
    switch(s){
        case SIGUSR1:
            if (getpid() == PPID) {
                recieved_from_child++;
                printf("Recieved SIGUSR1 from child.\ttotal(%d)\n", recieved_from_child);
            } else {
                recieved_from_parent++;
                printf("Recieved SIGUSR1 from parent.\ttotal(%d)\n", recieved_from_parent);
                kill(PPID, SIGUSR1);
            }
            break;


        case SIGUSR2:
            printf("Recieved %d from parent.\n", recieved_from_parent);
            exit(recieved_from_parent);
            break;
        case SIGCHLD:
            printf("Recieved %d from child.\n", recieved_from_child);
            exit(0);

            break;
        default:
            break;
    }
}


void sig_handler(int sig) {
    //sig==SIGRTMIN?SIGUSR1:sig;
   // sig==SIGRTMAX?SIGUSR2:sig;
    if(sig==SIGRTMIN+1) printf("rec");

    switch (sig) {



        case SIGUSR1:
            if (getpid() == PPID) {
                recieved_from_child++;
                printf("Recieved SIGUSR1 from child.\ttotal(%d)\n", recieved_from_child);
            } else {
                recieved_from_parent++;
                printf("Recieved SIGUSR1 from parent.\ttotal(%d)\n", recieved_from_parent);
                kill(PPID, SIGUSR1);
            }
            break;


        case SIGUSR2:
            printf("Recieved %d from parent.\n", recieved_from_parent);
            exit(recieved_from_parent);
            break;
        case SIGCHLD:
            printf("Recieved %d from child.\n", recieved_from_child);
            exit(0);

            break;
        default:
            break;


    }
}
void send_rt_signals(int L){
    int b=1;


}
void send_using_kill(int L, int SIG1, int SIG2){
    pid_t pid=fork();

    if (pid==0){

        while(1);

    }
    else if (pid>0){
        printf("%d rodzi utworzyl %d\n",PPID, pid);
        for(int i=0 ; i<L; i++){
            kill(pid,SIG1);
            printf("Parent sent SIGUSR1 (%d)\n",  i+1);
            usleep(100);
        }

        kill(pid, SIG2);
        wait(NULL);
    }

}
void send_RT(int L){
    pid_t pid=fork();

    if (pid==0){

        while(1);

    }
    else if (pid>0){

        for(int i=0 ; i<L; i++){
            kill(pid,SIGRTMIN+1);
            printf("Parent sent SIGTRM%d (%d)\n", SIGRTMIN+1, i+1);
            usleep(100);
        }

        kill(pid, SIGRTMIN+2);
        wait(NULL);
    }
}
void send_using_sigqueue(int L) {
    pid_t pid = fork();

    if (pid == 0) {

        while (1);

    } else if (pid > 0) {
        union sigval sv = {.sival_int= 0};
        for (int i = 0; i < L; i++) {
            sigqueue(pid, SIGUSR1, sv);
            printf("Parent sent SIGUSR1 (%d)\n", i + 1);
            usleep(1000);
        }

        sigqueue(pid, SIGUSR2, sv);
        wait(NULL);
    }
}
int main(int argc, char * argv[]){
    int L=atoi(argv[1]);
    int type=atoi(argv[2]);
    PPID=getpid();
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_sigaction = &hdl;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);

    int s =SIGRTMIN;
    for(s; s<SIGRTMAX; s++)
        sigaction(s,&act,NULL);

    switch(type){
        case 1:
            send_using_kill(L,SIGUSR1, SIGUSR2);
            break;
        case 2:
            send_using_sigqueue(L);
            break;
        case 3:
            send_RT(L);
            break;
        default:
            printf("Type don't recognized!");
            break;

    }


    return 0;
}
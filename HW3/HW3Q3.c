#include <stdio.h> //all includes here
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define FLAGS1 WUNTRACED | WCONTINUED
#define FLAGS2 WUNTRACED | WCONTINUED | WNOHANG
#define FLAGS3 WNOHANG
#define CONDITIONALS1 (!WIFEXITED(status))&&(!WIFSIGNALED(status))
#define CONDITIONALS2 ((!w)||((!WIFEXITED(status))&&(!WIFSIGNALED(status))))

// Main
int main(int argc, char *argv[]) {
    int i, status,parent_PID;
    pid_t pid, cpid, w;
    cpid = fork();
    
    if (cpid<0) return 1;
    else if (cpid == 0) {
        sleep (atoi(argv[2])); //assume argv[2] > 15
        exit(atoi(argv[1]));
    }
    else {
        do {
            w = waitpid(cpid, &status, WNOHANG);
            printf("parent= %d, child=%d, status=%d, w=%d\n", getpid(), cpid, status, w); //line A
            if (w==-1) exit(0);
            if (w) {
            if (WIFEXITED(status)) printf("exited naturally, status = %d\n", WEXITSTATUS(status));
            else if (WIFSIGNALED(status)) printf("killed by signal: %d\n",WTERMSIG(status));
            else if (WIFSTOPPED(status)) printf("stopped by signal %d\n",WSTOPSIG(status));
            else if (WIFCONTINUED(status))printf("continued\n");
            }
        }
        while (CONDITIONALS1);
        exit(1);
    }
}
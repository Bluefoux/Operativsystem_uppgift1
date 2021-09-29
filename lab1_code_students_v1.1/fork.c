#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t pid;
    pid_t pidC;
    unsigned i;
    unsigned niterations = 1000;
    pid = fork();
    if (pid != 0){
        pidC = fork();
    }
    if (pid == 0) { /*if the first child process run, print "A = i"*/
        for (i = 0; i < niterations; ++i)
            printf("A = %d, ", i);
     } 
    else if (pidC == 0){ /*if the second child process run, print "C = i"*/
        for (i = 0; i < niterations; ++i)
            printf("C = %d, ", i);
    }
    else{ /*if the parent process run, print "B = i"*/
        for (i = 0; i < niterations; ++i)
            printf("B = %d, ", i);
    }
    if (pid != 0 && pidC != 0){
        printf("\n First child process id = %d", pid);
        printf("\n Second child process id = %d", pidC);
    }
    printf("\n");
}

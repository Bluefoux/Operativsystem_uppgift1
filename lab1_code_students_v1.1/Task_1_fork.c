#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    /** 
    *** This program creates 2 child process
    *** The first child process print "A = 0...999"
    *** The second child process print "C = 0...999"
    *** The parent process print "B = 0...999". The parent process also print the process id of the cildren
    **/
    pid_t pid;
    pid_t pidC;
    unsigned i;
    unsigned niterations = 1000;
    pid = fork(); // Create the first process
    if (pid != 0){
        pidC = fork(); // Create the second process
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
        printf("\n First child process id = %d", pid); // print the first child process id
        printf("\n Second child process id = %d", pidC); // print the second child process id
    }
    printf("\n");
}

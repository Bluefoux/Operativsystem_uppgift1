#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> /* For O_* constants */

// Shared Variables
int professor[5] = {0, 1, 2, 3, 4};
sem_t chopsticks[5];

void take_chopstick(void* nmrprofessor){
    int timewait;
    int professnmr = *(int*)nmrprofessor;
    char* professorname;
    
    // Translate professor number to professor name
    if(professnmr == 0){
        professorname = "Tanenbaum";
    }
    else if(professnmr == 1){
        professorname = "Bos";
    }
    else if(professnmr == 2){
        professorname = "Lamport";
    }
    else if(professnmr == 3){
        professorname = "Stallings";
    }
    else{
        professorname = "Silberschatz";
    }

    while(1){
        // Wait 2-10 seconds (the professor think)
        timewait = ((rand() %8) +2);
        printf("Professor %s thinking for %d seconds \n", professorname, timewait);
        sleep(timewait); 

        // Take left chopstick
        sem_wait(&chopsticks[professnmr]); 
        printf("Professor %s took the left chopstick \n", professorname);

        // The professor think 1-3 seconds
        timewait = ((rand() %2) +1);
        printf("Professor %s thinking for %d seconds \n", professorname, timewait);
        sleep(timewait);

        // Take right chopstick
        sem_wait(&chopsticks[(professnmr+1)%5]);
        printf("professor %s took the right chopstick \n", professorname);

        // Eat for 10-20 seconds
        timewait = ((rand() %10) +10);
        printf("Professor %s eating for %d seconds \n", professorname, timewait);
        sleep(timewait);

        sem_post(&chopsticks[professnmr]); // Leave left chopstick
        sem_post(&chopsticks[(professnmr+1)%5]); // Leave right chopstick
    }
}

int main(){
    pthread_t children[5];
    int id = 0;


    for (id = 0; id < 5; id++){
        sem_init(&chopsticks[id], 0, 1); // initialize the threads
        pthread_create(&children[id], NULL, (void *)take_chopstick, (void*)&professor[id]); // Create thread
    }

    for (id = 0; id < 5; id++)
        pthread_join(children[id], NULL);
    return 0;
}
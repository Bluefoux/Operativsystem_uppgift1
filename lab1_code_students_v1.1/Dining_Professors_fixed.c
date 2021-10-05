#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> /* For O_* constants */

// Shared Variables
int nmr_occupied = 0;
int professor[5] = {0, 1, 2, 3, 4};
sem_t chopsticks[5];
sem_t occupied;

void take_chopstick(void* nmrprofessor){
    int timewait;
    int professnmr = *(int*)nmrprofessor;
    char* professorname;
    
    
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
        // Wait 2-10 seconds
        timewait = ((rand() %8) +2);
        printf("Professor %s thinking for %d seconds \n", professorname, timewait);
        sleep(timewait);

        // Take left chopstick
        sem_wait(&occupied);
        nmr_occupied++;
        sem_post(&occupied);
        printf("Professor %s tries to take the left chopstick \n", professorname);
        while(nmr_occupied > 4){
            sleep(1);
        }
        sem_wait(&chopsticks[professnmr]);
        printf("Professor %s took the left chopstick \n", professorname);

        // Wait for 1-3 seconds
        timewait = (rand() %2)+1;
        printf("Professor %s thinking for %d seconds \n", professorname, timewait);
        sleep(timewait);

        // Take right chopstick
        printf("Professor %s tries to take the right chopstick \n", professorname);
        sem_wait(&chopsticks[(professnmr+1)%5]);
        printf("professor %s took the right chopstick \n", professorname);

        // Eat for 10-20 seconds
        timewait = ((rand() %10) +10);
        printf("Professor %s eating for %d seconds \n", professorname, timewait);
        sleep(timewait);

        sem_wait(&occupied);
        nmr_occupied--;
        sem_post(&occupied);
        sem_post(&chopsticks[professnmr]);
        sem_post(&chopsticks[(professnmr+1)%5]);
    }
}

int main(){
    pthread_t children[5];
    int id = 0;
    sem_init(&occupied, 0, 1);

    for (id = 0; id < 5; id++){
        sem_init(&chopsticks[id], 0, 1);
        pthread_create(&children[id], NULL, (void *)take_chopstick, (void*)&professor[id]);
    }

    for (id = 0; id < 5; id++)
        pthread_join(children[id], NULL);
    
    //free(children);
    return 0;
}
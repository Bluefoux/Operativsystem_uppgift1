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
int test[5] = {-1, -1, -1, -1, -1};
//int chopstickcheck[5] = {1, 1, 1, 1, 1};
sem_t chopsticks[5];
//sem_t occupied;
//sem_t Right_occupied;

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
        // Wait 2-10 seconds
        timewait = ((rand() %8) +2);
        printf("Professor %s thinking for %d seconds \n", professorname, timewait);
        usleep(timewait);

        printf("Professor %s tries to take the left chopstick \n", professorname);

        // Take left chopstick
        test[professnmr] = sem_wait(&chopsticks[professnmr]);
        if(test[professnmr] != 0){
            int reminder = 0;
            for(int i=0; i<5; i++){
                if(test[i] == 0){
                    reminder =1;
                }
            }
            if(reminder != 1){
                sem_post(&chopsticks[professnmr]);
            }
        }
        else{
            printf("Professor %s took the left chopstick \n", professorname);

            // Wait for 1-3 seconds
            timewait = (rand() %2)+1;
            printf("Professor %s thinking for %d seconds \n", professorname, timewait);
            sleep(timewait);

            // Take right chopstick
            printf("Professor %s tries to take the right chopstick \n", professorname);
            
            test[(professnmr+1)%5] = sem_wait(&chopsticks[(professnmr+1)%5]);
            if(test[(professnmr+1)%5] != 0){
                int reminder = 0;
                for(int i=0; i<5; i++){
                    if(test[i] == 0){
                        reminder =1;
                    }
                }
                if(reminder != 1){
                    sem_post(&chopsticks[(professnmr+1)%5]);
                }
            }
            else{
                printf("professor %s took the right chopstick \n", professorname);

                // Eat for 10-20 seconds
                timewait = ((rand() %10) +10);
                printf("Professor %s eating for %d seconds \n", professorname, timewait);
                usleep(timewait);

                //nmr_occupied --; // decrease occupied left chopsticks
                sem_post(&chopsticks[(professnmr+1)%5]); // Leave right chopstick 
                sem_post(&chopsticks[professnmr]);// Leave left chopstick
            }
        }
    }
}

int main(){
    pthread_t children[5];
    int id = 0;
    //sem_init(&occupied, 0, 1); // initialize the threads
    //sem_init(&Right_occupied, 0, 1); // initialize the threads

    for(id=0; id<5; id++){
        sem_init(&chopsticks[id], 0, 1); // initialize the threads
    }

    for (id = 0; id < 1; id++){
        pthread_create(&children[id], NULL, (void *)take_chopstick, (void*)&professor[id]); // Create thread
    }

    for (id = 0; id < 5; id++)
        pthread_join(children[id], NULL);

    return 0;
}
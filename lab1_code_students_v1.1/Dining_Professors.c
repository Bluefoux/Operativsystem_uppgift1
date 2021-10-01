#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Shared Variables
int professor[5] = {0, 1, 2, 3, 4};
int chopsticks[5] = {0, 0, 0, 0, 0};


void take_chopstick(void* nmrprofessor){
    int timewait;
    int professnmr = *(int*)nmrprofessor;
    
    while(1){
        // Wait 2-10 seconds
        timewait = ((rand() %8) +2);
        printf("Professor nmr %d thinking for %d time \n", professnmr, timewait);
        sleep(timewait);

        // Take left chopstick
        chopsticks[professnmr] = 1;
        printf("professor nmr %d took the left chopstick \n", professnmr);

        // Wait for 1-3 seconds
        timewait = ((rand() %2) +1);
        printf("Professor nmr %d thinking for %d time \n", professnmr, timewait);
        sleep(timewait);

        // Take right chopstick
        chopsticks[professnmr+1] = 1;
        printf("professor nmr %d took the left chopstick \n", professnmr);

        // Eat for 10-20 seconds
        timewait = ((rand() %10) +10);
        printf("Professor nmr %d eating for %d time \n", professnmr, timewait);
        sleep(timewait);
    }
}

int main(){
    pthread_t children[5];
    int id = 0;

    //children = malloc( professor * sizeof(pthread_t) );
    for (id = 1; id < 5; id++){
        pthread_create(&children[id-1], NULL, (void *)take_chopstick, (void*)&professor[id]);
    }

    for (id = 1; id < 5; id++)
        pthread_join(children[id-1], NULL);
    
    //free(children);
    return 0;
}
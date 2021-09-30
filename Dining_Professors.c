#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// Shared Variables
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int dont = 1;

void waitfunction(){
    int timewait = ((rand() %8)+2); 
    sleep(timewait);
}

/*void* child(void* buf) {
    unsigned long childID = (unsigned long)buf;
    do1000Transactions(childID);
    return NULL;
}*/

int main(){
    pthread_t *children;
    unsigned long id = 0;
    unsigned long nThreads = 5;
    children = malloc( nThreads * sizeof(pthread_t) );
    for (id = 1; id < nThreads; id++)
        pthread_create(&(children[id-1]), NULL, child, (void*)id);
    
    pthread_cond_wait(&cond1, &lock)

    for (id = 1; id < nThreads; id++)
        pthread_join(children[id-1], NULL);
    
    free(children);
    pthread_mutex_destroy(&lock);
    return 0;
}
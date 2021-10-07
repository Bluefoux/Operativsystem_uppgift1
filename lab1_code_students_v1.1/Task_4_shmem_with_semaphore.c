#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> /* For O_* constants */

#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

const char *semName1 = "my_sema1";
const char *semName2 = "my_sema2";

int main(int argc, char **argv)
{
	/**
	*** The parent writes numbers from 0 to 99 to a shared buffer
	*** The child reads the numbers from the buffer
	**/
	struct shm_struct {
		int buffer[1];
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1, status;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
	// The semaphores makes sure the processes execute correctly
    sem_t *sem_id1 = sem_open(semName1, O_CREAT, O_RDWR, 1);
	sem_t *sem_id2 = sem_open(semName2, O_CREAT, O_RDWR, 0);

	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
			// The program will only send once and wait for the recieve before send again
            sem_wait(sem_id1);//wait until able to lock the semaphore
			// write to shmem
            var1++;
            printf("Sending %d\n", var1); fflush(stdout);
            shmp->buffer[0] = var1;
			randomnmr = (rand()%19)+1; // calculate waittime
			usleep(randomnmr*100000); // Wait a time between 100 and 2000 microseconds
            sem_post(sem_id2);//unlock the semaphore
		}
		shmdt(addr);// Detache from the shared memory
		shmctl(shmid, IPC_RMID, shm_buf);

        sem_close(sem_id1);// close sem_id1
		sem_close(sem_id2);// close sem_id2
		wait(&status);
		sem_unlink(semName1);
		sem_unlink(semName2);
	} 
	else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */
			// The program will only recieve once and wait for the send before recieve again
			sem_wait(sem_id2); //wait until able to lock the semaphore
			var2 = shmp->buffer[0];
			printf("Received %d\n", var2); fflush(stdout);
			randomnmr = (rand()%19)+1; // calculate waittime
			usleep(randomnmr*100000); // Wait a time between 100 and 2000 microseconds
            sem_post(sem_id1); //unlock the semaphore
		}
		shmdt(addr);// Detache from the shared memory
		shmctl(shmid, IPC_RMID, shm_buf);

        sem_close(sem_id1); // close sem_id1
		sem_close(sem_id2);// close sem_id2
	}
}
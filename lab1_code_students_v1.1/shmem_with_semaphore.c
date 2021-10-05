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
	struct shm_struct {
		int buffer[1];
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1, status;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
    sem_t *sem_id1 = sem_open(semName1, O_CREAT, O_RDWR, 1);
	sem_t *sem_id2 = sem_open(semName2, O_CREAT, O_RDWR, 0);

	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
            sem_wait(sem_id1);
            var1++;
            printf("Sending %d\n", var1); fflush(stdout);
            shmp->buffer[0] = var1;
            sem_post(sem_id2);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);

        sem_close(sem_id1);
		sem_close(sem_id2);
		wait(&status);
		sem_unlink(semName1);
		sem_unlink(semName2);
	} 
	else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */
			sem_wait(sem_id2);
			var2 = shmp->buffer[0];
			printf("Received %d\n", var2); fflush(stdout);
            sem_post(sem_id1);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);

        sem_close(sem_id1);
		sem_close(sem_id2);
	}
}
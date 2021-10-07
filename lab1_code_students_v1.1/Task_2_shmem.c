#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

int main(int argc, char **argv)
{
	/**
	*** The parent writes numbers from 0 to 99 to a shared buffer
	*** The child reads the numbers from the buffer
	**/
	struct shm_struct {
		int buffer[10];
		int buffernmr;
		int var2;
	};

	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, shmid = -1, buffernmrtwo = 0, randomnmr;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	shmp->buffernmr = 0;
	shmp->var2 = 0;
	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) 
		{
			if (var1 < (shmp->var2 + 9)){ // if the buffer contains less than 9 numbers
				/* write to shmem */
				var1++;
				printf("Sending %d\n", var1); fflush(stdout);
				shmp->buffer[shmp->buffernmr] = var1;
				shmp->buffernmr++;
				randomnmr = (rand()%19)+1; // calculate waittime
				usleep(randomnmr*100000); // Wait a time between 100 and 2000 microseconds
				if (shmp->buffernmr > 9){ // Makes the buffer circular
					shmp->buffernmr = 0;
				}
			}
		}
		shmdt(addr); // Detache from the shared memory
		shmctl(shmid, IPC_RMID, shm_buf);
	} 
	else {
		/* here's the child, acting as consumer */
		while (shmp->var2 < 100) {
			/* read from shmem */
			while (buffernmrtwo == shmp->buffernmr); /* busy wait until there is something */
				shmp->var2 = shmp->buffer[buffernmrtwo]; buffernmrtwo++;

			randomnmr = (rand()%19)+1; // calculate waittime
			usleep(randomnmr*100000); // Wait a time between 100 and 2000 microseconds
			if (buffernmrtwo > 9){ // Makes the buffer circular
				buffernmrtwo = 0;
			}
			printf("Received %d\n", shmp->var2); fflush(stdout);
		}
		shmdt(addr); // Detache from the shared memory
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}
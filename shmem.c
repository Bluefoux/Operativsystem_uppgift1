#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

int main(int argc, char **argv)
{
	struct shm_struct {
		int buffer[10];
		int buffernmr;
		int var2;
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, shmid = -1, buffernmrtwo = 0;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
	shmp->buffernmr = 0;
	shmp->var2 = 0;
	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
				
			if (var1 < (shmp->var2 + 9)){
				/* write to shmem */
				var1++;
				/*while (shmp->empty == 1); /* busy wait until the buffer is empty */
				printf("Sending %d\n", var1); fflush(stdout);
				shmp->buffer[shmp->buffernmr] = var1;
				shmp->buffernmr++;
				if (shmp->buffernmr > 9){
					shmp->buffernmr = 0;
				}
			}
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} 
	else {
		/* here's the child, acting as consumer */
		while (shmp->var2 < 100) {
			/* read from shmem */
			while (buffernmrtwo == shmp->buffernmr); /* busy wait until there is something */
				shmp->var2 = shmp->buffer[buffernmrtwo]; buffernmrtwo++;
			if (buffernmrtwo > 9){
				buffernmrtwo = 0;
			}
			printf("Received %d\n", shmp->var2); fflush(stdout);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}
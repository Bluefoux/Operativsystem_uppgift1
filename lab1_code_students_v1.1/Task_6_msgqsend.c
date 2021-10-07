#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   int mint;
};

int main(void) {
   /**
   *** The program creates a message queue to connect.
   *** The program generate 50 random values that are less 
   *** than max value of an int and less than min value of an int.
   *** These values are sent to the message queue.
   *** The program waits to terminate the message queue until user hit enter.
   **/
   struct my_msgbuf buf;
   int msqid;
   key_t key;
   size_t buflen;
   int systemRet = system("touch msgq.txt");
   if (systemRet== -1){
       printf("fel");
       return 0;
   }

   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }

   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to send messages.\n");
   
   buf.mtype = 1; /* we don't really care in this case */
   buf.mint = 0;
   for(int k=0; k< 50; k++){ //generate random values and send them to the message queue
      int tmp = (2*rand())-2147483648;
      if (tmp < 0){
         tmp += 1;
      }
      buf.mint = tmp;
      buflen = sizeof(buf);
      if (msgsnd(msqid, &buf, buflen, 0) == -1) /* +1 for '\0'*/
         perror("msgsnd");
   }

   printf("Run the receive program before press enter:\n");
   scanf("sdf"); // Wait to close the message queue until hit enter
   buf.mint = -2147483648; // Used to 
   if (msgsnd(msqid, &buf, buflen, 0) == -1) /* +1 for '\0' */ // Run send message last time to close the message queue
      perror("msgsnd");

   if (msgctl(msqid, IPC_RMID, NULL) == -1) { //terminate the message queue
      perror("msgctl");
      exit(1);
   }
 
   printf("message queue: done sending messages.\n");
   return 0;
}

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
   char mtext[200];
   //int mint[50];
};

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int toend;
   key_t key;
   size_t buflen;
   int i=0;

   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }



   if ((msqid = msgget(key, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }

      /*if (msgctl(msqid, IPC_RMID, NULL) == -1) {
         perror("msgctl");
         exit(1);
      }*/

   printf("message queue: ready to receive messages.\n");
    printf("len %ld" , sizeof(buf.mtext));
    printf("msqid %d" , msqid);

   for(;;) { /* normally receiving never ends but just to make conclusion */
             /* this program ends with string of end */
      //printf("%d \n", i);
      //i++;
      //buflen = sizeof(buf);
        printf("len %ld" , sizeof(buf.mtext));
        printf("msqid %d" , msqid);
      if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
         //printf(" %d \n %d \n", buf.mint[0], buf.mint[1]);
         perror("msgrcv");
         exit(1);
      }
      printf("recvd: \"%s\"\n", buf.mtext);
      int toend = strcmp(buf.mtext , "end");
      if(toend  == 0)
         break;
   }
   printf("message queue: done receiving messages.\n");
   //system("rm msgq.txt");
   return 0;
}

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
   struct my_msgbuf buf;
   int msqid;
   key_t key;
   size_t buflen;

   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }

   if ((msqid = msgget(key, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to receive messages.\n");

   for(;;) { /* normally receiving never ends but just to make conclusion */
             /* this program ends with int of -2147483648 */
      buflen = sizeof(buf);
      if (msgrcv(msqid, &buf, buflen, 0, 0) == -1) {
         perror("msgrcv");
         exit(1);
      }
      if(buf.mint == -2147483648){ // End the incoming messages
         break;
      }
      printf("recvd: \"%d\"\n", buf.mint);
      
   }
   printf("message queue: done receiving messages.\n");
   system("rm msgq.txt");
   return 0;
}

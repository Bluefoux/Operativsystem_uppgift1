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
   //char mtext[200];
   int mint[10];
};

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int len;
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
   printf("Enter lines of text, ^D to quit:\n");
   buf.mtype = 1; /* we don't really care in this case */
   buf.mint[1] = 0;
   for(int k=0; k< 50; k++){
      //for(int i=0; i < 50; i++){
      buf.mint[0] = k;
      //buf.mint[0] = rand();
      if(k == 49){
         buf.mint[1] = -1;
      }
      buflen = sizeof(buf);
      printf("len %ld \n", buflen);
      printf("%d \n", msqid);
      printf("%d \n", k);
      if (msgsnd(msqid, &buf, buflen, 0) == -1) /* +1 for '\0'*/
         perror("msgsnd");

      if (buf.mint[1] == -1){
         break;
      }
      if (msgsnd(msqid, &buf, buflen, 0) == -1) /* +1 for '\0' */
         perror("msgsnd");
      
   }
   
   /*
   while(fgets(buf.mint, sizeof buf.mint, stdin) != NULL) {
      len = strlen(buf.mtxt);
      
      /* remove newline at end, if it exists 
      */
   //strcpy(buf.mint, 0);
   //len = strlen(buf.mint);
   

   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
         perror("msgctl");
         exit(1);
      }
   
 
   printf("message queue: done sending messages.\n");
   return 0;
}

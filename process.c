#include "headers.h"
//april 29 last update
//argv[3] is the remaing time originally sent
//argv[4] is the new remaing time 
/*so the logic here will be that each time we enter the while loop
we will wait for one second then decrease the new_reamingtime by one and
update the argv array with the new reaming time a.
we will control process.c by sending 
SIGSTOP
SIGCONT
which will be sent by the kill(pid,SIGSTOP or SIGCONT) function 

*/ 
/* Modify this file as needed*/
int remainingtime;
void handleContinueSignal(int sig) {
    printf("The signal has continued the process\n");
    int  myGlobalStaticContinueVariable = 1; // Or some other handling code
}
void handleStopSignal(int sig) {
  printf("The signal has stopped the process\n");
   pause(); // Or some other handling code
}

int main(int agrc, char * argv[])
{   
    initClk(); 
    //signal(SIGSTOP,handleStopSignal);
    signal(SIGCONT,handleContinueSignal);
     printf("welcome to the procwess.c:%d\n",atoi(argv[2]));
    char buffer1[20];
    char buffer2[20];
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    int new_remaingtime;
    new_remaingtime= atoi(argv[3]);
    remainingtime= atoi(argv[3]);
    printf("this is the remining time in process:%d\n",remainingtime);

    while (remainingtime > 0)
    {
     // printf("inside the process.c while:%s",argv[3]);
        int intial_time=getClk();
        int second_has_passed=intial_time+1;
        //while(intial_time!=second_has_passed){
      // printf("inside the process.c while:%s",argv[3]);
        //}
        // remainingtime = ??;
        sleep(1);
        new_remaingtime--;
       //sprintf(buffer1, "%d", remainingtime);
        remainingtime--;
        sprintf(buffer2, "%d", new_remaingtime);
      //argv[3] = buffer1; 
        argv[4] = buffer2;
        printf("inside the process.c while:%s\n",argv[4]);
       argv[4] = buffer2;

      
    }
   // destroyClk(false);
  
    if(atoi(argv[2])==2)
    {
      kill(getppid(), SIGUSR1);
    }
    exit(0);
    return 0;
}

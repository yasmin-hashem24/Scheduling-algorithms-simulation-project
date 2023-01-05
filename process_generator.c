#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include "headers.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "Queue.h"


#define TRUE 1
#define FALSE 0
//added process strcuct in queue 
//modfied the Qnode to take structs 
//made an array of struct 
//fixed the excv command problem 
//msg buff now will send process strcut each time 
//msg buff will also have the alo type (may be of use later)
//added the sending and creating functions of msg queu
//april 29 
//add 1 to the process count se t sees the last messeage


void clearResources(int);


struct msgbuff
{
    long mtype;     //type of message
    //char process_prop[70];  //message text
   struct process p;
   //int algo_type;
};


int main(int argc, char * argv[])
{
    int key_id = ftok("key", 65);   
    int msg_queue_id = msgget(key_id, 0666 | IPC_CREAT);

    int send_val, msgq_id;

    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    int counter=0,num=0;
    
    FILE * pFile;
	pFile= fopen("processes.txt","r");

    if (pFile == NULL) {
        printf("no such file.");
        return 0;
    }

    char buffer[100];
    fgets(buffer, 100, pFile);

    //hazabat el asma2 msh mohem
    char line[100];
    int process_info[100];
    while (fscanf(pFile, "%s ", line) == 1){
    num=atoi(line);
    process_info[counter]=num;
    printf("printing in pd data :%d\n", num);
    //printf("%d\n", studentlist[counter]);
    counter++;
    }
    int process_count=  (counter/5)+1;
    //printf("%d",process_count);

    fclose(pFile);
       //creating an array of structs so we can send a a full process with each meassage  
    struct  process process_array[100];
 int j=0;
    for(int i=0;i<counter;i=i+5)
    {

        process_array[j].id=process_info[i];
       // printf("%d ", process_array[j].id);
        process_array[j].arrival=process_info[i+1];
        //printf(" %d", process_array[j].arrival);
        process_array[j].runtime=process_info[i+2];
               //printf(" %d", process_array[j].runtime);
        process_array[j].priority=process_info[i+3];
              // printf(" %d\n", process_array[j].priority);
         process_array[j].memory_size=process_info[i+4];
         printf("printing in pg :%d\n", process_array[j].pid);
            //process_array[j].status=-1;      
               j++;
    }
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
        int alg_num;
        printf("Please enter the schedueling algorithm: ");
        scanf("%d", &alg_num); //should be sent to the schedueler ba2a
    // 3. Initiate and create the scheduler and clock processes.
        int pid = fork();
    if (pid == 0)
        if (execv("./clk.out", argv) == -1)
            perror("failed to execv");
    // 4. Use this function after creating the clock process to initialize clock
   initClk();
  //  sleep(0);
   int x;


   pid = fork();
    if (pid == 0)
    {
        char buffer1[20];
        char buffer2[20];
        sprintf(buffer1, "%d", process_count);
        sprintf(buffer2, "%d", alg_num);
        argv[1] = buffer1; //process count
        argv[2] = buffer2;
         //argv[1] = process_count ;// needs to be fixeds
         if (execv("./s.out", argv) == -1)  //will be set free later 
      perror("failed to execv");
       
    }  
    // To get time use this
   

    //printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.

    // 6. Send the information to the scheduler at the appropriate time.
    
    for(int i=0;i<process_count-1;i++){
        while(getClk()<process_array[i].arrival){
          //  sleep(process_array[i].arrival-getClk());
        }
      printf("the process arrived at:%d\n",getClk());
    printf("the process id is:%d\n",process_array[i].id);
    //message sending:
    struct msgbuff message;
    message.p=process_array[i];
    //message.algo_type=alg_num;
    int send_val= msgsnd(msg_queue_id,&message,sizeof(message.p),!IPC_NOWAIT);
    }
    // 7. Clear clock resources
    sleep(70);    //lesa 3yzen nshof han sleep emta
    int delet_msg_queue = msgctl( msg_queue_id , IPC_RMID, 0);
    destroyClk(1);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    printf("the process generator has stopped\n");
    int msg_queue_id;
    int delet_msg_queue = msgctl( msg_queue_id , IPC_RMID, 0); //3ayza afham wtf is zes
   destroyClk(true);
    exit(0);
}

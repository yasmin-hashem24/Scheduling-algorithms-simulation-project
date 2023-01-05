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
#include<math.h>
////////////////////////////////////////////////////////////////////////
//we will set the least byte to be 8 byte 
//we will make counter for each type node 
//we will use the counters to record the start and end postion of each node 
//make the start and end postions public 
//added a new value to Qnode to get the memsize from pid memsize_value
//addedd start ad end variables tp proccess struct
//need to add them to enqueu and dequeu //done 
///////////////////////////////////////////////////////////////////////
/*
summary for phase 2
-added 3 variables in process struct start ,end,memory_size
-alocation is given root_tree ,memorysize ,id and alocation_flag variable should always set to zero before use
-dealloaction is given root_tree and id
-waiting queue and stopping when memory is full is still not implelmented


*/

int counter_512 = -1;
int counter_256 = -1;
int counter_128 = -1;
int counter_64 = -1;
int counter_32 = -1;
int counter_16 = -1;
int counter_8 = -1;
int alocation_flag=0;
int alocated_start=0;
int alocated_end=0;
struct node {

    int data;
    int flag;
    int id;
    int starting_mem;
    int ending_mem;
    struct node* left;
    struct node* right;
    struct node* parent;
  
};
 
/* tree_newNode() allocates a new node
with the given data and NULL left
and right pointers. */
struct node* tree_newNode(int data,int flag,int id, int starting_mem, int ending_mem)
{
    // Allocate memory for new node
    struct node* node
        = (struct node*)malloc(sizeof(struct node));
 
    // Assign data to this node
    node->data = data;
    node->flag=flag;
    node->id=id;
    node->starting_mem=starting_mem;
    node->ending_mem=ending_mem;
 
    // Initialize left and
    // right children as NULL
    node->left = NULL;
    node->right = NULL;
    node->parent=NULL;
    return (node);
}
void creattree(struct node* root,int i,int flag,int id){
    int counter=0;
    int start=0;
    //int end=0;
    if(i==8){
        return;
    }
   if(i==1024){
    counter_512++;
    counter=counter_512;
    start=512;
    }

       if(i==512){
    counter_256++;
    counter=counter_256;
    start=256;
    }
      if(i==256){
    counter_128++;
    counter=counter_128;
    start=128;
    }
    
       if(i==128){
    counter_64++;
    counter=counter_64;
    start=64;
    }
          if(i==64){
    counter_32++;
    counter=counter_32;
    start=32;
    }
              if(i==32){
    counter_16++;
    counter=counter_16;
    start=16;
    }
    
                  if(i==16){
    counter_8++;
    counter=counter_8;
    start=8;
    }
    




    root->left = tree_newNode(i/2,flag,id,counter*start,counter*start+start-1);
    start=0;
    counter=0;
  if(i==1024){
    counter_512++;
    counter=counter_512;
    start=512;
    }

       if(i==512){
    counter_256++;
    counter=counter_256;
    start=256;
    }
      if(i==256){
    counter_128++;
    counter=counter_128;
    start=128;
    }
    
       if(i==128){
    counter_64++;
    counter=counter_64;
    start=64;
    }
          if(i==64){
    counter_32++;
    counter=counter_32;
    start=32;
    }
              if(i==32){
    counter_16++;
    counter=counter_16;
    start=16;
    }
    
                  if(i==16){
    counter_8++;
    counter=counter_8;
    start=8;
    }
    
    //printf("%d\n",i/2);
    root->right = tree_newNode(i/2,flag,id,counter*start,counter*start+start-1);
    //printf("%d\n",i/2);
    if(root->left!=NULL || root->right!=NULL)
    {
    root->left->parent=root;
    root->right->parent=root;
    }
     creattree(root->left,i/2,flag,id);
     creattree(root->right,i/2,flag,id);


}
void printtree(struct node* root){
    if(root==NULL){

        return;
    }
printf("the node data:%d,,the node start:%d,,the node end:%d\n",root->data,root->starting_mem,root->ending_mem);
printtree(root->left);
printtree(root->right);

}
int size(int size_num){
    for(int i=3;i<=10;i++){
if(pow(2,i)>=size_num)
return pow(2,i);

    }

}
 int alocation (struct node* root,int memsize,int id){
int meme_value=size(memsize);
//printf("memsize:%d\n",meme_value);
if(root==NULL){
    return 0;
}

if(root->flag==1){
    printf("failed to alocate\n");
    return 0;
}
if(alocation_flag==1){
    return 0;
}

if(root->left==NULL){
if(root->data==meme_value&&root->flag==0){
root->flag=1;
root->id=id;
printf("alocated start %d,,,,,,,,,,,,,,,,,end %d\n",root->starting_mem,root->ending_mem);
//so what we do here is that evey node has its start and end psotions so we send them to a global variable 
//then this globa variable is sent to the qnode of the process
alocated_start=root->starting_mem;
alocated_end=root->ending_mem;
struct node* temp=root;
while(temp->parent != NULL){
    temp->parent->flag=-1;
    temp=temp->parent;
}
alocation_flag=1;
return 1;
}
if((root->data==meme_value)&&(root->flag!=0)){
printf("failed to alocate\n");
return 0;

}


}
if(root->data==meme_value&&root->flag==0&&root->left->flag==0&&root->right->flag==0){
root->flag=1;
root->id=id;
printf("alocated start %d,,,,,,,,,,,,,,,,,end %d\n",root->starting_mem,root->ending_mem);
alocated_start=root->starting_mem;
alocated_end=root->ending_mem;
struct node* temp=root;
while(temp->parent != NULL){
    temp->parent->flag=-1;
    temp=temp->parent;
}
alocation_flag=1;
return 1;
}
if((root->data==meme_value)&&(root->flag!=0||root->left->flag==1||root->right->flag==1)){
printf("failed to alocate\n");
return 0;

}

alocation( root->left,memsize,id) ;
alocation( root->right,memsize,id) ;


 }

 int dealocation(struct node* root,int id)
 {
    if(root==NULL){
        return 0;
    }


    if(root->id==id)
    {
        root->id=0;
        root->flag=0;
        struct node* temp=root;
        while(temp->parent != NULL){
            if(temp->parent->left->flag==0 && temp->parent->right->flag==0)
            {
            temp->parent->flag=0;
            }
            temp=temp->parent;
        }
        printf("dealocated\n");
        return 1;
    }
    dealocation( root->left,id) ;
    dealocation( root->right,id) ;
    
 }



////////////////////////////////////////////////////////////////////////phase 2 variables
  // const struct node* root = tree_newNode(1024,0,0,0,0);
    //creattree(root,1024,0,0);

////////////////////////////////////////////////////////////////////////
FILE *fptr;
FILE *fptr2;
FILE *memptr;
/// for us all
int useful = 0; // runtime of all
int total = 0;  // total time
int avgwta = 0;
int avgwait = 0;


struct QNode current_process;
struct node *root;

void handlestopsignal(int sig)
{
  if (current_process.p_info.id < 0)
  {
    current_process.p_info.id = current_process.p_info.id * -1;
  }

  printf("The signal has finished the process\n");

  // calculating turnaround finish-arrival
  current_process.turnaround_time = (getClk()) - (current_process.p_info.arrival);
  dealocation(root,current_process.p_info.id);
  fprintf(memptr,"At time %d freed %d bytes for process %d from %d to %d\n",getClk(),current_process.p_info.memory_size,current_process.p_info.id,current_process.p_info.start, current_process.p_info.end);
  // calculating weighted turn around //ta-runtime
  current_process.weightedturnaround_time = current_process.turnaround_time/current_process.p_info.runtime;
  // calculating useful time
  useful = (current_process.p_info.runtime) + useful;
  printf("useful: %d\n", useful);
  // calculating total time
  total = getClk();
  printf("total: %d\n", total);
  // calculating summation of avg  weighted turn around time
  current_process.waiting_time=(getClk()-current_process.p_info.arrival)-(current_process.p_info.runtime-current_process.Remaining_time);
    printf("remaining time: %d\n", current_process.Remaining_time);
  avgwta = avgwta + (current_process.weightedturnaround_time);
  printf("avgwta: %d\n", avgwta);
  // calculating summation of avg waiting time
  avgwait = avgwait + (current_process.waiting_time);
  printf("avgwait: %d\n", avgwait);

  fprintf(fptr, "At time %d process %d finished wait %d TA %d WTA %d\n", getClk(), current_process.p_info.id,  current_process.waiting_time, current_process.turnaround_time, current_process.weightedturnaround_time);

  printf("At time %d process %d finished wait %d TA %d WTA %d\n", getClk(), current_process.p_info.id, current_process.waiting_time, current_process.turnaround_time, current_process.weightedturnaround_time);
}

struct msgbuff
{
  long mtype; // type of message
  struct process p;
  // int algo_type;
};

int main(int argc, char *argv[])
{
  initClk();
  signal(SIGUSR1, handlestopsignal);
  fptr = fopen("schedular.log", "w");

  // fprintf(fptr, "#At time x process y state arr w total z remian y wait k \n");

  fptr2 = fopen("scheduler.perf", "w");
  memptr=fopen("memorysize.log","w");


  // TODO implement the scheduler :)

  // variables
  char buffer[20];
  int pid = 0;
  int alg_num;
  int process_count;
  int qcounter = 0;
  int timer = 0;
  //////////////////////////////////////////////phase 2 variable
struct node* root_tree=tree_newNode(1024,0,0,0,0);
root=root_tree;
creattree(root_tree,1024,0,0);
printtree(root_tree);

  // printf("%d\n", argc);

  process_count = atoi(argv[1]);
  //printf("process_count=%d\n",process_count);
    alg_num = atoi(argv[2]);
  if (alg_num == 2)
  {

    fprintf(fptr, "#PHPF \n");
  }
  // printf("\nprocess count:%d\n", process_count);
  // printf("\nalg num:%d\n", alg_num);

  // upon termination release the clock resources.
  // YASMINAZ VARIABLES
  int quantum = 3;
  struct QNode *process_temp;

  ///////////////////////
  // yahia vars
  struct QNode *mlf_process_temp;
  struct QNode *mlf_process_temp1;

  struct Queue *mlf_pri_Queue = createQueue();
  struct Queue *mlf_buffer = createQueue();
  int new_process_count;
  int currentlvl;
  int *oldpris = malloc((process_count + 1) * sizeof(int));
  int *runnings = malloc((process_count + 1) * sizeof(int));
  struct QNode *probuffer;
  int finished_pros = 0;

  // MAHMOUZAS VARIABLES

  // defined variables done by mahy for sjf

  int timerm = 0;
  struct QNode *print_process;
  int track_arrivalprev = -1;
  int pinrun = 1;
  int trackarrivalafter = -1;

  ///
  // recieving:
  int rec_val;
  int key_id = ftok("key", 65);
  int msg_queue_id = msgget(key_id, 0666 | IPC_CREAT);

  struct msgbuff message;

  // queue to put the recieved data in:
  struct Queue *process_queue = createQueue();
  struct Queue *priority_process_queue = createQueue();
  struct Queue *runtime_process_queue = createQueue();

  // printf("process stat finished:%d\n",current_process.stat=FINSIHED);

  while (qcounter < process_count - 1 || is_empty(priority_process_queue) == false || is_empty(runtime_process_queue) == false || is_empty(process_queue) == false || is_empty(mlf_pri_Queue) == false)
  { // printf("%d:%d\n",timer,getClk());

    // recieve all types of messages
    if (qcounter < process_count - 1)
    {

      do
      {
        rec_val = msgrcv(msg_queue_id, &message, sizeof(message.p), 0, IPC_NOWAIT); // doesnt wait for the messages

        if ((rec_val == -1) && ((is_empty(process_queue) == 1) || (is_empty(priority_process_queue) == 1) || (is_empty(runtime_process_queue) == 1) || (is_empty(mlf_pri_Queue) == 1))) // makes sure that at least one message is recieved so waits for it
        {
          rec_val = msgrcv(msg_queue_id, &message, sizeof(message.p), 0, !IPC_NOWAIT);
        }
        if (rec_val != -1)
        {
          printf("\nMessage received: %d\n", message.p.id);
          // fflush(stdout);
          qcounter++;

          enQueue(process_queue, message.p.id, message.p.arrival, message.p.runtime, message.p.priority, message.p.pid, WAITING,0,0,message.p.memory_size);
          prienQueue(priority_process_queue, message.p.id, message.p.arrival, message.p.runtime, message.p.priority, message.p.pid, WAITING,0,0,message.p.memory_size);
          prienQueue(runtime_process_queue, message.p.id, message.p.arrival, message.p.runtime, message.p.runtime, message.p.pid, WAITING,0,0,message.p.memory_size);
          prienQueue(mlf_pri_Queue, message.p.id, message.p.arrival, message.p.runtime, message.p.priority, message.p.pid, WAITING,0,0,message.p.memory_size);
          oldpris[message.p.id] = message.p.priority;
          runnings[message.p.id] = message.p.runtime;
          //process_queue->front->p_info.memory_size=process_queue->front->p_info.pid;
          //priority_process_queue->front->p_info.memory_size=priority_process_queue->front->p_info.pid;
          //runtime_process_queue->front->p_info.memory_size=runtime_process_queue->front->p_info.pid;
         // mlf_pri_Queue->front->p_info.memory_size=mlf_pri_Queue->front->p_info.pid;
          
          // priority_process_queue->front->process_status=WAITING;
           
          printf("the memsize:%d,,call the memsize:%d\n",process_queue->front->p_info.memory_size,size(process_queue->front->p_info.memory_size));
          //display(runtime_process_queue);
          //printf("%d\n", runtime_process_queue->front->p_info.runtime);
        }

      } while (rec_val != -1); // if no messages then complete else get them all at the same arrival time

      if (qcounter == process_count)
      {
        // break;
      }
    }

    if (!is_empty(runtime_process_queue) || !is_empty(priority_process_queue) || !is_empty(process_queue) || !is_empty(mlf_pri_Queue))
    { // MAHY EDITED HEREEEEEEEEEEEEEEEEEEEEEEEE
      switch (alg_num)
      {

      case 1: // SJF
       while(mlf_pri_Queue->front!=NULL){  //not sure anhy queue fehom
        deQueue(mlf_pri_Queue);
        deQueue(process_queue);
        deQueue(priority_process_queue);
      }
       
        sprintf(buffer, "%d", runtime_process_queue->front->p_info.runtime);
        argv[3] = buffer;
        print_process = deQueue(runtime_process_queue);


        //before forking, we will allocate in memory:
        alocation_flag=0;
          
          alocation(root_tree,print_process->p_info.memory_size,print_process->p_info.id);
          print_process->p_info.start=alocated_start;
          print_process->p_info.end=alocated_end;
          fprintf(memptr,"At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),print_process->p_info.memory_size,print_process->p_info.id,print_process->p_info.start, print_process->p_info.end);
          
        pid = fork();
        if (pid == 0)
        {
          if (execv("./p.out", argv) == -1)
            perror("failed to execv");
        }
        else
        {
          print_process->start_time = getClk();
          printf("we enterd the switch case and this is the pid number:%d \n", pid);
          // waiting time rule  //lesa 3yza a3delha
          print_process->waiting_time = print_process->start_time - print_process->p_info.arrival;
          print_process->p_info.pid = pid;
          fprintf(fptr, "at time %2d process %2d started  arr  %2d  remain %d  wait %2d \n", getClk(), print_process->p_info.id, print_process->p_info.arrival, print_process->p_info.runtime, print_process->waiting_time);
          printf("at time %2d process %2d started  arr  %2d  remain %d  wait %2d \n", getClk(), print_process->p_info.id, print_process->p_info.arrival, print_process->p_info.runtime, print_process->waiting_time);
        }

        if (print_process->p_info.runtime != 0)
        {
          sleep(print_process->p_info.runtime);
          //here we will deallocate:
          dealocation(root_tree,print_process->p_info.id);
          fprintf(memptr,"At time %d freed %d bytes for process %d from %d to %d\n",getClk(),print_process->p_info.memory_size,print_process->p_info.id,print_process->p_info.start, print_process->p_info.end);


          print_process->finish_time = getClk();
          print_process->process_status == FINSIHED;

          // calculating turnaround finish-arrival
          print_process->turnaround_time = (print_process->finish_time) - (print_process->p_info.arrival);

          // calculating weighted turn around //ta-runtime
          print_process->weightedturnaround_time = (print_process->turnaround_time) / (print_process->p_info.runtime);

          // calculating useful time
          useful = (print_process->p_info.runtime) + useful;
          printf("useful: %d\n", useful);
          // calculating total time
          total = getClk();
          printf("total: %d\n", total);
          // calculating summation of avg  weighted turn around time
          avgwta = avgwta + (print_process->weightedturnaround_time);
          printf("avgwta: %d\n", avgwta);
          // calculating summation of avg waiting time
          avgwait = avgwait + (print_process->waiting_time);
          printf("avgwait: %d\n", avgwait);

          print_process->p_info.runtime = 0;
          fprintf(fptr, "at time %2d process %2d finished  arr  %2d  remain %2d  wait %2d  TA %2d  WTA %2d\n", getClk(), print_process->p_info.id, print_process->p_info.arrival, print_process->p_info.runtime, print_process->waiting_time, print_process->turnaround_time, print_process->weightedturnaround_time);
          printf("at time %2d process %2d finished  arr  %2d  remain %2d  wait %2d  TA %2d  WTA %2d\n", getClk(), print_process->p_info.id, print_process->p_info.arrival, print_process->p_info.runtime, print_process->waiting_time, print_process->turnaround_time, print_process->weightedturnaround_time);
        }

        break;
      case 2: // HPF
        // statments
        while(process_queue->front!=NULL){
        deQueue(runtime_process_queue);
        deQueue(process_queue);
        deQueue(mlf_pri_Queue);
        }
        if (qcounter == 1)
        {
          // printf("qcounter=%d\n",qcounter);
          timer == getClk();

          current_process.p_info = priority_process_queue->front->p_info;

          priority_process_queue->front->p_info.pid = 11;
        }

        // display(priority_process_queue);ru
        // printf("%d:%d\n",timer,getClk());
        // printf("process stat:%d\n",current_process.stat);

        if (timer <= getClk())
        {
             current_process.Remaining_time=current_process.p_info.runtime;

          sprintf(buffer, "%d", priority_process_queue->front->p_info.runtime);
          argv[3] = buffer;

         // fprintf(memptr,"at")
          if (priority_process_queue->front->p_info.id >= 0)
          {
                      alocation_flag=0;
          alocation(root_tree,priority_process_queue->front->p_info.memory_size,priority_process_queue->front->p_info.id);
          priority_process_queue->front->p_info.start=alocated_start;
          priority_process_queue->front->p_info.end=alocated_end;
          fprintf(memptr,"At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),priority_process_queue->front->p_info.memory_size,priority_process_queue->front->p_info.id,priority_process_queue->front->p_info.start, priority_process_queue->front->p_info.end);
            priority_process_queue->front->p_info.pid = fork();
            if (priority_process_queue->front->p_info.pid == 0)
              if (execv("./p.out", argv) == -1)
                perror("failed to execv");
            // display(process_queue);
            printf("we enterd the switch case and this is the pid number:%d\n", priority_process_queue->front->p_info.pid);
            // sleep(100);

            timer = getClk() + priority_process_queue->front->p_info.runtime + 1;
            display(priority_process_queue);
          
            current_process.p_info = priority_process_queue->front->p_info;
            current_process.waiting_time = getClk() - current_process.p_info.arrival;
            //added start_mem and end mem to the currnet process variables 
            current_process.p_info.start=priority_process_queue->front->p_info.start;
             current_process.p_info.end=priority_process_queue->front->p_info.end;
            fprintf(fptr, "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current_process.p_info.id, current_process.p_info.arrival, current_process.p_info.runtime, current_process.p_info.runtime, current_process.waiting_time);
            deQueue(priority_process_queue);
            printf("apfter the dequee\n");
            display(priority_process_queue);
            printf("current process start:%d end:%d\n",current_process.p_info.start,current_process.p_info.end);

            printf("innn:%d:%d\n", timer, getClk());
          }
          else
          { // we enter here if the process was privously running
            // we will change the current process variable with the process on the top of the queue
            // printf("this process was working before\n");
            
            current_process.p_info = priority_process_queue->front->p_info;
            timer = timer - 1 - getClk() - current_process.p_info.runtime;
            timer = getClk() + priority_process_queue->front->p_info.runtime + 1;
            kill(current_process.p_info.pid, SIGCONT);
            current_process.p_info.id = -1 * current_process.p_info.id;
           
            current_process.p_info = priority_process_queue->front->p_info;
            current_process.waiting_time = getClk() - current_process.p_info.arrival;
            current_process.p_info.start=priority_process_queue->front->p_info.start;
             current_process.p_info.end=priority_process_queue->front->p_info.end;

          

            fprintf(fptr, "At time %d process %d resumed arr %d total %d remain %d wait %d \n", getClk(), -1 * current_process.p_info.id, current_process.p_info.arrival, current_process.p_info.runtime, current_process.Remaining_time, current_process.waiting_time);
            display(priority_process_queue);
            deQueue(priority_process_queue);
            display(priority_process_queue);
          }
          // sleep(2);
          // kill(current_process.pid,SIGSTOP);
          // sleep(2);
          // kill(current_process.pid,SIGCONT);

          // sleep(message.p.runtime);
          // kill(pid,SIGSTOP);
        }
        else
        {
          if (current_process.p_info.priority > priority_process_queue->front->p_info.priority)
          {
            printf("we entered the condition\n");
            // sleep(2);
            current_process.Remaining_time=timer-getClk();
            current_process.waiting_time = getClk() - current_process.p_info.arrival;

            fprintf(fptr, "At time %d process %d stopped arr %d total %d remain %d wait %d \n", getClk(), current_process.p_info.id, current_process.p_info.arrival, current_process.p_info.runtime, current_process.Remaining_time, current_process.waiting_time);
            kill(current_process.p_info.pid, SIGSTOP);
            current_process.p_info.id = -1 * current_process.p_info.id;
  

            // sleep(2);
            // printf("%d\n",kill(11,SIGCONT));
            prienQueue(priority_process_queue, current_process.p_info.id, current_process.p_info.arrival, current_process.p_info.runtime, current_process.p_info.priority, current_process.p_info.pid, WAITING,current_process.p_info.start,current_process.p_info.end,current_process.p_info.memory_size);
       
            
            timer = timer - 1 - getClk() - current_process.p_info.runtime;
            timer = getClk() + priority_process_queue->front->p_info.runtime + 1;

            display(priority_process_queue);

            current_process.p_info = priority_process_queue->front->p_info;

            // kill(current_process.pid,SIGSTOP);
            sprintf(buffer, "%d", priority_process_queue->front->p_info.runtime);
            argv[3] = buffer;
              alocation_flag=0;
          alocation(root_tree,priority_process_queue->front->p_info.memory_size,priority_process_queue->front->p_info.id);
          priority_process_queue->front->p_info.start=alocated_start;
          priority_process_queue->front->p_info.end=alocated_end;
          fprintf(memptr,"At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),priority_process_queue->front->p_info.memory_size,priority_process_queue->front->p_info.id,priority_process_queue->front->p_info.start, priority_process_queue->front->p_info.end);
            priority_process_queue->front->p_info.pid = fork();
            if (priority_process_queue->front->p_info.pid == 0)
              if (execv("./p.out", argv) == -1)
                perror("failed to execv");
         
          current_process.p_info.start=priority_process_queue->front->p_info.start;
             current_process.p_info.end=priority_process_queue->front->p_info.end;

            current_process.p_info = priority_process_queue->front->p_info;
            deQueue(priority_process_queue);
            display(priority_process_queue);
            current_process.waiting_time = getClk() - current_process.p_info.arrival;
                       
            fprintf(fptr, "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current_process.p_info.id, current_process.p_info.arrival, current_process.p_info.runtime, current_process.p_info.runtime, current_process.waiting_time);
          }
        }

        break;
      case 3: // RR
      while(mlf_pri_Queue->front!=NULL){
        deQueue(mlf_pri_Queue);
        deQueue(runtime_process_queue);
        deQueue(priority_process_queue);
      }


        process_temp = deQueue(process_queue);
        printf("this is process %d memory size %d\n",process_temp->p_info.id,process_temp->p_info.memory_size);
        sprintf(buffer, "%d", process_temp->p_info.runtime);
        argv[3] = buffer;
        // statments

        // case 1 : ready queue
        if (process_temp->process_status == WAITING) // LSA BAD2A AWEL MARA T RUN
        {


          alocation_flag=0;
          
          alocation(root_tree,process_temp->p_info.memory_size,process_temp->p_info.id);
          process_temp->p_info.start=alocated_start;
          process_temp->p_info.end=alocated_end;
          fprintf(memptr,"At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),process_temp->p_info.memory_size,process_temp->p_info.id,process_temp->p_info.start, process_temp->p_info.end);
          pid = fork();
          if (pid == 0)
          {
            if (execv("./p.out", argv) == -1)
            {
              perror("failed to execv");
            }
          }

          else

          {

            process_temp->p_info.priority = process_temp->p_info.runtime;
           
            printf("we enterd the switch case and this is the pid number:%d \n", pid);
             process_temp->waiting_time=  (getClk()- process_temp->p_info.arrival)-( process_temp->p_info.priority-process_temp->p_info.runtime);

            process_temp->p_info.pid = pid;
            fprintf(fptr, "at time: %d process: %d started , arr = %d , remain= %d , wait= %d \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
            // process_temp->process_status=RUNNING ;
            // currently have no idea wtf
            printf("at time: %d process: %d started , arr = %d , remain= %d , wait= %d \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
          
          
          
          }
        }

        else if (process_temp->process_status == STOPPED) // runnet abl kda
        {

          

          kill(process_temp->p_info.pid, SIGCONT); // sigcont btkml
          // process_temp->process_status=RUNNING ;
           process_temp->waiting_time=  (getClk()- process_temp->p_info.arrival)-( process_temp->p_info.priority-process_temp->p_info.runtime);
          //process_temp->waiting_time = (getClk() - process_temp->p_info.arrival) - (process_temp->stopped_time);

          fprintf(fptr, "at time: %d process: %d resumed , arr = %d , remain= %d , wait= %d \n ", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
          printf("at time: %d process: %d resumed , arr = %d , remain= %d , wait= %d \n ", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
        
         
        
        }

        if ((process_temp->p_info.runtime <= quantum) && (process_temp->p_info.runtime != 0))
        {
          sleep(process_temp->p_info.runtime);
          
          dealocation(root_tree,process_temp->p_info.id);
          fprintf(memptr,"At time %d freed %d bytes for process %d from %d to %d\n",getClk(),process_temp->p_info.memory_size,process_temp->p_info.id,process_temp->p_info.start, process_temp->p_info.end);

          process_temp->stopped_time = getClk();
          process_temp->process_status == FINSIHED;
          process_temp->p_info.runtime = 0;
          // process_temp->waiting_time= (getClk()- process_temp->p_info.arrival)-( process_temp->Remaining_time-process_temp->p_info.runtime);
          process_temp->finish_time = getClk();
          // calculating turnaround finish-arrival

          process_temp->turnaround_time = getClk() - (process_temp->p_info.arrival);
          

          // calculating weighted turn around //ta-runtime
          process_temp->weightedturnaround_time = process_temp->turnaround_time / (process_temp->p_info.priority);
          // calculating useful time
          useful = (process_temp->p_info.priority) + useful;
          printf("useful: %d\n", useful);
          // calculating total time
          total = getClk();
          printf("total: %d\n", total);
          // calculating summation of avg  weighted turn around time
          avgwta = avgwta + (process_temp->weightedturnaround_time);
          printf("avgwta: %d\n", avgwta);
          // calculating summation of avg waiting time
          avgwait = avgwait + (process_temp->waiting_time);
          printf("avgwait: %d\n", avgwait);


          fprintf(fptr, "at time: %d process: %d finished , arr = %d , remain= %d , wait= %d TA %d  WTA %d \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time, process_temp->turnaround_time, process_temp->weightedturnaround_time);
          printf("at time: %d process: %d finished , arr = %d , remain= %d , wait= %d TA %d  WTA %d \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time, process_temp->turnaround_time, process_temp->weightedturnaround_time);
        }
        else if (process_temp->p_info.runtime > quantum)
        {
          sleep(quantum);

          kill(process_temp->p_info.pid, SIGSTOP);

          process_temp->p_info.runtime = process_temp->p_info.runtime - quantum;

          process_temp->process_status == STOPPED;
          // process_temp->waiting_time= (getClk()- process_temp->p_info.arrival)-( process_temp->Remaining_time-process_temp->p_info.runtime);
          process_temp->stopped_time = getClk();
          
       
          enQueue(process_queue, process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->p_info.priority, process_temp->p_info.pid, STOPPED,process_temp->p_info.start,process_temp->p_info.end,process_temp->p_info.memory_size);
          fprintf(fptr, "at time:  %d process:  %d stopped , arr = %d , remain =  %d , wait=  %d  \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
          printf("at time:  %d process:  %d stopped , arr = %d , remain =  %d , wait=  %d  \n", getClk(), process_temp->p_info.id, process_temp->p_info.arrival, process_temp->p_info.runtime, process_temp->waiting_time);
        }

        break;
      case 4: // MLF


        mlf_process_temp = deQueue(mlf_pri_Queue);
        sprintf(buffer, "%d", mlf_process_temp->p_info.runtime);
        
        //mlf_process_temp = deQueue(mlf_pri_Queue);

        argv[3] = buffer;
        currentlvl = mlf_process_temp->p_info.priority;
        if (currentlvl != 11)
          printf("level: %d\n", currentlvl);

        if (mlf_process_temp->process_status == WAITING)
        {
          // mlf_process_temp->oldpriority = mlf_process_temp->p_info.priority;
          //printf("priority: %d\n", oldpris[mlf_process_temp->p_info.id]);

            alocation_flag=0;
          alocation(root_tree,mlf_process_temp->p_info.memory_size,mlf_process_temp->p_info.id);
          mlf_process_temp->p_info.start=alocated_start;
          mlf_process_temp->p_info.end=alocated_end;
          fprintf(memptr,"At time %d allocated %d bytes for process %d from %d to %d\n",getClk(),mlf_process_temp->p_info.memory_size,mlf_process_temp->p_info.id,mlf_process_temp->p_info.start, mlf_process_temp->p_info.end);
          pid = fork();
          if (pid == 0)
          {
            if (execv("./p.out", argv) == -1)
            {
              perror("failed to execv");
            }
          }
          mlf_process_temp->p_info.pid = pid;
          mlf_process_temp->process_status = RUNNING;
          mlf_process_temp->start_time= getClk();
          printf("process %d has been started\n", mlf_process_temp->p_info.id);
          fprintf(fptr, "at time %d process %d started  arr  %d  remain %d  wait %d \n", getClk(), mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->waiting_time);
        }
        else
        {
          kill(mlf_process_temp->p_info.pid, SIGCONT);
          mlf_process_temp->process_status = RUNNING;
          mlf_process_temp->waiting_time=  (getClk()- mlf_process_temp->p_info.arrival)-( runnings[mlf_process_temp->p_info.id]-mlf_process_temp->p_info.runtime);
          printf("process %d has been continued\n", mlf_process_temp->p_info.id);
          fprintf(fptr, "at time %d process %d resumed  arr  %d  remain %d  wait %d \n", getClk(), mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->waiting_time);
        }
        sleep(1);
        mlf_process_temp->p_info.runtime = mlf_process_temp->p_info.runtime - 1;
        if (mlf_process_temp->p_info.runtime == 0)
        {
          kill(mlf_process_temp->p_info.pid, SIGKILL);

            dealocation(root_tree,mlf_process_temp->p_info.id);
          fprintf(memptr,"At time %d freed %d bytes for process %d from %d to %d\n",getClk(),mlf_process_temp->p_info.memory_size,mlf_process_temp->p_info.id,mlf_process_temp->p_info.start, mlf_process_temp->p_info.end);

          finished_pros=finished_pros+1;

          mlf_process_temp->process_status = FINSIHED;
          mlf_process_temp->finish_time= getClk();

          // calculating turnaround finish-arrival
          mlf_process_temp->turnaround_time = (mlf_process_temp->finish_time) - (mlf_process_temp->p_info.arrival);

          // calculating weighted turn around //ta-runtime
          mlf_process_temp->weightedturnaround_time = (mlf_process_temp->turnaround_time) / (runnings[mlf_process_temp->p_info.id]);

          // calculating useful time
          useful = (runnings[mlf_process_temp->p_info.id]) + useful;
          printf("useful: %d\n", useful);
          // calculating total time
          total = getClk();
          printf("total: %d\n", total);
          // calculating summation of avg  weighted turn around time
          avgwta = (avgwta + (mlf_process_temp->weightedturnaround_time));
          printf("avgwta: %d\n", avgwta);
          // calculating summation of avg waiting time
          avgwait = (avgwait + (mlf_process_temp->waiting_time));
          printf("avgwait: %d\n", avgwait);

          mlf_process_temp->p_info.runtime = 0;
          fprintf(fptr, "at time %d process %d finished  arr  %d  remain %d  wait %d  TA %d  WTA %d\n", getClk(), mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->waiting_time, mlf_process_temp->turnaround_time, mlf_process_temp->weightedturnaround_time);
          printf("process %d has finished\n", mlf_process_temp->p_info.id);

          if(finished_pros==process_count-1)
          {
            while(!is_empty(process_queue) || !is_empty(runtime_process_queue) || !is_empty(priority_process_queue))
            {
              deQueue(process_queue);
              deQueue(runtime_process_queue);
              deQueue(priority_process_queue);
            }

          }
        }
        else
        {
          kill(mlf_process_temp->p_info.pid, SIGSTOP);

          mlf_process_temp->process_status = STOPPED;


          printf("process %d has stopped\n", mlf_process_temp->p_info.id);
          fprintf(fptr, "at time %d process %d stopped  arr  %d  remain %d  wait %d \n", getClk(), mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->waiting_time);

          mlf_process_temp->p_info.priority = mlf_process_temp->p_info.priority + 1;
          if (currentlvl == 10)
          {
            // while (!is_empty(mlf_pri_Queue))

            // mlf_process_temp1 = deQueue(mlf_pri_Queue);
            mlf_process_temp->p_info.priority = oldpris[mlf_process_temp->p_info.id];
            printf("priority*: %d\n", oldpris[mlf_process_temp->p_info.id]);
            prienQueue(mlf_buffer, mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->p_info.priority, mlf_process_temp->p_info.pid, mlf_process_temp->process_status,mlf_process_temp->p_info.start,mlf_process_temp->p_info.end,mlf_process_temp->p_info.memory_size);

            if (is_empty(mlf_pri_Queue))
            {
              while (!is_empty(mlf_buffer))
              {
                mlf_process_temp1 = deQueue(mlf_buffer);
                // mlf_process_temp1->p_info.priority=mlf_process_temp1->oldpriority;
                prienQueue(mlf_pri_Queue, mlf_process_temp1->p_info.id, mlf_process_temp1->p_info.arrival, mlf_process_temp1->p_info.runtime, mlf_process_temp1->p_info.priority, mlf_process_temp1->p_info.pid, mlf_process_temp1->process_status,mlf_process_temp->p_info.start,mlf_process_temp->p_info.end,mlf_process_temp->p_info.memory_size);
              }
            }
          }
          else
          {

            prienQueue(mlf_pri_Queue, mlf_process_temp->p_info.id, mlf_process_temp->p_info.arrival, mlf_process_temp->p_info.runtime, mlf_process_temp->p_info.priority, mlf_process_temp->p_info.pid, mlf_process_temp->process_status,mlf_process_temp->p_info.start,mlf_process_temp->p_info.end,mlf_process_temp->p_info.memory_size);
          }
        }

        break;
      default:
        // statment
        break;
      }
    }
  }
  printf("ending\n");
  sleep((20) + 1);

  // calculation of CPU Utilization  useful/total *100

  float cpu_uti = (useful * 1.0 / total) * 100.0;
  printf("cpu uti: %f\n", cpu_uti);
  fprintf(fptr2, "cpu uti: %f\n", cpu_uti);

  // Calculation of average weighted turnaround time  //summation/processcount
  avgwta = avgwta / (process_count-1);
  printf("average weighted ta: %d\n", avgwta);
  fprintf(fptr2, "WTA= %d\n", avgwta);

  // calculation of average waiting time   //summation/processcount
  avgwait =avgwait / (process_count-1);
  printf("average waiting: %d\n", avgwait);
  fprintf(fptr2, "Avg Waiting= %d\n", avgwait);

  fclose(fptr);
  fclose(fptr2);
  fclose(memptr);

  // fclose(fptr);
  // display(process_queue);  //3mltlha comment
  // display(priority_process_queue);

  msgctl(msg_queue_id, IPC_RMID, (struct msqid_ds *)0);

  destroyClk(true);

  return (0);
}

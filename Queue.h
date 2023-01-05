// A C program to demonstrate linked list based implementation of queue
#include <stdio.h>
#include <stdlib.h>
typedef short bool;
#define true 1
#define false 0
//added struct process in the node as the values we use 
struct process{

int id, arrival,runtime,priority,pid,start,end,memory_size;

//YASMINAZ VARIABLES


//for the output file
    //int finish_time;
    //int waiting_time;
    //int start_time;
    //int stopped_time;
    //int running_time;
    //int Remaining_time ;

/////


};
 enum STATUS
{
    RUNNING,
    WAITING,
    STOPPED,
    COUNTIUNE,
    FINSIHED
};
// A linked list (LL) node to store a queue entry

struct QNode {
    
    struct QNode* next;
    struct process p_info;

 enum STATUS process_status  ;
    enum STATUS stat;

    //for the output file
    int finish_time;
    int waiting_time;
    int start_time;
    int stopped_time;
    int running_time;
    int Remaining_time;
    int memsize_value;
    int start_mem;
    int end_mem;

    //MAHY addition
    int turnaround_time;
    int weightedturnaround_time;
};
 
// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    struct QNode *front, *rear;
};
 
// A utility function to create a new linked list node.
struct QNode* newNode(int P_ID, int Arr_T, int Run_T, int Priority,int inpid,enum STATUS ps,int start,int end,int memory_size) //see if we r gonna add status as parameter
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->next = NULL;
    temp->p_info.id=P_ID;
    temp->p_info.priority=Priority;
    //temp->stat = status;  //see if added
    temp->p_info.arrival=Arr_T;
    temp->p_info.runtime=Run_T;
    temp->p_info.pid=inpid;
    temp->process_status=ps;
    temp->p_info.start=start;
    temp->p_info.end=end;
    temp->p_info.memory_size=memory_size;
    return temp;


}
 
// A utility function to create an empty queue
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}
 
// The function to add a key k to q
void enQueue(struct Queue* q, int P_ID, int Arr_T, int Run_T, int Priority,int inpid,enum STATUS ps,int start,int end,int memory_size )
{
    // Create a new LL node
    struct QNode* temp = newNode(P_ID,Arr_T,Run_T,Priority,inpid,ps,start,end,memory_size);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
 
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}
// The function to add a priority item  to q
void prienQueue(struct Queue* q, int P_ID, int Arr_T, int Run_T, int Priority,int inpid,enum STATUS ps,int start,int end,int memory_size )
{
    // Create a new LL node
    struct QNode* temp = newNode(P_ID,Arr_T,Run_T,Priority,inpid,ps,start,end,memory_size);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    struct QNode *p = q->front;
    if(Priority<q->front->p_info.priority)
    {
        temp->next=q->front;
        q->front= temp;
    }
    else
    {
           // Traverse the list and find a
        // position to insert new node
        while (p->next != NULL && p->next->p_info.priority <= temp->p_info.priority)
            p = p->next;

        // Either at the ends of the list
        // or at required position
        temp->next = p->next;
        p->next = temp;
    }
    // Add the new node at the end of queue and change rear
    //q->rear->next = temp;
    //q->rear = temp;
}
 
// Function to remove a key from given queue q
struct QNode* deQueue(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return 0;
 
    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;
 
    q->front = q->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
 
    //free(temp); idk what is free temp
    return temp;
}

void display(struct Queue* q)
{
    //////////////
struct QNode *p = q->front;
    while (p != NULL)
    {
        printf("%d ", p->p_info.id);
       /* printf("%d ", p->p_info.arrival);
        printf("%d ", p->p_info.runtime);
        printf("%d ", p->p_info.priority);
        printf("%d ", p->p_info.pid);*/
        p = p->next;
    }
    printf("NULL\n");

    ////////////////////


}

bool is_empty(struct Queue *q)
{
    return (q->front == NULL) ? true : false;
}

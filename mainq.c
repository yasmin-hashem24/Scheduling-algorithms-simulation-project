#include<stdio.h>
#include<stdlib.h>
#include"Queue.h"

int main()
{
    struct Queue* q = createQueue();
    prienQueue(q,1,30,50,6,1);
    prienQueue(q,2,30,50,9,2);
    prienQueue(q,3,30,50,2,3);

    display(q);
    deQueue(q);
    display(q);


    return 0;
}

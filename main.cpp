#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <chrono>
#include <fstream>
#include <thread>
#include <pthread.h> 
#include <queue>
#include <sstream> 


#include <cstdlib>
int rand();


#define numFrames 10 //256
#define sizeOfPageTable 12
#define maxNumPages 9
#define QTIME 20
#define QTIME2 40
#define MAXNUMPR 100

#include "rapidxml.hpp"

#include "linkedlist.h"

#include "pcb.h"
#include "pcb.cpp"

#include "memory.h"
#include "memory.cpp"



#include "ipc.h"



#include "scheduler.h"
#include "scheduler.cpp"

#include "io.h"
#include "io.cpp"

#include "log.h"


using namespace std;
using namespace rapidxml;

int PrBkCtr::id = 0;

typedef void * (*THREADFUNCPTR)(void *);




#include <windows.h>



struct arg_struct{
    DoublyList<PrBkCtr*>* queue1;
    DoublyList<PrBkCtr*>* queue2;
    DoublyList<PrBkCtr*>* queue3;
    queue<short int>* freePages;
    short int*** TLB;
    int input;
    int numList;
    mem* M1;
    Sched* S1;

    queue<short int>* waitQ;
    int* numProcess;
};


void* PrintUI(void* arguments){
    
    struct arg_struct *args = (struct arg_struct *)arguments;
    DoublyList<PrBkCtr*>* queue1 = args->queue1;
    DoublyList<PrBkCtr*>* queue2 = args->queue2;
    int freeFrames;

    int page = 9;
    DoublyList<PrBkCtr*>::node* head;

    while(args->input != 1){
        ClearScreen();
        head = queue2->getHead();
        freeFrames = (*args->freePages).size();
        cout << "Numbers of procceses in READY queue " << endl;
        cout << "Queue1 size: " << queue1->size << endl;
        cout << "Queue2 size: " << queue2->size << endl;
        cout << "input " << args->input << endl;
        cout << "Translation Lookaside Buffer " << endl;
        cout << "----- valid ----------------------------------------" << endl;
        cout << "[1]:  " << *args->TLB[0][0] << "| PID: " << *args->TLB[0][1] << "| Frame number: "<< args->TLB[0][3]<< endl;
        cout << "[2]: page: " << page << "| frame: " << "| Valid bit: "<< endl;
        cout << "[3]: page: " << page << "| frame: " << "| Valid bit: "<< endl;
        cout << "[4]: page: " << page << "| frame: " << "| Valid bit: "<< endl;
        cout << "--------------------------------------------------" << endl;
        cout << "free frames" << freeFrames << endl;

        cout << "Enter 1 to exit:" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

bool exitSim(int num){
    
    if(num == -1)
        return true;
    else
        return false;
}

void* getUserIput(void* arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    int* num = &args->input;

    while(1){
        cin >> *num;
        if(exitSim(*num)){
            break;
        }
        else{
            //mutex
            //args->total = args->total + *num;
            for(int i = 0; i < *num; i++){
                args->waitQ->push( rand() % args->numList);
            }
            *num = 0;
            //release mutex
            
        }
    }
}

List<std::string> linker(int seed, mem* M1){
    List<std::string> process = M1->listOfProcesses.at(seed);
    return process;
}

void loader(List<std::string> process, Sched* S1, int* numProcess){
    PrBkCtr* pcb = new PrBkCtr(process.getHead());
    (*numProcess)--;
    pcb->state = READY;              //change PCB state to READY
    S1->queue1->insertNode(pcb);     //insert pcbs into READY QUEUE

    //tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
    //tmpIPC.PrTable.insert({pcb->PID, S1->queue1->tail});
}

void* waitingPrToBeAlloc(void* arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    int seed;
    List<std::string> process;
    //(*args->numProcess)--;

    while(!exitSim(args->input)){
        if(args->numProcess > 0 && args->waitQ->size() > 0){
            seed = args->waitQ->front();
            args->waitQ->pop();
            process = linker(seed, args->M1);
            loader(process, args->S1, args->numProcess);
        }
    }
}

void* runScheduler(void* arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    while(!exitSim(args->input)){
        args->S1->running();
    }
}

int main(){

    int numPr;
    cout << "Please enter numbers of processes to be created" << endl;
    cin >> numPr;

    mem M1(numPr);

    M1.createHD();

    //queue<List<std::string>> waitToBeAlloc;

    DoublyList<PrBkCtr*>* queue1 = new DoublyList<PrBkCtr*>();
    DoublyList<PrBkCtr*>* queue2 = new DoublyList<PrBkCtr*>();
    DoublyList<PrBkCtr*>* queue3 = new DoublyList<PrBkCtr*>();
    ipc tmpIPC;
    Sched* S1 = new Sched(queue1, queue2, queue3 ,M1, tmpIPC);


    /*

    for(int i=0; i < numPr; i++){
        if(M1.numProcess-- > 0){
            waitToBeAlloc.push(M1.listOfProcesses[rand() % M1.listOfProcesses.size() ]);
            create--;
        }         
    }
    */




    struct arg_struct args;
    args.queue1 = queue1;
    args.queue2 = queue2;
    args.TLB = &M1.TLB;
    args.freePages = &M1.freeFrames;
    args.input = 0;
    args.numList = M1.listOfProcesses.size();
    args.waitQ = new queue<short int>;
    args.numProcess = &M1.numProcess;
    args.M1 = &M1;
    args.S1 = S1;

    pthread_t threadId[4];    
    int rc;

    rc = pthread_create(&threadId[0], NULL, getUserIput, (void *)&args);
    
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    rc = pthread_create(&threadId[1], NULL, waitingPrToBeAlloc, (void *)&args);

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    rc = pthread_create(&threadId[2], NULL, runScheduler, (void *)&args);
    ////rc = pthread_create(&threadId[0], NULL, (THREADFUNCPTR) &Sched::running, S1);

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    //rc = pthread_create(&threadId[3], NULL, runScheduler, (void *)&args);
    

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    pthread_join(threadId[0], NULL);
    pthread_join(threadId[1], NULL);
    pthread_join(threadId[2], NULL);
    //pthread_join(threadId[3], NULL);

  
    //S1->running();

    //M1.linker();
 

    //DoublyList<PrBkCtr*>* queue1 = new DoublyList<PrBkCtr*>();
    //DoublyList<PrBkCtr*>* queue2 = new DoublyList<PrBkCtr*>();
    //DoublyList<PrBkCtr*>* queue3 = new DoublyList<PrBkCtr*>();


    


/*
    for(int i = 0; i < M1.memOfProcesses.size(); i++){

        PrBkCtr* pcb = new PrBkCtr(M1.memOfProcesses[i].getHead());
        M1.numProcess++;
        tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
        
       
        pcb->state = READY;              //change PCB state to READY
        
        S1->queue1->insertNode(pcb);     //insert pcbs into READY QUEUE

        tmpIPC.PrTable.insert({pcb->PID, S1->queue1->tail});

    }

    
    pthread_t threadId[3];    
    int rc;
    rc = pthread_create(&threadId[0], NULL, (THREADFUNCPTR) &Sched::running, S1);

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    rc = pthread_create(&threadId[1], NULL, (THREADFUNCPTR) &Sched::running, S1);

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }


    struct arg_struct args;
    args.queue1 = queue1;
    args.queue2 = queue2;
    args.TLB = &M1.TLB;
    args.freePages = &M1.freeFrames;
    args.input = 8;
    //rc = pthread_create(&threadId[1], NULL, PrintUI, (void *)&args);
    rc = pthread_create(&threadId[2], NULL, getUserIput, (void *)&args);
    
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    pthread_join(threadId[0], NULL);
    pthread_join(threadId[1], NULL);
    pthread_join(threadId[2], NULL);
    //();
    
    cout << "-!" << endl;
  */

    /*
    log(logINFO) << "foo " << "bar " << "baz";

    int count = 3;
    log(logDEBUG) << "A loop with "    << count << " iterations";
    for (int i = 0; i != count; ++i)
    {
        log(logDEBUG) << "the counter i = " << i;
    }

    */

    
    system("pause");
}





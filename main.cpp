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


#define numFrames 72 //256
#define sizeOfPageTable 12
#define maxNumPages 9
#define qtime 20

#include "rapidxml.hpp"


#include "timer.h"
#include "timer.cpp"

#include "linkedlist.h"

#include "pcb.h"
#include "pcb.cpp"

#include "memory.h"
#include "memory.cpp"



#include "ipc.h"

#include "io.h"
#include "io.cpp"

#include "scheduler.h"
#include "scheduler.cpp"



#include "log.h"


#include <cstdlib>



using namespace std;
using namespace rapidxml;

int PrBkCtr::id = 0;

typedef void * (*THREADFUNCPTR)(void *);




#include <windows.h>

/*
void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  ///* Get the number of cells in the current buffer
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  ///* Fill the entire buffer with spaces 
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  ///* Fill the entire buffer with the current colors and attributes
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  ///* Move the cursor home
  SetConsoleCursorPosition( hStdOut, homeCoords );
}
*/

struct arg_struct{
    DoublyList<PrBkCtr*>* queue1;
    DoublyList<PrBkCtr*>* queue2;
    DoublyList<PrBkCtr*>* queue3;
    queue<short int>* freePages;
    short int*** TLB;
    int input;
};


void PrintPrTable(PrBkCtr* process){


}

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

void* getUserIput(void* arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    int* frame = &args->input;

    while(1){
        cin >> *frame;
        if(*frame == 1){
            break;
        }
    }
}

int main(){

    int numPr;
    cout << "Please enter numbers of processes to be created" << endl;
    cin >> numPr;

    mem M1(numPr);

    M1.loadApps();
 

    DoublyList<PrBkCtr*>* queue1 = new DoublyList<PrBkCtr*>();
    DoublyList<PrBkCtr*>* queue2 = new DoublyList<PrBkCtr*>();
    DoublyList<PrBkCtr*>* queue3 = new DoublyList<PrBkCtr*>();
    ipc tmpIPC;


    //Sched S1(queue1, queue2, queue3 ,M1, tmpIPC);
    Sched* S1 = new Sched(queue1, queue2, queue3 ,M1, tmpIPC);

    for(int i = 0; i < M1.memOfProcesses.size(); i++){

        PrBkCtr* pcb = new PrBkCtr(M1.memOfProcesses[i].getHead());
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

    //S1.running();

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
    //pthread_join(threadId[1], NULL);
    pthread_join(threadId[2], NULL);
    //();
    
    cout << "-!" << endl;
  

    /*
    log(logINFO) << "foo " << "bar " << "baz";

    int count = 3;
    log(logDEBUG) << "A loop with "    << count << " iterations";
    for (int i = 0; i != count; ++i)
    {
        log(logDEBUG) << "the counter i = " << i;
    }

    */

    
    //system("pause");
}





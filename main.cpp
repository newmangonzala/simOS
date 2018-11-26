#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <chrono>
#include <fstream>
#include <thread> 
#include <queue>

#define numFrames 10 //256

#include "rapidxml.hpp"

#include "timer.h"
#include "timer.cpp"

#include "linkedlist.h"

#include "memory.h"
#include "memory.cpp"

#include "pcb.h"
#include "pcb.cpp"

#include "ipc.h"

#include "scheduler.h"
#include "scheduler.cpp"



#include "log.h"


#include <cstdlib>

using namespace std;
using namespace rapidxml;

int PrBkCtr::id = 0;


int main(){

    int numPr;
    cout << "Please enter numbers of processes to be created" << endl;
    cin >> numPr;

    mem M1(numPr);

    M1.loadApps();
 

    DoublyList<PrBkCtr*> queue1;
    DoublyList<PrBkCtr*> queue2;
    DoublyList<PrBkCtr*> queue3;
    ipc tmpIPC;


    Sched S1(queue1, queue2, queue3 ,M1, tmpIPC);

    for(int i = 0; i < M1.memOfProcesses.size(); i++){

        PrBkCtr* pcb = new PrBkCtr(M1.memOfProcesses[i].getHead());
        tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
        
       
        pcb->state = READY;              //change PCB state to READY
        
        S1.queue1->insertNode(pcb);     //insert pcbs into READY QUEUE

        tmpIPC.PrTable.insert({pcb->mailbox.id, S1.queue1->tail});

    }


    S1.running2();
    
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

    
    system("pause");
}





#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>

#include <queue>
#include <exception>
#include <vector>
#include <fstream>

#include <iomanip>



//#include "rapidxml.hpp"

//#include "linkedlist.h"

#include "pcb.h"

#include "io.h"

//class Io;

#include <windows.h>

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
}


class Io;

enum QState{Queue1 , Queue2 , Queue3};

//Process scheduler
//multilevel Feedback Queue
class Sched{

    
    public:
    
    Sched(DoublyList<PrBkCtr*>* ,DoublyList<PrBkCtr*>* , DoublyList<PrBkCtr*>* ,mem&, ipc&);

    DoublyList<PrBkCtr*>* servingQ;        //serving queue

    DoublyList<PrBkCtr*>* queue1;   //round robin //this is the ready queue
    DoublyList<PrBkCtr*>* queue2;   //round robin
    DoublyList<PrBkCtr*>* queue3;   //first come first serve

    mem* M1;
    ipc* MB; //mailboxes

    int numOfProcess;

    void terminatePr(DoublyList<PrBkCtr*>::node*);

    //runs the processes in READY

    bool runRR(int, PrBkCtr *);
    //void runFIFO(DoublyList<PrBkCtr *>*);
    void* running();
    void fork(PrBkCtr*, int);

    void insertBackToQ(DoublyList<PrBkCtr *>* , DoublyList<PrBkCtr*>::node* );

    int parseTime(xml_node<> *);

    DoublyList<PrBkCtr*>::node* findProcessNode(PrBkCtr*);

    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;



    void yield(PrBkCtr*);
    bool checkChilds(PrBkCtr*);
    string updateTime(string, int);
    

    void printProcess(PrBkCtr*);
    void printMainMem();

};




#endif
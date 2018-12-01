#ifndef IO_H
#define IO_H

#include <cstdlib>
int rand();

class Sched;

class Io{
    public:

   // int* queueState;

    static void send(xml_node<> *, PrBkCtr*, ipc*);
    static void receive(PrBkCtr*, ipc* );
    static void interruptHandler();

    static void write(Sched*, PrBkCtr*);
    static void read(Sched*, PrBkCtr*);
    static void wait(Sched*, semaphore*, PrBkCtr*);
    static void signal(Sched*, semaphore*, PrBkCtr*);

};

#endif
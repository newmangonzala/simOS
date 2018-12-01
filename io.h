#ifndef IO_H
#define IO_H

#include <cstdlib>
int rand();

class Io{
    public:
    static void send(xml_node<> *, PrBkCtr*, ipc*);
    static void receive(PrBkCtr*, ipc* );
    static void interruptHandler();


};

#endif



#include <iostream>
#include <string>

using namespace std;

class PCB{
    private:
        struct Registers{           //CPU register
            string R0;
            string R1;
            string R2;
            string R3;
            string R4;
        }Registers;
        unsigned short int state;   //Process state {new, ready, running, waiting, terminated}
        unsigned int PID;           //Process number
        unsigned int PC;            //Program Counter
        unsigned int Privalage;     //Privalage

        struct schedulingInfo{      // priorities, scheduling queue pointers
            unsigned int priority;
            unsigned int queue;
        }schedulingInfo;
        struct memory{              // memory allocated
            unsigned int address;
        }memory;
        struct IOinfo{              // memory allocated
            unsigned int temp;
        }IOinfo;
        struct accounting{
            //time constraints
            //CPU used
            //
        }accounting;

};


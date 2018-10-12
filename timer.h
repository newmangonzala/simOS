#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <chrono>

using namespace std::chrono;

class timer{

    public:

    std::chrono::_V2::system_clock::time_point time_point;

    std::time_t initTime;

     
    //std::chrono::_V2::system_clock::time_point time_point_ms;

    timer(){
        //time_point = system_clock::now();
        time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(system_clock::now());

        

        initTime = std::chrono::system_clock::to_time_t(time_point);
    }
    std::time_t getTime();
    int test();
};




#endif

    
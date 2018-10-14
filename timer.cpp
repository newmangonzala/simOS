#include "timer.h"

using namespace std::chrono;

std::time_t timer::getTime(){
    std::chrono::_V2::system_clock::time_point time = system_clock::now(); 
    return std::chrono::system_clock::to_time_t(time);
}

int timer::test(){
    return 102;
}
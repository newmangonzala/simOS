
#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>

enum loglevel {logERROR, logWARNING, logINFO, logDEBUG};

class log{
    public:
        log(loglevel level = logERROR) {
            fout.open("logs.txt", ios::app);
            fout << level << " : ";
        }

        template <typename T>
        log & operator<<(T const & value){
            fout << value;
            return *this;
        }

        ~log(){
            fout << std::endl;
            fout.close();
            //std::cerr << fout.str();
        }

    private:
        //std::ostringstream fout;
        std::ofstream fout;
};

#endif
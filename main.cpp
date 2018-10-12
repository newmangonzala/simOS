

#include <iostream>
#include "pcb.h"
#include "scheduler.h"
#include "timer.h"
#include "include.cpp"
#include <time.h>
#include <chrono>
#include <numeric>
#include "log.h"
#include <string>
#include "rapidxml.hpp"
#include <vector>


using namespace std;
using namespace rapidxml;

int main(){

    /*
    to do list:
    process scheduler

   */

    /*
    ifstream file("process.xml");

    xml_document<> doc;
    xml_node<> * root_node;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);

    root_node = doc.first_node("Processes");
    xml_node<> * process = root_node->first_node("p");

    for (xml_node<> * process = root_node->first_node("p"); process; process = process->next_sibling()){
	    
        xml_node<> * intruction = process->first_node("action");
        
        cout << intruction->value();
        cout << intruction->first_attribute("time")->value() << endl;
        
    }

    file.close();

    */

/*
    timer t1;
    cout << t1.initTime << endl;

    for(int i = 0; i < 1000000; i++){
        for(int j = 0; j < 100; j++){
                
    }
    }

    cout <<  t1.getTime() << endl;

    std::chrono::duration<double,std::milli> dif;
    auto end = std::chrono::system_clock::now(); 
    dif = end - t1.time_point  ;

    std::cout << dif.count() << std::endl;
   
*/

    Sched S1;
    //S1.queue1.insertNode(5);
    //S1.manage();
    S1.getApp(2,2);

    //cout << S1.queue1.getReference() << endl;

   
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

rapidxml::xml_node<>* loadApp(int index, int instruction){
    
    ifstream file("process.xml");

    xml_document<> doc;
    xml_node<> * root_node;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    //make sure to zero terminate the buffer
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);

    //access the root node
    root_node = doc.first_node("Processes");

    //iterate throught the apps until 
    xml_node<> * app = root_node->first_node("app");
    for (int i = 1; i < index; i++){
         app = app->next_sibling();
    }

    //iterate throught the instructions
    xml_node<> * inst = app->first_node("action");
    for (int i = 1; i < instruction; i++ ){
        inst = inst->next_sibling();
    }
      
    cout << inst->value() << " ";
    cout << inst->first_attribute("time")->value() << endl;

    file.close();

    return inst;

}
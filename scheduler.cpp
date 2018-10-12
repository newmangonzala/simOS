#include "scheduler.h"
#include "rapidxml.hpp"




using namespace rapidxml;

Sched::Sched(){

}

int Sched::manage(){
    int current;
    
    while(!queue1.isEmpty()){
        
        std::chrono::duration<double,std::milli> dif;
        auto start = std::chrono::system_clock::now(); 
        auto end = std::chrono::system_clock::now(); 
        dif = end - start;

        current = queue1.getReference();
        return current;
    }
    /*
    else if(!queue2.isEmpty()) {
        current = queue2.getReference();
        return current;
    }
    else if(!queue3.isEmpty()) {
        current = queue3.getReference();
        return current;
    }
    */
    
}

rapidxml::xml_node<>* Sched::getApp(int index, int instruction){
    
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

void Sched::maintanance(){
    
}
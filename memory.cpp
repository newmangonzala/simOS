#include "memory.h"
#include "rapidxml.hpp"

using namespace rapidxml;


mem::mem(int numPr){
    numProcess = numPr;
}


//loader
vector<mem::instrucion> mem::loadApps(){

    //open file of dummy proccesses 
    ifstream file("process.xml");

    xml_document<> doc;
    xml_node<> * root_node;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    //make sure to zero terminate the buffer
    buffer.push_back('\0');

    //parse buffer
    doc.parse<0>(&buffer[0]);

    //access the root node
    root_node = doc.first_node("Processes");

     
    instrucion I1; 

    //iterate throught the app to get all the instructions
    vector<instrucion> app4;

    xml_node<> * app = root_node->first_node("app");
    for (int i = 1; i <= numProcess; i++){
        List<instrucion> app1;
        for (xml_node<> * inst = app->first_node("action"); inst; inst = inst->next_sibling()){
            
            I1.type = inst->value();
            I1.time = stoi(inst->first_attribute("time")->value());
            app1.insertNode(I1);
            //app1.push_back(I1);
        }
        //load proccess into memory
        memory1.push_back(app1);

        app = app->next_sibling();
    }


    file.close();

    return app4;
}
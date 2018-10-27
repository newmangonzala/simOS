#include "memory.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;


mem::mem(int numPr){
    numProcess = numPr;
}


//loader
void mem::loadApps(){

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


    xml_document<> tmpPr;
     
    instrucion I1; 
    vector<instrucion> app4;



    xml_node<> * app = root_node->first_node("app");

    
    


    for (int i = 1; i <= numProcess; i++){
        
 
        xml_node<char>* root = tmpPr.clone_node(app);

        tmpPr.append_node(root);
        string xml_as_string = "";
        print(std::back_inserter(xml_as_string), tmpPr);
        //std::cout << xml_as_string << std::endl;

    
        //load proccess into memory
        memOfProcesses.push_back(xml_as_string);

    
        tmpPr.remove_first_node();

        //if last app then restart
        if(app->next_sibling() == NULL){
            app = root_node->first_node("app");
        }
        else{
            app = app->next_sibling();
        }
        
    }
    

    file.close();

    return;
}

void mem::loadPCBs(){

    xml_document<> doc;


    for(int i = 0; i < memOfProcesses.size(); i++){

        auto tmp = memOfProcesses[i];

        doc.parse<0>(&tmp[0]);

        List<mem::instrucion> app1;

        instrucion I;

        xml_node<> * app = doc.first_node("app");

    //iterate throught the app to get all the instructions
        for (xml_node<> * inst = app->first_node("action"); inst; inst = inst->next_sibling()){
            
            string c = (inst->name());
            //if(c.compare("fork") == 0){
            //    continue;
            //}

            I.type = inst->value();

            rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

            if(t != 0){
                I.time = stoi(t->value());
            }

            app1.insertNode(I);
        }
        qOfPr.push_back(app1);
    }

    return;
}

void mem::loadApp(string child){

xml_document<> doc;
doc.parse<0>(&child[0]);

List<mem::instrucion> app1;

instrucion I;

xml_node<> * app = doc.first_node("app");

//iterate throught the app to get all the instructions
        for (xml_node<> * inst = app->first_node("action"); inst; inst = inst->next_sibling()){
            
            string c = (inst->name());
            //if(c.compare("fork") == 0){
            //    continue;
            //}

            I.type = inst->value();

            rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

            if(t != 0){
                I.time = stoi(t->value());
            }

            app1.insertNode(I);
        }
        qOfPr.push_back(app1);

    return;
}


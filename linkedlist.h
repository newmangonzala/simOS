#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

using namespace std;

template <class type>
class List;

//template <typename type>
//ostream& operator<< (ostream& os, List<type>& print); ///this is needed so operator overload works



template <class type>
class List{
	
	private:
		struct node{
			type data;
            node *prev;
			node *next;
		};
        
	node *reference; //reference node from circular linked list
		
	public:
		List();
		~List();
		void insertNode(type);  //insert Node before the current Node in execusion
		void deleteNode();      //deletes node from the list
		type getReference();
		bool isEmpty();
		// display(ostream&);
		//friend ostream&  operator<< <>(ostream&, List<type>&);
};


template <class type>
bool List<type>::isEmpty(){
	if(reference == NULL)
		return true;
	else
		return false;
}

template <class type>
type List<type>::getReference(){
	if(reference != NULL){
		return reference->data;
	}
	else
		return -1;
}


template <class type>
List<type>::List(){
	reference = NULL;
}

template <class type>
List<type>::~List(){
	
	//while(top!=NULL){	List<type>::popNode();}
}

template <class type>
void List<type>::insertNode(type dat){
	node *nNode;
	nNode = new node;
	nNode->data = dat;
	nNode->next = NULL;
    nNode->prev = NULL;

	if(reference == NULL){
		reference = nNode;
	}
	else{
        node *ptr;
        ptr = reference->prev;
        ptr->next = nNode;
        nNode->prev = ptr;
        nNode->next = reference;
        reference->prev = nNode;
	}
	return;
}

template <class type>
void List<type>::deleteNode(){
	
	
	if(reference!=NULL){
		
		node *ptr; 
		ptr = reference->next; 
		
		if(ptr == NULL){
			free(reference);
			reference = NULL; 
		}
        else if(ptr == reference->prev){
            ptr->next =  NULL;
            ptr->prev = NULL;
            free(reference);
            reference = ptr;
        }
		else{
			reference->prev->next = ptr;
            ptr->prev = reference->prev;  
			free(reference);
            reference = ptr; 
		}
	}
}

/*
template <typename type>
ostream& operator<< (ostream& os, List<type>& print){
	
	print.display(os);

	return os;
}

template <class type>
void List<type>::display(ostream& os){
	int i = 1;
	node *ptr;
	ptr = top;
	while(ptr != NULL){
		os << i << ". " << ptr->data << endl;
		ptr = ptr->next;
		i++;
	}
	
	return;
}

*/

#endif
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

using namespace std;

template <class type>
class List;

//template <typename type>
//ostream& operator<< (ostream& os, List<type>& print); ///this is needed so operator overload works

class mem;

template <class type>
class List{
	
	public:
		struct node{
			type data;
			node *next;
		};
        
		node *head; 	//head node = current node running
		node *tail; 	//tail->next always point to null

		List();
		~List();
		void insertNode(type);  //insert Node before the the tail
		void deleteNode();      //deletes node from the list
		void deleteHead();      //deletes head from the list
		auto getHead();			//returns head
		bool isEmpty();			//checks if empty

		//functions recycled - might be useful
		// display(ostream&);
		//friend ostream&  operator<< <>(ostream&, List<type>&);
};

//checks if empty
template <class type>
bool List<type>::isEmpty(){
	if(head == NULL)
		return true;
	else
		return false;
}

//returns the head of the queue
template <class type>
auto List<type>::getHead(){
		return head;
}  

template <class type>
List<type>::List(){
	head = NULL;
	tail = NULL;
}

template <class type>
List<type>::~List(){
	//FIX ME
}

//Insert Node to the tail of queue
template <class type>
void List<type>::insertNode(type dat){
	node *nNode;
	nNode = new node;
	nNode->data = dat;
	nNode->next = NULL;

	if(tail == NULL){
		head = nNode;
		tail = nNode;
	}
	else if (tail == head){
		tail = nNode;
		head->next = nNode;
	}
	else{
        tail->next = nNode;
		tail = nNode;
	}
	return;
}

//FIX ME
template <class type>
void List<type>::deleteNode(){
	
	
	if(head!=NULL){
		
		node *ptr; 
		ptr = tail; 
		if(head != tail){
			while(ptr->next != head){
				ptr = ptr->next;
			}
			free(head);
			head = ptr;
		}
		else{
			free(head);
			head = NULL;
			tail = NULL;
		}
	}
}

template <class type>
void List<type>::deleteHead(){
	
	if(head!=NULL){
		
		node *ptr; 
		ptr = head; 

		head = head->next;
		delete ptr;
		if(head == tail){
			tail = NULL;
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
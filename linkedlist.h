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
        
	node *head; //head node = current node running
	node *tail; 
		
	public:
		List();
		~List();
		void insertNode(type);  //insert Node before the the tail
		void deleteNode();      //deletes node from the list
		auto getHead();
		//type getTail();
		bool isEmpty();
		// display(ostream&);
		//friend ostream&  operator<< <>(ostream&, List<type>&);
};


template <class type>
bool List<type>::isEmpty(){
	if(head == NULL)
		return true;
	else
		return false;
}

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
	
	node *nNode;
	
	while(head!=NULL){
		nNode = tail;
		tail = nNode->next;
		free(nNode);
	}
	head = NULL;
}

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
		tail->next = head;
	}
	else{
        nNode->next = tail;
		tail = nNode;
	}
	return;
}

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
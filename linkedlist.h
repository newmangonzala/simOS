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
		void deleteHead();      //deletes head from the list
		auto getHead();			//returns head
		auto getTail();
		bool isEmpty();			//checks if empty
		int size();

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
auto List<type>::getTail(){
		return tail;
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

template <class type>
void List<type>::deleteHead(){
	
	if(head!=NULL){
		
		node *ptr; 
		ptr = head; 

		head = head->next;
		delete ptr;

	}
}

template <class type>
int List<type>::size(){
	node* ptr = head;
	int size = 0;
	while(ptr != NULL){
		size++;
		ptr = ptr->next;
	}
	//cout << "current list has a size of " << size << endl;
	return size;
}

template <class type>
class DoublyList: public List<type> {
	public:

		DoublyList(){
			head = NULL;
			tail = NULL;
		}
	
		struct node{
			type data;
			node *next;
			node *prev;
		};
		node *head; 	//head node = current node running
		node *tail; 	//tail->next always point to null
        
		void insertNode(type);  //insert Node before the the tail
		void deleteNode(node*);  //insert Node before the the tail
		void deleteHead();      //deletes head from the list
		void popHead();
		node* getHead(){
			return head;
		}
		node* getTail(){
			return tail;
		}
		bool isEmpty(){
			if(head == NULL){
				return true;
			}
			else{
				return false;
			}
		}
};

//insert node in doubly linked list
template <class type>
void DoublyList<type>::insertNode(type dat){
	node *nNode;
	nNode = new node;
	nNode->data = dat;
	nNode->next = NULL;
	nNode->prev = NULL;

	if(tail == NULL){
		head = nNode;
		tail = nNode;
	}
	else if (tail == head){
		tail = nNode;
		tail->prev = head;
		head->next = nNode;
	}
	else{
        tail->next = nNode;
		nNode->prev = tail;
		tail = nNode;
	}
	return;
}
template <class type>
void DoublyList<type>::deleteHead(){
		
	node *ptr;
	if(head == tail){
		ptr = head;
		head = NULL;
		tail = NULL;
		delete ptr;
	}	
	else if(head!=NULL){
		
		node *ptr; 
		ptr = head; 

		head = head->next;
		if(head != NULL)
			head->prev = NULL;
		delete ptr;
	}
}

template <class type>
void DoublyList<type>::popHead(){
		
	node *ptr;
	if(head == tail){
		ptr = head;
		head = NULL;
		tail = NULL;
	}	
	else if(head!=NULL){
		
		node *ptr; 
		ptr = head; 

		head = head->next;
		if(head != NULL)
			head->prev = NULL;
	}
}

template <class type>
void DoublyList<type>::deleteNode(node* tmpNode){
		
	if(tmpNode->next == NULL){
		tmpNode->prev->next = NULL;
		delete tmpNode;
	}
	else if(tmpNode->prev == NULL){
		deleteHead();
	}
	else{
		tmpNode->prev->next = tmpNode->next;	
		delete tmpNode;
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
#ifndef CACHE_H
#define CACHE_H

#include "main.h"
class NodeQueue{
	public:
		Elem* elem;
		NodeQueue* next;
		NodeQueue(Elem* ele): elem(ele),next(NULL){};
		~NodeQueue() {
			this->next = NULL;}
	};
class Queue {
private:
	NodeQueue* head;
	NodeQueue* tail;
	int size;
public:
	Queue(): head(NULL),tail(NULL),size(0){};
    int getSize() {return this->size;}
    void enqueue(Elem* e) {
		if (head == NULL) {
			head = new NodeQueue(e);
			tail = head;}
		else {
            tail->next = new NodeQueue(e);
			tail = tail->next;}
		this->size++;}

	Elem* dequeue() {
		if (this->size == 0) return NULL;
		NodeQueue* temp = this->head;
		this->head = this->head->next;
		if (this->size == 1) this->tail = NULL;
        Elem* elem = temp->elem;
            delete temp;
            this->size--;
		if(this->size == 0){head = tail = NULL;}
		return elem;}

	void replace_node(Elem* elem) {
		NodeQueue* temp = this->head;
		while (temp && temp->elem->addr != elem->addr)temp = temp->next;
		if (temp) {
			delete temp->elem;
			temp->elem = elem;}
	}

	void print() {
		NodeQueue* temp = this->head;
		while(temp != NULL){
			temp->elem->print();
			temp = temp->next;}
	}
};

class Node {
public:
	Elem* value;
	Node* left;
	Node* right;
	int balance;
	Node(Elem* ele): value(ele),left(NULL),right(NULL),balance(0){};
	~Node(){left = right = NULL;};
};
class AVL {
private:
	Node* root;

protected:
	Node *rr_rotate(Node *&);
	Node *ll_rotate(Node *&);

	Node *leftBalance(Node *&, bool &);
	Node *rightBalance(Node *&, bool &);

	Node *insert_node(Node *&, Elem*, bool &);

	Data* get_node(const Node*, int) const;
    int get_height(Node*) const;

	Node *removeLeftBalance(Node *&, bool &);
	Node *removeRightBalance(Node *&, bool &);
	Node *delete_node(Node *&, const Elem* value, bool &, bool &);

	void print_preorder(Node*);
	void print_inorder(Node*);
public:
	AVL(){root = NULL;};
	~AVL(){};
    Data* getNode(int) const;
	int getHeight() const;
    void insertNode(Elem*);
	bool searchReplace(Elem*);
	void deleteNode(Elem*);
    void printPreOrder();
	void printInOrder();
};

class Cache {
private:
	AVL* tree;
	int size;
	Queue* q;
public:
	Cache(int s) : size(s) {
		tree = new AVL();
        q = new Queue();}
	~Cache() {
	    size = 0;
		delete tree;
		delete q;}
	Data* read(int addr);
	Elem* put(int addr, Data* cont);
	Elem* write(int addr, Data* cont);
	void print();
	void preOrder();
	void inOrder();
};
#endif

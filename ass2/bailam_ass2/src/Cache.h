#ifndef CACHE_H
#define CACHE_H

#include "main.h"
enum BalanceVal{LH = -1,EH = 0,RH = 1};
class Node
{   private:
        Elem* val;
        Node* left,* right;
        BalanceVal balance;
        friend class AVL;
    public:
        Node(Elem* key): val(key),left(NULL),right(NULL),balance(EH){}
        ~Node(){left = right = NULL;}
};

class AVL
{
    private:
        Node* root;
    protected:
        int get_Height(Node*);
        Data* get_node(Node*,int);

        Node* rot_LL(Node*&);
        Node* rot_RR(Node*&);

        Node* BalanceLeft(Node*&,bool&);
        Node* BalanaceRight(Node*&,bool&);
        Node* insert_AVL(Node*&,Elem*,bool&);

        Node* BalanceLeft_del(Node*&,bool&);
        Node* BalanaceRight_del(Node*&,bool&);
        Node* remove_node(Node*&,Elem*,bool&,bool&);

        void Print_Inorder(Node* node);
        void Print_PreOrder(Node* node);


    public:
        AVL(){root = NULL;}
        Data* getNode(int addr){return get_node(root,addr);}
        int getHeight(){return get_Height(root);}
        void insertAVL(Elem* key){bool t = 0;insert_AVL(root,key,t);}
        void deleteAVL(Elem*key){
        bool t = false;bool s = false;
        remove_node(root,key,t,s);}
        bool search_place(Elem*);

        void print_in(){Print_Inorder(this->root);}
        void print_pr(){Print_PreOrder(this->root);}
};
class Node_Q{
	public:
		Elem* elem;
		Node_Q* next;
		Node_Q(Elem* ele): elem(ele),next(NULL){};
		~Node_Q(){this->next = NULL;}
	};
class Queue{
private:
	Node_Q* head;
	Node_Q* tail;
	int size;
public:
	Queue(): head(NULL),tail(NULL),size(0){};
    int getSize() {return this->size;}
    void enqueue(Elem* e) {
		if (head == NULL) {
			head = new Node_Q(e);
			tail = head;}
		else {
            tail->next = new Node_Q(e);
			tail = tail->next;}
		this->size++;}

	Elem* dequeue() {
		if (this->size == 0) return NULL;
		Node_Q* temp = this->head;
		this->head = this->head->next;
		if (this->size == 1) this->tail = NULL;
        Elem* elem = temp->elem;
            delete temp;
            this->size--;
		if(this->size == 0){head = tail = NULL;}
		return elem;}

	void replace_node(Elem* elem) {
		Node_Q* temp = head;
		while (temp && temp->elem->addr != elem->addr)temp = temp->next;
		if (temp) {
			delete temp->elem;
			temp->elem = elem;}
	}

	void print() {
		Node_Q* temp = head;
		while(temp){temp->elem->print();
                    temp = temp->next;}
	}
};
class Cache {
private:
	AVL* avl_tree;
	int size_Cache;
	Queue* que;
public:
	Cache(int s) : size_Cache(s) {
		avl_tree = new AVL();
        que = new Queue();}
	~Cache() {
	    size_Cache = 0;
		delete avl_tree;
		delete que;}
	Data* read(int addr);
	Elem* put(int addr, Data* cont);
	Elem* write(int addr, Data* cont);
	void print();
	void preOrder();
	void inOrder();
};



#endif

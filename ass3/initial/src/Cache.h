#ifndef CACHE_H
#define CACHE_H

#include "main.h"
////////////////////////////////////////////////////////////////////////REPLACEMENT///////////////////////////////////////////////
////////////////////////REPLACEMENT_POLICY_ABSTRACT_CLASS//////////////////////
class ReplacementPolicy
{
protected:
    int count;
public:
    ReplacementPolicy():count(0){}
    virtual ~ReplacementPolicy(){count = 0;}
public: // Teacher abstract function
	virtual Elem* insert(Elem *e) = 0; 
	virtual void access(int addr) = 0;
	virtual Elem* remove() = 0;
	virtual void print() = 0;

public: // Main function
	bool isFull() { return count == MAXSIZE;}
	bool isEmpty() { return count == 0; }
	//Elem *getValue(int idx) { return arr[idx];}

	virtual Elem* replace(Elem *e) = 0;
};
///////////////////QUEUE_FIFO//////////////////////////
class NodeQueue{
	public:
		Elem* elem;
		NodeQueue* next;
		NodeQueue(Elem* ele): elem(ele),next(NULL){};
		~NodeQueue() {this->next = NULL;this->elem = NULL;}
	};
class FIFO : public ReplacementPolicy
{
private:
    NodeQueue* head;
	NodeQueue* tail;
public:
	FIFO(): head(NULL),tail(NULL){}
	~FIFO(){
        while(head)
        {   NodeQueue* temp = head;
            head = head->next;
            delete temp;
            if(head == tail)break;}
          head = tail = NULL;
	}
    // main function:
    Elem* enqueue(Elem*);
    Elem* dequeue();
    virtual Elem* replace(Elem*) override;

public:	//Teacher function // Override the virtual function
        virtual Elem* insert(Elem *e)override{return enqueue(e);}
        virtual void access(int idx) override{return;}
        virtual Elem* remove()override{return dequeue();}
        virtual void print() override;
};
/////////////////SELF_ORG_LIST////////////////////////////
class NodeList
{
    public:
    Elem* elem;
    NodeList* next;
    NodeList(Elem* ele): elem(ele),next(NULL){};
    ~NodeList() {this->next = NULL;this->elem = NULL;}
};
class MRU : public ReplacementPolicy
{
protected:
    NodeList* head;
    NodeList* tail;
public:
	MRU(): head(NULL),tail(NULL){}
	~MRU(){
        while(head)
             {
                NodeList* temp = head;
                head = head->next;
                delete temp;
                if(head == tail)break;
             }
        head = tail = NULL;
          }

	virtual Elem* replace(Elem*)override;
public:// Teacher function // Abstract function
	virtual Elem* insert(Elem *e)override;
	virtual void access(int addr)override;
	virtual Elem* remove() override;
	virtual void print()override;
};
class LRU : public MRU
{
public:// Teacher function
    LRU(){head = tail = NULL;}
    ~LRU()
    {
        while(head)
          { 
          NodeList* temp = head;
          head = head->next;
          delete temp;
          if(head == tail)break;
          }
        head = tail = NULL;
    }
	virtual Elem* remove() override;
};
/////////////////HEAP_TREE////////////////////////////////
class NodeHeap
{
public:
    int freq;
    Elem* value;
    NodeHeap(){value = NULL;freq = 0;}
    NodeHeap(Elem *v):value(v){freq = 1;}
    ~NodeHeap(){value = NULL;freq = 0;}
};
class LFU : public ReplacementPolicy // Also it is a heap_tree_array
{
private:
    NodeHeap **arr;
public:
	LFU() { arr = new NodeHeap*[MAXSIZE];
            for(int i = 0; i < MAXSIZE;i++)arr[i] = NULL;}
	~LFU() {
        for(int i = 0; i< MAXSIZE;i++) delete arr[i];
        }
public:
    void ReheapUp(int);
    void ReheapDown(int);
    virtual Elem* replace(Elem*)override;

public: // teacher function
    virtual Elem* insert(Elem *e)override;
	virtual void access(int idx)override;
	virtual Elem* remove()override;
	virtual void print()override
	{for(int i = 0; i < this->count;i++)
        {
        //cout<<arr[i]->freq<<" ";
        arr[i]->value->print();}
	}
};
////////////////////////////////////////////////////////////////////SEARCHING//////////////////////////////////////////////
///////////////////////SEARCH_ENGINE///////////////////////////
class SearchEngine
{
public:
    SearchEngine(){}
    virtual~SearchEngine(){}
public: // Teacher abstract function
    virtual Elem* replace(Elem* e) = 0;
	virtual Data* search(int addr) = 0; // NULL if not found
	virtual void insert(int addr, Data* value,bool) = 0;
	virtual void deleteNode(int key) = 0;
	virtual void print() = 0;
};

////////////////////////DBHASHING///////////////////////////////
class DBHashing : public SearchEngine
{
protected:
    int (*hash1)(int);
    int (*hash2)(int);
    int size; // Hash size
    int curr_size; // total numbers of variables
    Elem** a;
public:
    DBHashing(int (*h1)(int), int (*h2)(int), int s): hash1(h1),hash2(h2),size(s)
	{
	    curr_size = 0;
        a = new Elem*[this->size];
        for(int i=0;i < size;++i) a[i] = NULL;
	}
	~DBHashing()
     {
        curr_size = 0;
        
        for(int i = 0 ;i <size;i++)delete a[i];
        a = NULL;
        
        size = 0;
	 }

public:	// Teacher function // Abstract function
    virtual Elem* replace(Elem* e)override;
	virtual void insert(int key,Data* value,bool)override;
	virtual void deleteNode(int key) override;
	virtual Data* search(int addr) override;
    virtual void print()override
    {
        cout << "Prime memory:\n";
        for(int i = 0; i < size;i++)
        {
          if(a[i] != NULL) {a[i]->print();}
        }
    }
};

//////////////////////////////AVL////////////////////////////////
class Node {
public:
	Elem* value;
	Node* left;
	Node* right;
	int balance;
	Node(Elem* ele): value(ele),left(NULL),right(NULL),balance(0){};
	~Node(){left = right = NULL;};
};
////AVL mode:
////-1: LH
////0: EH
////1: RH
class AVL : public SearchEngine
{
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

    void clear(Node*);

public:
	AVL(){root = NULL;};
	~AVL()
    {
       clear(this->root);
       root = NULL;
    };
    
    Data* getNode(int) const;
	int getHeight() const;
    void insertNode(Elem*);
	bool searchReplace(Elem*);
	void delete_Node(Elem*);
    void printPreOrder();
	void printInOrder();

public:
    //Teacher function // Override function
    virtual Elem* replace(Elem* e)override
    {
        if(searchReplace(e) == true)return e;
        return NULL;
    }
    virtual Data* search(int addr)override
    {
        return getNode(addr);
    }
	virtual void insert(int key,Data* value,bool w) override
	{
        Elem* add_ele = new Elem(key,value,w);
        insertNode(add_ele);
	}
	virtual void deleteNode(int key) override
	{
	    Data* p = this->search(key);
	    if(p == NULL)return;
        Elem* re_ele = new Elem(key,p,false);
        delete_Node(re_ele);
	}
	virtual void print()override
	{
        cout << "Print AVL in inorder:\n";
        printInOrder();
        cout << "Print AVL in preorder:\n";
        printPreOrder();
    }

};

#endif

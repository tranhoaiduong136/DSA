//#include "main.h"
#include "Cache.h"
//////////////////////////////QUEUE_FIFO/////////////////////////////
Elem* FIFO::enqueue(Elem* e)
{
    if (head == NULL) {
			head = new NodeQueue(e);
			tail = head;}
		else {
            tail->next = new NodeQueue(e);
			tail = tail->next;}
		this->count++;
		return e;
}
Elem* FIFO::dequeue()
{
        if (isEmpty())return NULL;
		NodeQueue* temp = this->head;
		this->head = this->head->next;
		if (count == 1)  this->tail = NULL;
        Elem* elem = temp->elem;
            delete temp;
            count--;
		if(isEmpty()){head = tail = NULL;}
		return elem;
}
Elem* FIFO::replace(Elem* elem)
{
        NodeQueue* temp = this->head;
		while (temp && temp->elem->addr != elem->addr)temp = temp->next;
		if (temp) {
            Elem* t = temp->elem;
			delete temp->elem;
			temp->elem = elem;
			return t;}
        return NULL;
}
void FIFO::print()
{
        NodeQueue* temp = this->head;
		while(temp != NULL){
			temp->elem->print();
			temp = temp->next;}
        delete temp;
}
////////////////////////////SELF_ORG_LIST////////////////////////////////////
/////MRU//////
void MRU:: print()
{
     NodeList* t1 = head;
     while(t1 != NULL){
     t1->elem->print();
     t1 = t1->next;}
     delete t1;
}
void MRU::access(int addr)
{
    if(isEmpty())return;
    // Tranverse tp find addr
    NodeList* temp = this->head;
    NodeList* temp1 = temp; // previous node of temp
    while (temp && temp->elem->addr != addr)
            {   temp1 = temp; // prev node
                temp = temp->next;}
    if(!temp){
        temp1 = NULL;
        delete temp1;
        delete temp;
        return;} // Unfound

    // Swap node
    if(temp != head){
    temp1->next = temp->next;
    temp->next = head;
    head = temp;}
    if(tail == temp) tail = temp1;

    // Clear
    temp1 = NULL;
    temp = NULL;
    delete temp1;
    delete temp;

    return;
}
Elem* MRU::insert(Elem* e)
{
    NodeList* temp = new NodeList(e);
	if(head == NULL)
        {
            head = tail = temp;
            count++;
            return head->elem;
        }
	temp->next = this->head;
	this->head = temp;
	this->count++;
    return head->elem;
}
Elem* MRU::remove()
{
    //Remove most recently use (Head)
	if (count == 0)return NULL;

	NodeList* temp = this-> head;
	head = head->next;
	if (this->head == NULL)this->tail = NULL;

    Elem* value = temp->elem;
	delete temp;
	this->count--;
    return value;
}
Elem* MRU::replace(Elem* elem)
{
 NodeList* temp = this->head;
     while (temp && temp->elem->addr != elem->addr)temp = temp->next;
		if (temp) {
            Elem* t = temp->elem;
			access(temp->elem->addr);
			delete temp->elem;
			temp->elem = elem;
			return t;}
        return NULL;
}
//////////////LRU////////
Elem* LRU::remove()
{
     //Recent least recently use (Tail)
	if (isEmpty()) return NULL;

    Elem* re_elem = new Elem(tail->elem->addr,tail->elem->data,tail->elem->sync);
	if (head == tail) {
		this->head = NULL;
		this->tail = NULL;
		return re_elem;}

        NodeList* temp = head;
		while(temp->next != tail)temp = temp->next;
        NodeList* t1 = tail;
        temp->next = t1->next;
        this->tail = temp;
        delete t1;
        temp = NULL;
        delete temp;

	this->count--;
	return re_elem;
}

//////////////////////////////HEAP_TREE/////////////////////////////////////
void LFU::ReheapDown(int indx)
{
    if(indx < 0 || indx >= this->count)return;
    int temp = indx;
    int currPos;

    while(temp < count/2)
    {   
        currPos = temp; // parent
        if (arr[temp]->freq >= arr[2*currPos + 1]->freq) temp = 2*currPos + 1;
        if (   (count > 2* currPos + 2)
                &&arr[temp]->freq >= arr[2*currPos + 2]->freq)
        {   
            if(arr[temp]->freq > arr[currPos*2 +2]->freq )temp = 2*currPos + 2;
            else if(temp != (2*currPos +1) ) 
                                temp = 2*currPos + 2;
        }
        if(currPos == temp)break;
        //swap
        NodeHeap* t = arr[currPos];
        arr[currPos] = arr[temp];
        arr[temp] = t;
        t = NULL;
        delete t;
       
        
    }
}
void LFU::ReheapUp(int indx)
{
    if (indx < 0 || indx >= this->count)return;
    int temp = indx;
    while (temp > 0){
		int currPos = temp;
		if (arr[temp]->freq < arr[(currPos-1)/ 2]->freq)temp = (currPos - 1) / 2;
        if (currPos == temp)break;
        //swap
        NodeHeap* t = arr[currPos];
        arr[currPos] = arr[temp];
        arr[temp] = t;
        t = NULL;
        delete t;
	}
}
Elem* LFU::replace(Elem* e)
{   Elem* temp = NULL;
    for(int i = 0;i < count;i++){
    temp = arr[i]->value;
    if(temp->addr == e->addr)
             {
             arr[i]->value = e;
             ++arr[i]->freq;
             ReheapDown(i);
             return temp;
             }
    }
    return NULL;
}
Elem* LFU::insert(Elem* e)
{
    Elem *temp = NULL;
    arr[this->count] = new NodeHeap(e); // Add in tail
    this->count++;
    ReheapUp(this->count-1);
    return temp;
}
void LFU::access(int addr)
{   if(isEmpty())return;

    for(int i = 0;i < count;++i)
    { if(arr[i]->value->addr == addr)
        {
           ++arr[i]->freq;
           ReheapDown(i);
           break;
        }
    }
}
Elem* LFU::remove()
{
    if(isEmpty())return NULL;

    NodeHeap* temp = arr[0];
    arr[0] = arr[this->count-1]; // swap the last to the first
    arr[this->count - 1] = NULL;
    this->count--;

    ReheapDown(0);

    Elem* data = temp->value;
	delete temp;
	return data;
}
///////////////////////////////DBHASHING//////////////////////////////////////
void DBHashing::insert(int addr,Data* value,bool w)
{
    if(curr_size == size)return; // Full location
    int index1 = this->hash1(addr);
    if(a[index1 % size] != NULL)
    {
        int index2 = this->hash2(addr);
        int i = 1;
        while(i < 2*size)
        {
            int new_index = (index1 + i * index2) % size;
            if(a[new_index] == NULL)
            {   Elem *t = new Elem(addr,value,w);
                a[new_index] = t;
                t = NULL;
                delete t;
                ++curr_size;
                return;
            }
            i++;
        }
    }
    else
    {   index1 = index1 % size;
        Elem* t = new Elem(addr,value,w);
        a[index1] = t;
        t = NULL;
        delete t;
        ++curr_size;
        return;
    }
}
Data* DBHashing::search(int addr)
{
    int index1 = hash1(addr);
    int index2 = hash2(addr);
    int i = 0;
    if(curr_size == 0) return NULL;
    // Searching
    while (i < 2*size)
            {
            if(a[(index1 + i * index2) % size] != NULL)
                {   if (a[(index1 + i * index2) % size]->addr == addr)
                        {
                        return a[(index1+ i*index2)%size]->data;
                        }
                }
                i++;
            }
    return NULL;
}
void DBHashing::deleteNode(int addr)
{
        int index1 = hash1(addr);
        int index2 = hash2(addr);
        int i = 0;
         while (i < 2*size)
            {
            int t = (index1 + i * index2) % size;
            if(a[t] != NULL)
                {
                    if (a[t]->addr == addr)
                        {
                          a[t] = NULL;
                           --curr_size;
                          return;
                        }
                }
              i++;
            }
}
Elem* DBHashing::replace(Elem* e)
{
    for(int i =0;i< size;i++)
    {
        if(a[i] != NULL)if(a[i]->addr == e->addr){a[i] = e;break;}
    }
    return e;
}

///////////////////////////////AVL_TREE///////////////////////////////////////
Node* AVL::rr_rotate(Node* &node) {
	Node* leftNode = node->left;
	node->left = leftNode->right;
	leftNode->right = node;
	return leftNode;
}

Node* AVL::ll_rotate(Node* &node) {
	Node* rightNode = node->right;
	node->right = rightNode->left;
	rightNode->left = node;
	return rightNode;
}

Node* AVL::leftBalance(Node* &node, bool& taller) {
	Node* leftNode = node->left;
	taller = false;
	if (leftNode->balance == -1){
		node->balance = 0;
		leftNode->balance = 0;
		node = rr_rotate(node);
	}
	else if (leftNode->balance == 1){
		Node* rightNode = leftNode->right;
		if (rightNode->balance == 1) {
			leftNode->balance = -1;
			node->balance = 0;
		}
		else if (rightNode->balance == -1){
			leftNode->balance = 0;
			node->balance = 1;
		}
		else {
			leftNode->balance = 0;
			node->balance = 0;
		}
		rightNode->balance = 0;
		node->left = ll_rotate(leftNode);
		node = rr_rotate(node);
	}
	return node;
}

Node* AVL::rightBalance(Node* &node, bool& taller) {
	Node* rightNode = node->right;
	taller = false;
	if (rightNode->balance == 1) {
		node->balance = 0;
		rightNode->balance = 0;
		node = ll_rotate(node);
	}
	else if (rightNode->balance == -1) {
		Node* leftNode = rightNode->left;
		if  (leftNode->balance == -1){
			rightNode->balance = 1;
			node->balance = 0;
		}
		else if (leftNode->balance == 1){
			rightNode->balance = 0;
			node->balance = -1;
		}
		else {
			rightNode->balance = 0;
			node->balance = 0;
		}
		leftNode->balance = 0;
		node->right = rr_rotate(rightNode);
		node = ll_rotate(node);
	}
	return node;
}
int AVL::get_height(Node *node) const{
	if (node == NULL) return 0;
    int l_height = get_height(node->left);
    int r_height = get_height(node->right);
    if(l_height > r_height)return l_height + 1;
    else return r_height + 1;
}

int AVL::getHeight() const{
	return get_height(this->root);
}
Data* AVL::get_node(const Node* node, int addr) const {
    if (node == NULL)return NULL;
    if (node->value->addr == addr)return node->value->data;
    if (addr < node->value->addr) return get_node(node->left, addr);
    else return get_node(node->right, addr);
}

Data* AVL::getNode(int addr) const{
    return get_node(root, addr);
}

Node* AVL::insert_node(Node* &node, Elem* value, bool& taller) {
	if (node == NULL) {
		node = new Node(value);
		taller = true;
		return node;}
    if (value->addr < node->value->addr) {
		node->left = insert_node(node->left, value, taller);
		if (taller) {
			if (node->balance == 1) {
				node->balance = 0;
				taller = false;
			}
			else if (node->balance == 0) {
				node->balance = -1;
				taller = true;
			}
			else node = leftBalance(node, taller);
        }
	}
	else {
		node->right = insert_node(node->right, value, taller);
		if (taller) {
			if ( node->balance == -1) {

				node->balance = 0;
				taller = false;
			}
			else if (node->balance == 0) {
				node->balance = 1;
				taller = true;
			}
			else node = rightBalance(node, taller);}
	}
	return node;
}

void AVL::insertNode(Elem* value) {
    bool is_taller = false;
    insert_node(this->root, value, is_taller);}

Node* AVL::removeLeftBalance(Node* &node, bool &shorter) {
	if (node->balance == 1) {
		node->balance = 0;
		shorter = true;}
	else if (node->balance == 0) {
		node->balance = -1;
		shorter = false;}
	else {
		Node* leftNode = node->left;
		if (leftNode->balance == 1) {
			Node* rightNode = leftNode->right;
			if (rightNode->balance == 1) {
				rightNode->balance = 0;
				leftNode->balance = -1;
				node->balance = 0;
			}
			else if (rightNode->balance == -1) {
				rightNode->balance = 0;
				leftNode->balance = 0;
				node->balance = 1;
			}
			else {
				leftNode->balance = 0;
				node->balance = 0;
			}
			node->left = ll_rotate(leftNode);
			node = rr_rotate(node);
		}
		else if (leftNode->balance == -1) {
			node->balance = 0;
			leftNode->balance = 0;
			node = rr_rotate(node);
		}
		else {
			node->balance = -1;
			leftNode->balance = 1;
			shorter = false;
			node = rr_rotate(node);
		}
	}
	return node;
}

Node* AVL::removeRightBalance(Node* &node, bool &shorter) {
	if (node->balance == -1) {
		node->balance = 0;
		shorter = true;
	}
	else if (node->balance == 0) {
		node->balance = 1;
		shorter = false;
	}
	else {
		Node* rightNode = node->right;
		if (rightNode->balance == -1) {
			Node* leftNode = rightNode->left;
			if (leftNode->balance == -1) {
				rightNode->balance = 1;
				node->balance = 0;
			}
			else if (leftNode->balance == 0) {
				node->balance = 0;
				rightNode->balance = 0;
			}
			else {
				node->balance = -1;
				rightNode->balance = 0;
			}
			leftNode->balance = 0;
			node->right = rr_rotate(rightNode);
			node = ll_rotate(node);
		}
		else if (rightNode->balance == 1) {
			node->balance = 0;
			rightNode->balance = 0;
			node = ll_rotate(node);
		}
		else {
			node->balance = 1;
			rightNode->balance = -1;
			node = ll_rotate(node);
			shorter = false;}
	}
	return node;
}

Node* AVL::delete_node(Node* &node, const Elem* value, bool & shorter, bool &success) {
	if (node == NULL) {
		shorter = false;
		success = false;}

	else if (value->addr < node->value->addr) {
		node->left = delete_node(node->left, value, shorter, success);
		if (success)
            if (shorter)node = removeRightBalance(node, shorter);

	}
	else if (value->addr > node->value->addr) {
		node->right = delete_node(node->right, value ,shorter, success);
		if (success) {
			if (shorter)node = removeLeftBalance(node, shorter);
        }
	}
	else {
		if (node->left == NULL) {
			Node* temp = node;
			node = node->right;
			delete temp;
			success = true;
			shorter = true;
		}
		else if (node->right == NULL) {
			Node* temp = node;
			node = node->left;
			delete temp;
			success = true;
			shorter = true;
		}
		else {
			Node* temp = node->right;
			while(temp->left)temp = temp->left;
            node->value = temp->value;
			node->right = delete_node(node->right, temp->value, shorter, success);
			if (shorter) node = removeLeftBalance(node, shorter);
        }
	}
	return node;
}

void AVL::print_preorder(Node* node) {
    if (node == NULL) return;
    node->value->print();
    print_preorder(node->left);
    print_preorder(node->right);
}
void AVL::printPreOrder() {
    print_preorder(this->root);
}

void AVL::print_inorder(Node* node) {
    if (node == NULL) return;
    print_inorder(node->left);
    node->value->print();
    print_inorder(node->right);}
void AVL::printInOrder() {
    print_inorder(this->root);}

void AVL::delete_Node(Elem* key){
    bool is_short = false;
	bool is_success = false;
    delete_node(this->root, key, is_short, is_success);}

bool AVL::searchReplace(Elem* elem) {
    Node* node = this->root;
    while (node) {
        if (node->value->addr == elem->addr){
            node->value = elem;
            return true;}
        else if (elem->addr < node->value->addr) node = node->left;
        else node = node->right;}
    return false;
}

// Clear all the data
void AVL::clear(Node* node)
{
    if (!node->left && !node->right) {
		delete node;
		node = NULL;
		return;
    }
    if (node->left) clear(node->left);
	if (node->right) clear(node->right);
	delete node;
}
///////////////////////////////////////////////////////////CACHE///////////////////////////////////////////
Cache::Cache(SearchEngine *s, ReplacementPolicy *r) : rp(r), s_engine(s) {}
Cache:: ~Cache()
{
    delete rp;
    delete s_engine;
}
Data *Cache::read(int addr)
{
    if(rp->isEmpty())return NULL;
    Data* temp = s_engine->search(addr);// s_engine
    if(temp != NULL)rp->access(addr); // LFU vs MRU vs LRU
    return temp;
}
Elem *Cache::put(int addr, Data *cont)
{   Elem* remove_ele = NULL;
    Elem* add_ele = new Elem(addr, cont, true);
    if(rp->isFull())
    {
        remove_ele = rp->remove();
        s_engine->deleteNode(remove_ele->addr);
    }
    rp->insert(add_ele);
    s_engine->insert(addr,cont,true);
    return remove_ele;
}
Elem *Cache::write(int addr, Data *cont)
{
    Elem* ele = NULL;
    Elem* add_ele = new Elem(addr, cont, false);
    if(s_engine->search(addr) == NULL)
    {
        if(rp->isFull())
        {
        //cout<<"Full"<<endl;
        ele = rp->remove();
        s_engine->deleteNode(ele->addr);
        }
        //cout<<"Add in"<<endl;
        rp->insert(add_ele);
        s_engine->insert(addr,cont,false);
        return ele;
    }
    else
    {
        rp->replace(add_ele);
        s_engine->replace(add_ele);
        return NULL;
    }
}
void Cache::printRP()
{
    rp->print();
}
void Cache::printSE()
{
    s_engine->print();
}

//#include "main.h"
#include "Cache.h"
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

void AVL::deleteNode(Elem* key){
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
Data* Cache::read(int addr) {
    return tree->getNode(addr);
}
Elem* Cache::put(int addr, Data* cont) {
    Elem* re_ele = NULL;
    Elem* add_ele = new Elem(addr, cont, true);
    if (this->q->getSize() == MAXSIZE) {
        re_ele = this->q->dequeue();
        this->tree->deleteNode(re_ele);
    }
	this->tree->insertNode(add_ele);
    this->q->enqueue(add_ele);
    return re_ele;
}

Elem* Cache::write(int addr, Data* cont) {
    Elem* re_ele = NULL;
    Elem* add_ele = new Elem(addr, cont, false);
    if (this->tree->searchReplace(add_ele) == false)
        {
        if (this->q->getSize() == MAXSIZE)
            {
            re_ele = this->q->dequeue();
            this->tree->deleteNode(re_ele);
            }
        this->q->enqueue(add_ele);
        this->tree->insertNode(add_ele);
        return re_ele;
        }
    else {
        this->q->replace_node(add_ele);
        return NULL;}
}
void Cache::print() {
    this->q->print();
}
void Cache::preOrder() {
	this->tree->printPreOrder();
}
void Cache::inOrder() {
    this->tree->printInOrder();
}

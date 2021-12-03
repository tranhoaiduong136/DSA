//#include "main.h"
#include "Cache.h"
//AVL:
int AVL::get_Height(Node* node)
{
    if(!node) return 0;
    int l_h = get_Height(node->left);
    int r_h = get_Height(node->right);
    return(l_h > r_h)? l_h + 1: r_h + 1;
}
Node* AVL::insert_AVL(Node*&node, Elem* key,bool& is_tall)
{
    if (node == NULL) {
		node = new Node(key);
		is_tall = true;
		return node;}

	if (key->addr < node->val->addr) {
		node->left = insert_AVL(node->left, key, is_tall);
		if (is_tall) {
			if (node->balance == RH) {node->balance = EH;
				is_tall = false;}
			else if (node->balance == EH){node->balance = LH;
				is_tall = true;}
			else {
				node = BalanceLeft(node, is_tall);
			}
		}
	}
	else {
		node->right = insert_AVL(node->right,key,is_tall);
		if (is_tall) {
			if ( node->balance == LH) {
                node->balance = EH;
				is_tall = false;}
			else if (node->balance == EH) {
				node->balance = RH;
				is_tall = true;}
			else {
				node = BalanaceRight(node, is_tall);}
		}
	}
	return node;
}
Node* AVL::BalanceLeft_del(Node*& node, bool& is_short) {
        if (node->balance == RH) {
		node->balance = EH;
		is_short = true;
	}
	else if (node->balance == EH) {
		node->balance = RH;
		is_short = false;}
	else {
		Node* left_Node = node->left;
		if (left_Node->balance == RH) {
			Node* right_Node = left_Node->right;
			if (right_Node->balance == RH) {
				right_Node->balance = EH;
				left_Node->balance = LH;
				node->balance = EH;}
			else if (right_Node->balance == LH) {
				right_Node->balance = EH;
				left_Node->balance = EH;
				node->balance = RH;
			}
			else {
				left_Node->balance = EH;
				node->balance = EH;}
			node->left = rot_LL(left_Node);
			node = rot_RR(node);
		}
		else if (left_Node->balance == LH) {
			node->balance = EH;
			left_Node->balance = EH;
			node = rot_RR(node);
		}
		else {
			node->balance = LH;
			left_Node->balance = RH;
			is_short = false;
			node = rot_RR(node);
		}
	}
	return node;
}
Node* AVL::BalanaceRight_del(Node*& node,bool& shortt) {
        if (node->balance == LH) {node->balance = EH;shortt = true;}
	else if (node->balance == EH) {node->balance = RH;shortt = false;}
	else {
		Node* rightNode = node->right;
		if (rightNode->balance == LH) {
			Node* leftNode = rightNode->left;
			 if (leftNode->balance == EH) {
				node->balance = EH;
				rightNode->balance = EH;
			}
			else if (leftNode->balance == LH) {
				rightNode->balance = RH;
				node->balance = EH;
			}
			else {
				node->balance = LH;
				rightNode->balance = EH;
			}
			leftNode->balance = EH;
			node->right = rot_RR(rightNode);
			node = rot_LL(node);
		}
		else if (rightNode->balance == RH) {
			node->balance = EH;
			rightNode->balance = EH;
			node = rot_LL(node);
		}
		else {
			node->balance = RH;
			rightNode->balance = LH;
			node = rot_LL(node);
			shortt = false;
		}
	}
	return node;
}
Node* AVL::remove_node(Node*& node, Elem* value,bool& shortt,bool& success) {
       if (node == NULL) {
		shortt = false;
		success = false;}

	else if (value->addr < node->val->addr) {
		node->left = remove_node(node->left, value, shortt, success);
		if (success)if (shortt)node = BalanaceRight_del(node, shortt);
    }
	else if (value->addr > node->val->addr) {
		node->right = remove_node(node->right, value ,shortt, success);
    if (success)if (shortt)node = BalanceLeft_del(node, shortt);
    }
	else {
		if (node->right == NULL) {
			Node* temp = node;
			node = node->left;
			delete temp;
			shortt = true;
			success = true;}
		else if (node->left == NULL) {
			Node* temp = node;
			node = node->right;
			delete temp;
			success = true;
			shortt = true;
		}
        else {
			Node* temp = node->right;
			while(temp->left)temp = temp->left;
            node->val = temp->val;
			node->right = remove_node(node->right, temp->val, shortt, success);
			if (shortt) node = BalanceLeft_del(node, shortt);
        }
	}
	return node;
    }
Node* AVL::rot_LL(Node*& node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        return tmp;}
Node* AVL::rot_RR(Node*& node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        return tmp;}
Node* AVL::BalanceLeft(Node*& node, bool& is_tall) {
        is_tall = false;
        Node* left_Node = node->left;
        if(left_Node->balance == LH){
		node->balance = EH;
		left_Node->balance = EH;
		node = rot_RR(node);}

        else if(left_Node->balance == RH)
        {
            Node* right_Node = left_Node->right;
            if (right_Node->balance == RH) {
			left_Node->balance = LH;
			node->balance = EH;}
            else if (right_Node->balance == LH){
			left_Node->balance = EH;
			node->balance = RH;}
            else {
            left_Node->balance = EH;
            node->balance = EH;}

            right_Node->balance = EH;
            node->left = rot_LL(left_Node);
            node = rot_RR(node);}


	return node;
}
Node* AVL::BalanaceRight(Node*& node,bool& is_tall) {
        Node* right_Node = node->right;
        is_tall = false;
        if (right_Node->balance == RH) {
		node->balance = EH;
		right_Node->balance = EH;
		node = rot_LL(node);}

        else if(right_Node->balance == LH) {
		Node* left_Node = right_Node->left;
        if  (left_Node->balance == LH) {
			right_Node->balance = RH;
			node->balance = EH;
		}
		else if (left_Node->balance == RH){
			right_Node->balance = EH;
			node->balance = LH;
		}
		else {
			right_Node->balance = EH;
			node->balance = EH;
		}
		left_Node->balance = EH;
		node->right = rot_RR(right_Node);
		node = rot_LL(node);
	}

	return node;
}
void AVL ::Print_Inorder(Node* node)
{
            if (node == NULL) return;
            Print_Inorder(node->left);
            node->val->print();
            Print_Inorder(node->right);}
void AVL ::Print_PreOrder(Node* node)
{
    if (node == NULL) return;
    node->val->print();
    Print_PreOrder(node->left);
    Print_PreOrder(node->right);
}
Data* AVL::get_node( Node* node, int addr) {
    if (node == NULL)return NULL;
    if (node->val->addr == addr)return node->val->data;
    if (addr < node->val->addr) return get_node(node->left, addr);
    else return get_node(node->right, addr);
}

bool AVL::search_place(Elem* elem) {
    Node* node = this->root;
    while (node != NULL){
        if (node->val->addr == elem->addr){
            node->val = elem;
            return 1;
            }
        else if (elem->addr < node->val->addr) node = node->left;
        else node = node->right;}
    return 0;
}
// Cache:
Data* Cache::read(int addr) {
	return avl_tree->getNode(addr);
}
Elem* Cache::put(int addr, Data* cont) {
    Elem* remove_ele = NULL;
    Elem* add_ele = new Elem(addr, cont, true);
    if (que->getSize() == MAXSIZE) {
        remove_ele = que->dequeue();
        avl_tree->deleteAVL(remove_ele);
    }
	avl_tree->insertAVL(add_ele);
    que->enqueue(add_ele);
    return remove_ele;
}

Elem* Cache::write(int addr, Data* cont) {
    Elem* ele = NULL;
    Elem* add_ele = new Elem(addr, cont, false);

    if (!avl_tree->search_place(add_ele))
        {
        if (que->getSize() == MAXSIZE) {
            ele = que->dequeue();
            avl_tree->deleteAVL(ele);}
        que->enqueue(add_ele);
        avl_tree->insertAVL(add_ele);
        return ele;
        }
    else {
        que->replace_node(add_ele);
        return NULL;}
}
void Cache::print() {que->print();}
void Cache::preOrder() {avl_tree->print_pr();}
void Cache::inOrder() {avl_tree->print_in();}

//
// Created by guyro on 20/11/2022.
//

#ifndef WET2_AVLTREE_H
#define WET2_AVLTREE_H

#include <iostream>
#include "stdio.h"
using std::ostream;

template<class Key, class Value>
class Node {
public:
    Key key;
    int height;
    int balance_factor; //should be -2,-1,0,1,2
    Value data;
    Node* dad;
    Node* left_son;
    Node* right_son;
    int size_of_subTree; //number of nodes in the subTree

    Node() : key(), height(0),balance_factor(0),data(),
             dad(nullptr), left_son(nullptr), right_son(nullptr), size_of_subTree(1) {}

    Node(Key key, Value data) {
        this->key = key;
        this->height = 0;
        this->balance_factor = 0;
        this->data = data;
        this->dad = nullptr;
        this->left_son = nullptr;
        this->right_son = nullptr;
        this->size_of_subTree = 1;
    }

    //---------------------------------------------------------------------------------
    //--------------general fuctionality
    //---------------------------------------------------------------------------------
    friend ostream& operator<<(ostream& os, Node node) {

        os << "Key: "<< node.key;
        if(node.right_son != nullptr){
            os << "     Right Key: "<< node.right_son->key;
        } else{
            os << "     Right Key: null";
        }

        if(node.left_son != nullptr){
            os << "     Left Key: "<< node.left_son->key;
        } else{
            os << "     left Key: null";
        }

        if(node.dad != nullptr){
            os << "     Dad Key: "<< node.dad->key;
        } else{
            os << "     Dad Key: null";
        }

        os << "   height: " <<node.height;
        os<< "   Bf: "<<node.balance_factor;
        os<< "   Size: "<<node.size_of_subTree;

        os << std::endl;
        return os;
    }
    //---------------------------------------------------------------------------------
    Node* next_in_order(){
        Node* to_return = nullptr;
        Node* curr = this;
        //----------------------Has right son
        if(right_son != nullptr){
            if(right_son->left_son == nullptr){
                return right_son;
            }else{
                to_return = right_son->left_son;
                while(to_return->left_son != nullptr){
                    to_return = to_return-> left_son;
                }
                return to_return;
            }
        }
        //----------------------Not right son
        if(right_son == nullptr){
            //----------------------No dad
            if(curr->dad == nullptr){
                return nullptr;
            }
            //----------------------Has dad
            if(dad != nullptr){
                while(curr->dad == nullptr){
                    if(curr == curr->dad->left_son){
                        break;
                    }

                    curr = curr->dad;
                }
                return curr->dad;
            }
        }
        return nullptr; //shouldn't get here I think
    }
    //---------------------------------------------------------------------------------
    Node* previous_in_order(){
        Node* to_return = nullptr;
        Node* curr = this;
        //----------------------Has left son
        if(left_son != nullptr){
            if(left_son->right_son == nullptr){
                return left_son;
            }else{
                to_return = left_son->right_son;
                while(to_return->right_son != nullptr){
                    to_return = to_return->right_son;
                }
                return to_return;
            }
        }
        //----------------------No left son
        if(left_son == nullptr){
            //----------------------No dad
            if(dad == nullptr){
                return nullptr;
            }
            //----------------------Has dad
            if(dad != nullptr){
                while(curr->dad == nullptr){
                    if(curr == curr->dad->right_son){
                        break;
                    }

                    curr = curr->dad;
                }
                return curr->dad;
            }
        }
        return nullptr; //shouldn't get here I think
    }
    //---------------------------------------------------------------------------------

};


template<class Key, class Value>
class AVLTree{
//    typedef class Node<Key,Value> Node;

public:

    //---------------------------------------------------------------------------------
    AVLTree() : root(nullptr) ,num_of_nodes(0){};
    ~AVLTree(){
        //deletion in postorder
        DeleteTree(root);
    }
    void DeleteTree(Node<Key,Value> *root){
        if(root == nullptr ){
            return;
        }
        //----------Doing the deleting------------------
        //delete works only for deleting actual data placed as a "Values"
        DeleteTree(root->left_son);
        DeleteTree(root->right_son);
        delete root;
    }
    //---------------------------------------
    Node<Key,Value>* find(const Key& key, bool insert = false) const{
        Node<Key,Value>* ptr  = this->root;
        Node<Key,Value>* prev = ptr;
        while(ptr != nullptr && ptr->key != key){
            prev = ptr;
            if(key < ptr->key){
                ptr = ptr->left_son;
            } else{
                ptr = ptr->right_son;
            }
        }
        if(insert == false){
            return ptr;
        } else{
            if(ptr != nullptr){
                return nullptr;
            }
            return prev;
        }
    }
    //---------------------------------------------------------------------------------
    bool insert(const Key& key, Value value){
        if(this->root == nullptr){
            num_of_nodes ++;
            Node<Key,Value>* new_node = new Node<Key,Value>(key, value);
            this->root = new_node;
            return true;
        }

        Node<Key,Value>* prev = find(key, true);
        if(prev == nullptr){
            return false;
        } else{
            num_of_nodes ++;
            Node<Key,Value>* new_node = new Node<Key,Value>(key, value);
            if(key < prev->key){
                prev->left_son = new_node;
                new_node->dad = prev;
            } else{
                prev->right_son = new_node;
                new_node->dad = prev;
            }

            fix_size_in_insert(new_node);

            Node<Key,Value>* to_fix = update_route(new_node );
            fix_node(to_fix);
            update_route(new_node);

            return true;
        }
    }
    //---------------------------------------------------------------------------------
    void remove(const Key& key){
        Node<Key,Value>* to_delete = find(key);
        if(to_delete == nullptr){
            return;//Needs to inform of failure
        }
        //-------------------------------------
        if(to_delete->left_son != nullptr && to_delete->right_son != nullptr){
            //--------------next inorder-----------
            Node<Key,Value>* to_switch_with = to_delete->right_son;
            while (to_switch_with->left_son != nullptr){
                to_switch_with = to_switch_with->left_son;
            }
            //-------------------------------------
            switch_nodes(to_delete, to_switch_with);
            to_delete = to_switch_with;
        }
        //----------------------------------------------------------------------
        if(to_delete->left_son == nullptr && to_delete->right_son == nullptr){
            if(to_delete == this->root){
                this->root = nullptr;

                delete to_delete;
                to_delete = nullptr;
                this->num_of_nodes--;
                return;
            }

            fix_size_in_remove(to_delete);

            if(to_delete->dad->left_son == to_delete){
                to_delete->dad->left_son = nullptr;
                update_height_balance(to_delete->dad);
                fix_route(to_delete->dad);
                delete to_delete;
                to_delete = nullptr;
                this->num_of_nodes--;
                return;
            } else{
                to_delete->dad->right_son = nullptr;
                update_height_balance(to_delete->dad);
                fix_route(to_delete->dad);
                delete to_delete;
                to_delete = nullptr;
                this->num_of_nodes--;
                return;
            }
        }
        //----------------------------------------------------------------------
        if((to_delete->left_son == nullptr && to_delete->right_son != nullptr) ||
           (to_delete->left_son != nullptr && to_delete->right_son == nullptr)){
            if(to_delete == this->root){
                if(to_delete->left_son != nullptr){
                    this->root = to_delete->left_son;
                    to_delete->left_son->dad = nullptr;
                }else{
                    this->root = to_delete->right_son;
                    to_delete->right_son->dad = nullptr;
                }

                delete to_delete;
                to_delete = nullptr;
                this->num_of_nodes--;
                return;
            }

            fix_size_in_remove(to_delete);

            if(to_delete->dad->right_son == to_delete){
                if(to_delete->right_son != nullptr){
                    to_delete->dad->right_son = to_delete->right_son;
                    to_delete->right_son->dad = to_delete->dad;
                }else {
                    to_delete->dad->right_son = to_delete->left_son;
                    to_delete->left_son->dad = to_delete->dad;
                }
            }
            if(to_delete->dad->left_son == to_delete){
                if(to_delete->right_son != nullptr){
                    to_delete->dad->left_son = to_delete->right_son;
                    to_delete->right_son->dad = to_delete->dad;
                }else {
                    to_delete->dad->left_son = to_delete->left_son;
                    to_delete->left_son->dad = to_delete->dad;
                }
            }
            update_height_balance(to_delete->dad);
            fix_route(to_delete->dad);
            delete to_delete;
            to_delete = nullptr;
            this->num_of_nodes--;
            return;
        }
        //----------------------------------------------------------------------
    }
    //---------------------------------------------------------------------------------
    int get_height(const Node<Key,Value>* node) const{
        return  (node == nullptr) ? -1 : node->height;
    }
    //---------------------------------------------------------------------------------
    void update_height_balance(Node<Key,Value>* node){
        int l_height = get_height(node->left_son);
        int r_height = get_height(node->right_son);

        if(l_height >= r_height){
            node->height = 1 + l_height;
        } else{
            node->height = 1 + r_height;
        }
        node->balance_factor = l_height - r_height;
    }
    //---------------------------------------------------------------------------------
    //Returns first bad node from the bottom
    Node<Key,Value>* update_route(Node<Key,Value>* ptr){
        while (ptr != nullptr){
            update_height_balance(ptr);
            if(ptr->balance_factor == -2 || ptr->balance_factor == 2){
                break;
            }
            ptr = ptr->dad;
        }
        return ptr;
    }
    //---------------------------------------------------------------------------------
    void fix_node(Node<Key,Value>* ptr){
        if(ptr == nullptr){
            return;
        }

        if(ptr->balance_factor == 2 && ptr->left_son != nullptr && ptr->left_son->balance_factor >= 0){
            roll_LL(ptr);
        }
        if(ptr->balance_factor == 2 && ptr->left_son != nullptr && ptr->left_son->balance_factor == -1){
            roll_LR(ptr);
        }
        if(ptr->balance_factor == -2 && ptr->right_son != nullptr && ptr->right_son->balance_factor <= 0){
            roll_RR(ptr);
        }
        if(ptr->balance_factor == -2 && ptr->right_son != nullptr && ptr->right_son->balance_factor == 1){
            roll_RL(ptr);
        }

    }
    //---------------------------------------------------------------------------------
    void fix_route(Node<Key,Value>* ptr){
        Node<Key,Value>* next_to_fix = update_route(ptr);

        while (next_to_fix != nullptr){
            fix_node(next_to_fix);
            next_to_fix = update_route(next_to_fix->dad);
        }
    }
    //---------------------------------------------------------------------------------
    //update heights and balance
    void roll_LL(Node<Key,Value>* ptr){
        Node<Key,Value>* b = ptr;// bad node
        Node<Key,Value>* a = b->left_son;
        Node<Key,Value>* ar = a->right_son;
        if(this->root == b){
            this->root = a;
        }

        if(b->dad != nullptr){
            if(b->dad->right_son == b){
                b->dad->right_son = a;
            } else{
                b->dad->left_son = a;
            }
        }

        a->dad = b->dad;
        a->right_son = b;
        b->dad = a;
        b->left_son = ar;

        int ar_size = 0;
        if(ar != nullptr){
            ar->dad = b;
            ar_size = ar->size_of_subTree;
        }

        update_height_balance(a);
        update_height_balance(b);

        int br_size = 0;
        if(b->right_son != nullptr){
            br_size = b->right_son->size_of_subTree;
        }
        int al_size = 0;
        if(a->left_son != nullptr){
            al_size = a->left_son->size_of_subTree;
        }
        b->size_of_subTree = 1 + br_size + ar_size;
        a->size_of_subTree = 1 + al_size + b->size_of_subTree;
    }

    //---------------------------------------------------------------------------------
    //update heights and balance
    void roll_RR(Node<Key,Value>* ptr){
        Node<Key,Value>* b = ptr;
        Node<Key,Value>* a = b->right_son;
        Node<Key,Value>* al = a->left_son;
        if(this->root == b){
            this->root = a;
        }

        if(b->dad != nullptr){
            if(b->dad->right_son == b){
                b->dad->right_son = a;
            } else{
                b->dad->left_son = a;
            }
        }

        a->dad = b->dad;
        a->left_son = b;
        b->dad = a;
        b->right_son = al;

        int al_size = 0;
        if(al != nullptr){
            al->dad = b;
            al_size = al->size_of_subTree;
        }

        update_height_balance(a);
        update_height_balance(b);

        int bl_size = 0;
        if(b->left_son != nullptr){
            bl_size = b->left_son->size_of_subTree;
        }
        int ar_size = 0;
        if(a->right_son != nullptr){
            ar_size = a->right_son->size_of_subTree;
        }
        b->size_of_subTree = 1 + bl_size + al_size;
        a->size_of_subTree = 1 + ar_size + b->size_of_subTree;
    }
    //---------------------------------------------------------------------------------
    void roll_LR(Node<Key,Value>* ptr){
        roll_RR(ptr->left_son);
        roll_LL(ptr);
    }
    //---------------------------------------------------------------------------------
    void roll_RL(Node<Key,Value>* ptr){
        roll_LL(ptr->right_son);
        roll_RR(ptr);
    }
    //---------------------------------------------------------------------------------
    void switch_nodes(Node<Key,Value>* node1, Node<Key,Value>* node2){
        Key temp_key = node1->key;
        Value temp_data = node1->data;
        node1->key = node2->key;
        node1->data = node2->data;
        node2->key = temp_key;
        node2->data = temp_data;

    }
    //---------------------------------------------------------------------------------
    void switch_sub_trees(Node<Key,Value>* node1, Node<Key,Value>* node2){
        Node<Key,Value>* temp = node1;
        node1 = node2;
        node2 = temp;
    }
    //---------------------------------------------------------------------------------
    Node<Key,Value>* get_root() const{
        return this->root;
    }
    //---------------------------------------------------------------------------------
    void set_root(Node<Key,Value>* new_root){
        this->root = new_root;
    }
    //---------------------------------------------------------------------------------
    void inorderWriteToArray(Node<Key,Value>* ptr, int *const output_array, int* curr_index){

        if(ptr == nullptr){
            return;
        }
        inorderWriteToArray(ptr->left_son, output_array,curr_index);
        //-----------Doing something--------------
        output_array[*curr_index] = ptr->data;
        (*curr_index) ++;
        //------------------------------------
        inorderWriteToArray(ptr->right_son, output_array,curr_index);
    }
    //---------------------------------------------------------------------------------
    Node<Key,Value>* merge_array(Node<Key,Value>* arr1, int arr1_size, Node<Key,Value>* arr2, int arr2_size, Node<Key,Value>* arr3){
        int a = 0, b = 0, c = 0;
        while (a < arr1_size && b < arr2_size){
            if ((arr1[a].key) < (arr2[b].key)){
                arr3[c] = arr1[a];
                a++;
            }else{
                arr3[c] = arr2[b];
                b++;
            }
            c++;
        }
        while (a < arr1_size)
        {
            arr3[c] = arr1[a];
            a++;
            c++;
        }
        while (b < arr2_size)
        {
            arr3[c] = arr2[b];
            b++;
            c++;
        }
        return arr3;
    }
    //---------------------------------------------------------------------------------
    Node<Key,Value>* sorted_arr_to_avl_tree(Node<Key,Value>* arr, int begin, int end){
        if(begin > end){
            return nullptr;
        }

        int mid = (begin + end)/2;
        Node<Key,Value>* re_root = new Node<Key,Value>(arr[mid].key,arr[mid].data);

        re_root->left_son = sorted_arr_to_avl_tree(arr, begin, mid - 1);

        re_root->right_son = sorted_arr_to_avl_tree(arr, mid + 1, end);

        if(re_root->left_son != nullptr){
            re_root->left_son->dad = re_root;
        }
        if(re_root->right_son != nullptr){
            re_root->right_son->dad = re_root;
        }

        update_height_balance(re_root);

        return re_root;

    }
    //---------------------------------------------------------------------------------
    void inorderInRangeByKeyToArray(Node<Key,Value>* ptr, int min, int max, int *output_array, int *curr_index){

        if(output_array == nullptr){
            return;
        }
        if(ptr == nullptr){
            return;
        }
        inorderInRangeByKeyToArray(ptr->left_son, min, max,output_array,curr_index);
        //-----------Doing something--------------
        if(ptr->key >= min && ptr->key <= max){

            output_array[*curr_index] = ptr->key;
            (*curr_index)++;
        }
        //------------------------------------
        inorderInRangeByKeyToArray(ptr->right_son, min, max, output_array,curr_index);
    }
    //---------------------------------------------------------------------------------
    void inorderNodesToArray(Node<Key,Value>* root, Node<Key,Value> *output_array, int *curr_index){

        if(output_array == nullptr){
            return;
        }
        if(root == nullptr){
            return;
        }
        inorderNodesToArray(root->left_son,output_array,curr_index);
        //-----------Doing something--------------
        Node<Key,Value> root_copy = *root;
        output_array[*curr_index] = root_copy;
        (*curr_index)++;
        //------------------------------------
        inorderNodesToArray(root->right_son, output_array,curr_index);
    }
    //---------------------------------------------------------------------------------
    void inorderInRange(Node<Key,Value>* ptr, int min, int max, int* res, int *curr_index){

        if(ptr == nullptr){
            return;
        }
        inorderInRange(ptr->left_son, min, max, res, curr_index);
        //-----------Doing something--------------
        if(ptr->key >= min && ptr->key <= max){
            (*res)++;
        }
        (*curr_index)++;
        //------------------------------------
        inorderInRange(ptr->right_son, min, max, res, curr_index);
    }
    //---------------------------------------------------------------------------------
    void inorderPrintTree(Node<Key,Value>* root){

        if(root == nullptr){
            return;
        }
        inorderPrintTree(root->left_son);
        //-----------Doing something--------------
        int rank = calculate_rank(root->key);
        Node<Key, Value>* node = find_node_by_index(rank);
        Key id = node->key;
        std::cout<< *root <<" index: "<< rank <<" id: "<< id << std::endl;
        //------------------------------------
        inorderPrintTree(root->right_son);
    }
    //---------------------------------------------------------------------------------
    void fix_size_in_insert(Node<Key,Value>* new_node){
        Node<Key,Value>* curr_node = new_node;
        while (curr_node->dad != nullptr){
            curr_node = curr_node->dad;
            curr_node->size_of_subTree++;
        }
    }
    //---------------------------------------------------------------------------------
    void fix_size_in_remove(Node<Key,Value>* node_to_remove){
        Node<Key,Value>* curr_node = node_to_remove;
        while (curr_node->dad != nullptr){
            curr_node = curr_node->dad;
            curr_node->size_of_subTree--;
        }
    }
    //---------------------------------------------------------------------------------
    int calculate_rank(const Key& key) const{
        int rank = 1, ptr_l_size = 0;
        Node<Key,Value>* ptr = this->root;
        while(ptr != nullptr && ptr->key != key){
            if(key < ptr->key){
                ptr = ptr->left_son;
            } else{
                if(ptr->left_son != nullptr){
                    ptr_l_size = ptr->left_son->size_of_subTree;
                } else{
                    ptr_l_size = 0;
                }
                rank += (1 + ptr_l_size);
                ptr = ptr->right_son;
            }
        }
        if(ptr != nullptr && ptr->left_son != nullptr){
            ptr_l_size = ptr->left_son->size_of_subTree;
            rank +=  ptr_l_size;
        }
        return rank;
    }
    //---------------------------------------------------------------------------------
    Node<Key, Value>* find_node_by_index(int index) const{
        Node<Key,Value>* ptr = this->root;
        if(index < 1 || index > num_of_nodes){
            return nullptr;
        }
        int final_i = 0, i = 0;
        while(ptr != nullptr){
            i = 0;
            if(ptr->left_son != nullptr){
                i = (ptr->left_son->size_of_subTree) + 1;
                final_i += i;
            } else{
                final_i++;
            }
            if(final_i == index){
                return ptr;
            }
            if(final_i > index){
                final_i -= i;
                ptr = ptr->left_son;
            } else{
                ptr = ptr->right_son;
            }
        }
        return ptr;
    }


    int get_num_of_nodes() const{
        return this->num_of_nodes;
    }

private:
    Node<Key,Value>* root;
    int num_of_nodes;

    //---------------------------------------------------------------------------------


};



#endif //WET2_AVLTREE_H
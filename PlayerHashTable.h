//
// Created by king H on 12/23/2022.
//

#ifndef WET2_PLAYERHASHTABLE_H
#define WET2_PLAYERHASHTABLE_H


#include "UpTreeNode.h"

#define Starter1 13
#define Max_pressure 0.70

class Chain_node{
public:
    UpTreeNode* player_node;
    Chain_node* next;

    //-----------------------------------
    Chain_node(UpTreeNode* p_node):player_node(p_node),next(nullptr){

    }
    ~Chain_node() = default;
};
//!-------------------------------------------------------------------------------------
class List{
public:
    Chain_node* first_node;
    //-----------------------------------
    List(): first_node(nullptr){

    }

    ~List(){
        while(first_node != nullptr){
            Chain_node* temp = first_node;
            first_node = first_node->next;
            delete temp;
        }
    }
    //-----------------------------------
    UpTreeNode* find_player_node(int player_id) const{
        Chain_node* curr_node = first_node;
        while(curr_node != nullptr && curr_node->player_node != nullptr){//!to check
            if(curr_node->player_node->player.get_player_id() == player_id){
                return curr_node->player_node;
            }
            curr_node = curr_node->next;
        }
        return nullptr;
    }
    //-----------------------------------
    void insert(Chain_node* to_insert){
        to_insert->next = first_node;
        this->first_node = to_insert;
    }
    //-----------------------------------
    void print_List(){
        Chain_node* curr_node = first_node;
        while(curr_node != nullptr){
            std::cout <<"--  "<< curr_node->player_node->player.get_player_id() << " --";
            curr_node = curr_node->next;
        }
        std::cout <<" end"<<std::endl;
    }
};
//!-------------------------------------------------------------------------------------
class PlayerHashTable{
private:
    //!Notes:
    //!takes into account that players wouldn't be removed
    //-----------------------------------
    List** list_array;
    int curr_size;
    int max_size;
    //-----------------------------------
    double pressure_factor;
public:
    void print_size(int print_id){
        std::cout << "Print number: " << print_id << std::endl;
        std::cout << "---- Max size: "<<max_size<<" ---- Curr size: " << curr_size<< std::endl;
    }
    void print_array(int print_id = 0){
        std::cout << "------------  "<< print_id <<"  --------------" << std::endl;
        for(int i = 0; i < max_size ; i ++){
            std::cout<<"List: "<< i <<" | ";
            list_array[i]->print_List();
        }
        std::cout << "------------------------------" << std::endl;
    }
    //-----------------------------------
    PlayerHashTable():list_array(new List*[Starter1]),curr_size(0),max_size(Starter1),
                      pressure_factor(0){
        for(int i = 0; i < Starter1 ;i++){
            list_array[i] = new List();
        }
    }
    ~PlayerHashTable(){

        for(int i = 0; i< max_size;i++){
            Chain_node* node_to_delete = list_array[i]->first_node;
            while(list_array[i]->first_node != nullptr){
                list_array[i]->first_node = list_array[i]->first_node->next;
                delete node_to_delete->player_node;
                delete node_to_delete;
                node_to_delete = list_array[i]->first_node;
            }
        }
        for(int i = 0; i< max_size;i++) {
            delete list_array[i];
        }
        delete[] list_array;
    }
    //-----------------------------------
    void insert(UpTreeNode* player_node){
        //only the UpTreeNode ptr is connected here
        //there should always be place
        if(player_node == nullptr){
            return;
        }
        int index_of_insertion = hash(player_node->player.get_player_id());
        list_array[index_of_insertion]->insert(new Chain_node(player_node));
        curr_size++;
        adjust_pressure();
    }
    //-----------------------------------
    UpTreeNode* find(int player_id) const{
        //finding by !!player!! id

        int found_index = hash(player_id);
        List* curr_list = list_array[found_index];
        UpTreeNode* player_node = curr_list->find_player_node(player_id);

        return player_node;
    }
    //-----------------------------------
    int hash(int player_id) const{
        //returns an available index for insertion fitting for player_id
        int index_to_ret = player_id % max_size;
        return index_to_ret;
    }
private:
    //-----------------------------------
    void adjust_pressure(){
        this->pressure_factor = (double)curr_size/max_size;
        resize();
    }
    //-----------------------------------
    void resize(){
        if(pressure_factor >= Max_pressure){
            UpTreeNode** temp = new UpTreeNode*[curr_size];
            //transferring only non nullptr UpTreeNode ptrs, to re-insert later
            int j = 0;
            for(int i = 0 ; i < max_size ; i ++){
                Chain_node* curr_node = list_array[i]->first_node;
                while (curr_node != nullptr){
                    temp[j] = curr_node->player_node;
                    j++;
                    curr_node = curr_node->next;
                }
            }
            //j should be curr_size by the end


            for(int i = 0 ; i < max_size ; i ++){
                delete list_array[i];
            }

            //Updated max_size here, insert works according to it, so it must be updated before
            max_size = max_size*2-1;//just to be weird,for distribution reasons
            pressure_factor = (double)curr_size/max_size;

            delete[] list_array;
            //----------------------------------------

            list_array = new List*[max_size];
            //initializing the new bigger array to nullptrs
            for(int i = 0 ; i < max_size ; i ++){
                list_array[i] = new List();
            }
            int temp_size = curr_size;//for resetting the size before the rehash
            curr_size = 0;
            for(int i = 0 ; i < temp_size ; i ++){
                insert(temp[i]);
                temp[i] = nullptr;
            }

            delete[] temp;
        }
    }

};
#endif //WET2_PLAYERHASHTABLE_H
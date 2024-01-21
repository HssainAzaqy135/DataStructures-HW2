//
// Created by guyro on 21/12/2022.
//

#ifndef WET2_UNIONFINDPLAYERS_CPP
#define WET2_UNIONFINDPLAYERS_CPP

#include "UnionFindPlayers.h"

//---------------------------------------------------------------------------------
void UnionFindPlayers::add_player(Player player){
    //team_node shouldn't be null. Need to check in add_player in worldCup
    Team* player_team = player.get_player_team();
    UpTreeNode* root_of_team = player_team->get_first_player();
    //Making player node in the UpTree
    UpTreeNode* new_player_node = new UpTreeNode(player, root_of_team);
    new_player_node->player.set_player_team(player_team);

    players_table.insert(new_player_node);
    //should inform team of addition (permutation and ability and addition of player)
    player_team->add_player(new_player_node);
    int team_games = player_team->get_team_num_of_games();

    //new_player_node->player.set_player_team_games(0);//!should stay that way
    new_player_node->player.set_team_games_without_player(team_games);
    //Need to remember to return partial_spirit*player_spirit in the relevant function in worldCup!!!!!!!!
    //!changes were made here
    root_of_team = player_team->get_first_player();
    new_player_node->player.set_partial_spirit((root_of_team->player.get_partial_spirit().inv())*
                                                (player_team->get_team_spirit())*
                                               (new_player_node->player.get_player_spirit().inv()));

}
//---------------------------------------------------------------------------------
UpTreeNode* UnionFindPlayers::find_in_array(int player_id) const{
    return players_table.find(player_id);
}
//---------------------------------------------------------------------------------
int UnionFindPlayers::find(int player_id) const{
    UpTreeNode* player_node = find_in_array(player_id);
    if(player_node == nullptr){
        return -1;//player doesn't exist
    }

    //Finding the root + maintaining partial_spirit
    UpTreeNode* root = player_node;
    int team_games = 0;
    permutation_t total_spirit = permutation_t().neutral();
    while (root->parent != nullptr){
        team_games += root->player.get_player_team_games();
        total_spirit = (root->player.get_partial_spirit())*(total_spirit);
        root = root->parent;
    }

    //Path_shrinking + maintaining partial_spirit
    UpTreeNode* curr = player_node;
    UpTreeNode* next = curr;
    int to_subtract_team_games = 0, new_team_games = 0;
    permutation_t to_cut_spirit = permutation_t().neutral();
    permutation_t new_spirit = total_spirit;
    while (curr != nullptr && curr != root){
        //Maintaining the ranks(games, spirit)
        int curr_team_games = curr->player.get_player_team_games();
        to_subtract_team_games += curr_team_games;
        permutation_t curr_spirit = curr->player.get_partial_spirit();
        to_cut_spirit = (to_cut_spirit)*(curr_spirit.inv());
        next = next->parent;
        if(next != nullptr && next != root){
            curr->parent = root;//!shrinking
            new_team_games = team_games + curr_team_games - to_subtract_team_games;
            curr->player.set_player_team_games(new_team_games);

            new_spirit = (total_spirit)*(to_cut_spirit)*(curr_spirit);
            curr->player.set_partial_spirit(new_spirit);
        }
        curr = next;
    }
    //Check if the team is still playing
    Team* player_team = root->player.get_player_team();
    if(player_team == nullptr){
        return -1;
    }
    int team_id = player_team->get_team_id();
    return team_id;
}
//---------------------------------------------------------------------------------
void UnionFindPlayers::teams_union(Node<int, Team*>* team1, Node<int, Team*>* team2){
    //!team1 should be buyer, team2 should be bought
    UpTreeNode* team1_root = team1->data->get_first_player();
    UpTreeNode* team2_root = team2->data->get_first_player();
    if(team1->data->get_team_num_of_players() >= team2->data->get_team_num_of_players()){
        fix_extra_fields_in_union_op1(team1, team2);
        team2_root->parent = team1_root;
    } else{
        //Team 1 shouldn't be empty here, took care in buy team
        fix_extra_fields_in_union_op2(team1, team2);
        team1_root->parent = team2_root;
    }
}
//---------------------------------------------------------------------------------
void UnionFindPlayers::fix_extra_fields_in_union_op1(Node<int, Team*>* team1, Node<int, Team*>* team2){
    ///team1 is bigger!
    UpTreeNode* team1_root = team1->data->get_first_player();
    UpTreeNode* team2_root = team2->data->get_first_player();


    //Maintaining partial_spirit
    permutation_t old_partial_spirit2 = team2_root->player.get_partial_spirit();
    permutation_t new_partial_spirit2 = (team1_root->player.get_partial_spirit().inv())*(team1->data->get_team_spirit())*(old_partial_spirit2);
    team2_root->player.set_partial_spirit(new_partial_spirit2);

    //Maintaining games_played
    int old_team2_games = team2->data->get_team_num_of_games();
    int team1_games = team1->data->get_team_num_of_games();
    int new_team2_games = old_team2_games - team1_games;
    team2->data->set_team_num_of_games(new_team2_games);
    team2_root->player.set_player_team_games(new_team2_games);
}
//---------------------------------------------------------------------------------
void UnionFindPlayers::fix_extra_fields_in_union_op2(Node<int, Team*>* team1, Node<int, Team*>* team2){
    ///team2 is bigger!
    UpTreeNode* team1_root = team1->data->get_first_player();
    UpTreeNode* team2_root = team2->data->get_first_player();


    //Maintaining partial_spirit
    permutation_t old_partial_spirit2 = team2_root->player.get_partial_spirit();
    permutation_t new_partial_spirit2 = (team1->data->get_team_spirit())*(old_partial_spirit2);
    team2_root->player.set_partial_spirit(new_partial_spirit2);

    permutation_t old_partial_spirit1 = team1_root->player.get_partial_spirit();
    permutation_t new_partial_spirit1 = (new_partial_spirit2.inv())*(old_partial_spirit1);
    team1_root->player.set_partial_spirit(new_partial_spirit1);

    //Maintaining games_played
    int team2_games = team2->data->get_team_num_of_games();
    int old_team1_games = team1->data->get_team_num_of_games();
    int new_team1_games = old_team1_games - team2_games;
    team1->data->set_team_num_of_games(new_team1_games);
    team1_root->player.set_player_team_games(new_team1_games);
}
#endif
//
// Created by guyro on 18/11/2022.
//
#ifndef WET2_TEAMS_CPP
#define WET2_TEAMS_CPP

#include "Team.h"

int Team::get_team_id() const {
    return this->team_id;
}
void Team::set_team_id(int new_id){
    this->team_id = new_id;
}
//---------------------------------------------------------------------------------
int Team::get_team_points() const{
    return this->points;
}
//---------------------------------------------------------------------------------
int Team::get_team_num_of_players() const{
    return this->num_of_players;
}
//---------------------------------------------------------------------------------
int Team::get_team_num_of_keepers() const{
    return this->num_of_keepers;
}
//---------------------------------------------------------------------------------
int Team::get_team_num_of_games() const{
    return this->num_of_games;
}
//---------------------------------------------------------------------------------
int Team::get_team_ability() const{
    return this->ability;
}
//---------------------------------------------------------------------------------
permutation_t Team::get_team_spirit() const{
    return this->team_spirit;
}
//---------------------------------------------------------------------------------
UpTreeNode* Team::get_first_player() const{
    return this->first_player;
}
//---------------------------------------------------------------------------------
int Team::get_team_match_strength() const{
    return (this->points + this->ability);
}
//---------------------------------------------------------------------------------
bool Team::is_team_valid() const{
    if(this->num_of_keepers > 0){
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------------
void Team::update_team_stats_with_player(int ability, const permutation_t& player_spirit){
    this->ability += ability;
    this->team_spirit = this->team_spirit*player_spirit;
}
//---------------------------------------------------------------------------------
void Team::update_team_stats_in_purchase(int num_of_players, int num_of_keepers,
                                         int ability, int points, const permutation_t& team_spirit){
    this->num_of_players += num_of_players;
    this->num_of_keepers += num_of_keepers;
    this->ability += ability;
    this->points += points;
    this->team_spirit = this->team_spirit*team_spirit;
}
//---------------------------------------------------------------------------------
void Team::add_team_points(int points){
    this->points += points;
}
//---------------------------------------------------------------------------------
void Team::increase_team_games_played(){
    this->num_of_games++;
}
//---------------------------------------------------------------------------------
void Team::increase_team_num_of_players(){
    this->num_of_players++;
}
//---------------------------------------------------------------------------------
void Team::set_team_first_player(UpTreeNode* first_player){
    this->first_player = first_player;
}
//---------------------------------------------------------------------------------
void Team::set_team_num_of_games(int games){
    this->num_of_games = games;
}
//---------------------------------------------------------------------------------
void Team::add_player(UpTreeNode* player_node){
    //assuming player_node isn't null
    if(num_of_players == 0){
        this->first_player = player_node;
    }

    if(player_node->player.is_player_keeper() == true){
        this->num_of_keepers++;
    }

    int player_ability = player_node->player.get_player_ability();
    permutation_t player_spirit = player_node->player.get_player_spirit();
    this->update_team_stats_with_player(player_ability, player_spirit);
    //should update player of games played before joining the team
    this->num_of_players++;
}
//---------------------------------------------------------------------------------
void Team::set_first_player(UpTreeNode* new_root){
    this->first_player = new_root;
}



#endif //WET2_TEAMS_CPP

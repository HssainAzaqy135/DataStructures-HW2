//
// Created by guyro on 18/11/2022.
//
#ifndef WET2_PLAYERS_CPP
#define WET2_PLAYERS_CPP

#include "Player.h"

//---------------------------------------------------------------------------------
int Player::get_player_id() const{
    return this->player_id;
}
//---------------------------------------------------------------------------------
int Player::get_player_games_played_alone() const{
    return this->games_played_alone;
}
//---------------------------------------------------------------------------------
int Player::get_player_team_games() const{
    return this->team_games;
}
//---------------------------------------------------------------------------------
int Player::get_team_games_without_player() const{
    return this->team_games_without_player;
}
//---------------------------------------------------------------------------------

int Player::get_player_ability() const{
    return this->ability;
}
//---------------------------------------------------------------------------------
int Player::get_player_cards() const{
    return this->cards;
}
//---------------------------------------------------------------------------------
Team* Player::get_player_team() const{
    return this->player_team;
}
//---------------------------------------------------------------------------------
permutation_t Player::get_player_spirit() const{
    return this->player_spirit;
}
//---------------------------------------------------------------------------------
permutation_t Player::get_partial_spirit() const{
    return this->partial_spirit;
}
//---------------------------------------------------------------------------------
bool Player::is_player_keeper() const{
    return this->goalKeeper;
}
//---------------------------------------------------------------------------------
void Player::set_team_games_without_player(int new_games_without) {
    team_games_without_player = new_games_without;
}
//---------------------------------------------------------------------------------
void Player::set_player_team(Team* new_team){
    this->player_team =new_team;
}
//---------------------------------------------------------------------------------
void Player::set_player_team_games(int games){
    this->team_games = games;
}
//---------------------------------------------------------------------------------
void Player::set_partial_spirit(const permutation_t& partial_spirit){
    this->partial_spirit = partial_spirit;
}
//---------------------------------------------------------------------------------
void Player::add_player_cards(int cards){
    this->cards += cards;
}


#endif //WET2_PLAYERS_CPP
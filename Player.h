//
// Created by guyro on 18/11/2022.
//

#ifndef WET2_PLAYERS_H
#define WET2_PLAYERS_H

#include "AVLTree.h"
#include "wet2util.h"

class Team;
class Player{
public:
    //---------------------------------------------------------------------------------
    Player() = default;
    //---------------------------------------------------------------------------------
    Player(int player_id,int games_played, int ability, int cards,
           bool goalKeeper, Team* team_node,
           const permutation_t& player_spirit):
            player_id(player_id)
            ,games_played_alone(games_played)
            ,team_games(0)/*updated in add player in the tournament
 *                                              to be the games of the desired team*/
            ,team_games_without_player(0)/*updated in add player in the tournament
 *                                              to be the games of the desired team*/
            ,ability(ability)
            ,cards(cards)
            ,goalKeeper(goalKeeper)
            ,player_team(team_node)
            ,player_spirit(player_spirit)
            ,partial_spirit(permutation_t().neutral())
    {
    }
    //---------------------------------------------------------------------------------
    ~Player() = default;
    //---------------------------------------------------------------------------------
    int get_player_id() const;

    int get_player_games_played_alone() const;

    int get_player_team_games() const;

    int get_team_games_without_player() const;

    int get_player_ability() const;

    int get_player_cards() const;

    Team* get_player_team() const;

    permutation_t get_player_spirit() const;

    permutation_t get_partial_spirit() const;

    bool is_player_keeper() const;

    void set_team_games_without_player(int new_games_without);

    void set_player_team(Team* new_team);

    void set_player_team_games(int games);

    void set_partial_spirit(const permutation_t& partial_spirit);

    void add_player_cards(int cards);

    //---------------------------------------------------------------------------------
    friend ostream& operator<<(ostream& os, Player player) {

        os << " player id: "<< player.player_id;
        os << " games played: "<< player.games_played_alone;
        os << " ability: "<< player.ability;
        os << " cards: "<< player.cards;
        os << std::endl;
        return os;
    }
    //---------------------------------------------------------------------------------

private:
    int player_id;
    int games_played_alone;//games of the player before joining the team
    int team_games;
    int team_games_without_player;//games that the team played before the player joined
    int ability;
    int cards;
    bool goalKeeper;
    Team* player_team;//valid only for root of players upTree
    permutation_t player_spirit;
    permutation_t partial_spirit;

};
#endif //WET2_PLAYERS_H
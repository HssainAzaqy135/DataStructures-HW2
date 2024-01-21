//
// Created by guyro on 18/11/2022.
//

#ifndef WET2_TEAMS_H
#define WET2_TEAMS_H

#include "Player.h"
#include "AVLTree.h"
#include "wet2util.h"
#include "UpTreeNode.h"

class TeamStats{
private:
    int id;
    int ability;

public:
    TeamStats() : id(0), ability(0){}

    TeamStats(int id, int ability) : id(id), ability(ability){}

    ~TeamStats() = default;

    bool operator==(const TeamStats& teamStats1) const{
        if(this->ability == teamStats1.ability && this->id == teamStats1.id)
        {
            return true;
        }
        return false;
    }

    bool operator!=(const TeamStats& teamStats1) const{
        return !(*this == teamStats1);
    }

    bool operator>(const TeamStats& teamStats1) const{
        if(this->ability > teamStats1.ability)
        {
            return true;
        } else if(this->ability == teamStats1.ability){
            if(this->id > teamStats1.id){
                return true;
            }
            return false;

        }
        return false;
    }

    bool operator<(const TeamStats& teamStats1) const{
        return (!(*this > teamStats1) && !(*this == teamStats1));
    }
    //----------------------printing operator-----------------
    friend ostream& operator<<(ostream& os, TeamStats stats) {
        os<<"Team Id: "<<stats.id<<" ,Team ability: "<<stats.ability<<std::endl;
        return os;
    }
};

class Team{
public:
    Team(int id):team_id(id)
            ,points(0)
            ,num_of_players(0)
            ,num_of_keepers(0)
            ,num_of_games(0)
            ,ability(0)
            ,team_spirit(permutation_t().neutral())
            ,first_player(nullptr) {}
    //---------------------------------------------------------------------------------
    Team():team_id(0)
            ,points(0)
            ,num_of_players(0)
            ,num_of_keepers(0)
            ,num_of_games(0)
            ,ability(0)
            ,team_spirit(permutation_t().neutral())
            ,first_player(nullptr) {}
    //---------------------------------------------------------------------------------
    ~Team() = default;
    //---------------------------------------------------------------------------------
    int get_team_id() const;

    int get_team_points() const;

    int get_team_num_of_players() const;

    int get_team_num_of_keepers() const;

    int get_team_num_of_games() const;

    int get_team_ability() const;

    int get_team_match_strength() const;

    permutation_t get_team_spirit() const;

    UpTreeNode* get_first_player() const;

    void set_team_id(int new_id);

    bool is_team_valid() const;

    void update_team_stats_with_player(int ability, const permutation_t& player_spirit);

    void add_team_points(int points);

    void increase_team_games_played();

    void increase_team_num_of_players();

    void set_team_first_player(UpTreeNode* first_player);

    void set_team_num_of_games(int games);

    void set_first_player(UpTreeNode* new_root);

    void add_player(UpTreeNode* player_node);

    void update_team_stats_in_purchase(int num_of_players, int num_of_keepers,
                                       int ability, int points, const permutation_t& team_spirit);

    //---------------------------------------------------------------------------------
    friend ostream& operator<<(ostream& os, Team team) {

        os << "team id: "<< team.team_id;
        os << "points: "<< team.points;
        return os;
    }
    //---------------------------------------------------------------------------------
private:
    int team_id;
    int points;
    int num_of_players;
    int num_of_keepers;
    int num_of_games;
    int ability;
    permutation_t team_spirit;
    UpTreeNode* first_player;
};
#endif //WET2_TEAMS_H
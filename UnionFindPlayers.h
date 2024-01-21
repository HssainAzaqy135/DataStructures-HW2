//
// Created by guyro on 21/12/2022.
//

#ifndef WET2_UNIONFINDPLAYERS_H
#define WET2_UNIONFINDPLAYERS_H

#include "AVLTree.h"
#include "Player.h"
#include "Team.h"
#include "UpTreeNode.h"
#include "PlayerHashTable.h"


class UnionFindPlayers {
private:
    PlayerHashTable players_table;


public:
    UnionFindPlayers() : players_table(PlayerHashTable()){}

    ~UnionFindPlayers() = default;

    UpTreeNode* find_in_array(int player_id) const;

    void add_player(Player player);

    int find(int player_id) const;//team_id, in which the player plays.

    void teams_union(Node<int, Team*>* team1, Node<int, Team*>* team2);

    void fix_extra_fields_in_union_op1(Node<int, Team*>* team1, Node<int, Team*>* team2);

    void fix_extra_fields_in_union_op2(Node<int, Team*>* team1, Node<int, Team*>* team2);

};


#endif //WET2_UNIONFINDPLAYERS_H
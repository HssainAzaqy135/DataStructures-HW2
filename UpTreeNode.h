//
// Created by guyro on 21/12/2022.
//

#ifndef WET2_UPTREENODE_H
#define WET2_UPTREENODE_H

#include "Player.h"

class UpTreeNode{
public:
    Player player;
    UpTreeNode* parent;


    UpTreeNode(Player player, UpTreeNode* parent) : player(player), parent(parent){}

    ~UpTreeNode() = default;
};


#endif //WET2_UPTREENODE_H
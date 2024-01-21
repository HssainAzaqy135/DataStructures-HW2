#include "worldcup23a2.h"

world_cup_t::world_cup_t()
{
    this->teams_tree_by_id = new AVLTree<int, Team*>();
    this->teams_tree_by_stats = new AVLTree<TeamStats, Team*>();
    this->players_union_find = new UnionFindPlayers();
}
//-----------------------------------------------------------------------------
void world_cup_t::DeleteTheTeams(Node<int,Team*>* root){
    //!postorder deletion of the actual teams because encapsulation is for children
    if(root == nullptr ){
        return;
    }
    //----------Doing the deleting------------------
    //delete works only for deleting actual data placed as a "Values"
    DeleteTheTeams(root->left_son);
    DeleteTheTeams(root->right_son);
    delete root->data;
}
//-----------------------------------------------------------------------------
world_cup_t::~world_cup_t()
{
    DeleteTheTeams(teams_tree_by_id->get_root());
    //!deleted teams before trees
    delete teams_tree_by_stats;
    delete teams_tree_by_id;
    delete players_union_find;
}
//-----------------------------------------------------------------------------
StatusType world_cup_t::add_team(int teamId)
{
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }

    Node<int, Team*>* team_node = this->teams_tree_by_id->find(teamId, false);
    try{
        if(team_node != nullptr){
            return StatusType::FAILURE;
        }
        Team* team_to_add = new Team(teamId);
        this->teams_tree_by_id->insert(teamId,team_to_add);
        team_node = this->teams_tree_by_id->find(teamId, false);
        TeamStats new_team_stats = TeamStats(teamId, 0);
        this->teams_tree_by_stats->insert(new_team_stats, team_to_add);
    }catch(std::bad_alloc &e){
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Node<int, Team*>* team_node = this->teams_tree_by_id->find(teamId, false);
    if(team_node == nullptr){
        return StatusType::FAILURE;
    }

    if(team_node->data->get_team_num_of_players() > 0){
        UpTreeNode* first_player = team_node->data->get_first_player();
        first_player->player.set_player_team(nullptr);
    }
    TeamStats team_stats = TeamStats(team_node->data->get_team_id(), team_node->data->get_team_ability());
    Team* team_to_delete = team_node->data;
    this->teams_tree_by_stats->remove(team_stats);
    this->teams_tree_by_id->remove(teamId);
    delete team_to_delete;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || cards < 0 || spirit.isvalid() == false){
        return StatusType::INVALID_INPUT;
    }

    Node<int, Team*>* team_node = this->teams_tree_by_id->find(teamId, false);
    if(team_node == nullptr){
        return StatusType::FAILURE;
    }
    UpTreeNode* new_player_node = this->players_union_find->find_in_array(playerId);
    if(new_player_node != nullptr){
        return StatusType::FAILURE;
    }

    try{
        Player new_player = Player(playerId, gamesPlayed, ability,
                                   cards, goalKeeper, team_node->data, spirit);
        TeamStats old_team_stats = TeamStats(team_node->data->get_team_id(),
                                             team_node->data->get_team_ability());
        this->players_union_find->add_player(new_player);//other fields are updated here
        TeamStats new_team_stats = TeamStats(team_node->data->get_team_id(),
                                             team_node->data->get_team_ability());
        this->teams_tree_by_stats->remove(old_team_stats);
        this->teams_tree_by_stats->insert(new_team_stats, (team_node->data));

        new_player_node = this->players_union_find->find_in_array(playerId);
        new_player_node->player.set_player_team(team_node->data);
    }catch(std::bad_alloc &e){
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Node<int, Team*>* team1_node = this->teams_tree_by_id->find(teamId1, false);
    Node<int, Team*>* team2_node = this->teams_tree_by_id->find(teamId2, false);
    if(team1_node == nullptr || team2_node == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    if(team1_node->data->is_team_valid() == false || team2_node->data->is_team_valid() == false){
        return output_t<int>(StatusType::FAILURE);
    }

    team1_node->data->increase_team_games_played();
    team2_node->data->increase_team_games_played();
    int updated_team_games1 = team1_node->data->get_team_num_of_games();
    int updated_team_games2 = team2_node->data->get_team_num_of_games();
    team1_node->data->get_first_player()->player.set_player_team_games(updated_team_games1);
    team2_node->data->get_first_player()->player.set_player_team_games(updated_team_games2);

    int strength1 = team1_node->data->get_team_match_strength();
    int strength2 = team2_node->data->get_team_match_strength();
    if(strength1 > strength2){
        team1_node->data->add_team_points(3);
        return output_t<int>(1);
    }else if(strength2 > strength1){
        team2_node->data->add_team_points(3);
        return output_t<int>(3);
    } else{
        int spirit_strength1 = team1_node->data->get_team_spirit().strength();
        int spirit_strength2 = team2_node->data->get_team_spirit().strength();
        if(spirit_strength1 > spirit_strength2){
            team1_node->data->add_team_points(3);
            return output_t<int>(2);
        }else if(spirit_strength2 > spirit_strength1){
            team2_node->data->add_team_points(3);
            return output_t<int>(4);
        } else{
            team1_node->data->add_team_points(1);
            team2_node->data->add_team_points(1);
            return output_t<int>(0);
        }
    }
    return output_t<int>(StatusType::FAILURE);//shouldn't reach here
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    UpTreeNode* player_node = this->players_union_find->find_in_array(playerId);
    if(player_node == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    //---------------------------------
    players_union_find->find(playerId);

    int games_alone = player_node->player.get_player_games_played_alone();
    int team_games_without_player = player_node->player.get_team_games_without_player();
    int team_games = player_node->player.get_player_team_games();
    int num_of_games = games_alone - team_games_without_player + team_games;

    if(player_node->parent != nullptr){
        num_of_games += player_node->parent->player.get_player_team_games();
    }

    return output_t<int>(num_of_games);
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0){
        return StatusType::INVALID_INPUT;
    }

    UpTreeNode* player_node = this->players_union_find->find_in_array(playerId);
    if(player_node == nullptr){
        return StatusType::FAILURE;
    }

    int teamId = this->players_union_find->find(playerId);
    if(teamId == -1){
        return StatusType::FAILURE;
    }

    player_node->player.add_player_cards(cards);

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    UpTreeNode* player_node = this->players_union_find->find_in_array(playerId);
    if(player_node == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }

    int num_of_cards = player_node->player.get_player_cards();

    return output_t<int>(num_of_cards);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Node<int, Team*>* team_node = this->teams_tree_by_id->find(teamId, false);
    if(team_node == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }

    int points = team_node->data->get_team_points();
    return output_t<int>(points);
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    int index = i+1;//for implementation reasons in AVLTree.
    int num_of_teams = this->teams_tree_by_id->get_num_of_nodes();
    if(index < 1 || index > num_of_teams){
        return output_t<int>(StatusType::FAILURE);
    }

    Node<TeamStats, Team*>* team_node = teams_tree_by_stats->find_node_by_index(index);
    if(team_node == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    int team_id = team_node->data->get_team_id();

    return output_t<int>(team_id);
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0){
        return output_t<permutation_t>(StatusType::INVALID_INPUT);
    }

    UpTreeNode* player_node = this->players_union_find->find_in_array(playerId);
    if(player_node == nullptr){
        return output_t<permutation_t>(StatusType::FAILURE);
    }

    int teamId = this->players_union_find->find(playerId);
    if(teamId == -1){
        return output_t<permutation_t>(StatusType::FAILURE);
    }

    permutation_t res_partial_spirit = (player_node->player.get_partial_spirit())*
                                       (player_node->player.get_player_spirit());
    if(player_node->parent != nullptr){
        res_partial_spirit = (player_node->parent->player.get_partial_spirit())*(res_partial_spirit);
    }

    return output_t<permutation_t>(res_partial_spirit);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    Node<int, Team *> *team1_node = this->teams_tree_by_id->find(teamId1, false);
    Node<int, Team *> *team2_node = this->teams_tree_by_id->find(teamId2, false);
    if (team1_node == nullptr || team2_node == nullptr) {
        return StatusType::FAILURE;
    }
    //----------- first players ------------
    UpTreeNode* first_p_1 = team1_node->data->get_first_player();
    UpTreeNode* first_p_2 = team2_node->data->get_first_player();
    int num_of_players_team1 = team1_node->data->get_team_num_of_players();
    int num_of_players_team2 = team2_node->data->get_team_num_of_players();
    //--------------------------------------
    Team* new_team = new Team(teamId1);//!this is to be team1 after buying team2
    TeamStats new_team_stats = TeamStats(teamId1,team1_node->data->get_team_ability() + team2_node->data->get_team_ability());
    //! keeping both teams for deletion
    Team* team1_to_delete = team1_node->data;
    Team* team2_to_delete = team2_node->data;
    TeamStats team1_stats = TeamStats(team1_node->data->get_team_id(),
                                          team1_node->data->get_team_ability());
    TeamStats team2_stats = TeamStats(team2_node->data->get_team_id(),
                                      team2_node->data->get_team_ability());
    //--------------------------------------
    //updating both team stats to the new team
    new_team->update_team_stats_in_purchase(team1_node->data->get_team_num_of_players(),
                                            team1_node->data->get_team_num_of_keepers(),
                                            team1_node->data->get_team_ability(),
                                            team1_node->data->get_team_points(),
                                            team1_node->data->get_team_spirit());

    new_team->update_team_stats_in_purchase(team2_node->data->get_team_num_of_players(),
                                  team2_node->data->get_team_num_of_keepers(),
                                  team2_node->data->get_team_ability(),
                                  team2_node->data->get_team_points(),
                                  team2_node->data->get_team_spirit());
    //--------------------------------------
    //setting the new_team first player here
    if(num_of_players_team1 >= num_of_players_team2){
        new_team->set_first_player(first_p_1);
    } else{
        new_team->set_first_player(first_p_2);
    }
    //--------------------------------------
    //calling union <=> both teams aren't empty
    if(num_of_players_team1 != 0 && num_of_players_team2 != 0){
        players_union_find->teams_union(team1_node, team2_node);
    }

    //--------------------------------------
    UpTreeNode* new_first_p = new_team->get_first_player();
    if(new_first_p != nullptr){
        //updating num_of_games played in the new team to be the original
        new_team->set_team_num_of_games(new_first_p->player.get_player_team_games());
        //telling the new_team
        new_first_p->player.set_player_team(new_team);
    }
    //--------------------------------------
    //deleting the old teams
    teams_tree_by_stats->remove(team1_stats);
    teams_tree_by_stats->remove(team2_stats);
    teams_tree_by_id->remove(teamId1);
    teams_tree_by_id->remove(teamId2);
    delete team1_to_delete;
    delete team2_to_delete;
    try{
        //inserting the new_team
        teams_tree_by_id->insert(new_team->get_team_id(), new_team);
        teams_tree_by_stats->insert(new_team_stats, new_team);
    }catch(std::bad_alloc &e){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}
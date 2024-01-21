#include <iostream>
#include "AVLTree.h"
#include "PlayerHashTable.h"
#include "Team.h"
#include "wet2util.h"
#include "math.h"
#include "worldcup23a2.h"

void Hash_Table_Tester(int Test_volume);
void AvlRankTree_Tester1(int Test_volume);
void AvlRankTree_Tester2(int Test_volume);
void AvlRankTree_Tester3(int Test_volume);
void Students_Tester1();

int main() {
    //!testing for hashtable
    //Hash_Table_Tester(2000);
    //!AvlRankTree tester
    //AvlRankTree_Tester1(10);
    //AvlRankTree_Tester2(769);
    //AvlRankTree_Tester3(7653);
    Students_Tester1();
    return 0;
}

//-------------------------------------------------------------------------------------------
void Hash_Table_Tester(int Test_volume){
    PlayerHashTable test_Table = PlayerHashTable();
    Team* test_team = new Team(12340000);
    Node<int , Team*>* team_node = new Node<int,Team*>(test_team->get_team_id(),test_team);
    int per_array[5] = {1,2,4,3,5};

    Player players_arr[Test_volume];
    UpTreeNode* node_arr[Test_volume];
    for(int i = 0; i < Test_volume ; i++){
        Player temp_player = Player(3*i*i+i/7+1,3,12,0, true,team_node->data,
                                    permutation_t(per_array));

        node_arr[i] = new UpTreeNode(temp_player, nullptr);
    }

    for(int i = 0; i < Test_volume ; i++){
        test_Table.insert(node_arr[i]);
        //test_Table.print_array(i+1);
    }
    //test_Table.print_array(Test_volume);
    test_Table.print_size(Test_volume);
    int i = 9;
    int player_to_find = 3*i*i+i/7+1;
    UpTreeNode* found_node =test_Table.find(player_to_find);
    if(found_node == nullptr){
        std::cout <<"Player to find: " << player_to_find<< "  Found Player: "<< "Null";
    } else{
        int found_player = found_node->player.get_player_id();
        std::cout <<"Player to find: " << player_to_find<< "  Found Player: "<< found_player;
    }



    //---------------------------------------
    //for leaks
    delete team_node->data;
    delete team_node;

}
//-------------------------------------------------------------------------------------------
void AvlRankTree_Tester1(int Test_volume){
    //Todo: make the tester

    int id_array[] = {9,3,15,1,7,13,19,5,11,17,21,23};
    TeamStats teamStats_arr[12];
    for(int i = 0; i < 12; i++){
        teamStats_arr[i] = TeamStats(id_array[i],0);
        std::cout<< teamStats_arr[i]<< std::endl;
    }
    //---------------------------------------------
    //-------Testing
    //---------------------------------------------
    std::cout << "-----------------------------------"<< std::endl;
    AVLTree<int ,TeamStats> *test_tree = new AVLTree<int,TeamStats>();
    for (int i = 0; i < 12; i++){
        test_tree->insert(id_array[i], teamStats_arr[i]);
    }
    //---------------------------------------------

    test_tree->inorderPrintTree(test_tree->get_root());
    std::cout << "-----------------------------------"<< std::endl;

    TeamStats team_stats_to_add = TeamStats(22,22);
    test_tree->insert(22,team_stats_to_add);

    std::cout << "-----------------------------------"<< std::endl;

    test_tree->remove(15);
    test_tree->remove(1);
    test_tree->remove(17);

    test_tree->insert(15,team_stats_to_add);
    test_tree->insert(1,team_stats_to_add);

    test_tree->insert(225,team_stats_to_add);
    //test_tree->inorderPrintTree(test_tree->get_root());
    std::cout << "-----------------------------------"<< std::endl;

    //------------Deletion--------------
    delete test_tree;

}
//-------------------------------------------------------------------------------------------
void AvlRankTree_Tester2(int Test_volume){
    Team teams_arr[Test_volume];
    TeamStats stats_arr[Test_volume];
    for(int i = 0; i < Test_volume; i++){
        if(i%2 == 0){
            teams_arr[i] = Team((i+1)*(i+1)+2);
            stats_arr[i] = TeamStats((i+1)*(i+1)+2,0);
        }else{
            teams_arr[i] = Team(Test_volume - i);
            stats_arr[i] = TeamStats(Test_volume - i,0);
        }
    }
    //---------------------------------------------
    //-------Testing
    //---------------------------------------------
    std::cout << "-----------------------------------"<< std::endl;
    AVLTree<int ,Team>* teams_tree = new AVLTree<int,Team>();
    AVLTree<int ,TeamStats>* stats_tree = new AVLTree<int,TeamStats>();
    for (int i = 0; i < Test_volume; i++){
        if(i%2 == 0){
            teams_tree->insert((i+1)*(i+1)+2, teams_arr[i]);
            stats_tree->insert((i+1)*(i+1)+2, stats_arr[i]);
        }else{
            teams_tree->insert(Test_volume - i, teams_arr[i]);
            stats_tree->insert(Test_volume - i, stats_arr[i]);
        }
    }

    for(int i = 0; i < Test_volume - 5; i+=2){
        stats_tree->remove((i+1)*(i+1)+2);
        teams_tree->remove((i+1)*(i+1)+2);
    }
    for(int i = 1; i < Test_volume - 5; i+=2){
        stats_tree->remove(Test_volume - i);
        teams_tree->remove(Test_volume - i);
    }

    teams_tree->inorderPrintTree(teams_tree->get_root());
    std::cout << "-----------------------------------"<< std::endl;
    stats_tree->inorderPrintTree(stats_tree->get_root());

    //---------------------------------------------
    delete teams_tree;
    delete stats_tree;
}
//-------------------------------------------------------------------------------------------
void AvlRankTree_Tester3(int Test_volume){
    Team teams_arr[Test_volume];
    TeamStats stats_arr[Test_volume];
    for(int i = 0; i < Test_volume; i++){
        if(i%2 == 0){
            teams_arr[i] = Team((i+1)*(i+1)+2);
            stats_arr[i] = TeamStats((i+1)*(i+1)+2,10*abs(sin(i*i)));
        }else{
            teams_arr[i] = Team(Test_volume - i);
            stats_arr[i] = TeamStats(Test_volume - i, 10*abs(sin(i+(i*i))));
        }
    }
    //---------------------------------------------
    //-------Testing
    //---------------------------------------------
    std::cout << "-----------------------------------"<< std::endl;
    AVLTree<int ,Team>* teams_tree = new AVLTree<int,Team>();
    AVLTree<TeamStats ,Team*>* stats_tree = new AVLTree<TeamStats,Team*>();
    for (int i = 0; i < Test_volume; i++){
        if(i%2 == 0){
            teams_tree->insert((i+1)*(i+1)+2, teams_arr[i]);
            stats_tree->insert(stats_arr[i], &(teams_arr[i]));
        }else{
            teams_tree->insert(Test_volume - i, teams_arr[i]);
            stats_tree->insert(stats_arr[i], &(teams_arr[i]));
        }
    }

    for(int i = 0; i < Test_volume - 5; i+=2){
        stats_tree->remove(stats_arr[i]);
        teams_tree->remove((i+1)*(i+1)+2);
    }
    for(int i = 1; i < Test_volume - 5; i+=2){
        stats_tree->remove(stats_arr[i]);
        teams_tree->remove(Test_volume - i);
    }

    teams_tree->inorderPrintTree(teams_tree->get_root());
    std::cout << "-----------------------------------"<< std::endl;
    stats_tree->inorderPrintTree(stats_tree->get_root());

    //---------------------------------------------
    delete teams_tree;
    delete stats_tree;
}
//-------------------------------------------------------------------------------------------
void Students_Tester1(){
    int basePerm[5] = {1, 2, 3, 4, 0};
    permutation_t perm = permutation_t(basePerm).inv();

    world_cup_t *obj = new world_cup_t();

    StatusType res = obj->add_team(10);
    res == StatusType::SUCCESS;
    res = obj->add_player(11, 10, perm, 1, 1, 1, true);
    res == StatusType::SUCCESS;

    output_t<int> resn20 = obj->num_played_games_for_player(11);
    resn20.status() == StatusType::SUCCESS;
    resn20.ans() == 1;

    res = obj->add_team(20);
    res == StatusType::SUCCESS;
    res = obj->add_player(21, 20, perm, 1, 3, 1, true);
    res == StatusType::SUCCESS;

    res = obj->add_team(30);
    res == StatusType::SUCCESS;
    res = obj->add_player(31, 30, perm, 1, 6, 1, true);
    res == StatusType::SUCCESS;

    output_t<int> resn1 = obj->play_match(10, 20);
    resn1.status() == StatusType::SUCCESS;
    resn1.ans() == 3;

    output_t<int> resn21 = obj->num_played_games_for_player(11);
    resn21.status() == StatusType::SUCCESS;
    resn21.ans() == 2;

    res = obj->buy_team(10, 20);
    res == StatusType::SUCCESS;

    output_t<int> resn22 = obj->num_played_games_for_player(11);
    resn22.status() == StatusType::SUCCESS;
    resn22.ans() == 2;

    output_t<int> resn2 = obj->get_team_points(10);
    resn2.status() == StatusType::SUCCESS;
    resn2.ans() == 3;

    output_t<int> resn3 = obj->get_team_points(20);
    resn3.status() == StatusType::FAILURE;

    output_t<int> resn4 = obj->get_ith_pointless_ability(1);
    resn4.status() == StatusType::SUCCESS;
    resn4.ans() == 30;

    output_t<int> resn5 = obj->play_match(10, 30);
    resn5.status() == StatusType::SUCCESS;
    resn5.ans() == 1;

    output_t<int> resn23 = obj->num_played_games_for_player(11);
    resn23.status() == StatusType::SUCCESS;
    resn23.ans() == 3;

    obj->add_player(32, 30, perm, 1, 4, 1, true) == StatusType::SUCCESS;

    output_t<int> resn6 = obj->play_match(10, 30);
    resn6.status() == StatusType::SUCCESS;
    resn6.ans() == 0;

    output_t<int> resn24 = obj->num_played_games_for_player(11);
    resn24.status() == StatusType::SUCCESS;
    resn24.ans() == 4;

    res = obj->buy_team(30, 10);
    res == StatusType::SUCCESS;

    // Partial spirit is correct
    output_t<permutation_t> resn7 = obj->get_partial_spirit(31);
    resn7.status() == StatusType::SUCCESS;
    //str(resn7.ans()) == str(perm));

    output_t<permutation_t> resn8 = obj->get_partial_spirit(11);
    resn8.status() == StatusType::SUCCESS;
    //str(resn8.ans()) == str(perm * perm * perm));

    // Num played games is correct
    output_t<int> resn10 = obj->num_played_games_for_player(11);
    resn10.status() == StatusType::SUCCESS;
    resn10.ans() == 4;

    output_t<int> resn11 = obj->num_played_games_for_player(21);
    resn11.status() == StatusType::SUCCESS;
    resn11.ans() == 4;

    output_t<int> resn12 = obj->num_played_games_for_player(31);
    resn12.status() == StatusType::SUCCESS;
    resn12.ans() == 3;

    output_t<int> resn13 = obj->num_played_games_for_player(32);//!failed assert
    resn13.status() == StatusType::SUCCESS;
    resn13.ans() == 2;

    // All players get removed together
    res = obj->remove_team(30);
    res == StatusType::SUCCESS;

    obj->add_player_cards(11, 1) == StatusType::FAILURE;
    obj->add_player_cards(21, 1) == StatusType::FAILURE;
    obj->add_player_cards(31, 1) == StatusType::FAILURE;
    obj->add_player_cards(32, 1) == StatusType::FAILURE;


    delete obj;
}
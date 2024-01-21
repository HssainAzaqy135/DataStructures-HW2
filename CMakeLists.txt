cmake_minimum_required(VERSION 3.21)
project(wet2)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet2 main23a2.cpp UnionFindPlayers.cpp UnionFindPlayers.h Player.h Player.cpp
        Team.h Team.cpp wet2util.h worldcup23a2.h worldcup23a2.cpp UpTreeNode.h PlayerHashTable.h)

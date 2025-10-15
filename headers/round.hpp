#pragma once
#include <utility>
#include <vector>


struct Round {
 
    std::pair<std::vector<int>, int> algorithm;
    int player;
    int result;
    
    Round(std::pair<std::vector<int>, int> _algorithm, int _player, int _result) : algorithm(_algorithm), player(_player), result(_result) {}
};

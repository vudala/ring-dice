#ifndef DICES_INCLUDED
#define DICES_INCLUDED

#include <bits/stdc++.h>

enum Combinations {
    PAIR = 1,
    TRIO,
    D_PAIR,
    FULL_HOUSE,
    LOW_SEQ,
    HIGH_SEQ,
    QUATRAIN,
    QUINTET  
};

bool is_pair(std::vector<int> dices);
bool is_trio(std::vector<int> dices);
bool is_d_pair(std::vector<int> dices);
bool is_full_house(std::vector<int> dices);
bool is_low_seq(std::vector<int> dices);
bool is_high_seq(std::vector<int> dices);
bool is_quatrain(std::vector<int> dices);
bool is_quintet(std::vector<int> dices);

#endif
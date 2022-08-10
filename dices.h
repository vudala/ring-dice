#ifndef DICES_INCLUDED
#define DICES_INCLUDED

#include <bits/stdc++.h>

enum Combinations {
    NONE = 0,
    PAIR,
    TRIO,
    D_PAIR,
    FULL_HOUSE,
    LOW_SEQ,
    HIGH_SEQ,
    QUATRAIN,
    QUINTET  
};

int has_pair(std::vector<int>& dices);
int has_trio(std::vector<int>& dices);
int has_d_pair(std::vector<int>& dices);
int has_full_house(std::vector<int>& dices);
int has_low_seq(std::vector<int>& dices);
int has_high_seq(std::vector<int>& dices);
int has_quatrain(std::vector<int>& dices);
int has_quintet(std::vector<int>& dices);


unsigned get_score(unsigned chosen, std::vector<int>& dices);
char * get_label(unsigned chosen);

#endif
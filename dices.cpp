#include "dices.h"

using namespace std;

vector<int> scores = {2, 3, 4, 5, 7, 7, 10, 15};

int has_pair(vector<int>& dices) {
    for(int i = 0; i < 4; i++) {
        int count = 1;
        for(int j = i; j < 5; j++)
            if (dices[i] == dices[j])
                count++;
        if (count >= 2)
            return 1;
    }
    return 0;
}


int has_trio(vector<int>& dices) {
    for(int i = 0; i < 3; i++) {
        int count = 1;
        for(int j = i; j < 5; j++)
            if (dices[i] == dices[j])
                count++;
        if (count >= 3)
            return 1;
    }
    return 0;
}


int has_d_pair(vector<int>& dices) {
    sort(dices.begin(), dices.end());
    list<int> l (dices.begin(), dices.end());
    int set = false;
    for(auto itr = l.begin(); itr != prev(l.end(), 1);) {
        if (*itr == *(next(itr, 1))) {
            if (set)
                return 1;
            set = true;
            itr = l.erase(l.erase(itr));
        }
        else
            itr++;
    }
    return 0;
}


int has_full_house(vector<int>& dices) {
    sort(dices.begin(), dices.end());
    list<int> l (dices.begin(), dices.end());
    int set = false;
    for(auto itr = l.begin(); itr != prev(l.end(), 1);) {
        if (*itr == *(next(itr, 1))) {
            if (*itr == *(next(itr, 2))) {
                itr = l.erase(l.erase(l.erase(itr)));
                if (*itr == *next(itr, 1))
                    return 1;
                return 0;
            }
            if (set)
                return 0;
            set = true;
            itr = l.erase(l.erase(itr)); 
        }
        else
            return 0;
    }
    return 0;
};


int has_low_seq(vector<int>& dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 4; i++)
        if (dices[i] + 1 != dices[i + 1])
            return 0;
    if (dices[0] == 1)
        return 1;
    return 0;
}


int has_high_seq(vector<int>& dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 4; i++)
        if (dices[i] + 1 != dices[i + 1])
            return 0;
    if (dices[0] == 2)
        return 1;
    return 0;
}


int has_quatrain(vector<int>& dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 3; i++)
        if (dices[i] != dices[i + 1])
            for(int i = 1; i < 4; i++)
                if (dices[i] != dices[i + 1])
                    return 0;
    return 1;
}


int has_quintet(vector<int>& dices) {
    for(int i = 0; i < 4; i++)
        if (dices[i] != dices[i + 1])
            return 0;
    return 1;
};


int get_score(int chosen, vector<int>& dices) {
    switch(chosen) {
        case PAIR: return has_pair(dices) * scores[PAIR];
        case TRIO: return has_trio(dices) * scores[TRIO];
        case D_PAIR: return has_d_pair(dices) * scores[D_PAIR];
        case FULL_HOUSE: return has_full_house(dices) * scores[FULL_HOUSE];
        case LOW_SEQ: return has_low_seq(dices) * scores[LOW_SEQ];
        case HIGH_SEQ: return has_high_seq(dices) * scores[HIGH_SEQ];
        case QUATRAIN: return has_quatrain(dices) * scores[QUATRAIN];
        case QUINTET: return has_quintet(dices) * scores[QUINTET];
        default:
            return 0;
    }
}
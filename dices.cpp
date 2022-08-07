#include "dices.h"

using namespace std;

bool is_pair(vector<int> dices) {
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


bool is_trio(vector<int> dices) {
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


bool is_d_pair(vector<int> dices) {
    sort(dices.begin(), dices.end());
    list<int> l (dices.begin(), dices.end());
    bool set = false;
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


bool is_full_house(vector<int> dices) {
    sort(dices.begin(), dices.end());
    list<int> l (dices.begin(), dices.end());
    bool set = false;
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


bool is_low_seq(vector<int> dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 4; i++)
        if (dices[i] + 1 != dices[i + 1])
            return 0;
    if (dices[0] == 1)
        return 1;
    return 0;
}


bool is_high_seq(vector<int> dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 4; i++)
        if (dices[i] + 1 != dices[i + 1])
            return 0;
    if (dices[0] == 2)
        return 1;
    return 0;
}


bool is_quatrain(vector<int> dices) {
    sort(dices.begin(), dices.end());
    for(int i = 0; i < 3; i++)
        if (dices[i] != dices[i + 1])
            for(int i = 1; i < 4; i++)
                if (dices[i] != dices[i + 1])
                    return 0;
    return 1;
}


bool is_quintet(vector<int> dices) {
    for(int i = 0; i < 4; i++)
        if (dices[i] != dices[i + 1])
            return 0;
    return 1;
};
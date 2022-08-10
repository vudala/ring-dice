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

// Determina se combinação contem Par
int has_pair(std::vector<int>& dices);

// Determina se combinação contem Trinca
int has_trio(std::vector<int>& dices);

// Determina se combinação contem Dois pares
int has_d_pair(std::vector<int>& dices);

// Determina se combinação contem Full House
int has_full_house(std::vector<int>& dices);

// Determina se combinação contem Sequência baixa
int has_low_seq(std::vector<int>& dices);

// Determina se combinação contem Sequência alta
int has_high_seq(std::vector<int>& dices);

// Determina se combinação contem Quadra
int has_quatrain(std::vector<int>& dices);

// Determina se combinação contem Quinteto
int has_quintet(std::vector<int>& dices);


// Retorna o score obtido dada um sequência de dados e a combinação esperada
unsigned get_score(unsigned chosen, std::vector<int>& dices);

// Retorna uma string representando a combinação
char * get_label(unsigned chosen);

#endif
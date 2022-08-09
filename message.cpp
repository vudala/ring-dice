#include "message.h"
using namespace std;
#define PN cout << '\n'
#include <bits/stdc++.h>


unsigned count_1s(Message * msg)
{
    unsigned aux;
    unsigned count = 0;
    for(aux = msg->origin_addr; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->target_addr; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->chosen_addr; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->origin_port; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->target_port; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->chosen_port; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->type; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->status; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->bet; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->combination; aux; aux >>= 1)
        count += (aux & 1);
    return count;
}

unsigned addr_to_int(char addr[]) {
    unsigned x = 0;
    for(int i = 0; i < 4; i++)
        x += unsigned(addr[i]) << i;
    return x;
}


void print_bits(unsigned x){
    for(unsigned i = 0; i < 32; i++)
        if (x >> i & 1) cout << 1;
        else cout << 0;       
}
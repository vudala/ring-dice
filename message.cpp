#include "message.h"

unsigned count_1s(Message * msg)
{
    unsigned aux;
    unsigned count = 0;
    for(aux = msg->chosen_addr; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->chosen_port; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->type; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->bet; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->combination; aux; aux >>= 1)
        count += (aux & 1);
        
    return count;
}
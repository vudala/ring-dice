#include "message.h"

unsigned count_1s(Message * msg)
{
    unsigned aux;
    unsigned count = 0;
    for(aux = msg->chosen_id; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->type; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->tokens; aux; aux >>= 1)
        count += (aux & 1);
    for(aux = msg->combination; aux; aux >>= 1)
        count += (aux & 1);
        
    return count;
}
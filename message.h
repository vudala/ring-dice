//  4 bytes      4 bytes       2 bytes       2 bytes       2 byte   1 bytes       1 byte 
//  addr_origm | target_addr | origin_port | target_port | bet    | combination | count
#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

typedef struct message_t {
    unsigned        origin_addr;
    unsigned        target_addr;
    unsigned short  origin_port;
    unsigned short  target_port;
    unsigned short  bet;
    unsigned char   combination;
    unsigned char   count;
} Message;


// conta quantos 1s tem na mensagem
char count_1s(Message * msg);

unsigned addr_to_int(char addr[]);

#endif
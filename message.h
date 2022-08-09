//  4 bytes      4 bytes       2 bytes       2 bytes       2 byte   1 bytes       1 byte 
//  addr_origm | target_addr | origin_port | target_port | bet    | combination | count
#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED


enum Msg_Type {
    FINISH_GAME = 3,
    RESET
};


typedef struct message_t {
    unsigned        origin_addr;
    unsigned        target_addr;
    unsigned        chosen_addr;
    unsigned short  origin_port;
    unsigned short  target_port;
    unsigned short  chosen_port;
    unsigned char   type;
    unsigned char   status;
    unsigned short  bet;
    unsigned char   combination;
    unsigned char   count;
} Message;


// conta quantos 1s tem na mensagem
unsigned count_1s(Message * msg);


void print_bits(unsigned x);

#endif
//  4 bytes      4 bytes       4 bytes       2 bytes       2 bytes       2 bytes     | 1 byte   1 bytes    2 byte   1 byte        1 byte
//  addr_origm | target_addr | chosen_addr | origin_port | target_port | chosen_port | type   | status   | bet    | combination | count
#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED


enum Msg_Type {
    RESET,
    BET
};


enum Msg_Status {
    PLAY = 0,
    FINISH = 255
};


typedef struct message_t {
    unsigned        origin_addr; // endereco de origem
    unsigned        target_addr; // endereco do alvo
    unsigned        chosen_addr; // endereco do escolhido pra fazer a jogada
    unsigned short  origin_port; // porta de origem
    unsigned short  target_port; // porta do alvo
    unsigned short  chosen_port; // porta do escolhido pra fazer a jogada
    unsigned char   type;        // tipo da mensagem
    unsigned char   status;      // status da mensagem
    unsigned short  bet;         // valor de aposta
    unsigned char   combination; // combinacao escolhida
    unsigned char   count;       // contador da paridade horizontal
} Message;


// Faz a paridade horizontal
unsigned count_1s(Message * msg);

#endif
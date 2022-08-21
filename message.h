//  4 bytes      4 bytes       4 bytes       2 bytes       2 bytes       2 bytes     | 1 byte   1 bytes    2 byte   1 byte        1 byte
//  addr_origm | target_addr | chosen_addr | origin_port | target_port | chosen_port | type   | status   | bet    | combination | count
#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#define FLAG 22358

enum Msg_Type {
    BET = 3,
    RESET,
    BAT,
    BAL_UPDATE,
    FINISH,
    RESULT
};


typedef struct message_t {
    unsigned char   count;       // contador da paridade horizontal
    unsigned char   combination; // combinacao escolhida
    unsigned short  tokens;      // valor de aposta ou novo saldo do jogador que jogou
    unsigned char   type;        // tipo da mensagem
    unsigned short  chosen_id;   // id do jogador referenciado
    unsigned short  flag;
} Message;


// Faz a paridade horizontal
unsigned count_1s(Message * msg);

#endif
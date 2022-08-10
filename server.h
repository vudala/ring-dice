#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED

#include "message.h"

// Inicia a conexão com o socket
void init_con(char * addr1, unsigned short port1, char * addr2, unsigned short port2);

// Envia uma mensagem
void send_msg(Message * msg);

// Recebe uma mensagem, valida e a retorna
Message * recv_msg();

// Esperar receber o bastão
int recv_bat();

// Passa o bastão
void send_bat();


// Preenche uma mensagem com os campos a serem enviados
Message * build_msg(unsigned char combination, unsigned char bet, unsigned char type, unsigned char status);


struct sockaddr_in get_origin();

struct sockaddr_in get_target();

int validate_msg(Message * msg);

#endif

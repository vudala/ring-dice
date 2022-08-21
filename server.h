#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED

#include "message.h"

// Inicia a conexão com o socket
void init_con();

// Envia uma mensagem
void send_msg(Message * msg);

// Recebe uma mensagem, valida e a retorna
Message * recv_msg();

// Esperar receber o bastão
int recv_bat();

// Passa o bastão
void send_bat();

// Preenche uma mensagem com os campos a serem enviados
Message * build_msg(unsigned chos_id, unsigned char combination, unsigned char tokens, unsigned char type);

int validate_msg(Message * msg);

#endif

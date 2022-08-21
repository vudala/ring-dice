#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>

#include "server.h"

// socket file descriptor
int sockfd; 


// buffer pra receber dados
#define MAXLINE 1024
char buffer[MAXLINE];

// dados da conexao
struct sockaddr_in Origin, Target;
extern char * Origin_Addr, * Target_Addr;
extern unsigned short Origin_Port, Target_Port;


// seta dados da origem
void set_origin()
{
    memset(&Origin, 0, sizeof(Origin));
    Origin.sin_family = AF_INET; // IPv4 
    inet_aton(Origin_Addr, &Origin.sin_addr);
    Origin.sin_port = htons(Origin_Port); 
}


// setta dados do alvo
void set_target()
{
    memset(&Target, 0, sizeof(Target));
    Target.sin_family = AF_INET; // IPv4 
    inet_aton(Target_Addr, &Target.sin_addr);

    Target.sin_port = htons(Target_Port); 
}


// inicializa estruturas
void init_con()
{
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }
        
    set_origin();
    set_target();
    
    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr *) &Origin, sizeof(Origin)) < 0) { 
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }
}


// envia msg pra frente
void send_msg(Message * msg)
{
    sendto(
        sockfd,
        (const char *) msg,
        sizeof(Message),  
        MSG_CONFIRM,
        (const struct sockaddr *) &Target, 
        sizeof(Target)
    );
}


// valida mensagem por paridade horizontal
int validate_msg(Message * msg)
{
    if (rand() % 10 == 9) return 0;
    if (count_1s(msg) == msg->count)
        return 1;
    return 0;
}


// recebe mensagem e valida
Message * recv_msg()
{
    recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, NULL, 0);
    Message * msg = (Message*) malloc(sizeof(Message));
    memcpy(msg, buffer, sizeof(Message));
    if (validate_msg(msg))
        return msg;
    return NULL;
}


// constroi uma mensagem
Message * build_msg(unsigned chos_id, unsigned char combination, unsigned char tokens, unsigned char type)
{
    Message * msg = (Message*) malloc(sizeof(msg));
    msg->flag = FLAG;
    msg->combination = combination;
    msg->tokens = tokens;
    msg->chosen_id = chos_id;
    msg->type = type;
    msg->count = count_1s(msg);
    return msg;
}


struct sockaddr_in get_origin()
{
    return Origin;
}

struct sockaddr_in get_target()
{
    return Target;
}
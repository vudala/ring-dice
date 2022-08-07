#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/stdc++.h>

#include "server.h"

#define MAXLINE 1024

int sockfd; 
char buffer[MAXLINE];
struct sockaddr_in Origin, Target;
char * _Origin_Addr, * _Target_Addr;
unsigned short _Origin_Port, _Target_Port;


// set the info for the origin
void set_origin() {
    memset(&Origin, 0, sizeof(Origin));
    Origin.sin_family = AF_INET; // IPv4 
    inet_aton(_Origin_Addr, &Origin.sin_addr);
    Origin.sin_port = htons(_Origin_Port); 
}


// set the info for the target
void set_target() {
    memset(&Target, 0, sizeof(Target));
    Target.sin_family = AF_INET; // IPv4 
    inet_aton(_Target_Addr, &Target.sin_addr);

    Target.sin_port = htons(_Target_Port); 
}


void init_con(char * addr1, unsigned short port1, char * addr2, unsigned short port2) {
    _Origin_Addr = addr1;
    _Origin_Port = port1;
    _Target_Addr = addr2;
    _Target_Port = port2;

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


void send_bat() {
    char bat[] = "BAT";
    sendto(
        sockfd,
        (const char *) bat,
        strlen(bat),  
        MSG_CONFIRM,
        (const struct sockaddr *) &Target, 
        sizeof(Target)
    );
}


void send_msg(Message * msg) {
    sendto(
        sockfd,
        (const char *) msg,
        sizeof(Message),  
        MSG_CONFIRM,
        (const struct sockaddr *) &Target, 
        sizeof(Target)
    );
}


int recv_bat() {
    recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, NULL, 0);
    char bat[] = "BAT";
    if (strcmp(bat, buffer))
        return 1;
    return 0;
}


int validate_msg(Message * msg) {
    if (count_1s(msg) == msg->count)
        return 1;
    return 0;
}


Message * recv_msg() {
    recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, NULL, 0);
    Message * msg = (Message*) malloc(sizeof(Message));
    memcpy(msg, buffer, sizeof(Message));
    if (validate_msg(msg))
        return msg;
    return NULL;
}
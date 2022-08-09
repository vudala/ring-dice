// Server side implementation of UDP client-server model 

#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <bits/stdc++.h>

#include "message.h"
#include "server.h"
#include "dices.h"
#include "game.h"

#define PN cout << '\n'

using namespace std;

char * Origin_Addr, * Target_Addr;
unsigned short Origin_Port, Target_Port;
unsigned Is_Origin = 0;




// Driver code 
int main(int argc, char * argv[]) {
    Origin_Addr = argv[1];
    Origin_Port = stoi(argv[2]);
    Target_Addr = argv[3];
    Target_Port = stoi(argv[4]);
    Is_Origin = stoi(argv[5]);

    srand(time(NULL));
    
    init_con(Origin_Addr, Origin_Port, Target_Addr, Target_Port);

    init_game(5, Is_Origin);

    play_game();
}
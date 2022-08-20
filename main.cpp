#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <bits/stdc++.h>

#include "message.h"
#include "server.h"
#include "dices.h"
#include "game.h"

using namespace std;

unsigned ID;
char * Origin_Addr, * Target_Addr;
unsigned short Origin_Port, Target_Port;
bool Is_Origin;

// driver code 
int main(int argc, char * argv[]) {
    ID = stoi(argv[1]);
    Origin_Addr = argv[2];
    Origin_Port = stoi(argv[3]);
    Target_Addr = argv[4];
    Target_Port = stoi(argv[5]);
    Is_Origin = stoi(argv[6]);

    srand(time(NULL));
    
    init_con();

    init_game();

    play_game();
}
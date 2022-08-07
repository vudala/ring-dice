// Server side implementation of UDP client-server model 

#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <bits/stdc++.h>

#include "message.h"
#include "server.h"
#include "dices.h"

#define PN cout << '\n'

using namespace std;

char * Origin_Addr, * Target_Addr;
unsigned short Origin_Port, Target_Port;
unsigned Tokens = 5, Bat = 0;


int read_combination() {
    unsigned opt = 0;
    while(opt <= 0 || opt > 8) {
        cout << "Escolha uma combinação:\n1 - Par\n2 - Trio\n3 - Dois Pares\n4 - Full House";
        cout << "\n5 - Sequência baixa\n6 - Sequência alta\n7 - Quadra\n8 - Quinteto\n";
        cin >> opt;
    }
    return opt;
}

int read_bet() {
    unsigned bet = 0;
    while (bet <= 0 || bet > Tokens) {
        cout << "Quanto você deseja apostar?\n";
        cin >> bet;
    }
    return bet;
}

Message * build_msg() {
    Message * msg = (Message*) malloc(sizeof(msg));
    msg->combination = read_combination();
    msg->bet = read_bet();
    msg->origin_addr = addr_to_int(Origin_Addr);
    msg->origin_port = Origin_Port;
    msg->target_addr = addr_to_int(Target_Addr);
    msg->target_port = Target_Port;
    msg->count = count_1s(msg);
    return msg;
}

void do_magic(char * argv[]) {
    while(Tokens > 0) {
        if (!Bat)
            recv_bat();
        
        Tokens--;
        Message * msg = build_msg();

        send_bat();
        send_msg(msg);

        recv_bat();
        recv_msg();
    }
}


void roll_dices() {
    cout << "Hora de rolar os dados\n";
    cout << "3 rolagens restantes\n";
    cout << "Envie algo para rolar\n";
    int opt = 0;
    cin >> opt;
    sleep(1);
    vector<int> dices (5);
    for(int& dice : dices) {
        dice = rand() % 6 + 1;
        cout << dice << ' ';
    }
    PN;
    cout << "2 rolagens restantes\n";
    unsigned qnt = 0;
    cout << "Quantos dados você quer rerrolar?\n";
    cin >> qnt;

    vector<int> choice;
    if (qnt > 0) {
        choice = vector<int> (qnt);
        cout << "Quais são os dados? (0 ~ 5)\n";
        for(int& c : choice)
            cin >> c;

        cout << "Rerrolando...\n";
        sleep(1);
        for(int& c : choice)
            dices[c] = rand() % 6 + 1;

        cout << "Seus dados agora são: \n";
        for(int& dice : dices) cout << dice << ' '; PN;
        choice.clear();

        cout << "1 rolagem restante\n";
        qnt = 0;
        cout << "Quantos dados você quer rerrolar?\n";
        cin >> qnt;

        if (qnt > 0) {
            choice = vector<int> (qnt);
            cout << "Quais são os dados? (0 ~ 5)\n";
            for(int& c : choice)
                cin >> c;

            cout << "Rerrolando...\n";
            sleep(1);
            for(int& c : choice)
                dices[c] = rand() % 6 + 1;
        }
    }
    
    cout << "Seus dados são estes: \n";
    for(int& dice : dices) cout << dice << ' '; PN;
    
}

// Driver code 
int main(int argc, char * argv[]) {
    // Origin_Addr = argv[1];
    // Origin_Port = stoi(argv[2]);
    // Target_Addr = argv[3];
    // Target_Port = stoi(argv[4]);
    // Bat = stoi(argv[5]);

    // srand(time(NULL));
    
    // init_con(Origin_Addr, Origin_Port, Target_Addr, Target_Port);
    // do_magic(argv);
}
#include "server.h"
#include "game.h"
#include "dices.h"

#include <bits/stdc++.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;
#define PN cout << '\n'

struct sockaddr_in Origin_, Target_;
unsigned Tokens = 5, Is_Origin_ = 0;


void init_game(unsigned tok, unsigned ori) {
    Origin_ = get_origin();
    Target_ = get_target();
    Tokens = tok;
    Is_Origin_ = ori;
}

unsigned read_combination() {
    unsigned opt = 0;
    while(opt < 1 || opt > 8) {
        cout << "Escolha uma combinação:\n1 - Par\n2 - Trinca\n3 - Dois Pares\n4 - Full House";
        cout << "\n5 - Sequência baixa\n6 - Sequência alta\n7 - Quadra\n8 - Quinteto\n";
        cin >> opt;
    }
    return opt;
}

unsigned read_bet() {
    unsigned bet = 0;
    cout << "Você tem " << Tokens << " fichas\n";
    while (bet < 1 || bet > Tokens) {
        cout << "Quanto você deseja apostar?\n";
        cin >> bet;
    }
    return bet;
}


void make_bet(Message * msg) {
    cout << "Combinação escolhida: " << get_label(msg->combination) << '\n';
    cout << "Aposta de " << msg->bet << " fichas\n";
    cout << "Você tem " << Tokens << " fichas restantes\n";

    if (Tokens >= unsigned(msg->bet + 1)) {
        cout << "Quer aumentar (!0) ou correr (0) ?\n";
        unsigned choice;
        cin >> choice;
        if (choice) {
            msg->bet += 1;
            msg->chosen_addr = Origin_.sin_addr.s_addr;
            msg->chosen_port = Origin_.sin_port;
        }
    }
}


vector<int> roll_dices() {
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
        cout << "Quais são os dados? (0 ~ 4)\n";
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
            cout << "Quais são os dados? (0 ~ 4)\n";
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
    return dices;
}


bool is_for_me(Message * msg){
    return msg->target_addr == Origin_.sin_addr.s_addr && msg->target_port == Origin_.sin_port;
}

bool am_i_chosen(Message * msg) {
    return msg->chosen_addr == Origin_.sin_addr.s_addr && msg->chosen_port == Origin_.sin_port;
}


int make_play(Message * msg){
    cout << "Você deve formar um(a) " << get_label(msg->combination) << '\n'; 
    vector<int> dices = roll_dices();
    int score = get_score(msg->combination, dices);
    if (score > 0) {
        cout << "Você ganhou " << score << " fichas\n";
        Tokens += score;
        cout << "Seu saldo agora é de " << Tokens << " fichas\n";
    }
    else {
        cout << "Você perdeu " << score << " fichas\n";
        Tokens -= msg->bet;
        cout << "Seu saldo agora é de " << Tokens << " fichas\n";
    }
    if (Tokens <= 0)
        return 0;
    return 1;
}


void print_msg(Message * m) {
    cout << m->bet << ' ' << m->combination;PN;
}


void play_game() {
    bool first = true;
    for(;;) {
        if (Is_Origin_) {
            if (!first) {
                recv_bat();
                recv_msg();
            }
            first = false;

            Message * msg = build_msg(read_combination(), read_bet(), 0, 0);
            send_bat();
            send_msg(msg);
            free(msg);

            recv_bat();
            msg = recv_msg();

            if (am_i_chosen(msg)) {
                if (!make_play(msg)) {
                    cout << "Suas fichas acabaram, encerrando o jogo\n";
                    msg = build_msg(0, 0, FINISH_GAME, 0);
                    send_bat();
                    send_msg(msg);
                    exit(0);
                }
                free(msg);
                msg = build_msg(0, 0, RESET, 0);
                send_bat();
                send_msg(msg);
            }
            else if (msg->type == FINISH_GAME) {
                cout << "Algum jogador está sem fichas, encerrando o jogo\n";
                free(msg);
                msg = build_msg(0, 0, FINISH_GAME, 0);
                send_bat();
                send_msg(msg);
                exit(0);
            }
            else {
                send_bat();
                send_msg(msg);
            }
        }
        else {
            // Recebe a mensagem
            recv_bat();
            Message * msg = recv_msg();

            make_bet(msg);

            send_bat();
            send_msg(msg);
            recv_bat();

            free(msg);
            msg = recv_msg();
            if (am_i_chosen(msg)) {
                if (!make_play(msg)) {
                    cout << "Suas fichas acabaram, encerrando o jogo\n";
                    msg = build_msg(0, 0, FINISH_GAME, 0);
                    send_msg(msg);
                    exit(0);
                };
                free(msg);
                msg = build_msg(0, 0, RESET, 0);
                send_bat();
                send_msg(msg);
            }
            else if (msg->type == FINISH_GAME) {
                cout << "Algum jogador está sem fichas, encerrando o jogo\n";
                free(msg);
                msg = build_msg(0, 0, FINISH_GAME, 0);
                send_bat();
                send_msg(msg);
                exit(0);
            }
            else {
                send_bat();
                send_msg(msg);
            }
        }
    }
    
}
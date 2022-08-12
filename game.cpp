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


// inicializa estruturas
void init_game(unsigned tok, unsigned ori) {
    Origin_ = get_origin();
    Target_ = get_target();
    Tokens = tok;
    Is_Origin_ = ori;
}


// determina a combinação
unsigned read_combination() {
    unsigned opt = 0;
    while(opt < 1 || opt > 8) {
        cout << "Escolha uma combinação:\n1 - Par\n2 - Trinca\n3 - Dois Pares\n4 - Full House";
        cout << "\n5 - Sequência baixa\n6 - Sequência alta\n7 - Quadra\n8 - Quinteto\n";
        cin >> opt;
    }
    return opt;
}


// lê a aposta da origem
unsigned read_bet() {
    unsigned bet = 0;
    cout << "Você tem " << Tokens << " fichas\n";
    while (bet < 1 || bet > Tokens) {
        cout << "Quanto você deseja apostar?\n";
        cin >> bet;
    }
    return bet;
}


// faz a aposta
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
            msg->count = count_1s(msg);
        }
    }
    else {
        cout << "Você não tem fichas o suficiente para entrar na aposta\n";
    }
    cout << "Aguardando a jogada ser concluida\n";
}



// rola os dados
vector<int> roll_dices() {
    cout << "Hora de rolar os dados\n";
    cout << "3 rolagens restantes\n";
    cout << "Envie algo para rolar\n";
    int opt = 0;
    cin >> opt;
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
    if (qnt > 0 && qnt < 6) {
        choice = vector<int> (qnt);
        cout << "Quais são os dados? (0 ~ 4)\n";
        for(int& c : choice)
            cin >> c;

        cout << "Rerrolando...\n";
        for(int& c : choice) {
            dices[c] = rand() % 6 + 1;
        }
        
        cout << "Seus dados agora são: \n";
        for(int& dice : dices) cout << dice << ' '; PN;
        choice.clear();

        cout << "1 rolagem restante\n";
        qnt = 0;
        cout << "Quantos dados você quer rerrolar?\n";
        cin >> qnt;

        if (qnt > 0 && qnt < 6) {
            choice = vector<int> (qnt);
            cout << "Quais são os dados? (0 ~ 4)\n";
            for(int& c : choice)
                cin >> c;

            cout << "Rerrolando...\n";
            for(int& c : choice) {
                dices[c] = rand() % 6 + 1;
            }
        }
    }
    
    cout << "Seus dados são estes: \n";
    for(int& dice : dices) cout << dice << ' '; PN;
    return dices;
}


// verifica se é o escolhido pra fazer a jogada
bool am_i_chosen(Message * msg) {
    return msg->chosen_addr == Origin_.sin_addr.s_addr && msg->chosen_port == Origin_.sin_port;
}


// realiza a jogada
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
        cout << "Você perdeu " << msg->bet << " fichas\n";
        Tokens -= msg->bet;
        cout << "Seu saldo agora é de " << Tokens << " fichas\n";
    }
    if (Tokens <= 0)
        return 0;
    return 1;
}


void send_finish() {
    send_bat();
    send_msg(build_msg(0, 0, 0, FINISH));
    exit(0);
}


void send_reset() {
    send_bat();
    send_msg(build_msg(0, 0, RESET, PLAY));
}


bool first = true;
void origin_side() {
    if (!first) {
        recv_bat();
        Message * m = recv_msg();
        if (m && m->status != PLAY) {
            cout << "Algum jogador está sem fichas, encerrando o jogo\n";
            send_finish();
        }
    }
    first = false;

    // Manda a mensagem inicial
    Message * msg = build_msg(read_combination(), read_bet(), BET, 0);
    send_bat();
    send_msg(msg);

    // Recebe a mensagem com as informacoes da aposta
    recv_bat();
    msg = recv_msg();
    
    // Se houve erro na mensagem ou reset
    if (!msg || msg->type == RESET) {
        send_reset();
        return;
    }
    // Caso o jogo tenha encerrado por algum motivo
    else if (msg->status != PLAY) {
        cout << "Algum jogador está sem fichas, encerrando o jogo\n";
        send_finish();
    }
    // Se for o escolhido
    else if (am_i_chosen(msg)) {
        // Faz a jogada, e se acabou as fichas encerra o jogo
        if (!make_play(msg)) {
            cout << "Suas fichas acabaram, encerrando o jogo\n";
            send_finish();
            exit(0);
        }
        // Reseta
        send_reset();
    }
    // Apenas passa pra frente a mensagem
    else {
        send_bat();
        send_msg(msg);
    }
}

void player_side() {
    // Recebe a mensagem
    recv_bat();
    Message * msg = recv_msg();

    // Caso o jogo tenha encerrado por algum motivo
    if (msg && msg->status != PLAY) {
        cout << "Algum jogador está sem fichas, encerrando o jogo\n";
        send_finish();
    }
    // Caso a mensagem tenha vindo com erro ou reset
    if (!msg || msg->type == RESET) {
        send_reset();
        return;
    }

    // Faz a aposta e envia pra frente
    make_bet(msg);
    send_bat();
    send_msg(msg);

    recv_bat();
    msg = recv_msg();
    
    // Se recebeu um comando para encerrar o jogo
    if (msg && msg->status != PLAY) {
        cout << "Algum jogador está sem fichas, encerrando o jogo\n";
        send_finish();
    }
    // Se houve um erro na mensagem
    else if (!msg) {
        cout << "Erro detectado, resetando o jogo\n";
        send_reset();
        return;
    }
    else if (msg->type == RESET) {
       send_reset();
       return;
    }
    // Se for o escolhido para fazer a jogada
    else if (am_i_chosen(msg)) {
        // Faz a jogada, e se acabou as fichas encerra o jogo
        if (!make_play(msg)) {
            cout << "Suas fichas acabaram, encerrando o jogo\n";
            send_finish();
        };
        // Reseta
        send_reset();
    }
    // Apenas passa pra frente a mensagem
    else {
        send_bat();
        send_msg(msg);
    }
}


void play_game() {
    for(;;)
        if (Is_Origin_)
            origin_side();
        else
            player_side();
}

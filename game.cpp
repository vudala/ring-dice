#include "server.h"
#include "game.h"
#include "dices.h"

#include <bits/stdc++.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;
#define PN cout << '\n'

unsigned ID_;
struct sockaddr_in Origin_, Target_;
unsigned Tokens = 5, Is_Origin_ = 0;

vector<unsigned> Scores;


// inicializa estruturas
void init_game(unsigned id, unsigned tok, unsigned ori) {
    ID_ = id;
    Origin_ = get_origin();
    Target_ = get_target();
    Tokens = tok;
    Is_Origin_ = ori;
    Scores = {tok, tok, tok, tok};
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
            msg->chosen_id = ID_;
            msg->count = count_1s(msg);
        }
    }
    else {
        cout << "Você não tem fichas o suficiente para entrar na aposta\n";
    }
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
    return msg->chosen_id == ID_;
}


// realiza a jogada
void make_play(Message * msg){
    cout << "Você deve formar um(a) " << get_label(msg->combination) << '\n'; 
    vector<int> dices = roll_dices();
    int score = get_score(msg->combination, dices);
    cout << "Você perdeu " << msg->bet << " fichas da aposta\n";
    cout << "E ganhou " << score << " fichas da jogada\n";
    Tokens += score;
    Tokens -= msg->bet;
}


bool is_over() {
    for (unsigned& s : Scores)
        if (s <= 0)
            return 1;
    return 0;
}


void send_reset() {
    send_msg(build_msg(0, 0, 31, RESET));
}


void send_bat() {
    send_msg(build_msg(0, 0, 33, BAT));
}


void send_update_bal() {
    send_msg(build_msg(ID_, 0, Tokens, BAL_UPDATE));
}



void print_scores() {
    cout << "\nx-------- SCORES --------x\n";
    for(unsigned i = 0; i < 4; i++)
        if (i == ID_)
            cout << "Você:     " << Scores[i] << " fichas\n";
        else
            cout << "Player " << i << ": "  << Scores[i] << " fichas\n";
    cout << "x-------- xxxxxx --------x\n\n";
}

void origin_side() {
    cout << "Sua vez de fazer as apostas\n";
    // Manda a mensagem inicial
    Message * msg = build_msg(ID_, read_combination(), read_bet(), BET);
    send_msg(msg);

    msg = recv_msg();
    
    // Se for o escolhido
    if (am_i_chosen(msg)) {
        // Faz a jogada, e se acabou as fichas encerra o jogo
        make_play(msg);

        print_scores();

        send_update_bal();

        recv_msg();
    }
    // Apenas passa pra frente a mensagem
    else {
        cout << "Aguardando a jogada ser concluida\n";
        send_msg(msg);

        msg = recv_msg();

        if (msg->type == BAL_UPDATE) {
            cout << "Player " << msg->chosen_id << " realizou a jogada\n";
            Scores[msg->chosen_id] = msg->bet;
            print_scores();
            send_msg(msg);
            if (is_over()) {
                cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
                cout << "Encerrando o jogo\n";
                exit(0);
            }
        }
    }
}

void player_side() {
    cout << "Aguardando origem\n";
    // Recebe a mensagem
    Message * msg = recv_msg();

    // Faz a aposta e envia pra frente
    make_bet(msg);
    send_msg(msg);

    cout << "Aguardando a jogada ser concluida\n";
    // Recebe a jogada
    msg = recv_msg();
    
    // Se for o escolhido para fazer a jogada
    if (am_i_chosen(msg)) {
        // Faz a jogada
        make_play(msg);

        print_scores();

        send_update_bal();

        recv_msg();
    }
    else if (msg->type == BAL_UPDATE) {
        cout << "Player " << msg->chosen_id << " realizou a jogada\n";
        Scores[msg->chosen_id] = msg->bet;
        send_msg(msg);
        print_scores();
        if (is_over()) {
            cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
            cout << "Encerrando o jogo\n";
            exit(0);
        }
    }
    // Apenas passa pra frente a mensagem
    else {
        send_msg(msg);

        // Fica aguardando um update bal
        msg = recv_msg();
        if (msg->type == BAL_UPDATE) {
            cout << "Player " << msg->chosen_id << " realizou a jogada\n";
            Scores[msg->chosen_id] = msg->bet;
            print_scores();
            send_msg(msg);
            if (is_over()) {
                cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
                cout << "Encerrando o jogo\n";
                exit(0);
            }
        }
    }    
}


void play_game() {
    cout << "Bem vindo ao jogo!\n";
    cout << "Aqui estão os scores iniciais\n";
    print_scores();
    for(;;) {
        if (Is_Origin_) {
            origin_side();
            Is_Origin_ = false;
            send_bat();
        }
        else {
            player_side();
            send_reset();
        }
        Message * msg = recv_msg();
        if (msg->type == BAT)
            Is_Origin_ = true;
    }
}

#include "server.h"
#include "game.h"
#include "dices.h"

#include <bits/stdc++.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;
#define PN cout << '\n'

extern unsigned ID;
extern struct sockaddr_in Origin, Target;
extern bool Is_Origin;
vector<unsigned> Scores;


// inicializa estruturas
void init_game() {
    Scores = {5, 5, 5, 5};
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
    cout << "Você tem " << Scores[ID] << " fichas\n";
    cout << "E gastará uma para escolher a jogada\n";
    return 1;
}


// faz a aposta
void make_bet(Message * msg) {
    cout << "Combinação escolhida: " << get_label(msg->combination) << '\n';
    cout << "Aposta de " << msg->tokens << " fichas\n";
    cout << "Você tem " << Scores[ID] << " fichas restantes\n";

    if (Scores[ID] >= unsigned(msg->tokens + 1)) {
        cout << "Quer aumentar (!0) ou correr (0) ?\n";
        unsigned choice;
        cin >> choice;
        if (choice) {
            msg->tokens += 1;
            msg->chosen_id = ID;
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
    return msg->chosen_id == ID;
}


// realiza a jogada
void make_play(Message * msg){
    cout << "Você deve formar um(a) " << get_label(msg->combination) << '\n'; 
    vector<int> dices = roll_dices();
    int score = get_score(msg->combination, dices);
    cout << "Você perdeu " << msg->tokens << " fichas da aposta\n";
    cout << "E ganhou " << score << " fichas da jogada\n";
    Scores[ID] += score - msg->tokens;
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


void send_result(unsigned id, unsigned score) {
    send_msg(build_msg(id, 0, score, RESULT));
}


void send_bal_update(unsigned id, unsigned score){
    send_msg(build_msg(id, 0, score, BAL_UPDATE));
}


void send_finish() {
    cout << "Erro detectado, encerrando execução\n";
    send_msg(build_msg(0, 0, 37, FINISH));
    exit(-1);
}


void print_scores() {
    cout << "\nx-------- SCORES --------x\n";
    for(unsigned i = 0; i < 4; i++)
        if (i == ID)
            cout << "Você:     " << Scores[i] << " fichas\n";
        else
            cout << "Player " << i << ": "  << Scores[i] << " fichas\n";
    cout << "x-------- xxxxxx --------x\n\n";
}

int origin_side() {
    cout << "Sua vez de fazer as apostas\n";
    // Manda a mensagem inicial
    Message * msg = build_msg(ID, read_combination(), read_bet(), BET);
    send_msg(msg);
    cout << "Coletando as apostas\n";

    msg = recv_msg();
    // Se detectou erro
    if (!msg || msg->type == FINISH) {
        send_finish();
    }
    // Se for o escolhido
    else if (am_i_chosen(msg)) {
        // Faz a jogada, e se acabou as fichas encerra o jogo
        make_play(msg);

        print_scores();

        send_bal_update(ID, Scores[ID]);

        if (is_over()) {
            cout << "Você está sem fichas\n"; 
            cout << "Encerrando o jogo\n";
            exit(0);
        }

        msg = recv_msg();
        // Se detectou erro
        if (!msg || msg->type == FINISH) {
            send_finish();
        }
    }
    // Apenas passa pra frente a mensagem
    else {
        cout << "Aguardando a jogada ser concluida\n";
        send_msg(msg);

        msg = recv_msg();
        // Se detectou erro
        if (!msg || msg->type == FINISH) {
            send_finish();
        }
        else if (msg->type == RESULT) {
            cout << "Player " << msg->chosen_id << " realizou a jogada\n";
            Scores[msg->chosen_id] = msg->tokens;
            print_scores();

            send_bal_update(msg->chosen_id, msg->tokens);

            if (is_over()) {
                cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
                cout << "Encerrando o jogo\n";
                exit(0);
            }

            msg = recv_msg();
            if (!msg || msg->type == FINISH) {
                send_finish();
            }
        }
    }

    return 1;
}

int player_side() {
    cout << "Aguardando origem\n";
    // Recebe a mensagem
    Message * msg = recv_msg();
    // Se detectou erro
    if (!msg || msg->type == FINISH) {
        send_finish();
    }

    // Faz a aposta e envia pra frente
    make_bet(msg);
    send_msg(msg);

    cout << "Aguardando a jogada ser concluida\n";
    // Recebe a jogada
    msg = recv_msg();
    // Se detectou erro
    if (!msg || msg->type == FINISH) {
        send_finish();
    }

    // Se for o escolhido para fazer a jogada
    else if (am_i_chosen(msg)) {
        // Faz a jogada
        make_play(msg);

        send_result(ID, Scores[ID]);

        msg = recv_msg();
        if (!msg || msg->type == FINISH) {
            send_finish();
        }
        else if (msg->type == BAL_UPDATE) {
            cout << "Player " << msg->chosen_id << " realizou a jogada\n";
            Scores[msg->chosen_id] = msg->tokens;
            send_msg(msg);
            print_scores();
            if (is_over()) {
                cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
                cout << "Encerrando o jogo\n";
                exit(0);
            }
        }
    }
    else if (msg->type == BAL_UPDATE) {
        cout << "Player " << msg->chosen_id << " realizou a jogada\n";
        Scores[msg->chosen_id] = msg->tokens;
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
        // Se detectou erro
        if (!msg || msg->type == FINISH) {
            send_finish();
        }
        else if (msg->type == BAL_UPDATE) {
            cout << "Player " << msg->chosen_id << " realizou a jogada\n";
            Scores[msg->chosen_id] = msg->tokens;
            print_scores();
            send_msg(msg);
            if (is_over()) {
                cout << "Player " << msg->chosen_id << " está sem fichas\n"; 
                cout << "Encerrando o jogo\n";
                exit(0);
            }
        }
    }

    return 1;
}


void play_game() {
    cout << "Bem vindo ao jogo Player " << ID << " !\n";
    cout << "Aqui estão os scores iniciais\n";
    print_scores();
    for(;;) {
        if (Is_Origin) {
            origin_side();
            Is_Origin = false;
            Message * msg = recv_msg();
            // Se detectou erro
            if (!msg || msg->type == FINISH) {
                send_finish();
            }

            send_bat();
        }
        else {
            player_side();

            send_reset();
            Message * msg = recv_msg();
            // Se detectou erro
            if (!msg || msg->type == FINISH) {
                send_finish();
            }
            if (msg->type == BAT)
                Is_Origin = true;
        }
    }
}

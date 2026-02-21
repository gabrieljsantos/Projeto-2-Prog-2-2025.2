#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "headers.h"
#include "login_matricula.h"
#include "admin.h"
#include "professor.h"
#include "aluno.h"
#include "eventos.h"
#include "instrumentos.h"
#include "lanchonete.h"
#include "interface_grafica.h"

using namespace std;

void pausar();
void exibirCabecalhoEscola();
// void exibirCabecalhoSistema();
saida_menu exibirMenuLogin();

int main() {
    exibirCabecalhoEscola();
    
    Usuario usuario;
    saida_menu saidaLogin;
    bool executando = true;

    Login_mat::inicializarArquivos();
    Login_mat::realizarCadastroAdmin();

    while (executando) {
        if (!usuario.logado) {
            saidaLogin = exibirMenuLogin();

            switch (saidaLogin.indice_da_opcao) {
                case 0: {
                    bool login = Login_mat::realizarLogin(usuario);
                    string informacao;

                    if (login) {
                        informacao = "Bem-vindo(a), " + string(usuario.nome) + "!";
                        mostrar_caixa_informacao("AVISO", informacao);
                    } else {
                        mostrar_caixa_informacao("ERRO", "Falha no login. Verifique suas credenciais.");
                    }
                    break;
                }
                case 1: {
                    int idNovoUsuario = Login_mat::realizarCadastro();
                    string informacao = "Sucesso! Seu ID: " + to_string(idNovoUsuario);

                    if (idNovoUsuario != 0)
                        mostrar_caixa_informacao("AVISO", informacao);
                    break;
                }
                case 2:
                case -1:
                    executando = false;
                    break;
            }
        } else {
            switch (usuario.categoria) {
                case ADMINISTRADOR: {
                    abrir_menu_admin(&usuario);
                    if (usuario.logado) pausar();
                    break;
                }
                case PROFESSOR: {
                    abrir_menu_professor(&usuario);
                    if (usuario.logado) pausar();
                    break;
                }
                case ALUNO: {
                    abrir_menu_aluno(&usuario);
                    if (usuario.logado) pausar();
                    break;
                }
                default:
                    mostrar_caixa_informacao("ERRO", "Perfil invalido!");
                    usuario.logado = false;
            }
        }
    }

    return 0;
}

void pausar() {
    // Interface gráfica já trata as pausas
}

void exibirCabecalhoEscola() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
    }

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    clear();

    // Gera padrão aleatório (0-3)
    srand(time(0));
    int padrao_escolhido = rand() % 4;
    
    // Padrão gráfico de fundo - aleatório
    attron(COLOR_PAIR(4) | A_DIM);
    
    for (int y = 0; y < max_y; y++) {
        string linha;
        
        switch (padrao_escolhido) {
            case 0: { // Xadrez
                for (int x = 0; x < max_x; x++) {
                    if ((x + y) % 2 == 0) {
                        linha += ".";
                    } else {
                        linha += " ";
                    }
                }
                break;
            }
            case 1: { // Pontos Horizontal Alternado
                string base = (y % 2 == 0) ? ". . . . " : "  .  .  ";
                while ((int)linha.length() < max_x) {
                    linha += base;
                }
                linha = linha.substr(0, max_x);
                break;
            }
            case 2: { // Diagonal
                for (int x = 0; x < max_x; x++) {
                    int diagonal = (x + y) % 4;
                    if (diagonal == 0 || diagonal == 1) {
                        linha += ".";
                    } else {
                        linha += " ";
                    }
                }
                break;
            }
            case 3: { // Linhas Verticais
                for (int x = 0; x < max_x; x++) {
                    if (x % 3 == 0) {
                        linha += ".";
                    } else {
                        linha += " ";
                    }
                }
                break;
            }
        }
        
        mvprintw(y, 0, "%s", linha.c_str());
    }
    attroff(COLOR_PAIR(4) | A_DIM);

    // Título
    string titulo = "ESCOLA DE MUSICA BENICASA";
    int pos_titulo = (max_x - titulo.length()) / 2;

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(max_y / 2 - 2, pos_titulo, "%s", titulo.c_str());
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Subtítulo
    string subtitulo = "Sistema de Gestao";
    int pos_subtitulo = (max_x - subtitulo.length()) / 2;

    attron(COLOR_PAIR(2));
    mvprintw(max_y / 2, pos_subtitulo, "%s", subtitulo.c_str());
    attroff(COLOR_PAIR(2));

    // Instrução
    string aviso = "Pressione ENTER para continuar...";
    int pos_aviso = (max_x - aviso.length()) / 2;

    attron(COLOR_PAIR(3));
    mvprintw(max_y - 3, pos_aviso, "%s", aviso.c_str());
    attroff(COLOR_PAIR(3));

    refresh();

    // Aguardar ENTER
    int ch;
    while ((ch = getch()) != '\n' && ch != KEY_ENTER) {}

    endwin();
}

saida_menu exibirMenuLogin() {
    constexpr int qtdOpcoes = 3;
    string opcoes[qtdOpcoes] = {"Fazer Login", "Realizar Cadastro", "Sair do Sistema"};
    ConfigMenu config;
    config.titulo = "SISTEMA DE GESTAO - ESCOLA DE MUSICA";
    config.descricao = "MENU DE ACESSO";
    return interface_para_menu(qtdOpcoes, opcoes, config);
}

#include <iostream>
#include <locale.h>
#include "../utilidades.h"
#include "../headers.h"
#include "servico_cadastros.h"
using namespace std;

void janela_cadastro() {
    int escolha = -1;
    while (escolha != 0) {
        limpar_tela();
        cout << "----------------------------------------\n";
        cout << "Navegação: Admin/Cadastro/\n";
        cout << "----------------------------------------\n";
        cout << "\n";
        cout << "#########  CADASTRO DE USUÁRIOS  ######\n";
        cout << "########################################\n";
        cout << "#                                      #\n";
        cout << "#  1 - Cadastrar Alunos                #\n";
        cout << "#  2 - Cadastrar Professores           #\n";
        cout << "#  3 - Cadastrar Administradores       #\n";
        cout << "#  4 - Cadastrar Vendedores            #\n";
        cout << "#  5 - Cadastrar Locadores             #\n";
        cout << "#                                      #\n";
        cout << "#  0 - Voltar                          #\n";
        cout << "#                                      #\n";
        cout << "########################################\n";
        cout << "\n";
        cout << "Escolha uma opção: ";

        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                // Cadastro de alunos
                gerenciar_cadastro_usuario(ALUNO);
                break;
            case 2:
                // Cadastro de professores
                gerenciar_cadastro_usuario(PROFESSOR);
                break;
            case 3:
                // Cadastro de administradores
                gerenciar_cadastro_usuario(ADMIN);
                break;
            case 4:
                // Cadastro de vendedores
                gerenciar_cadastro_usuario(VENDEDOR);
                break;
            case 5:
                // Cadastro de locadores
                gerenciar_cadastro_usuario(LOCADOR);
                break;
            case 0:
                cout << "Voltando ao painel anterior...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
                cout << "Pressione Enter para continuar...";
                cin.get();
        }
    }
}

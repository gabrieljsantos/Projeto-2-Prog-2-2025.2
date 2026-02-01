#include <iostream>
#include <locale.h>
#include "headers.h"
#include "mod_login_e_matricula/servico_login.h"
#include "mod_login_e_matricula/servico_cadastros.h"
#include "mod_adminitrativo/entrada_admin.h"
#include "mod_lanchonete/entrada_vendedor.h"
#include "mod_instrumentos/entrada_locador.h"
#include "mod_area_do_professor/entrada_professor.h"
#include "mod_area_do_aluno/entrada_aluno.h"
#include <cstdlib> // ou stdlib.h
#include "utilidades.h"
using namespace std;

int main() {

    setlocale(LC_ALL, "Portuguese_Brazil");
    
    // Inicializar banco de dados com usuário ROOT se não existir
    inicializar_banco_dados();
    
    while (true)
    {
        limpar_tela();
        cout << "----------------------------------------\n";
        cout << "> INICIO/\n";
        cout << "----------------------------------------\n";
        cout << "\n";
        cout << "#  SISTEMA DE GESTÃO ESCOLAR  #######\n";
        cout << "########################################\n";
        cout << "#                                      #\n";
        cout << "#  Bem-vindo ao Sistema!               #\n";
        cout << "#                                      #\n";
        cout << "#  1 - Fazer Login                     #\n";
        cout << "#  0 - Sair                            #\n";
        cout << "#                                      #\n";
        cout << "########################################\n";
        cout << "\n";
        cout << "Escolha uma opção: ";
        int escolha;
        cin >> escolha;
        cin.ignore();

        if (escolha == 0) {
            cout << "Saindo do sistema. Até logo!\n";
            break;
        } else if (escolha == 1) {
            login_info info = janela_login();

            limpar_tela();

            switch (info.info_basica.categoria) {
                case ALUNO:
                    janela_aluno(info);
                    break;
                case PROFESSOR:
                    janela_professor(info);
                    break;
                case ADMIN:
                    janela_admin(info);
                    break;
                case VENDEDOR:
                    janela_vendedor(info);
                    break;
                case LOCADOR:
                    janela_locador(info);
                    break;
                default:
                    cout << "TIPO DE USUARIO DESCONHECIDO\n";
                    break;
            }
        } else {

            cout << "Opção inválida. Tente novamente.\n";
        }
    }

    return 0;
}

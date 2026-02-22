#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "eventos.h"

using namespace std;

vector<Evento> eventos;
int proximoID = 1;

int main()
{
    setlocale(LC_ALL, "portuguese");

    CarregarEventos();


    int opcao = -1;
        while (opcao != 0)
        {
            cout << "\n--------Gerenciamento de Eventos da Escola--------\n";
            cout << "1 - Criar Novo Evento\n";
            cout << "2 - Editar Evento\n";
            cout << "3 - Listar Eventos Agendados\n";
            cout << "4 - Visualizar Inscritos em Evento\n";
            cout << "5 - Excluir Evento\n";
            cout << "6 - Listar alunos insritos\n";
            cout << "0 - Voltar\n";
            cout << "Escolha uma opcao: ";
            cin >> opcao;
            cin.ignore();

            switch (opcao)
            {
                case 1:
                    adicionar_evento();
                    break;
                case 2:
                    editar_evento (eventos);
                    break;
                case 3:
                    listar_eventos(eventos);
                    break;
                case 4:
                    listar_alunos_inscritos();
                    break;
                case 5:
                {
                    int idExc;
                    cout << "Digite o id do evento que voc� quer excluir: \n";
                    cin >> idExc;
                    excluir_evento(idExc);
                    break;
                }

                case 0:
                    exit(1);

                default:
                    cout << "Opcao invalida. Tente novamente.\n";

            }
        }
        return 0;
}

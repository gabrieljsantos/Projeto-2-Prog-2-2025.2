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
            cout << "1 - Listar Eventos\n";
            cout << "2 - Inscrever-se\n";
            cout << "Escolha uma opcao: ";
            cin >> opcao;
            cin.ignore();

            switch (opcao)
            {
                case 1:
                     listar_eventos(eventos);
                    break;
                case 2:
                    inscrever_alunos();
                    break;
                case 0:
                    exit(1);
                }
                default:
                    cout << "Opcao invalida. Tente novamente.\n";

            }
        }
        return 0;
}

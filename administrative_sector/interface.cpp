#include "services.h"
#include <iostream>

using namespace std;

void student_window() {
    cout << "Painel do Aluno\n";
    
}


void admin_window() {
    int i = 0;
    int choice = -1;
    while (choice != 0) {
        cout << "Painel do Administrador\n";
        cout << "1 - Cadastrar Professor\n";
        cout << "2 - Cadastrar Aluno\n";
        cout << "3 - Cadastrar Vendedor\n";
        cout << "4 - Cadastrar Locador\n";
        cout << "0 - Sair do Painel Administrativo\n";
        cout << "Escolha uma opção: ";
        cin >> choice;  
        cin.ignore();
        switch (choice)
        {
        case 2:
            /* code */
            break;
        
        default:
            break;
        }
    }
}
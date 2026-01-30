#include "services.h"
#include <iostream>
#include <cstring>

using namespace std;

static person_role read_role() {
    int option;

    cout << "Categoria:\n";
    cout << "0 - Aluno\n";
    cout << "1 - Professor\n";
    cout << "2 - Admin\n";
    cout << "3 - Vendedor\n";
    cout << "4 - Locador\n";
    cout << "Escolha: ";
    cin >> option;
    cin.ignore();

    switch(option) {
        case 0: return ROLE_ALUNO;
        case 1: return ROLE_PROFESSOR;
        case 2: return ROLE_ADMIN;
        case 3: return ROLE_VENDEDOR;
        case 4: return ROLE_LOCADOR;
        default: return ROLE_ALUNO;
    }
}

void register_person(identidade *person) {
    if (person == nullptr) return;

    cout << "ID: ";
    cin >> person->id;
    cin.ignore();

    cout << "Nome: ";
    cin.getline(person->nome, 100);

    cout << "CPF: ";
    cin.getline(person->cpf, 20);

    cout << "Senha: ";
    cin.getline(person->senha, 30);

    person->category = read_role();
    person->ativo = 1;
}

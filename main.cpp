#include "services.h"
#include <iostream>

int main() {
    identidade pessoa;          // cria a struct

    register_person(&pessoa);   // passa o endereço

    std::cout << "Nome: " << pessoa.nome << "\n";

    return 0;
}

#include <iostream>
#include <fstream>
#include "login_matricula.h"
#include "headers.h"

int cadastro (int u){
    ofstream registroDeUsuáriosNovos("Cadastro.txt", ios::binary | ios:: app);

    cout<< "Nome: " << endl;
    cin.getline(usuario.nome,30);
    cout<< "Email: " << endl;
    cin.getline(usuario.email,100);
    cout<< "Função: " << endl;
    cin.getline(usuario.categoria);
    cout<< "Senha: " << endl;
    cin.getline(usuario.senha30);

    registroDeUsuáriosNovos.write((char*)&usuario,sizeof(usuario));

    registroDeUsuáriosNovos.close();

    ofstream registroDeUsuáriosNovos("Cadastro.txt", ios::binary | ios:: app);

    // falta retornar o id;

    return id;
}

int login(int u){
    bool verificaRetorno;
    cout<< "ID: "<< endl;
    cin >> usuario.id;
    cout<< "Senha: " << endl;
    cin.getline(usuario.senha30);

    verificaRetorno = verificarUsuarioExistente(u); //int;

    if(verificaRetorno){
        //entra em alguma função ou só
    }
    else{
        cout<< "Usuário não existente!" << endl;
    }
}

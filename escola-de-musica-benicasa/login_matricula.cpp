#include <iostream>
#include <fstream>
#include "login_matricula.h"
#include "headers.h"

fstream registroDeUsuáriosNovos;

int tamanhoArquivo();

int cadastro (int u){
    registroDeUsuáriosNovos.open("Cadastro.txt", ios :: in | ios :: out | ios::binary | ios:: app);

    usuario.id = tamanhoArquivo(registroDeUsuáriosNovos);

    cout<< "Nome: " << endl;
    cin.getline(usuario.nome, 30);
    cout<< "Email: " << endl;
    cin.getline(usuario.email, 100);
    cout<< "Função: " << endl;
    cin.getline(usuario.categoria);
    cout<< "Senha: " << endl;
    cin.getline(usuario.senha, 30);

    registroDeUsuáriosNovos.write((char*)&usuario,sizeof(usuario));

    int idNovo = usuario.id;

    registroDeUsuáriosNovos.close();

    return idNovo;
}

int login(int u){
    bool verificaRetorno;
    cout<< "ID: "<< endl;
    cin >> usuario.id;
    cin.ignore();
    cout<< "Senha: " << endl;
    cin.getline(usuario.senha, 30);

    if(verificarUsuarioExistente(usuario.id)){
        //entra em alguma função ou só
    }
    else{
        cout<< "Usuário não existente!" << endl;
    }
}

bool verificarUsuarioExistente(int id){
    registroDeUsuáriosNovos.open("Cadastro.txt", ios:: in | ios:: binary );
    registroDeUsuáriosNovos.seekg((id - 1) * sizeof(usuario));

    usuario verificaId;

    if (registroDeUsuáriosNovos.read((char *)&verificaId, sizeof(usuario))){
        registroDeUsuáriosNovos.close();
        return verificaId.id == id;
    }
    registroDeUsuáriosNovos.close();
    return false;
}

int tamanhoArquivo(fstream &arquivo) {
    return arquivo.tellp()/sizeof(usuario);
}

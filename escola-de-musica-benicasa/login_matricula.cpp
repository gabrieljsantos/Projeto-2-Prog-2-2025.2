#include <iostream>
#include <fstream>
#include <cstring>
#include "login_matricula.h"
#include "headers.h"

//Feito por Luiz Felipe
const char* ARQUIVO_USUARIOS = "usuarios.txt";

namespace Login_mat {

//Feito por Luiz Felipe
void inicializarArquivo() {
    Usuario usuarioVazio = inicializarUsuarioVazio();

    ofstream outUsuarios;
    outUsuarios.open(ARQUIVO_USUARIOS, ios::out | ios::binary);

    if (outUsuarios.fail()) {
        cout << "A abertura do arquivo falhou.\n";
        exit(1);
    }

    for (int i = 0; i < 100; i++)
        outUsuarios.write((const char *)(&usuarioVazio), sizeof(Usuario));

    outUsuarios.close();
}

//Feito por Jeanderson
int realizarMatricula(Usuario novoUsuario){
    fstream registroDeUsuariosNovos;
    registroDeUsuariosNovos.open(ARQUIVO_USUARIOS, ios :: in | ios :: out | ios::binary | ios:: app);

    usuario.id = tamanhoArquivo(registroDeUsuariosNovos);

    cout<< "Nome: " << endl;
    cin.getline(usuario.nome, 30);

    //Feito por Jhones
    cout<< "Nome: " << endl;
    cin.getline(usuario.nome, 30);

    if (verificarUsuarioExistente(usuario.nome)) {
        cout << "Nome ja cadastrado!\n";
        registroDeUsuariosNovos.close();
        return -1; // código de erro
    }

    cout<< "Email: " << endl;
    cin.getline(usuario.email, 100);
    cout<< "Função: " << endl;
    cin.getline(usuario.categoria);
    cout<< "Senha: " << endl;
    cin.getline(usuario.senha, 30);

    registroDeUsuariosNovos.write((char*)&usuario,sizeof(usuario));

    int idNovo = usuario.id;

    registroDeUsuariosNovos.close();

    return idNovo;
}

bool realizarLogin(int id, const char senha[30], Usuario &usuario){
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

//Feito por Luiz Felipe
Usuario lerUsuario(int id) {
    Usuario usuario = inicializarUsuarioVazio();

    if (id > verificarUltimoId()) {
        cout << "Usuário inexistente!" << endl;
        return usuario;
    }

    ifstream inUsuarios;
    inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

    if (inUsuarios.fail()) {
        cout << "A abertura do arquivo falhou.\n";
        exit(1);
    }

    inUsuarios.seekg((id - 1) * sizeof(Usuario));
    inUsuarios.read((char*)(&usuario), sizeof(Usuario));
    inUsuarios.close();

    return usuario;
}

//Feito por Luiz Felipe
int verificarUltimoId() {
    int ultimoId = 0;
    Usuario usuario;
    ifstream inUsuarios;
    inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

    if (inUsuarios.fail()) {
        cout << "A abertura do arquivo falhou.\n";
        exit(1);
    }

    while (inUsuarios.read((char*)(&usuario), sizeof(Usuario)))
        if (usuario.id > ultimoId)
            ultimoId = usuario.id;

    inUsuarios.close();

    return ultimoId;
}

//Feito por Jhones
bool verificarUsuarioExistente(const char nome[30]) {
    ifstream inUsuarios;
    inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

    if (inUsuarios.fail()) {
        cout << "A abertura do arquivo falhou.\n";
        return false;
    }

    Usuario u;
    while (inUsuarios.read((char*)(&u), sizeof(Usuario))) {
        // ignora registros vazios
        if (u.id != 0) {
            if (strncmp(u.nome, nome, 30) == 0) {
                inUsuarios.close();
                return true;
            }
        }
    }

    inUsuarios.close();
    return false;
}

//Feito por Jeanderson
bool verificarUsuarioExistente(int id){
    fstream registroDeUsuariosNovos;
    registroDeUsuariosNovos.open(ARQUIVO_USUARIOS, ios:: in | ios:: binary );
    registroDeUsuariosNovos.seekg((id - 1) * sizeof(Usuario));

    Usuario verificaId;

    if (registroDeUsuariosNovos.read((char *)&verificaId, sizeof(Usuario))){
        registroDeUsuariosNovos.close();
        return verificaId.id == id;
    }
    registroDeUsuariosNovos.close();
    return false;
}

//Feito por Jeanderson
int tamanhoArquivo(fstream &arquivo) {
    return arquivo.tellp()/sizeof(Usuario);
}

//Feito por Luiz Felipe
Usuario inicializarUsuarioVazio() {
    Usuario usuarioVazio;

    usuarioVazio.id = 0;
    usuarioVazio.ativo = true;

    usuarioVazio.nome[0] = '\0';
    usuarioVazio.email[0] = '\0';
    usuarioVazio.senha[0] = '\0';
    usuarioVazio.especialidade[0] = '\0';

    usuarioVazio.categoria = NENHUMA;
    usuarioVazio.saldo = 0.0;
    usuarioVazio.logado = false;

    return usuarioVazio;
}

}

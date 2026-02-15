#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H
#include <string>

#include "headers.h"

using namespace std;

namespace Login_mat {

    void inicializarArquivo();

    int realizarMatricula(Usuario novoUsuario);
    bool realizarLogin(int id, const char senha[30], Usuario &usuario);
    int verificarUltimoId();
    Usuario lerUsuario(int id);
    bool verificarUsuarioExistente(int id);
    bool verificarUsuarioExistente(const char nome[100]);
    void atualizarUsuario(int id, Usuario usuarioAtualizado);
    int tamanhoArquivo(fstream &arquivo);
    Usuario inicializarUsuarioVazio();
}

#endif

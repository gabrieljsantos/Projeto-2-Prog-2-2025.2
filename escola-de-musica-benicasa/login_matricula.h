#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H

#include "headers.h"

namespace login {
    void inicializarArquivo();
}

int cadastro(usuario u);
int verificarUltimoId();
usuario lerUsuario(int id);
int login(usuario u);
bool verificarUsuarioExistente(int id);
bool verificarUsuarioExistente(char[100] nome);
void atualizarUsuario(int id, usuario u);



#endif

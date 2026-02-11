#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H
#include <string>

#include "headers.h"

using namespace std;

namespace login {
    void inicializarArquivo();
}

void realizarMatricula();
int verificarUltimoId();
usuario lerUsuario(int id);
void realizarLogin(int id, string nome, TipoUsuario tipoUsuario );
bool verificarUsuarioExistente(int id);
bool verificarUsuarioExistente(string nome);
void atualizarUsuario(int id, usuario u);



#endif

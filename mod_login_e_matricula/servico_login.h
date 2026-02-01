#ifndef SERVICO_LOGIN_H
#define SERVICO_LOGIN_H

#include "../headers.h"

// Exibe a janela de login e retorna as informações do usuário autenticado
login_info janela_login();

// Valida login por ID
login_info validar_login(const int* id_usuario, const char* senha);

// Valida login de forma dinâmica (por ID ou Email)
// tipo_login: 1 = ID, 2 = Email
login_info validar_login_dinamico(int tipo_login, const char* credencial, const char* senha);

#endif

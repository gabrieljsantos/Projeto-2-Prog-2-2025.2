#ifndef MODULO_ADMIN_H
#define MODULO_ADMIN_H
#include <iostream>
#include <fstream>
#include "headers.h"
#include <limits>
#include <cstring>

    void pausar();
    void limparbuffer();
    void openFile(std::fstream &f, const std::string Nome);
    Professor buscaProf(std::fstream &file,int buscaId);
    Disciplina buscaDisciplina(std::fstream &file, int buscaId);
    void consultarPendenciasInstrumentos();

void exibirMenuAdministrador();
void abrir_menu_admin(Usuario* usuario);

namespace Modulo_admin {
    int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, std::string dados[100][6]);
    void menuCadastroUsuarios();
    void menuGerenciarUsuarios();
    void menuCadastroCursos();
    void consultarRelatoriosAcademicos();
    void menuEventos();
    void menuCadastroInstrumentos();
    void gerarRelatorioFinanceiro();
    void gerarRelatorioPatrimonial();
    void realizarBackup();
    void restaurarBackup();
}
#endif

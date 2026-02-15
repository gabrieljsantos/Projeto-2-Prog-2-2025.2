#ifndef MODULO_ADMIN_H
#define MODULO_ADMIN_H
#include <iostream>
#include <fstream>
#include "headers.h"
#include <limits>

    void pausar();
    void limparbuffer();
    void openFile(std::fstream &f, const std::string Nome);

namespace Modulo_admin {
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

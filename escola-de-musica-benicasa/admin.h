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

namespace mod_ADM {
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

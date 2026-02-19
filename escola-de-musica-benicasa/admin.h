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
    Aluno buscaAluno(std::fstream &file, int buscaId);
    Instrumento buscarInstrumento(std::fstream &file, int buscaId);
    Produto buscaProduto(std::fstream &file, int buscaId);
    int gerarNovoId(std::fstream &file, size_t tamanhoStruct);
    void consultarPendenciasInstrumentos();

void exibirMenuAdministrador();
void abrir_menu_admin(Usuario* usuario);

                                                    
namespace mod_ADM {
    int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, std::string dados[100][6]);
    int listar_disciplinas_especificas(int ativo, std::string dados[100][6]);
    int listar_eventos_especificos(int autorizado, std::string dados[100][5]);
    int listar_instrumentos_especificos(int autorizado, std::string dados[100][6]);
    void menuCadastroUsuarios();
    void menuGerenciarUsuarios();
    void menuCadastroCursos();
    void consultarRelatoriosAcademicos();
    void menuEventos(std::fstream &file);
    void menuCadastroInstrumentos();
    void gerarRelatorioFinanceiro();
    void gerarRelatorioPatrimonial();
    void realizarBackup();
    void restaurarBackup();
    void cadastrarProdutos();
    bool verificaTurmasProf(Professor &prof, int &Index_turma);
    void cadastrarTurma();
    void matricularAlunoTurma();
}
#endif

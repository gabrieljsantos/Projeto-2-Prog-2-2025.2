#ifndef MODULO_ADMIN_H
#define MODULO_ADMIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <limits>
#include "headers.h"

constexpr size_t TAMANHO_BUFFER = 256;

// ===== CONSTANTES DE ID BASE =====
constexpr int USUARIO_VAZIO_ID = 20260000;           // ID para usuários desalocados/vazios
constexpr int INDICE_ALUNO_BASE = 20260000;          // ID base para alunos (20260001+)
constexpr int INDICE_PROFESSOR_BASE = 20260000;      // ID base para professores (20260001+)
constexpr int INDICE_ADMINISTRADOR_BASE = 20260000;  // ID base para administradores (20260001+)

// ===== CONSTANTES DE FILTRO =====
constexpr int FILTRO_INATIVO = 0;
constexpr int FILTRO_ATIVO = 1;
constexpr int FILTRO_AMBOS = 2;

// ===== CONSTANTES DE AUTORIZAÇÃO =====
constexpr int NAO_AUTORIZADO = 0;
constexpr int AUTORIZADO = 1;

void limparbuffer();
void pausar();
void openFile(std::fstream &f, const std::string Nome);

Professor buscaProf(std::fstream &file, int buscaId);
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
    int listar_disciplinas_avancado(int ativo, int comProfessor, std::string dados[100][6]);
    int listar_eventos_especificos(int autorizado, std::string dados[100][5]);
    int listar_instrumentos_especificos(int autorizado, std::string dados[100][6]);
    
    void menuCadastroUsuarios();
    void menuGerenciarUsuarios();
    void menuCadastroCursos();
    void consultarRelatoriosAcademicos();
    void menuEventos(std::fstream &file);
    void menuCadastroInstrumentos();
    
    // Funções de gerenciamento de disciplinas
    void gerenciar_disciplina_menu(int idDisciplina);
    void vincular_professor_disciplina(int idDisciplina, const std::string &nomeDisciplina);
    void vincular_professor_por_tabela(int idDisciplina, const std::string &nomeDisciplina);
    void vincular_professor_por_id(int idDisciplina, const std::string &nomeDisciplina);
    void remover_vinculo_professor_disciplina(const std::string &nomeDisciplina);
    
    void gerarRelatorioFinanceiro();
    void gerarRelatorioPatrimonial();
    
    void realizarBackup();
    void restaurarBackup();
    
    bool verificaTurmasProf(Professor &prof, int &Index_turma);
    void cadastrarTurma();
    void matricularAlunoTurma();
    
    // Menus reorganizados
    void menuGestaoAcademica();
    void menuConsultarPendencias();
    void menuGestaoLanchonete();
    void menuRelatorios();
    void menuBackup();
}

#endif


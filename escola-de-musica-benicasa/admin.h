#ifndef MODULO_ADMIN_H
#define MODULO_ADMIN_H
#include <iostream>
#include <fstream>
#include "headers.h"
#include <cstring>
#include <limits>

namespace mod_ADM{
        //prontos
        void pausar();
        void limparbuffer();
        void openFile(std::fstream &file, const std::string Nome);
        int gerarNovoId(std::fstream &file, size_t tamanhoStruct);
        Disciplina buscaDisciplina(std::fstream &file, int buscaId );
        Professor buscaProf(std::fstream &file,int buscaId);
        Aluno buscaAluno(std::fstream &file,int buscaId);
        Produto buscaProduto(std::fstream &file,int buscaId);
        Instrumento buscarInstrumento(std::fstream &file, int buscaId );
        bool verificaTurmasProf(Professor &prof,int &Index_turma);
        void menuEventos();
        // void menuGerenciarUsuarios();//refazer
        void menuCadastroCursos();
        void cadastrarProdutos();
        void adicionarCredito();
        void consultarPendenciasInstrumentos();
        void menuInstrumentos();
        void cadastrarTurma();
        void matricularAlunoTurma();

        //falta
        void menuRelatorios();
        void relatorioAcademico();
        void relatorioFinanceiro();
        void relatorioPatrimonial();
    }
#endif 

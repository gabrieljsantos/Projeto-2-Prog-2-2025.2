#ifndef ALUNO_H
#define ALUNO_H

#include "../include/headers.h"

namespace Modulo_aluno {

    Aluno carregarAluno(int idAluno);
    void salvarAluno(const Aluno &aluno);
    void limparBuffer();

    void consultarNotas(int idAluno);
    void consultarMedias(int idAluno);
    void consultarSituacaoAcademica(int idAluno);

    void visualizarEventosDisponiveis();
    void inscreverEmEvento(int idAluno);
    void consultarMinhasInscricoes(int idAluno);

    void visualizarInstrumentosDisponiveis();
    void solicitarEmprestimo(int idAluno);
    void consultarMeusEmprestimos(int idAluno);
    void realizarDevolucao(int idAluno);

    void visualizarProdutos();
    void comprarProduto(int idAluno);
    void consultarSaldo(int idAluno);

    void solicitarCreditos(int idAluno);
}

void abrir_menu_aluno(Usuario* usuario);

#endif

#ifndef ALUNO_H
#define ALUNO_H

#include "headers.h"

namespace Modulo_aluno {
    // ===== FUNÇÕES INTERNAS =====
    Aluno carregarAluno(int idAluno);
    void salvarAluno(const Aluno &aluno);
    void limparBuffer();

    // ===== CONSULTAS ACADÊMICAS =====
    void consultarNotas(int idAluno);
    void consultarMedias(int idAluno);
    void consultarSituacaoAcademica(int idAluno);

    // ===== EVENTOS =====
    void visualizarEventosDisponiveis();
    void inscreverEmEvento(int idAluno);
    void consultarMinhasInscricoes(int idAluno);

    // ===== INSTRUMENTOS =====
    void visualizarInstrumentosDisponiveis();
    void solicitarEmprestimo(int idAluno);
    void consultarMeusEmprestimos(int idAluno);
    void realizarDevolucao(int idAluno);

    // ===== LANCHONETE =====
    void visualizarProdutos();
    void comprarProduto(int idAluno);
    void consultarSaldo(int idAluno);

    // ===== CRÉDITOS =====
    void solicitarCreditos(int idAluno);
}

// ===== PONTO DE ENTRADA DO MENU ALUNO =====
void abrir_menu_aluno(Usuario* usuario);

#endif

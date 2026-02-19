#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <iostream>
#include "headers.h"

#define MAX_TURMAS 30
#define MAX_ALUNOS 20
#define MAX_TURMAS_PROF 5
#define MEDIA_MINIMA 7.0
#define FALTAS_MAXIMAS 10
#define MAX_AVALIACOES 10

// ============================
// Estruturas Base
// ============================

struct Avaliacao
{
    char data[12];        // dd/mm/aaaa
    char descricao[100];  // assunto da avaliacao
};

struct Turma
{
    int id;
    char nome[100];
    int id_prof;

    Aluno alunos[MAX_ALUNOS];

    // NOVO BLOCO
    Avaliacao avaliacoes[MAX_AVALIACOES];
    int qtdAvaliacoes;
};

// ============================
// Namespace de Cálculos
// ============================

namespace Calculos
{
    float somaRecursivaNotas(float notas[], int n);
    float calcularMedia(float notas[], int n);
}

// ============================
// Funções Auxiliares
// ============================

bool turmaPermitida(int id_prof, int turma_index);

// ============================
// Módulo Professor
// ============================

namespace ModuloProfessor
{
    void consultarSaldoLanchonete(int id_prof);
    void consultarSaldoLanchonete(Professor &prof);

    void registrarNotas(int id_prof);
    void registrarNotasTodo(int id_prof);

    void consultarAlunosMatriculados(int id_prof);
    void consultarDesempenhoAcademico(int id_prof);
    void consultarTurma(int id_prof); // ← ADICIONADA

    void registrarFaltas(int id_prof);
    void registrarAvaliacao(int id_prof);
}

#endif

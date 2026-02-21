#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <iostream>
#include "headers.h"

#define MAX_TURMAS 30
#define MAX_ALUNOS 20
#define MAX_TURMAS_PROF 5
#define MEDIA_MINIMA 7.0
#define FALTAS_MAXIMAS 10
#define MAX_AVALIACOES 10 // Adicionado para suportar a estrutura de Avaliacao

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
    // Sobrecargas de Registro de Notas
    void registrarNotas(int id_prof);                               // Via interface (Original)
    void registrarNotasTodo(int id_prof);                           // Turma toda (Original)
    void registrarNotas(int id_prof, int turma_index, int id_aluno); // Aluno específico (Nova sobrecarga)

    void consultarAlunosMatriculados(int id_prof);
    void consultarTurma(int id_prof); 
    void consultarDesempenhoAcademico(int id_prof);

    void registrarFaltas(int id_prof);
    void registrarAvaliacao(int id_prof);

    // Seletores graficos
    int selecionarTurma(int id_prof);
    int selecionarAluno(Turma &turma);
}

void abrir_menu_professor(Usuario* usuario);

#endif
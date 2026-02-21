#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <iostream>
#include "../include/headers.h"

#define MAX_TURMAS 30
#define MAX_ALUNOS 20
#define MAX_TURMAS_PROF 5
#define MEDIA_MINIMA 7.0
#define FALTAS_MAXIMAS 10
#define MAX_AVALIACOES 10

namespace Calculos
{
    float somaRecursivaNotas(float notas[], int n);
    float calcularMedia(float notas[], int n);
}

bool turmaPermitida(int id_prof, int turma_index);

namespace ModuloProfessor
{

    void registrarNotas(int id_prof);
    void registrarNotasTodo(int id_prof);
    void registrarNotas(int id_prof, int turma_index, int id_aluno);

    void consultarAlunosMatriculados(int id_prof);
    void consultarTurma(int id_prof); 
    void consultarDesempenhoAcademico(int id_prof);

    void registrarFaltas(int id_prof);
    void registrarAvaliacao(int id_prof);

    int selecionarTurma(int id_prof);
    int selecionarAluno(Turma &turma);
}

void abrir_menu_professor(Usuario* usuario);

#endif
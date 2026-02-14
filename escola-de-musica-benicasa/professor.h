#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <string>

namespace Professor {
    void registrarNotas(int idProfessor, float notas[], int n);
    void registrarAvaliacoes(int idProfessor);
    void consultarAlunosMatriculados(int idProfessor);
    void calcularMediasTurma(int idProfessor);
    void consultarDesempenhoAcademico(int idProfessor);
    void consultarEventosDisponiveis();
    void consultarInstrumentosDisponiveis();
}

#endif

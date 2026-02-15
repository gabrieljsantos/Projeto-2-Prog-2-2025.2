#ifndef MODULO_PROFESSOR_H
#define MODULO_PROFESSOR_H

#include <string>

namespace Modulo_professor {
    void registrarNotas(int idProfessor, float notas[], int n);
    void registrarAvaliacoes(int idProfessor);
    void consultarAlunosMatriculados(int idProfessor);
    void calcularMediasTurma(int idProfessor);
    void consultarDesempenhoAcademico(int idProfessor);
    void consultarEventosDisponiveis();
    void consultarInstrumentosDisponiveis();
}

#endif

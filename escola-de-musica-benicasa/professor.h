#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <string>

// Funções chamadas no switch(PROFESSOR) da sua main
void registrarNotas(Usuario u, float notas[], int n);
void registrarAvaliacoes(int idProfessor); //observação 
void consultarAlunosMatriculados(int idProfessor); //parte do cadastro
float calcularMediasTurma(Usuario u);
void consultarDesempenhoAcademico(int idProfessor); //Função de acesso de aléatorio
void consultarEventosDisponiveis(); //discutir sobre o modulos de eventos
void consultarInstrumentosDisponiveis(); //tipo as materias //modulo de instrumentos
void consultarSaldoLanchonete(int idProfessor); //saldo
void consultarFaltas(); //Função para faltas (arquivo.faltas)
void consultarRecados(); //Função para criar recados
 


#endif

#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H

#include <iostream>
#include "headers.h"
#include "login_matricula.h"

#define TAM_NOME 30

void inicializarInstrumentos();
void finalizarInstrumentos();

void cadastrarInstrumento();
void listarInstrumentos();
void excluirInstrumento();

void salvarEmprestimo(Emprestimo emp);
void realizarEmprestimo();
void realizarEmprestimo(int idAluno);
void realizarDevolucao();
void realizarDevolucao(int idAluno);
void listarInstrumentosDisponiveis();
void listarMeusEmprestimos(int idAluno);

void editarInstrumento();

void menuInstrumentos();

#endif
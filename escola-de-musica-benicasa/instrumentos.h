#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H

#include <iostream>
#include "headers.h"
#include "login_matricula.h"
// Inclui o header do módulo de login para acessar as structs e funções do aluno

// Constantes
#define TAM_NOME 30

// --- Funções de Sistema ---
void inicializarInstrumentos(); // Carrega dados do arquivo
void finalizarInstrumentos();   // Salva dados no arquivo

// --- Funções de CRUD ---
void cadastrarInstrumento();
void listarInstrumentos();
void excluirInstrumento();

// --- Funções de Empréstimo ---
void salvarEmprestimo(Emprestimo emp);
void realizarEmprestimo();
void realizarDevolucao();

// --- Edição ---
void editarInstrumento();

// --- Menu Principal ---
void menuInstrumentos();

#endif
#ifndef ENTRADA_ALUNO_H
#define ENTRADA_ALUNO_H

#include "../headers.h"
#include <iomanip>

/*
 * ??  GAMBIARRA TEMPORÁRIA - STRUCTS DEFINIDAS AQUI (não devem estar aqui)
 * 
 * PROBLEMA: aluno.cpp usava struct Aluno e Emprestimo
 * SOLUÇÃO TEMPORÁRIA: Definir as structs aqui para compilar
 * 
 * TODO - REMOVER ESTAS STRUCTS QUANDO USAR headers.h CORRETAMENTE:
 * Substituir por:
 *   - Identidade (id, nome, cpf, categoria)
 *   - RegistroNotas (id_aluno, id_turma, notas[], qtd_notas)
 *   - Saldo_Cantina (id_aluno, saldo_cantina, qtd_compras)
 *   - Atraso_Instrumento (id_aluno, id_instrumento, dias_atraso)
 */

struct Aluno 
{
    int id;
    char nome[50];
    float notas[4];
    
    // Lanchonete
    float credito;
    char compras[20][50];
    int quantCompras;
    
    // Instrumentos aprendidos
    char instrumentos[10][30];
    int quantInstrumentos;
};

struct Emprestimo 
{
    int idAluno;
    int idInstrumento;
    bool devolvido;
};

// Exibe a janela do painel do aluno
void janela_aluno(login_info info);

// Funções de visualização (Recicladas de aluno.cpp)
void mostrarNotas(const Aluno &a);
float calcularMedia(const Aluno &a);
void mostrarExtratoLanchonete(const Aluno &a);
void mostrarHistoricoEmprestimos(Emprestimo emp[], int totalEmp, int idAluno);
void mostrarInstrumentosAprendidos(const Aluno &a);

#endif


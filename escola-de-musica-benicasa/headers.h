#ifndef HEADERS_H
#define HEADERS_H

typedef enum {
    ALUNO,
    PROFESSOR,
    ADMINISTRADOR
} funcao;

typedef struct {
    int id;                       // ID único no sistema
    int ativo;                    // 1 = Ativo/Matriculado, 0 = Inativo/Deletado
    char nome[100];               // Nome completo
    char email[100];              // Email de contato
    char senha[30];               // Senha de login
    funcao categoria;             // Categoria da pessoa (Aluno, Professor, etc.)
    char especialidade[50];
} usuario;

#endif

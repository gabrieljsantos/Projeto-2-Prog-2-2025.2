#ifndef HEADERS_H
#define HEADERS_H

typedef enum {
    ALUNO,
    PROFESSOR,
    ADMINISTRADOR
} funcao;

struct Usuario{
    int id;                       
    int ativo;                    
    char nome[100];               
    char email[100];              
    char senha[30];                      
    char especialidade[50];
    funcao categoria;
    float saldo;

   Usuario() : id(0), ativo(0), categoria(ALUNO){
        nome[0] = '\0';
        email[0] = '\0';
        senha[0] = '\0';
        especialidade[0] = '\0';
    }    
};
struct Produto {
    int id;
    int ativo;
    char nome[50];
    float preco
    int estoque;
};
struct Curso {
    int id;
    int ativo;
    char nome[50];
    int cargaHoraria;
    int vagas;
};

struct Instrumento {
    int id;
    int ativo;          // 1 = ativo, 0 = removido
    int autorizado;     // 0 = não autorizado, 1 = autorizado
    char nome[30];
    int disponivel;     // 1 = disponível, 0 = indisponível
};

struct Emprestimo {
    int idEmprestimo;        // Identificador único do empréstimo
    
    int idAluno;             // ID do aluno que realizou o empréstimo
    int idInstrumento;       // ID do instrumento emprestado

    char dataEmprestimo[11]; // Data do empréstimo (dd/mm/aaaa)
    char dataPrevista[11];   // Data prevista para devolução

};

#endif

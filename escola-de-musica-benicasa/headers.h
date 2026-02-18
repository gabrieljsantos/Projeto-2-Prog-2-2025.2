#ifndef HEADERS_H
#define HEADERS_H

enum Funcao {
    NENHUMA = -1,
    ALUNO,
    PROFESSOR,
    ADMINISTRADOR
};

struct Usuario{
    int id;
    bool ativo;
    char nome[100];
    char email[100];
    char senha[30];
    Funcao categoria;
    bool logado;
};

struct Aluno {
    Usuario base;
    double saldo;
    int notas[2];
    int faltas;
    int instrumento;
    int turmasId[10];
};

struct Professor {
    Usuario base;
    double saldo;
    char disciplina[50];
    int turmasId[5];
};

struct Admin {
    Usuario base;
};

struct Disciplina {
    int id;
    int ativo;
    char nome[50];
    int cargaHoraria;
    int idProfessor;
    int vagas;
};
struct Produto {
    char nome[50];
    int id;
    int ativo;
    double preco;
    int estoque;
};
struct Instrumento {
    int id;
    int ativo;          // 1 = ativo, 0 = removido
    int autorizado;     // 0 = não autorizado, 1 = autorizado
    char nome[30];
    bool disponivel;     // 1 = disponível, 0 = indisponível
    int estoque;
};

struct Emprestimo {
    int idEmprestimo;        // Identificador único do empréstimo
    int idAluno;             // ID do aluno que realizou o empréstimo
    char nome_Alu[100];

    int idInstrumento;       // ID do instrumento emprestado
    char nome_In[15];

    char dataEmprestimo[11]; // Data do empréstimo (dd/mm/aaaa)
    char dataPrevista[11];   // Data prevista para devolução

};
struct Evento {
    int id = 0;            // Identificador único
    int ativo = 0 ;               // 1 = ativo no sistema, 0 = removido

    char nome[50] ="";           // Nome do evento
    char descricao[100]="";     // Descrição
    char data[11]="";           // Data (dd/mm/aaaa)
    char local[50]="";          // Local do evento

    int totalVagas=0;          // Número total de vagas
    int vagasOcupadas=0;       // Quantidade já inscrita

    int autorizado=0;          // 0 = não autorizado | 1 = autorizado
};

struct Credito {
    int id_opera = 0;
    int id_user = 0;
    double saldo = 0;
    bool realizado = false;
};

#endif

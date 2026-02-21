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
    float notas[2];
    float media;
    int faltas;
    int turmasId[10];
    int turma;
    int eventos[20];
    int qtdEventos;
    int instrumentos[10];
    int qtdInstrumentos;
    int idInstrumento;
};

struct Professor {
    Usuario base;
    double saldo;
    char disciplina[50];
    int turmas[5];
};

struct Admin {
    Usuario base;
};

#define MAX_ALUNOS 20
struct Turma {
    int id;
    int ativo;
    int idDisciplina;
    int idProfessor;
    int alunos[MAX_ALUNOS];
    int qtdAlunos;
};
struct Disciplina {
    int id;
    int ativo;
    char nome[50];
    int cargaHoraria;
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
    int autorizado;     // 0 = nao autorizado, 1 = autorizado
    char nome[30];
    bool disponivel;    // true = disponivel, false = emprestado
    int estoque;
    int responsavel;
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
    Funcao tipo_user;
    double saldo = 0;
    bool realizado = false;
};

void limparBuffer();

#endif

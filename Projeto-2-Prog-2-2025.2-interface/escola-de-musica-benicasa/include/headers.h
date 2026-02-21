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
    float notas[2];
    float media;
    int faltas;
    int instrumento;
    int idInstrumento;
    int turma;
};

struct Professor {
    Usuario base;
    char disciplina[50];
    int turmas[5];
};

struct Admin {
    Usuario base;
};

#define MAX_ALUNOS 20
#define MAX_AVALIACOES 10

struct Avaliacao
{
    char data[12];
    char descricao[100];
};

struct Turma {
    int id;
    int ativo;
    int idDisciplina;
    int idProfessor;
    char nome[100];
    int id_prof;
    Aluno alunos[MAX_ALUNOS];
    int qtdAlunos;
    Avaliacao avaliacoes[MAX_AVALIACOES];
    int qtdAvaliacoes;
};

struct Disciplina {
    int id;
    int ativo;
    char nome[50];
    int cargoHoraria;
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
    int ativo;
    int autorizado;
    char nome[30];
    bool disponivel;
    int estoque;
    int idAluno;
};

struct Emprestimo {
    int idEmprestimo;
    int idAluno;
    char nome_Alu[100];

    int idInstrumento;
    char nome_In[15];

    char dataEmprestimo[11];
    char dataPrevista[11];

};

struct Horario {
    int hora;
    int minuto;
};

struct Data {
    int dia;
    int mes;
    int ano;
};

struct Evento {

    int id = 0;            
    int ativo = 0 ;               
    int autorizado = 0;          

    char nome[100] = "";           
    char descricao[200] = "";     
    char local[100] = "";          
    char professorResponsavel[100] = "";
    

    Data data;           
    Horario horario;

    int totalVagas = 0;
    int vagasOcupadas = 0;
    int vagasDisponiveis = 0;
    int totalinscritos = 0;
    

    bool finalizado = false;
    char alunos[100][100];
};

struct Credito {
    int id_opera = 0;
    int id_user = 0;
    double saldo = 0; 
    bool realizado = false;
};

#endif
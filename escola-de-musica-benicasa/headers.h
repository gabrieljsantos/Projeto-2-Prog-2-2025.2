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
    char data[12];        // dd/mm/aaaa
    char descricao[100];  // assunto da avaliacao
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
    int autorizado;     // 0 = não autorizado, 1 = autorizado
    char nome[30];
    bool disponivel;    // 1 = disponível, 0 = indisponível
    int estoque;        // Quantidade física disponível
    int idAluno;        // ID do aluno que pegou o instrumento emprestado
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

// --- ESTRUTURAS DO NOVO MÓDULO DE EVENTOS ---
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
    // Identificadores e status (Mantidos do sistema base)
    int id = 0;            
    int ativo = 0 ;               
    int autorizado = 0;          

    // Informações gerais (Tamanhos expandidos para comportar o novo módulo)
    char nome[100] = "";           
    char descricao[200] = "";     
    char local[100] = "";          
    char professorResponsavel[100] = "";
    
    // Data e Hora (Substituído o char data[11] pela nova struct Data)
    Data data;           
    Horario horario;

    // Vagas e Inscrições (Unificando as lógicas antigas e novas)
    int totalVagas = 0;          // Antigo: Número total de vagas criadas
    int vagasOcupadas = 0;       // Antigo: Quantidade já inscrita
    int vagasDisponiveis = 0;    // Novo: Vagas restantes
    int totalinscritos = 0;      // Novo: Controle exato de alunos inscritos
    
    // Controle de andamento e lista de alunos
    bool finalizado = false;
    char alunos[100][100];       // Matriz com os nomes dos inscritos
};
// --------------------------------------------

struct Credito {
    int id_opera = 0;
    int id_user = 0;
    double saldo = 0; 
    bool realizado = false;
};

#endif

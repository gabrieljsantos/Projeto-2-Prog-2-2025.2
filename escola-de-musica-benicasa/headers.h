#ifndef HEADERS_H
#define HEADERS_H

#include <string>

typedef enum {
    ALUNO,
    PROFESSOR,
    ADMINISTRADOR
} funcao;

struct Usuario {
    int id;                       
    int ativo;                    
    char nome[100];               
    char email[100];              
    char senha[30];                      
    char especialidade[50];
    funcao categoria;
    double saldo;

   Usuario() : id(0), ativo(0), categoria(ALUNO){
        nome[0] = '\0';
        email[0] = '\0';
        senha[0] = '\0';
        especialidade[0] = '\0';
    }    
};
struct Produto {
    char nome[50];
    int id;
    int ativo;
    double preco;
    int estoque;

    Produto() : id(0), ativo(0), preco(0.0), estoque(0) {
        nome[0] = '\0';
    }
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
    int status=0;              
    // 0 = inativo
    // 1 = ativo
    // 2 = encerrado
    // 3 = cancelado
};

struct Credito {
    int id_opera = 0;
    int id_user = 0;
    double saldo = 0; 
    bool realizado = false
};


struct UsuarioLogado {
    int id;
    std::string nome; //NÂO SEI EXPLICAR O PORQUE DE USAR STRING AQUI, MAS VAI QUE PRECISA DE UM NOME COM ESPAÇO, ENTÃO VAI STRING
    std::string email; // MESMA RAZÃO DO NOME, VAI QUE PRECISA DE UM EMAIL COM ESPAÇO, ENTÃO VAI STRING
    bool logado;
    funcao perfil;
};

#endif


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
#endif

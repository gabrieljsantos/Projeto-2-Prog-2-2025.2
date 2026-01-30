#ifndef SERVICES_H
#define SERVICES_H

#include <stdio.h>

typedef enum {
    ROLE_ALUNO,
    ROLE_PROFESSOR,
    ROLE_ADMIN,
    ROLE_VENDEDOR,
    ROLE_LOCADOR
} person_role;

typedef struct {
    int id;
    int ativo;
    char nome[100];
    char cpf[15];
    char senha[30];
    person_role category;
} identidade;

// -------- services --------

void register_person(
    identidade *person
);

#endif

#ifndef EVENTOS_H
#define EVENTOS_H

#include <vector>
#include "headers.h" // Importa as estruturas centralizadas

// Variáveis globais compartilhadas (a definição real fica no .cpp)
extern std::vector<Evento> eventos;
extern int proximoID;

// Declaração das Funções do módulo
bool SalvarEventosNoArquivo(const std::vector<Evento>& eventos);
void CarregarEventos();
void adicionar_evento();
void editar_evento(std::vector<Evento>& eventos);
void excluir_evento(int id_excluir);
void listar_alunos_inscritos();
void listar_eventos(const std::vector<Evento>& eventos);
void inscrever_alunos();

#endif // EVENTOS_H

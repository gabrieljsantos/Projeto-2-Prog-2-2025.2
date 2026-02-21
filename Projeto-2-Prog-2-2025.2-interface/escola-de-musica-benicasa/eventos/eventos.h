#ifndef EVENTOS_H
#define EVENTOS_H

#include <vector>
#include "../include/headers.h"

using namespace std;

extern const char* ARQUIVO_EVENTOS;

extern vector<Evento> eventos;
extern int proximoID;

bool SalvarEventosNoArquivo(const vector<Evento>& eventos);
void CarregarEventos();
void adicionar_evento();
void editar_evento(vector<Evento>& eventos);
void excluir_evento(int id_excluir);
void finalizar_evento();
void listar_alunos_inscritos();
void listar_eventos(const vector<Evento>& eventos);
void inscrever_alunos();
void menu_eventos();

#endif

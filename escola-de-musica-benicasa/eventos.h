#ifndef EVENTOS_H
#define EVENTOS_H

#include <vector>
#include "headers.h"

using namespace std;

// ─── Constante do Arquivo ═════════════════════════════════════
extern const char* ARQUIVO_EVENTOS;

// ─── Variáveis Globais ════════════════════════════════════════
extern vector<Evento> eventos;
extern int proximoID;

// ─── Declaração das Funções ═══════════════════════════════════
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

#endif // EVENTOS_H

#ifndef SERVICO_CADASTROS_H
#define SERVICO_CADASTROS_H

#include "../headers.h"

// Função para inicializar o banco de dados com usuário ROOT
// Se o arquivo não existir, cria e adiciona usuário USUARIO_ROOT (ADMIN)
void inicializar_banco_dados();

// Função para gerenciar cadastro e modificação de usuários no banco de dados binário
// categoria: tipo de usuário (ALUNO, PROFESSOR, ADMIN, VENDEDOR, LOCADOR)
// Retorna true se bem-sucedido, false caso contrário
bool gerenciar_cadastro_usuario(funcao_pessoa categoria);

// Função auxiliar para salvar uma identidade no arquivo .dat
bool salvar_identidade(const char* nome_arquivo, const Identidade& usuario);

// Função auxiliar para listar usuários de um tipo específico
void listar_usuarios(const char* nome_arquivo);

// Função auxiliar para obter o nome do arquivo .dat baseado na categoria
void obter_caminho_arquivo(funcao_pessoa categoria, char* caminho);

#endif

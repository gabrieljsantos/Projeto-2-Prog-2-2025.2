#ifndef INICIALIZADORES_H
#define INICIALIZADORES_H

#include "../headers.h"

// ========== INICIALIZADORES DE STRUCTS ==========

// Identidade
Identidade criar_identidade_padrao();
Identidade criar_identidade_usuario_root();

// Login
login_identidadade criar_login_identidade();

// Turma
Turma criar_turma_padrao();

// Historico
Historico_Individual criar_historico_padrao();

// Recursos
Recursos_Individuais criar_recursos_padrao();

// Produtos da Cantina
Produto_Cantina criar_produto_cantina_padrao();

// Inventário de Instrumentos
Inventario_Instrumentos criar_inventario_instrumentos_padrao();

// Registro de Notas
RegistroNotas criar_registro_notas_padrao();

// Eventos
EventoAgenda criar_evento_padrao();

// Vendas
Venda_Cantina criar_venda_cantina_padrao();

// Resumo de Vendedor
Resumo_Vendedor criar_resumo_vendedor_padrao();

// Saldo da Cantina
Saldo_Cantina criar_saldo_cantina_padrao();

// Atraso de Instrumento
Atraso_Instrumento criar_atraso_instrumento_padrao();

// ========== INICIALIZADORES DE BANCOS DE DADOS ==========

// Inicializa o arquivo binário de usuários (usuarios.dat)
void inicializar_banco_dados_usuarios();

// Inicializa o arquivo binário de recursos individuais (recursos_individuais.dat)
void inicializar_banco_dados_recursos();

// Inicializa o arquivo binário de turmas (turmas.dat)
void inicializar_banco_dados_turmas();

// Inicializa o arquivo binário de históricos (historicos.dat)
void inicializar_banco_dados_historicos();

// Inicializa o arquivo binário de produtos da cantina (produtos_cantina.dat)
void inicializar_banco_dados_produtos_cantina();

// Inicializa o arquivo binário de inventário de instrumentos (inventario_instrumentos.dat)
void inicializar_banco_dados_inventario_instrumentos();

// Inicializa o arquivo binário de registros de notas (registros_notas.dat)
void inicializar_banco_dados_registros_notas();

// Inicializa o arquivo binário de eventos (eventos.dat)
void inicializar_banco_dados_eventos();

// Inicializa o arquivo binário de vendas da cantina (vendas_cantina.dat)
void inicializar_banco_dados_vendas_cantina();

// Inicializa o arquivo binário de resumos de vendedor (resumos_vendedor.dat)
void inicializar_banco_dados_resumos_vendedor();

// Inicializa o arquivo binário de saldos da cantina (saldos_cantina.dat)
void inicializar_banco_dados_saldos_cantina();

// Inicializa o arquivo binário de atrasos de instrumentos (atrasos_instrumentos.dat)
void inicializar_banco_dados_atrasos_instrumentos();

// Inicializa todos os bancos de dados
void inicializar_todos_bancos_dados();

#endif

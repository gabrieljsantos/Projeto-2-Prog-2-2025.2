#ifndef INTERFACE_GRAFICA_H
#define INTERFACE_GRAFICA_H

#include <string>

using namespace std;

struct saida_menu
{
    int    indice_da_opcao;
    string opcao;
};

struct saida_tabela
{
    int    indice_linha;
    string valor_retorno;
    int    indice_coluna;
};

struct CoresMenu
{
    int cor_titulo            = 5;
    int cor_descricao         = 3;
    int cor_instrucao         = 4;
    int cor_caminho           = 6;
    int cor_opcao             = 2;
    int cor_opcao_selecionada = 1;
    int cor_borda             = 7;
    int cor_paginacao         = 8;
    int cor_controles         = 9;
};

struct ConfigMenu
{
    string titulo              = "";
    string descricao           = "";
    string instrucao           = "";
    string caminho             = "";
    string instrucao_controles = "[Setas] Navegar  [ENTER] Selecionar  [ESC] Voltar";

    int opcaoAtual        = 0;
    int x                 = 4;
    int largura_janela    = 60;
    int opcoes_por_janela = 5;

    CoresMenu cores;
};

struct ConfigTabela
{
    string titulo              = "";
    string descricao           = "";
    string instrucao           = "";
    string caminho             = "";
    string instrucao_controles = "[Setas] Navegar  [ENTER] Selecionar  [ESC] Voltar";

    int linhaSelecionada = 0;
    int colunaSelecionada = 0;
    int x                = 4;
    int largura_janela   = 80;
    int linhas_por_janela = 5;
    int max_colunas      = 5;

    // Larguras de cada coluna (em caracteres)
    int* larguras_colunas = nullptr;
    int  num_colunas      = 0;

    CoresMenu cores;
};

saida_menu interface_para_menu(
    int              total_opcoes,
    string           opcoes[],
    const ConfigMenu &config = ConfigMenu()
);

saida_tabela interface_para_tabela(
    int                      linhas,
    int                      colunas,
    const string**           matriz,
    const string             titulos[],
    int                      coluna_retorno,
    const ConfigTabela&      config
);

struct saida_configurador
{
    bool confirmado;
    string* valores_selecionados;
    int numero_variaveis;
};

struct ConfigConfigurador
{
    string titulo              = "";
    string descricao           = "";
    string instrucao           = "";
    string caminho             = "";
    string instrucao_controles = "[CIMA/BAIXO] Variáveis  [ESQ/DIR] Valores  [ENTER] OK  [ESC] Cancelar";

    int x                 = 4;
    int largura_janela    = 80;
    int max_variaveis     = 20;

    CoresMenu cores;
};

saida_configurador interface_para_configurador(
    int                           numero_variaveis,
    const string                  nomes_variaveis[],
    const string**                opcoes_variaveis,
    const int*                    num_opcoes_por_variavel,
    const ConfigConfigurador&     config = ConfigConfigurador()
);

// ═══════════════════════════════════════════════════════════════
// ENTRADA DE TEXTO
// ═══════════════════════════════════════════════════════════════

enum TipoEntrada
{
    TIPO_TEXTO,
    TIPO_NUMERO,
    TIPO_EMAIL,
    TIPO_TELEFONE,
    TIPO_SENHA
};

struct saida_entrada_texto
{
    bool confirmado;
    string valor;
};

struct ConfigEntradaTexto
{
    string titulo              = "";
    string descricao           = "";
    string label               = "Digite: ";
    string caminho             = "";
    string instrucao_controles = "[ENTER] Confirmar  [ESC] Cancelar";

    TipoEntrada tipo_entrada   = TIPO_TEXTO;
    int x                      = 4;
    int largura_janela         = 70;
    int tamanho_maximo         = 100;
    string valor_inicial       = "";

    CoresMenu cores;
};

saida_entrada_texto interface_para_entrada_texto(
    const ConfigEntradaTexto& config = ConfigEntradaTexto()
);

// ═══════════════════════════════════════════════════════════════
// BOTÕES COM RETORNO
// ═══════════════════════════════════════════════════════════════

struct BotaoOpcao
{
    char tecla;           // Tecla a pressionar (ex: 'F', 'T', 'N')
    string label;         // Rótulo do botão (ex: "Sim", "Não")
    int valor_retorno;    // Valor retornado (ex: 1, 0, -1)
};

struct saida_botoes
{
    bool confirmado;
    int valor_retorno;
    char tecla_pressionada;
};

struct ConfigBotoes
{
    string titulo              = "";
    string descricao           = "";
    string pergunta            = "Escolha uma opção: ";
    string caminho             = "";
    string instrucao_controles = "[Tecla] Selecionar  [ESC] Cancelar";

    BotaoOpcao botoes[4];      // Máximo 4 botões
    int numero_botoes          = 0;

    int x                      = 4;
    int largura_janela         = 70;

    CoresMenu cores;
};

saida_botoes interface_para_botoes(
    const ConfigBotoes& config
);

void mostrar_caixa_informacao(const string& titulo, const string& mensagem);

#endif


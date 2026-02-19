# Documentação da Interface Gráfica

## Índice
1. [Visão Geral](#visão-geral)
2. [Arquitetura](#arquitetura)
3. [Estruturas de Dados](#estruturas-de-dados)
4. [Componentes Principais](#componentes-principais)
5. [Sistema de Cores](#sistema-de-cores)
6. [Funções Públicas](#funções-públicas)
7. [Exemplos de Uso](#exemplos-de-uso)
8. [Dependências](#dependências)
9. [Referência Rápida de Funções](#referência-rápida-de-funções)

---

## Visão Geral

A interface gráfica é um sistema modular e personalizável implementado em C++ usando a biblioteca **ncurses**. Ela fornece componentes visuais interativos para a aplicação Escola de Música Benicasa, permitindo a criação de menus, tabelas, formulários de entrada de texto e caixas de diálogo.

### Características Principais:
- ✅ Interface baseada em terminal com suporte a cores
- ✅ Navegação com teclado (setas, ENTER, ESC)
- ✅ Componentes reutilizáveis e configuráveis
- ✅ Sistema de paginação para listas grandes
- ✅ Paleta de cores customizada (marfim sobre preto)
- ✅ Suporte a word wrap para textos longos
- ✅ Feedback visual com sombreados sutis

---

## Arquitetura

### Estrutura de Arquivos:
```
interface_grafica.h          → Declarações e estruturas
interface_grafica.cpp        → Implementação com ~1933 linhas
```

### Dependências Internas:
- `strings` (manipulação de texto)
- `curses.h` (NCurses - biblioteca gráfica para terminal)
- `algorithm` (funções utilitárias)
- `cmath` (cálculos matemáticos)
- `locale` (suporte a UTF-8)

---

## Estruturas de Dados

### 1. **Retorno de Menu**
```cpp
struct saida_menu
{
    int    indice_da_opcao;    // Índice da opção selecionada (0-based)
    string opcao;              // Texto da opção selecionada
};
```

### 2. **Retorno de Tabela**
```cpp
struct saida_tabela
{
    int    indice_linha;       // Linha selecionada
    string valor_retorno;      // Valor da célula na coluna de retorno
    int    indice_coluna;      // Coluna selecionada
};
```

### 3. **Configuração de Cores**
```cpp
struct CoresMenu
{
    int cor_titulo            = 5;  // Cor do título
    int cor_descricao         = 3;  // Cor da descrição
    int cor_instrucao         = 4;  // Cor das instruções
    int cor_caminho           = 6;  // Cor do caminho (breadcrumb)
    int cor_opcao             = 2;  // Cor das opções normais
    int cor_opcao_selecionada = 1;  // Cor da opção selecionada
    int cor_borda             = 7;  // Cor das bordas
    int cor_paginacao         = 8;  // Cor dos indicadores de paginação
    int cor_controles         = 9;  // Cor dos controles/instruções
};
```

### 4. **Configuração de Menu**
```cpp
struct ConfigMenu
{
    string titulo              = "";     // Título exibido acima da janela
    string descricao           = "";     // Descrição da funcionalidade
    string instrucao           = "";     // Instruções customizadas
    string caminho             = "";     // Caminho/breadcrumb do menu
    string instrucao_controles = "[Setas] Navegar  [ENTER] Selecionar  [ESC] Voltar";

    int opcaoAtual        = 0;     // Opção inicialmente selecionada
    int x                 = 4;     // Posição X de indentação
    int largura_janela    = 60;    // Largura da janela (caracteres)
    int opcoes_por_janela = 5;     // Número de opções por página

    CoresMenu cores;               // Paleta de cores personalizada
};
```

### 5. **Configuração de Tabela**
```cpp
struct ConfigTabela
{
    string titulo              = "";
    string descricao           = "";
    string instrucao           = "";
    string caminho             = "";
    string instrucao_controles = "[Setas] Navegar  [ENTER] Selecionar  [ESC] Voltar";

    int linhaSelecionada = 0;      // Linha inicialmente selecionada
    int colunaSelecionada = 0;     // Coluna inicialmente selecionada
    int x                = 4;      // Posição X de indentação
    int largura_janela   = 80;     // Largura da janela
    int linhas_por_janela = 5;     // Linhas visíveis por página
    int max_colunas      = 5;      // Máximo de colunas

    int* larguras_colunas = nullptr;  // Array com largura de cada coluna
    int  num_colunas      = 0;        // Número de colunas customizadas

    CoresMenu cores;
};
```

### 6. **Retorno de Configurador**
```cpp
struct saida_configurador
{
    bool confirmado;               // Se foi confirmado (true) ou cancelado (false)
    string* valores_selecionados;  // Array com valores selecionados
    int numero_variaveis;          // Número de variáveis configuradas
};
```

### 7. **Configuração de Entrada de Texto**
```cpp
struct ConfigEntradaTexto
{
    string titulo              = "";
    string descricao           = "";
    string label               = "Digite: ";
    string caminho             = "";
    string instrucao_controles = "[ENTER] Confirmar  [ESC] Cancelar";

    TipoEntrada tipo_entrada   = TIPO_TEXTO;    // Ver enum abaixo
    int x                      = 4;
    int largura_janela         = 70;
    int tamanho_maximo         = 100;           // Max de caracteres
    string valor_inicial       = "";

    CoresMenu cores;
};

enum TipoEntrada
{
    TIPO_TEXTO,       // Texto livre
    TIPO_NUMERO,      // Números apenas
    TIPO_EMAIL,       // Validação de email
    TIPO_TELEFONE,    // Formato telefônico
    TIPO_SENHA        // Oculta caracteres digitados
};
```

### 8. **Estrutura de Botão**
```cpp
struct BotaoOpcao
{
    char tecla;        // Tecla a pressionar (ex: 'F', 'T', 'N')
    string label;      // Rótulo do botão (ex: "Sim", "Não")
    int valor_retorno; // Valor retornado quando pressionado
};
```

### 9. **Retorno de Botões**
```cpp
struct saida_botoes
{
    bool confirmado;           // Se foi confirmado ou cancelado
    int valor_retorno;         // Valor associado ao botão pressionado
    char tecla_pressionada;    // Qual tecla foi pressionada
};
```

### 10. **Configuração de Botões**
```cpp
struct ConfigBotoes
{
    string titulo              = "";
    string descricao           = "";
    string pergunta            = "Escolha uma opção: ";
    string caminho             = "";
    string instrucao_controles = "[Tecla] Selecionar  [ESC] Cancelar";

    BotaoOpcao botoes[10];     // Array com até 10 botões
    int numero_botoes          = 1;

    int x                      = 4;
    int largura_janela         = 70;

    CoresMenu cores;
};
```

### 11. **Tópico para Detalhes**
```cpp
struct TopicDetalhes
{
    string titulo;      // Título do tópico
    string descricao;   // Descrição/conteúdo do tópico
};
```

### 12. **Retorno de Detalhes**
```cpp
struct saida_detalhes
{
    bool confirmado;    // true se ENTER foi pressionado, false se ESC/Q
    int pagina_atual;   // Página em que estava quando confirmou
};
```

### 13. **Configuração de Detalhes**
```cpp
struct ConfigDetalhes
{
    string titulo              = "";
    string descricao           = "";
    string caminho             = "";
    string instrucao_controles = "[Setas] Navegar  [ENTER] Confirmar  [ESC] Voltar";

    int x                      = 4;
    int largura_janela         = 80;
    int topicos_por_pagina     = 5;    // Quantos tópicos por página

    CoresMenu cores;
};
```

### 14. **Configuração de Configurador** (Seleção Múltipla)
```cpp
struct ConfigConfigurador
{
    string titulo              = "";
    string descricao           = "";
    string instrucao           = "";
    string caminho             = "";
    string instrucao_controles = "[CIMA/BAIXO] Variáveis  [ESQ/DIR] Valores  [ENTER] OK  [ESC] Cancelar";

    int x                 = 4;
    int largura_janela    = 80;
    int max_variaveis     = 20;    // Máximo de variáveis

    CoresMenu cores;
};
```

---

## Componentes Principais

### Componentes Implementados:

| Componente | Função | Uso |
|-----------|--------|-----|
| **Menu** | Seleção de uma entre várias opções | Navegação principal, seleções |
| **Tabela** | Visualização e seleção em dados tabulares | Listagem de alunos, professores, etc |
| **Detalhes** | Exibição de tópicos com descrições detalhadas | Informações, tutoriais, manuais |
| **Configurador** | Seleção múltipla com opções por variável | Configurações complexas |
| **Entrada de Texto** | Preenchimento de dados de texto/número | Formulários, cadastros |
| **Botões** | Seleção binária ou múltipla com hotkeys | Confirmações, diálogos |
| **Caixa de Informação** | Exibição de mensagens simples | Notificações, avisos |

---

## Sistema de Cores

### Paleta Implementada: "Marfim sobre Preto"

A interface utiliza uma paleta sofisticada de cores customizadas para criar um visual elegante com baixíssimo contraste:

#### Cores Customizadas (Indices 16-23):
| Índice | Cor | RGB (NCurses) | Propósito |
|--------|-----|----------------|----------|
| 16 | Preto | 59, 59, 59 | Fundo geral |
| 17 | Marfim Principal | 600, 600, 520 | Texto normal |
| 18 | Marfim Escuro | 380, 380, 320 | Bordas, paginação |
| 19 | Sombreado Título | 102, 98, 86 | Fundo do título |
| 20 | Sombreado Seleção | 133, 129, 110 | Fundo selecionado |
| 21 | Marfim Claro | 680, 680, 600 | Título (primário) |
| 22 | Marfim Mais Claro | 720, 720, 640 | Texto selecionado |
| 23 | Marfim Muito Apagado | 300, 300, 260 | Instruções, controles |

#### Pares de Cores (Combinações):
| Pair | Cor Texto | Cor Fundo | Uso |
|------|-----------|-----------|-----|
| 1 | Marfim Claro (22) | Sombreado (20) | Opção selecionada |
| 2 | Marfim (17) | Preto (16) | Opções normais |
| 3 | Marfim (17) | Preto (16) | Descrição |
| 4 | Marfim Apagado (23) | Preto (16) | Instrução |
| 5 | Marfim Claro (21) | Sombreado (19) | Título |
| 6 | Marfim Escuro (18) | Preto (16) | Caminho |
| 7 | Marfim Escuro (18) | Preto (16) | Bordas |
| 8 | Marfim Escuro (18) | Preto (16) | Paginação |
| 9 | Marfim Apagado (23) | Preto (16) | Controles |

### Filosofia de Design:
- **Minimalista**: Foco no conteúdo sem distrações
- **Acessível**: Alto contraste para legibilidade
- **Elegante**: Sombreados sutis para profundidade
- **Consistente**: Paleta unificada em toda a interface

---

## Funções Públicas

### 1. **interface_para_menu()**
Exibe um menu interativo com lista de opções.

```cpp
saida_menu interface_para_menu(
    int              total_opcoes,           // Número de opções
    string           opcoes[],               // Array com textos das opções
    const ConfigMenu &config = ConfigMenu()  // Configuração (opcional)
);
```

**Retorno**: `saida_menu`
- `indice_da_opcao`: Índice da opção selecionada (-1 se cancelado)
- `opcao`: Texto da opção selecionada

**Controles**:
- `↑ ↓`: Navegar por opções
- `PgUp/PgDn`: Pular página
- `← →`: Navegar por páginas
- `ENTER`: Selecionar opção
- `ESC`: Cancelar (retorna -1)

**Exemplo**:
```cpp
string opcoes[] = {"Alunos", "Professores", "Eventos", "Sair"};
ConfigMenu config;
config.titulo = "Menu Principal";
config.descricao = "Escolha uma opção";
saida_menu resultado = interface_para_menu(4, opcoes, config);

if (resultado.indice_da_opcao != -1) {
    cout << "Selecionou: " << resultado.opcao << endl;
}
```

---

### 2. **interface_para_tabela()**
Exibe uma tabela com navegação e seleção.

```cpp
saida_tabela interface_para_tabela(
    int                 linhas,            // Número de linhas totais
    int                 colunas,           // Número de colunas
    const string**      matriz,            // Dados (array de strings)
    const string        titulos[],         // Títulos das colunas
    int                 coluna_retorno,    // Qual coluna retornar
    const ConfigTabela& config             // Configuração
);
```

**Retorno**: `saida_tabela`
- `indice_linha`: Linha selecionada
- `valor_retorno`: Conteúdo da célula na coluna especificada
- `indice_coluna`: Coluna selecionada

**Controles**:
- `↑ ↓`: Navegar por linhas
- `← →`: Navegar por colunas
- `PgUp/PgDn`: Pular linhas
- `ENTER`: Selecionar linha
- `ESC`: Cancelar

**Exemplo**:
```cpp
string exemplo[][3] = {
    {"João Silva", "Piano", "Ativo"},
    {"Maria Santos", "Violão", "Ativo"},
    {"Pedro Costa", "Flauta", "Inativo"}
};

string titulos[] = {"Nome", "Instrumento", "Status"};
const string** matriz_ptr = (const string**)exemplo;

ConfigTabela config;
config.titulo = "Alunos Cadastrados";
config.linhas_por_janela = 5;
config.largura_janela = 80;

saida_tabela resultado = interface_para_tabela(
    3, 3, matriz_ptr, titulos, 0, config
);
```

---

### 3. **interface_para_entrada_texto()**
Captura entrada de texto com validação opcional.

```cpp
saida_entrada_texto interface_para_entrada_texto(
    const ConfigEntradaTexto& config = ConfigEntradaTexto()
);
```

**Retorno**: `saida_entrada_texto`
- `confirmado`: true se ENTER foi pressionado, false se ESC
- `valor`: Texto digitado

**Tipos de entrada**:
- `TIPO_TEXTO`: Aceita qualquer caractere
- `TIPO_NUMERO`: Apenas dígitos
- `TIPO_EMAIL`: Validação de email
- `TIPO_TELEFONE`: Formato (XX) XXXXX-XXXX
- `TIPO_SENHA`: Oculta caracteres com asteriscos

**Controles**:
- `ENTER`: Confirmar entrada
- `ESC`: Cancelar
- `Backspace`: Deletar caractere anterior
- `Delete`: Deletar caractere atual

**Exemplo**:
```cpp
ConfigEntradaTexto config;
config.titulo = "Cadastro de Aluno";
config.label = "Nome completo: ";
config.tipo_entrada = TIPO_TEXTO;
config.tamanho_maximo = 100;

saida_entrada_texto resultado = interface_para_entrada_texto(config);

if (resultado.confirmado) {
    cout << "Nome digitado: " << resultado.valor << endl;
}
```

---

### 4. **interface_para_botoes()**
Exibe botões com hotkeys para seleção rápida.

```cpp
saida_botoes interface_para_botoes(
    const ConfigBotoes& config
);
```

**Retorno**: `saida_botoes`
- `confirmado`: true se botão foi pressionado
- `valor_retorno`: Valor associado ao botão
- `tecla_pressionada`: Caractere da tecla

**Exemplo**:
```cpp
ConfigBotoes config;
config.titulo = "Confirmação";
config.pergunta = "Deseja realmente deletar este registro?";

config.botoes[0] = {'S', "Sim", 1};
config.botoes[1] = {'N', "Não", 0};
config.numero_botoes = 2;

saida_botoes resultado = interface_para_botoes(config);

if (resultado.confirmado && resultado.valor_retorno == 1) {
    cout << "Deletando..." << endl;
}
```

---

### 5. **interface_para_configurador()**
Permite seleção múltipla com várias opções por variável.

```cpp
saida_configurador interface_para_configurador(
    int                        numero_variaveis,      // Qtd de variáveis
    const string               nomes_variaveis[],     // Nomes das variáveis
    const string**             opcoes_variaveis,      // Opções por variável
    const int*                 num_opcoes_por_variavel, // Qtd opções cada
    const ConfigConfigurador&  config = ConfigConfigurador()
);
```

**Retorno**: `saida_configurador`
- `confirmado`: true se confirmado
- `valores_selecionados`: Array com seleções
- `numero_variaveis`: Número total de variáveis

**Controles**:
- `↑ ↓`: Navegar entre variáveis
- `← →`: Navegar entre opções
- `ENTER`: Confirmar
- `ESC`: Cancelar

---

### 6. **mostrar_caixa_informacao()**
Exibe uma caixa de mensagem simples.

```cpp
void mostrar_caixa_informacao(
    const string& titulo,      // Título da caixa
    const string& mensagem     // Mensagem a exibir
);
```

**Controles**:
- `ENTER`: Fechar caixa

**Exemplo**:
```cpp
mostrar_caixa_informacao("Sucesso", "Aluno cadastrado com êxito!");
```

---

### 7. **mostrar_detalhes()**
Exibe tópicos com descrições detalhadas e paginação, com word wrap automático.

```cpp
saida_detalhes mostrar_detalhes(
    const TopicDetalhes topicos[],      // Array de tópicos
    int numero_topicos,                 // Quantidade de tópicos
    const ConfigDetalhes& config = ConfigDetalhes()  // Configuração
);
```

**Retorno**: `saida_detalhes`
- `confirmado`: true se ENTER foi pressionado, false se ESC/Q
- `pagina_atual`: Página em que o usuário confirmou

**Controles**:
- `↑ ↓ ou K/J`: Navegar entre tópicos
- `← → ou H/L`: Navegar entre páginas
- `ENTER`: Confirmar
- `ESC ou Q`: Cancelar

**Características**:
- Word wrap automático nas descrições
- Expandir descrição do tópico selecionado
- Paginação para múltiplos tópicos
- Navegação por vim-like keys (hjkl) + setas

**Exemplo**:
```cpp
TopicDetalhes topicos[] = {
    {"Cadastro de Alunos", "Nesta seção você pode cadastrar novos alunos, editar dados existentes e visualizar o histórico."},
    {"Gestão de Professores", "Aqui é possível gerenciar os dados dos professores, suas especialidades e horários."},
    {"Eventos", "Calendário de eventos e apresentações da escola."}
};

ConfigDetalhes config;
config.titulo = "Manual da Aplicação";
config.descricao = "Consulte os tópicos abaixo para obter ajuda";
config.topicos_por_pagina = 5;

saida_detalhes resultado = mostrar_detalhes(topicos, 3, config);

if (resultado.confirmado) {
    cout << "Confirmado até a página " << resultado.pagina_atual << endl;
}
```

---

---

### 8. **interface_para_botoes()**
Exibe botões com hotkeys para seleção rápida.

```cpp
saida_botoes interface_para_botoes(
    const ConfigBotoes& config
);
```

**Retorno**: `saida_botoes`
- `confirmado`: true se botão foi pressionado
- `valor_retorno`: Valor associado ao botão
- `tecla_pressionada`: Caractere da tecla

**Exemplo**:
```cpp
ConfigBotoes config;
config.titulo = "Confirmação";
config.pergunta = "Deseja realmente deletar este registro?";

config.botoes[0] = {'S', "Sim", 1};
config.botoes[1] = {'N', "Não", 0};
config.numero_botoes = 2;

saida_botoes resultado = interface_para_botoes(config);

if (resultado.confirmado && resultado.valor_retorno == 1) {
    cout << "Deletando..." << endl;
}
```

---

## Exemplos de Uso

### Exemplo 1: Menu Simples
```cpp
#include "interface_grafica.h"

int main() {
    string opcoes[] = {"Opção 1", "Opção 2", "Opção 3", "Sair"};
    
    ConfigMenu config;
    config.titulo = "Menu Teste";
    config.descricao = "Sistema de Escola de Música";
    config.opcoes_por_janela = 5;
    
    saida_menu resultado = interface_para_menu(4, opcoes, config);
    
    if (resultado.indice_da_opcao != -1) {
        cout << "Você selecionou: " << resultado.opcao << endl;
    }
    
    return 0;
}
```

### Exemplo 2: Tabela com Dados
```cpp
// Preparar dados
string dados[][2] = {
    {"João", "Piano"},
    {"Maria", "Violão"},
    {"Pedro", "Flauta"}
};
const string** matriz = (const string**)dados;

string titulos[] = {"Nome", "Instrumento"};

// Configurar
ConfigTabela config;
config.titulo = "Alunos";
config.larguras_colunas = new int[]{20, 20};
config.num_colunas = 2;

// Exibir
saida_tabela resultado = interface_para_tabela(3, 2, matriz, titulos, 0, config);

cout << "Selecionado: " << resultado.valor_retorno << endl;

delete[] config.larguras_colunas;
```

### Exemplo 3: Formulário de Entrada
```cpp
ConfigEntradaTexto config;
config.titulo = "Novo Aluno";
config.label = "Nome: ";
config.tipo_entrada = TIPO_TEXTO;

saida_entrada_texto resultado = interface_para_entrada_texto(config);

if (resultado.confirmado) {
    cout << "Nome: " << resultado.valor << endl;
}
```

### Exemplo 4: Tópicos com Detalhes
```cpp
TopicDetalhes topicos[] = {
    {"Bem-vindo", "Bem-vindo ao sistema de Escola de Música Benicasa. Este aplicativo gerencia alunos, professores, instrumentos e eventos."},
    {"Cadastrar Aluno", "Para cadastrar um novo aluno, acesse o menu principal e selecione 'Novo Aluno'. Preencha os dados solicitados."},
    {"Gerar Relatórios", "Você pode gerar relatórios de alunos, professores e eventos através da seção de relatórios."}
};

ConfigDetalhes config;
config.titulo = "Manual de Uso";
config.descricao = "Consulte os tópicos abaixo";
config.topicos_por_pagina = 5;

saida_detalhes resultado = mostrar_detalhes(topicos, 3, config);
```

---

## Referência Rápida de Funções

### Tabela de Todas as Funções Públicas

| Função | Parâmetros | Retorno | Propósito |
|--------|-----------|---------|----------|
| `interface_para_menu()` | total_opcoes, opcoes, config | `saida_menu` | Exibir menu interativo |
| `interface_para_tabela()` | linhas, colunas, matriz, titulos, coluna_retorno, config | `saida_tabela` | Exibir tabela com seleção |
| `interface_para_entrada_texto()` | config | `saida_entrada_texto` | Capturar entrada de texto |
| `interface_para_configurador()` | numero_variaveis, nomes, opcoes, num_opcoes, config | `saida_configurador` | Seleção múltipla |
| `interface_para_botoes()` | config | `saida_botoes` | Exibir botões com hotkeys |
| `mostrar_caixa_informacao()` | titulo, mensagem | void | Exibir mensagem simples |
| `mostrar_detalhes()` | topicos, numero_topicos, config | `saida_detalhes` | Exibir tópicos detalhados |

### Teclas Comuns em Todos os Componentes

| Tecla | Ação |
|-------|------|
| `ENTER` ou `↵` | Confirmar/Selecionar |
| `ESC` | Cancelar/Voltar |
| `↑ ↓` | Navegar para cima/baixo |
| `← →` | Navegar esquerda/direita ou pular página |
| `PgUp/PgDn` | Pular página (menus e tabelas) |

---

## Exemplo Completo: Fluxo Integrado

```cpp
#include "interface_grafica.h"
#include <iostream>

using namespace std;

int main() {
    while (true) {
        // 1. Menu Principal
        string opcoes_menu[] = {"Alunos", "Professores", "Sair"};
        ConfigMenu config_menu;
        config_menu.titulo = "Sistema de Gerenciamento";
        
        saida_menu menu_resultado = interface_para_menu(3, opcoes_menu, config_menu);
        
        if (menu_resultado.indice_da_opcao == 2 || menu_resultado.indice_da_opcao == -1) {
            break;
        }
        
        if (menu_resultado.indice_da_opcao == 0) {
            // 2. Submenu de Alunos
            string opcoes_alunos[] = {"Novo Aluno", "Listar Alunos", "Voltar"};
            ConfigMenu config_alunos;
            config_alunos.titulo = "Gerenciar Alunos";
            
            saida_menu aluno_resultado = interface_para_menu(3, opcoes_alunos, config_alunos);
            
            if (aluno_resultado.indice_da_opcao == 0) {
                // 3. Capturar nome do aluno
                ConfigEntradaTexto config_entrada;
                config_entrada.titulo = "Cadastrar Novo Aluno";
                config_entrada.label = "Nome do aluno: ";
                config_entrada.tipo_entrada = TIPO_TEXTO;
                
                saida_entrada_texto entrada_resultado = interface_para_entrada_texto(config_entrada);
                
                if (entrada_resultado.confirmado) {
                    // 4. Confirmar cadastro
                    ConfigBotoes config_botoes;
                    config_botoes.titulo = "Confirmação";
                    config_botoes.pergunta = "Confirmar cadastro de " + entrada_resultado.valor + "?";
                    config_botoes.botoes[0] = {'S', "Sim", 1};
                    config_botoes.botoes[1] = {'N', "Não", 0};
                    config_botoes.numero_botoes = 2;
                    
                    saida_botoes botoes_resultado = interface_para_botoes(config_botoes);
                    
                    if (botoes_resultado.valor_retorno == 1) {
                        mostrar_caixa_informacao("Sucesso", "Aluno cadastrado com êxito!");
                    }
                }
            }
        }
    }
    
    return 0;
}
```

---

## Funções Auxiliares Internas

Estas funções são internas e não devem ser usadas diretamente:

```cpp
// Desenhar linha horizontal com bordas
static void desenhar_linha_horizontal(WINDOW *win, int y, int largura, int cor_pair);

// Preencher linha com uma cor
static void preencher_linha(WINDOW *win, int y, int x_inicio, int largura, int cor_pair);

// Calcular posição centralizada
static int centro_x(int largura_janela, int tamanho_texto);

// Contar linhas necessárias com word wrap
static int contar_linhas_com_wrap(const string& texto, int largura_disponivel);

// Desenhar texto com quebra automática de linhas
static int desenhar_texto_wrap(WINDOW *win, int y, int x, const string& texto,
                               int largura_disponivel, int cor_pair);
```

---

## Dependências

### Bibliotecas Externas:
- **NCurses** (`curses.h`): Biblioteca para criar interfaces gráficas em terminal
  - Fornece: gerenciamento de cores, janelas, entrada de teclado
  - Instalação (Linux): `sudo apt-get install libncurses-dev`
  - Compilação: `-lncurses`

### Bibliotecas Padrão C++:
- `<iostream>`: Entrada/saída
- `<string>`: Manipulação de strings
- `<algorithm>`: Funções de algoritmo (min, max)
- `<cmath>`: Cálculos matemáticos (ceil)
- `<locale>`: Suporte a UTF-8 e localização

### Flags de Compilação Recomendadas:
```bash
g++ -std=c++11 -Wall -Wextra -lncurses -o aplicacao main.cpp interface_grafica.cpp
```

---

## Notas Importantes

### 1. Inicialização e Finalização NCurses
- A biblioteca ncurses é inicializada automaticamente em cada função
- `initscr()` → Inicia o modo curses
- `endwin()` → Finaliza o modo curses
- Sempre chamadas ao início e fim de cada componente

### 2. Alocação de Memória
- Arrays de configuração customizados (como `larguras_colunas`) devem ser alocados dinamicamente
- Use `new` para alocar e `delete` para liberar
- Exemplo:
```cpp
config.larguras_colunas = new int[3]{20, 30, 25};
// ... usar ...
delete[] config.larguras_colunas;
```

### 3. Validação de Entrada
- A função `interface_para_tabela()` valida dados antes de exibir
- Se dados inválidos forem passados, retorna -1 em `indice_linha`
- Sempre verificar o retorno antes de usar

### 4. Word Wrap
- Textos longos são quebrados automaticamente
- Quebra ocorre em espaços ou caracteres especiais
- Linhas são ajustadas conforme a largura da janela

### 5. Suporte a UTF-8
```cpp
setlocale(LC_ALL, "pt_BR.UTF-8");
```
- Necessário para caracteres especiais: ç, ã, é, etc.
- Já incluído nas funções da interface

---

## Troubleshooting

| Problema | Causa | Solução |
|----------|-------|---------|
| Cores não aparecem | Terminal sem suporte a cores | Usar terminal compatível (xterm, konsole, gnome-terminal) |
| Caracteres distorcidos | Encoding incorreto | Verificar localização com `locale -a` |
| Menu não responde | Teclas não configuradas | Verificar `keypad(stdscr, TRUE)` |
| Texto cortado | Janela muito pequena | Aumentar tamanho do terminal ou reduzir `largura_janela` |
| Erro de segmentação | Array de dados inválido | Verificar alocação e dimensões da matriz |

---

## Changelog

### Versão 1.0 (Atual)
- ✅ Menu com paginação
- ✅ Tabela com navegação horizontal/vertical
- ✅ Entrada de texto com validação
- ✅ Sistema de cores customizado
- ✅ Configurador de múltiplas variáveis
- ✅ Botões com hotkeys
- ✅ Word wrap automático

---

## Autor e Licença

Desenvolvido para o projeto **Escola de Música Benicasa**

Licença: Projeto Acadêmico

---

## Contato e Suporte

Para dúvidas ou sugestões sobre a interface gráfica:
- Revisar exemplos nos arquivos `.cpp` da aplicação
- Consultar a seção de Exemplos de Uso deste documento
- Verificar estruturas de configuração no arquivo `.h`

---

**Última Atualização**: Fevereiro de 2026

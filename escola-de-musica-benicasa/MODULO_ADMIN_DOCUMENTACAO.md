# Módulo Administrador - Documentação Completa

## Disciplina: Introdução à Programação II

**Desenvolvimento Acadêmico - 2026**

---

## 📌 Tópicos Acadêmicos Implementados

Este módulo demonstra a implementação dos principais tópicos da disciplina de Introdução à Programação II:

- ✅ **Funções e Procedimentos**: Definição, declaração e chamada em múltiplas variações
- ✅ **Procedimentos por Valor e Referência**: Parâmetros formais, reais e argumentos
- ✅ **Structs em C++**: Manipulação de estruturas de dados complexas
- ✅ **Arquivos**: I/O binário e sequencial com manipulação de dados persistentes
- ✅ **Ponteiros**: Passagem por referência, alocação dinâmica e aritmética de ponteiros
- ✅ **Strings**: Manipulação avançada de C-strings e std::string
- ✅ **Bibliotecas**: Organização modular com namespaces
- ✅ **Recursividade**: Padrões iterativos e recursivos

---

## 📋 Visão Geral do Módulo

### Responsabilidades Principais

O módulo administrador oferece uma visão global e administrativa do sistema de gestão da Escola de Música Benincasa.

#### O que o módulo faz:
- Cadastro de usuários (alunos, professores, administradores)
- Gestão completa de disciplinas e turmas
- Administração de eventos e instrumentos
- Controle de produtos da lanchonete
- Geração de relatórios consolidados (acadêmicos, financeiros, patrimoniais)
- Execução de rotinas de backup e restauração automática

#### O que o módulo NÃO faz:
- Executar tarefas acadêmicas próprias do professor
- Realizar compras ou inscrições como usuário comum
- Alterar dados operacionais de outros módulos diretamente

#### Permissões de Acesso
- **Administrador**: Acesso total
- **Professor**: Sem acesso direto
- **Aluno**: Sem acesso direto

---

## 🔧 Constantes Globais

```cpp
constexpr size_t TAMANHO_BUFFER = 256;            // Tamanho padrão de buffer
constexpr int INDICE_ALUNO_BASE = 2026000;         // ID base para alunos
constexpr int INDICE_PROFESSOR_BASE = 2026000;     // ID base para professores
```

**Tópico Acadêmico**: Variáveis e Constantes Globais

---

## 📚 Seção 1: Funções Utilitárias Básicas

### `void limparbuffer()`

**Tipo**: Procedimento (função void)
**Parâmetros**: Nenhum
**Retorno**: Void (sem retorno útil)

Limpa o buffer de entrada padrão após leitura de dados.

**Tópico Acadêmico**: 
- Funções e Procedimentos
- Escopo local de variáveis
- Uso de constantes (std::numeric_limits)

**Implementação**:
```cpp
std::cin.clear();                                              // Limpa estado de erro
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora até nova linha
```

---

### `void pausar()`

**Tipo**: Procedimento
**Parâmetros**: Nenhum
**Retorno**: Void

Pausa a execução até o usuário pressionar uma tecla.

**Tópico Acadêmico**: Procedimentos com efeito colateral

---

## 📁 Seção 2: Operações com Arquivos (E/S)

### `void openFile(std::fstream &f, const std::string Nome)`

**Tipo**: Procedimento
**Parâmetros**:
- `f` [Referência] - Objeto fstream a ser inicializado
- `Nome` [Valor] - Nome do arquivo a abrir

**Retorno**: Void

Abre um arquivo em modo binário para leitura e escrita simultânea.

**Tópicos Acadêmicos**:
- **Funções por Referência vs Valor**: `f` por referência permite modificação; `Nome` por valor para segurança
- **Arquivos - I/O Binário**: Abre em modo `ios::binary`
- **Arquivos - Modo Simultâneo**: `ios::in | ios::out` permite leitura e escrita
- **Tratamento de Erro**: Cria arquivo se não existir

**Fluxo de Funcionamento**:
1. Tenta abrir arquivo em modo binário read/write
2. Se falhar, cria novo arquivo
3. Reabre para modo simultâneo

---

## 🔍 Seção 3: Operações de Busca

### Padrão de Busca Direto

As funções de busca implementam um padrão de **busca direta por posição** (não sequencial).

**Fórmula de Cálculo**:
```
posição = (ID - BASE_INDEX) * sizeof(Estrutura)
```

**Vantagens**:
- O(1) - Acesso em tempo constante
- Muito mais rápido que busca sequencial
- Aproveita armazenamento binário indexado

---

### `Professor buscaProf(std::fstream &file, int buscaId)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo aberto com professores
- `buscaId` [Valor] - ID do professor procurado

**Retorno**: `Professor` (struct inicializada ou vazia)

Busca um PROFESSOR pelo ID no arquivo.

**Tópicos Acadêmicos**:
- **Structs em C++**: Retorno de estruturas
- **Aritmética de Ponteiros**: Cálculo de posição em arquivo
- **Funções por Referência**: Arquivo é modificado (seekg, read)
- **Validação de Intervalo**: Verificação de ID válido

---

### `Disciplina buscaDisciplina(std::fstream &file, int buscaId)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo com disciplinas
- `buscaId` [Valor] - ID da disciplina

**Retorno**: `Disciplina`

Similar a `buscaProf`, implementa reutilização de padrão de busca.

---

### `Aluno buscaAluno(std::fstream &file, int buscaId)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo com alunos
- `buscaId` [Valor] - ID do aluno

**Retorno**: `Aluno`

**Tópicos Acadêmicos Adicionais**:
- **Strings em C++**: Conversão de `char*` para `std::string`
- **Manipulação de Strings**: Concatenação com operador `+`

---

### `Instrumento buscarInstrumento(std::fstream &file, int buscaId)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo com instrumentos
- `buscaId` [Valor] - ID do instrumento

**Retorno**: `Instrumento`

---

### `Produto buscaProduto(std::fstream &file, int buscaId)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo com produtos
- `buscaId` [Valor] - ID do produto

**Retorno**: `Produto`

**Padrão de Erro**: Campo `id = 0` indica não encontrado

---

## 🆔 Seção 4: Geração de IDs Únicos

### `int gerarNovoId(std::fstream &file, size_t tamanhoStruct)`

**Tipo**: Função
**Parâmetros**:
- `file` [Referência] - Arquivo para determinar tamanho
- `tamanhoStruct` [Valor] - `sizeof()` da estrutura em bytes

**Retorno**: `int` - Próximo ID sequencial disponível

**Algoritmo**:
```
1. Busca fim do arquivo: seekg(0, ios::end)
2. Obtém posição em bytes: tellg()
3. Calcula registros: totalRegistros = posição / tamanhoStruct
4. Retorna: totalRegistros + 1
```

**Tópicos Acadêmicos**:
- **Aritmética de Ponteiros**: Conversão de bytes para índices
- **Operações com Arquivos**: Navegação e posicionamento
- **Cálculo de Offsets**: Conversão posição ↔ índice

---

## 📊 Seção 5: Consultas Especializadas

### `void consultarPendenciasInstrumentos()`

**Tipo**: Procedimento
**Parâmetros**: Nenhum
**Retorno**: Void

Consulta pendências de INSTRUMENTOS EMPRESTADOS por alunos.

**Funcionalidades**:
1. Listar todas as pendências
2. Filtrar por aluno específico

**Tópicos Acadêmicos**:
- **Vetores Dinâmicos / Matrizes**: Uso de arrays bidimensionais `dados[100][6]`
- **Múltiplos Arquivos**: Abre `emprestimos.dat` e `alunos.dat` simultaneamente
- **Manipulação de Strings**: Conversão de dados struct para string

**Fluxo de Dados**:
```
1. Menu de opções
2. Seleciona opção (listar todas ou filtrar)
3. Abre arquivo de empréstimos e alunos
4. Itera lendo registros
5. Monta tabela com dados relevantes
6. Exibe interface gráfica
```

---

## 🎓 Seção 6: Namespace mod_ADM

**Tópico Acadêmico**: Bibliotecas em C++ - Uso de Namespaces

O namespace `mod_ADM` (Módulo Administrativo) agrupa todas as funções especializadas.

**Benefícios**:
- Evita colisão de nomes entre módulos
- Agrupa funcionalidades por responsabilidade
- Facilita manutenção e organização
- Uso: `mod_ADM::funcao()`

---

### 6.1 - Listagens e Consultas

#### `int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, std::string dados[100][6])`

**Tipo**: Função
**Retorno**: `int` - Quantidade de usuários encontrados (0-100)

Filtra usuários por tipo e status.

**Parâmetros**:
- `tipo_usuario` [Valor] - ALUNO, PROFESSOR, ADMIN
- `ativo` [Valor] - 1=ativos, 0=inativos, 2=ambos
- `dados` [Referência] - Array 100x6 para resultado

**Tópicos Acadêmicos**:
- **Vetores e Matrizes**: Passagem de array como parâmetro
- **Switch/Case**: Filtragem por tipo
- **Conversão de Dados**: De struct para string

---

#### `int listar_disciplinas_especificas(int ativo, std::string dados[100][6])`

Lista disciplinas com filtro de status.

---

#### `int listar_eventos_especificos(int autorizado, std::string dados[100][5])`

Lista eventos com filtro de autorização.

---

#### `int listar_instrumentos_especificos(int autorizado, std::string dados[100][6])`

Lista instrumentos com filtro de autorização.

---

### 6.2 - Menus de Cadastro

#### `void menuCadastroUsuarios()`

**Tipo**: Procedimento

Menu interativo para cadastro de novos usuários (alunos e professores).

**Opções**:
1. Cadastrar Aluno
2. Cadastrar Professor
3. Voltar

**Fluxo**:
- Coleta dados via interface gráfica
- Valida campos (email, nome, etc)
- Gera novo ID único
- Escreve struct no arquivo binário
- Confirma com mensagem de sucesso

---

#### `void menuGerenciarUsuarios()`

Menu para gerenciar usuários existentes.

**Operações**:
- Editar usuário
- Deletar (desativar)
- Reativar
- Filtros avançados

**Tópico Acadêmico**: Manipulação de registros binários

---

#### `void menuCadastroCursos()`

Menu para cadastro e gerenciamento de CURSOS (Disciplinas).

**Operações**:
1. Criar nova disciplina
2. Editar disciplina existente
3. Listar disciplinas
4. Autorizar/desautorizar disciplinas
5. Vincular professor à disciplina

**Tópicos Acadêmicos**:
- **Arquivos - I/O Binário**: Leitura e escrita em posições específicas
- **Atualização de Registros**: Modifica dados existentes
- **Strings**: Manipulação de nomes e dados

---

#### `void consultarRelatoriosAcademicos()`

Consulta e exibe RELATÓRIOS ACADÊMICOS.

**Relatórios Gerados**:
- Notas por aluno
- Desempenho por turma
- Frequência de alunos (faltas)
- Disciplinas com maior demanda

**Tópic Acadêmico**: Manipulação de múltiplos arquivos
- Lê de: alunos.dat, notas.dat, disciplinas.dat
- Agrega informações
- Formata saída tabular

---

#### `void menuEventos(std::fstream &file)`

Menu para gerenciamento de EVENTOS.

**Parâmetro**:
- `file` [Referência] - Arquivo de eventos aberto

**Operações**:
1. Criar novo evento
2. Autorizar evento para oferta
3. Cancelar evento
4. Listar eventos

**Dados Coletados**:
- Nome do evento
- Local
- Descrição
- Data (formato dd/mm/aaaa)
- Total de vagas

---

#### `void menuCadastroInstrumentos()`

Menu para cadastro de INSTRUMENTOS.

**Dados Coletados**:
- Nome do instrumento
- Quantidade em estoque
- Valor patrimonial
- Localização no acervo
- Condição (operacional, manutenção)

**Tópico**: Gestão de Inventário

---

### 6.3 - Relatórios - Análise Consolidada

#### `void gerarRelatorioFinanceiro()`

Gera RELATÓRIO FINANCEIRO consolidado da lanchonete.

**Informações**:
- Total de vendas
- Receita por período
- Produtos mais vendidos
- Análise de tendências

**Saída**: Exibe tabela na interface gráfica

**Tópico Acadêmico**: Leitura agregada de múltiplos registros

---

#### `void gerarRelatorioPatrimonial()`

Gera RELATÓRIO PATRIMONIAL (inventário).

**Informações**:
- Relação completa de instrumentos
- Quantidade em estoque
- Valor total do acervo
- Instrumentos em manutenção
- Recomendações de reposição
- Instrumentos emprestados

**Tópico**: Relacionamento entre múltiplas tabelas

---

### 6.4 - Backup e Restauração

#### `void realizarBackup()`

Realiza BACKUP automático de todos os arquivos.

**Processo**:
1. Gera timestamp: `backup/YYYY-MM-DD_HH-MM-SS/`
2. Cria diretório
3. Copia todos os arquivos .dat e .txt
4. Registra log de backup

**Arquivos Salvos**:
- alunos.dat
- professores.dat
- administradores.dat
- disciplinas.dat
- eventos.dat
- instrumentos.dat
- emprestimos.dat
- usuarios.dat
- notas.dat
- cadastros.txt
- lanchonete.txt
- instrumentos.txt
- eventos.txt

**Tópicos Acadêmicos**:
- **Strings em C++**: Geração de timestamp como string
- **Manipulação de Nomes**: Construção de caminhos dinâmicos
- **Funções do Sistema**: Criação de diretórios
- **I/O Binário**: Cópia de arquivos

---

#### `void restaurarBackup()`

Restaura arquivos de dados de um BACKUP anterior.

**Funcionalidades**:
- Listar backups disponíveis
- Selecionar backup desejado
- Restaurar todos os arquivos
- Manter backup anterior como segurança
- Opção de apagar backup

**Tópico Acadêmico**: Navegação de diretórios e I/O com arquivos

---

### 6.5 - Lanchonete

#### `void cadastrarProdutos()`

Menu para cadastro de PRODUTOS da lanchonete.

**Dados Coletados**:
- Nome do produto
- Preço unitário
- Quantidade em estoque
- Fornecedor
- Categoria (bebida, alimento)

**Operações**:
1. Cadastrar novo produto
2. Ativar/Desativar produto
3. Editar produto
4. Listar produtos

**Tópico**: Criação e Persistência de Estruturas

---

### 6.6 - Gestão de Turmas e Matrículas

#### `bool verificaTurmasProf(Professor &prof, int &Index_turma)`

Verifica se um PROFESSOR coordena turmas.

**Parâmetros**:
- `prof` [Referência] - Professor a verificar
- `Index_turma` [Referência] - Retorna índice da turma encontrada

**Retorno**: `bool`
- `true` se professor tem vaga para new turma
- `false` se professor já tem 5 turmas

**Busca**:
- Iterativa: Percorre array de turmas do professor
- Pode ser implementada recursivamente: `bool verificaTurmas_recursivo(int indice, int prof_id)`

---

#### `void cadastrarTurma()`

Menu para CADASTRO de novas TURMAS.

**Dados Coletados**:
- Disciplina (referência)
- Professor responsável
- Horário das aulas
- Sala/Local
- Vagas disponíveis

**Validações**:
- Professor não pode ter > 5 turmas
- Disciplina deve estar ativa
- Professor deve estar ativo

**Tópicos Acadêmicos**:
- **Associação de Estruturas**: Relaciona Disciplina + Professor + Alunos
- **Alocação Dinâmica**: `new`/`delete` para estruturas temporárias
- **Ponteiros**: Manipulação de dados via ponteiro

---

#### `void matricularAlunoTurma()`

Menu para MATRÍCULA de alunos em TURMAS.

**Processo**:
1. Seleciona aluno
2. Seleciona turma/disciplina
3. Valida requisitos
4. Verifica vagas disponíveis
5. Verifica duplicata (aluno já matriculado)
6. Efetua matrícula
7. Gera confirmação

**Validações**:
- Aluno deve estar ativo
- Turma deve estar ativa
- Verificação de capacidade
- Verificação de duplicata

**Tópico Acadêmico**: Gerenciamento de Relacionamentos
- Múltiplos alunos por turma
- Múltiplas turmas por aluno
- Verificação de conflitos

---

## 🔄 Interface Principal

### `void exibirMenuAdministrador()`

Exibe o menu inicial do administrador com todas as opções.

---

### `void abrir_menu_admin(Usuario* usuario)`

Abre o menu interativo do administrador.

**Parâmetro**:
- `usuario` [Ponteiro] - Usuário autenticado como administrador

**Tópico Acadêmico**: Ponteiros e Memória
- Transmissão de referência por ponteiro
- Acesso a membros via operador `->` ou `(*usuario).membro`
- Necessário para logging e auditoria

**Opções do Menu**:
1. Ativar Créditos Pendentes
2. Cadastrar Cursos
3. Consultar Estoque
4. Consultar Pendências
5. Gerenciar Eventos
6. Gerenciar Instrumentos
7. Gerenciar Produtos
8. Gerenciar Usuários
9. Realizar Backup
10. Relatório Financeiro
11. Relatório Patrimonial
12. Relatórios Acadêmicos
13. Restaurar Backup
14. Logout

---

## 📈 Padrões de Programação Utilizados

### 1. Passagem por Referência vs Valor

**Por Referência** (`&`):
- Arquivos: `openFile(std::fstream &f, ...)`
- Structs que serão modificadas: `Professor &prof`
- Arrays: `std::string dados[100][6]`

**Por Valor**:
- Valores primitivos: `int buscaId`
- Strings constantes: `const std::string Nome`
- Dados que não serão modificados

### 2. Inicialização de Structs

```cpp
Professor prof{};      // Inicializa com zeros
Disciplina disc{};     // Membros com valores padrão
```

### 3. Tratamento de Erro com Campo de Validação

```cpp
if (aluno.base.id == 0)  // id = 0 indica erro/não encontrado
    return;
```

### 4. Matrizes de Strings para Dados

```cpp
string dados[100][6];   // 100 registros, 6 colunas
```

### 5. Conversão de Arquivo para Array

Lê múltiplos registros do arquivo binário e converte para array de strings para exibição.

---

## 🚀 Fluxo Típico de Operação

### Cadastro de Recurso (ex: Disciplina)

```
1. Menu → Opção: Cadastrar
2. Coletar dados via interface_para_entrada_texto()
3. Validar entrada
4. Gerar novo ID com gerarNovoId()
5. Criar struct com dados
6. Abrir arquivo com openFile()
7. Posicionar em fim de arquivo: seekp(0, ios::end)
8. Escrever struct: file.write()
9. Fechar arquivo
10. Confirmar com mostrar_caixa_informacao()
```

### Busca de Recurso

```
1. Coletar ID via interface
2. Abrir arquivo com openFile()
3. Calcular posição: (id - base) * sizeof()
4. Posicionar com seekg()
5. Ler com file.read()
6. Validar (verificar id e ativo)
7. Retornar struct ou vazio se na encontrado
```

### Listar Recursos

```
1. Abrir arquivo
2. Posicionar em início: seekg(0)
3. Loop: while(file.read()) até limite ou EOF
4. Validar critérios de filtro
5. Converter struct → string
6. Armazenar em array bidimensional
7. Fechar arquivo
8. Exibir com interface_para_tabela()
9. Permitir seleção e ações
```

---

## 💾 Estruturas de Dados Utilizadas

### Professor
```cpp
struct {
    int id;
    char nome[50];
    char email[50];
    bool ativo;
    int turmas[5];      // Array de IDs de turmas
    char disciplina[30];
};
```

### Aluno
```cpp
struct {
    int id;
    char nome[50];
    char email[50];
    bool ativo;
    int faltas;
};
```

### Disciplina
```cpp
struct {
    int id;
    char nome[30];
    int cargaHoraria;
    bool ativo;
};
```

### Turma
```cpp
struct {
    int id;
    int idDisciplina;
    int idProfessor;
    int alunos[MAX_ALUNOS];
    int qtdAlunos;
    bool ativo;
};
```

### Evento
```cpp
struct {
    int id;
    char nome[50];
    char local[50];
    char descricao[100];
    char data[11];       // dd/mm/aaaa
    int totalVagas;
    int vagasOcupadas;
    bool autorizado;
    bool ativo;
};
```

### Instrumento
```cpp
struct {
    int id;
    char nome[30];
    int estoque;
    bool ativo;
    bool autorizado;
    bool disponivel;
};
```

### Produto
```cpp
struct {
    int id;
    char nome[50];
    double preco;
    int estoque;
    bool ativo;
};
```

---

## 🎯 Conceitos-Chave por Tópico

### Funções e Procedimentos
- Definição e declaração antecipada (forward declaration)
- Procedimentos vs Funções
- Escopo: funções globais vs métodos de namespace
- Visibilidade pública

### Structs
- Definição e uso de estruturas
- Inicialização com `{}`
- Passagem como parâmetro
- Retorno de estruturas

### Arquivos
- Modo binário: `ios::binary`
- Modo texto: padrão
- Leitura: `seekg()`, `read()`
- Escrita: `seekp()`, `write()`
- Positão: `tellg()`, `tellp()`

### Ponteiros
- Declaração: `int* ptr`
- Desreferência: `*ptr`
- Acesso a membro: `ptr->membro`
- Alocação: `new`
- Liberação: `delete`

### Strings
- C-strings: `char nome[50]`
- `std::string`: tipos modernos
- Conversão: `string(char_ptr)`, `char_ptr.c_str()`
- Concatenação: `str1 + str2`
- Função: `strncpy()`, `stoi()`, `stod()`

### Recursividade
- Implementação iterativa de `verificaTurmasProf`
- Pode ser adaptada para recursve com accumulator

---

## 📝 Desenvolvimento Futuro

Possíveis extensões do módulo:

1. **Validação Avançada**: Implementar classe validador
2. **Criptografia**: Senhas seguras para administradores
3. **Auditoria**: Log detalhado de todas as operações
4. **Relatórios Avançados**: Export para PDF/Excel
5. **Sincronização**: Backup em nuvem
6. **Otimização**: Árvore B+ para arquivos grandes
7. **Transações**: ACID para operações críticas

---

**Desenvolvimento Acadêmico**  
Alunos da Disciplina de Introdução à Programação II - 2026

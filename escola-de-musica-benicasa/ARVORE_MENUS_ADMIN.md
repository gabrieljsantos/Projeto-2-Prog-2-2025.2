# Árvore de Menus - Módulo Administrador

## 📊 Estrutura Hierárquica dos Menus

```
ADMIN
└── MENU PRINCIPAL (abrir_menu_admin)
    ├── 0. Ativar Créditos Pendentes
    │   └── Função: Lanchonete::ativarCreditosPendentes()
    │
    ├── 1. Cadastrar Cursos
    │   └── mod_ADM::menuCadastroCursos()
    │       ├── 0. Cadastrar Disciplina
    │       ├── 1. Autorizar Disciplina
    │       ├── 2. Inativar Disciplina
    │       ├── 3. Vincular Professor
    │       ├── 4. Listar Disciplinas
    │       └── 5. Voltar
    │
    ├── 2. Consultar Estoque
    │   └── Função: Lanchonete::consultarEstoque()
    │
    ├── 3. Consultar Pendências
    │   └── consultarPendenciasInstrumentos()
    │       ├── 0. Listar Todas as Pendências
    │       ├── 1. Filtrar por Aluno
    │       └── 2. Voltar
    │
    ├── 4. Gerenciar Eventos
    │   └── mod_ADM::menuEventos()
    │       ├── 0. Cadastrar Evento
    │       ├── 1. Autorizar Evento
    │       ├── 2. Inativar Evento
    │       ├── 3. Listar Eventos
    │       │   ├── 0. Listar Autorizados
    │       │   ├── 1. Listar Pendentes
    │       │   └── 2. Voltar
    │       └── 4. Voltar
    │
    ├── 5. Gerenciar Instrumentos
    │   └── mod_ADM::menuCadastroInstrumentos()
    │       ├── 0. Cadastrar Instrumento
    │       ├── 1. Autorizar Instrumento
    │       ├── 2. Inativar Instrumento
    │       ├── 3. Listar Instrumentos
    │       └── 4. Voltar
    │
    ├── 6. Gerenciar Produtos
    │   └── mod_ADM::cadastrarProdutos()
    │       ├── 0. Cadastrar Produto
    │       ├── 1. Ativar Produto
    │       ├── 2. Inativar Produto
    │       ├── 3. Editar Produto
    │       ├── 4. Listar Produtos
    │       └── 5. Voltar
    │
    ├── 7. Gerenciar Usuários
    │   └── mod_ADM::menuGerenciarUsuarios()
    │       ├── Filtro: Tipo de Usuário (Aluno/Professor/Admin)
    │       ├── Filtro: Estado do Usuário (Ativo/Inativo/Ambos)
    │       ├── 0. Filtros de Pesquisa
    │       ├── 1. Listar Usuários
    │       ├── 2. Buscar Usuário por ID
    │       └── 3. Voltar
    │
    ├── 8. Realizar Backup
    │   └── mod_ADM::realizarBackup()
    │       └── Cria pasta com timestamp: backup/YYYY-MM-DD_HH-MM-SS
    │
    ├── 9. ⭐ Relatorio Financeiro
    │   └── mod_ADM::gerarRelatorioFinanceiro()
    │       └── Exibe análise de produtos da lanchonete
    │
    ├── 10. ⭐ Relatorio Patrimonial
    │   └── mod_ADM::gerarRelatorioPatrimonial()
    │       └── Exibe status de instrumentos e empréstimos
    │
    ├── 11. ⭐ Relatorios Academicos
    │   └── mod_ADM::consultarRelatoriosAcademicos()
    │       └── Exibe dados de alunos e faltas
    │
    ├── 12. Restaurar Backup
    │   └── mod_ADM::restaurarBackup()
    │       ├── Listar backups disponíveis
    │       ├── Selecionar backup
    │       ├── Restaurar (copia arquivos)
    │       └── Apagar backup
    │
    └── 13. Logout
        └── Define usuario->logado = false
```

---

## 📋 Detalhamento dos Menus Principais

### Menu 1: Cadastrar Cursos (Disciplinas)
**Função:** `mod_ADM::menuCadastroCursos()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Cadastrar Disciplina | Entrada: Nome, Carga Horária |
| 1 | Autorizar Disciplina | Tabela interativa de disciplinas inativas |
| 2 | Inativar Disciplina | Tabela interativa de disciplinas ativas |
| 3 | Vincular Professor | Seleciona disciplina e professor para associação |
| 4 | Listar Disciplinas | Exibe todas as disciplinas com status |
| 5 | Voltar | Retorna ao menu principal |

---

### Menu 3: Consultar Pendências (Instrumentos)
**Função:** `consultarPendenciasInstrumentos()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Listar Todas Pendências | Exibe empréstimos pendentes |
| 1 | Filtrar por Aluno | Entrada: ID do aluno, exibe empréstimos específicos |
| 2 | Voltar | Retorna ao menu principal |

---

### Menu 4: Gerenciar Eventos
**Função:** `mod_ADM::menuEventos()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Cadastrar Evento | Entrada: Nome, Local, Descrição, Data, Vagas |
| 1 | Autorizar Evento | Tabela de eventos não autorizados |
| 2 | Inativar Evento | Tabela de eventos ativos |
| 3 | Listar Eventos | Submenu: Autorizados/Pendentes/Voltar |
| 4 | Voltar | Retorna ao menu principal |

---

### Menu 5: Gerenciar Instrumentos
**Função:** `mod_ADM::menuCadastroInstrumentos()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Cadastrar Instrumento | Entrada: Nome, Quantidade em Estoque |
| 1 | Autorizar Instrumento | Tabela de instrumentos não autorizados |
| 2 | Inativar Instrumento | Tabela de instrumentos autorizados |
| 3 | Listar Instrumentos | Exibe todos com status de autorização |
| 4 | Voltar | Retorna ao menu principal |

---

### Menu 6: Gerenciar Produtos (Lanchonete)
**Função:** `mod_ADM::cadastrarProdutos()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Cadastrar Produto | Entrada: Nome, Preço, Quantidade |
| 1 | Ativar Produto | Entrada: ID, Confirma ativação |
| 2 | Inativar Produto | Entrada: ID, Confirma inativação |
| 3 | Editar Produto | Entrada: ID, Edita Nome/Preço/Estoque |
| 4 | Listar Produtos | Exibe todos com preços e estoque |
| 5 | Voltar | Retorna ao menu principal |

---

### Menu 7: Gerenciar Usuários
**Função:** `mod_ADM::menuGerenciarUsuarios()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| 0 | Filtros de Pesquisa | Tipo: Aluno/Professor/Admin, Estado: Ativo/Inativo/Ambos |
| 1 | Listar Usuários | Tabela com filtros aplicados, opções: Ativar/Desativar/Modificar |
| 2 | Buscar por ID | Entrada: ID, Toggle ativo/inativo |
| 3 | Voltar | Retorna ao menu principal |

---

### Menu 12: Restaurar Backup
**Função:** `mod_ADM::restaurarBackup()`

| Opção | Descrição | Detalhes |
|-------|-----------|----------|
| - | Listar Backups | Tabela com datas/horas disponíveis |
| 1 | Restaurar | Copia arquivos do backup selecionado |
| 2 | Apagar | Remove pasta do backup |
| 3 | Voltar | Retorna ao menu principal |

---

## 📁 Arquivos e Estrutura de Dados

### Arquivos Utilizados
- `alunos.dat` - Registros de alunos
- `professores.dat` - Registros de professores
- `administradores.dat` - Registros de administradores
- `disciplinas.dat` - Registros de disciplinas
- `eventos.dat` - Registros de eventos
- `instrumentos.dat` - Registros de instrumentos
- `emprestimos.dat` - Registros de empréstimos
- `lanchonete.dat` - Registros de produtos
- `turmas.dat` - Registros de turmas

### Pastas
- `backup/` - Armazena backups com formato `YYYY-MM-DD_HH-MM-SS/`

---

## 🔄 Fluxo de Navegação

### Padrão de Retorno
- Todos os submenus retornam ao menu anterior ao selecionar "Voltar"
- O menu principal mantém-se em loop até selecionar "Logout"
- Cada seleção abre um novo menu ou executa uma ação

### Padrão de Confirmação
- Ações críticas (autorizar, inativar, deletar) pedindo confirmação via botões
- Tabelas interativas permitem seleção de item
- Caixas de informação mostram resultados

---

## 🔐 Controles de Segurança

- **Validação de ID:** Todos os acessos verificam ID válido
- **Status Ativo/Inativo:** Apenas usuários ativos podem ser manipulados
- **Filtros de Pesquisa:** Menus de usuários oferecem filtros customizáveis
- **Confirmação de Ações:** Operações destrutivas requerem confirmação

---

## 📊 Funções Auxiliares Principais

| Função | Descrição |
|--------|-----------|
| `gerarNovoId()` | Gera próximo ID para novo registro |
| `openFile()` | Abre arquivo .dat com tratamento de erro |
| `buscaAluno()` | Busca aluno por ID |
| `buscaProf()` | Busca professor por ID |
| `buscaDisciplina()` | Busca disciplina por ID |
| `buscarInstrumento()` | Busca instrumento por ID |
| `buscaProduto()` | Busca produto por ID |
| `listar_usuarios_especificos()` | Lista usuários com filtros |
| `atualizar_estado_de_usuario()` | Ativa/desativa usuário |

---

## 🚀 Melhorias e Organização Recomendadas

1. **Separar submenus em funções dedicadas** - Alguns menus contêm muitas linhas
2. **Criar namespace para eventos** - Funções de eventos poderiam ter namespace separado
3. **Padronizar nomes de funções** - Usar prefixo `menu_` ou `gerenciar_` consistentemente
4. **Extrair lógica de tabelas** - Muita repetição de código de tabela interativa
5. **Consolidar backups** - Lógica de backup/restauração poderia ser separada

---

**Data de Geração:** 19 de fevereiro de 2026  
**Autor:** Documentação Automática  
**Versão:** 1.0

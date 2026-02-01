# 📋 Sistema de Gerenciamento Escolar - Projeto 2 (Prog 2 - 2025.2)

## 📖 **LEIA O ARQUIVO headers.h ANTES DE TUDO** 📖

**Este arquivo contém TODAS as estruturas de dados fundamentais do projeto. Você DEVE entender o fluxo e as estruturas nele definidas antes de implementar qualquer módulo ou função!**

📁 **Abra agora:** [headers.h](headers.h)

---

## ⚠️ **IMPORTANTE: Projetos Separados NÃO Funcionam**

Este projeto **NÃO pode ser compilado em partes isoladas** (como em CodeBlocks ou módulos separados). 

### **Por que?**
- Todos os módulos dependem de `headers.h` (estruturas centralizadas)
- Todos os módulos precisam acessar `database/` (camada de dados)
- As funções são chamadas via `servico_*` compartilhados
- Sem centralização, cada módulo teria suas próprias estruturas duplicadas

### **Exemplo de Funcionamento ERRADO (Separado):**

```
❌ CodeBlocks Projeto 1 (Aluno):
   📁 mod_area_do_aluno/
   📁 headers.h (CÓPIA 1)
   📁 database/

❌ CodeBlocks Projeto 2 (Professor):  
   📁 mod_area_do_professor/
   📁 headers.h (CÓPIA 2 - CONFLITA COM 1)
   📁 database/ (DUPLICADO)
   
RESULTADO: 
   ❌ Duas definições de headers.h
   ❌ Dois databases separados
   ❌ Um aluno não consegue comprar na cantina
   ❌ Dados fragmentados e inconsistentes
```

### **Exemplo de Funcionamento CORRETO (Centralizado):**

```
✅ Sistema Único em VS Code:
   📁 main.cpp (PONTO DE ENTRADA ÚNICO)
   📁 headers.h (ESTRUTURAS CENTRALIZADAS)
   📁 database/ (BANCO ÚNICO E COMPARTILHADO)
   📁 mod_login_e_matricula/
   📁 mod_area_do_aluno/
   📁 mod_area_do_professor/
   📁 mod_adminitrativo/
   📁 mod_eventos/
   📁 mod_instrumentos/
   📁 mod_lanchonete/

RESULTADO:
   ✅ Uma única definição de headers.h
   ✅ Um database compartilhado
   ✅ Um aluno pode logar → comprar → pegar instrumento
   ✅ Todos os dados sincronizados
```

**Não tente compilar módulos em CodeBlocks separadamente. Use APENAS VS Code com main.cpp como ponto de entrada único.**

---

## 📊 Sobre o Projeto

Este é um **Sistema de Gerenciamento Escolar Modular** desenvolvido em C++. Funciona como um portal unificado tipo SIGAA, onde diferentes usuários (alunos, professores, administradores, funcionários) fazem login e acessam funcionalidades conforme seus cargos.

O sistema foi arquitetado em **módulos independentes** que se comunicam entre si, permitindo escalabilidade e manutenção facilitada.

---

## 🏗️ Arquitetura: Relação Entre Módulos, Headers, Database e Inicializadores

### **Estrutura em 4 Camadas:**

```
╔════════════════════════════════════════════════════════════════╗
║                    CAMADA 1: INTERFACE (Entrada)              ║
║  mod_login_e_matricula/ → mod_area_do_aluno/ → mod_.../ etc   ║
╠════════════════════════════════════════════════════════════════╣
║                    CAMADA 2: SERVIÇOS (Lógica)                ║
║  servico_venda.cpp → servico_emprestimo.cpp → servico_login.cpp
╠════════════════════════════════════════════════════════════════╣
║                    CAMADA 3: ESTRUTURAS (Dados em Memória)    ║
║                          headers.h                             ║
║         (Aluno, Professor, Turma, Evento, etc)                ║
╠════════════════════════════════════════════════════════════════╣
║                CAMADA 4: PERSISTÊNCIA (Arquivo)               ║
║                          database/                             ║
║     (registros_notas.dat, saldos_cantina.dat, etc)            ║
╚════════════════════════════════════════════════════════════════╝
```

### **Fluxo de Dados Completo:**

```
[Usuário Interage com Interface]
        ↓
[entrada_aluno.cpp - CAMADA 1: INTERFACE]
        ↓
[Chama servico_venda.cpp - CAMADA 2: LÓGICA]
        ↓
[Usa estruturas em headers.h - CAMADA 3: MEMÓRIA RAM]
        ↓
[Chama inicializadores em inicializacao/ - LEITURA/ESCRITA]
        ↓
[Arquivo em database/ - CAMADA 4: PERSISTÊNCIA]
```

### **Explicação Detalhada de Cada Camada:**

#### **CAMADA 1: Interface (entrada_*.cpp)**
**O que faz:** Exibe menus, coleta entrada do usuário, chama serviços
- Exemplo: `mod_area_do_aluno/entrada_aluno.cpp`
- Responsabilidade: Menu do aluno, escolher op��o
- N�O calcula, apenas exibe e coleta dados

#### **CAMADA 2: Servi�os (servico_*.cpp)**
**O que faz:** Implementa a l�gica de neg�cio, processa dados, valida
- Exemplo: `mod_lanchonete/servico_venda.cpp`
- Responsabilidade: Validar compra, atualizar saldo, chamar inicializadores
- N�O exibe (interface faz isso), apenas processa

#### **CAMADA 3: Estruturas (headers.h)**
**O que faz:** Define tipos de dados que vivem em RAM
- Exemplo: `struct Aluno { int id; float notas[4]; };`
- Responsabilidade: Definir campos, tipo de dado
- Existe APENAS enquanto o programa roda

#### **CAMADA 4: Persist�ncia (database/)**
**O que faz:** Armazena dados em arquivo bin�rio
- Exemplo: `database/registros_notas.dat`
- Responsabilidade: Guardar dados permanentemente
- Sobrevive ao encerramento do programa

---

## 📈 Ciclo de Vida dos Dados: Exemplo Completo

### **Cenário: Aluno Comprando na Lanchonete**

```
┌───────────────────────────────────────────────────────────────┐
│ 1️⃣  STARTUP DO PROGRAMA                                      │
├───────────────────────────────────────────────────────────────┤
│ database/saldos_cantina.dat (arquivo permanente)            │
│    ↓                                                         │
│ [fread() - inicializadores.cpp]                             │
│    ↓                                                         │
│ Aluno aluno; aluno.credito = 100.0; (em RAM)               │
└───────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────┐
│ 2️⃣  USUÁRIO INTERAGE (CAMADA 1: INTERFACE)                  │
├───────────────────────────────────────────────────────────────┤
│ entrada_aluno.cpp mostra menu:                              │
│    "4 - Acessar Lanchonete"                                 │
│    Usuário escolhe: comprar um refrigerante                │
└───────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────┐
│ 3️⃣  LÓGICA DE NEGÓCIO (CAMADA 2: SERVIÇO)                   │
├───────────────────────────────────────────────────────────────┤
│ servico_venda.cpp valida:                                  │
│    ✓ Produto existe?                                        │
│    ✓ Há estoque?                                            │
│    ✓ Aluno tem saldo suficiente?                           │
│    ✓ Se tudo ok: aluno.credito -= preco;                  │
└───────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────┐
│ 4️⃣  ATUALIZAR ARQUIVO (PERSISTÊNCIA)                        │
├───────────────────────────────────────────────────────────────┤
│ inicializadores.cpp chama:                                  │
│    [fwrite() - salvar aluno atualizado]                    │
│    ↓                                                         │
│ database/saldos_cantina.dat (arquivo atualizado)           │
│    Novo credito: 90.0 (persistido)                         │
└───────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────┐
│ 5️⃣  CONFIRMAÇÃO AO USUÁRIO (CAMADA 1: INTERFACE)            │
├───────────────────────────────────────────────────────────────┤
│ entrada_aluno.cpp exibe:                                    │
│    "✓ Compra realizada!"                                    │
│    "Saldo atual: R$ 90.00"                                  │
└───────────────────────────────────────────────────────────────┘

┌───────────────────────────────────────────────────────────────┐
│ 6️⃣  PRÓXIMO LOGIN                                            │
├───────────────────────────────────────────────────────────────┤
│ Quando o aluno logar novamente:                             │
│    [fread() - carrega credito = 90.0]                      │
│    Dados persistiram! ✅                                     │
└───────────────────────────────────────────────────────────────┘
```

---

## 💾 Database: Estrutura de Arquivos

Cada arquivo em `database/` armazena um tipo espec�fico de dado:

| Arquivo | Conte�do | Estrutura em headers.h | Acessado Por |
|---------|----------|--------|-------------|
| `identidades.dat` | Usu�rios (ID, nome, CPF, senha) | `Identidade` | servico_login, entrada_cadastro |
| `registros_notas.dat` | Notas de alunos | `RegistroNotas` | entrada_aluno, servico_notas |
| `saldos_cantina.dat` | Cr�dito do aluno | `Saldo_Cantina` | entrada_aluno, servico_venda |
| `vendas_cantina.dat` | Hist�rico de compras | `Venda_Cantina` | entrada_aluno, servico_venda |
| `atrasos_instrumentos.dat` | Empr�stimos ativos | `Atraso_Instrumento` | entrada_aluno, servico_emprestimo |
| `inventario_instrumentos.dat` | Instrumentos dispon�veis | `Instrumento` | entrada_locador, servico_emprestimo |
| `turmas.dat` | Classes/disciplinas | `Class` | mod_adminitrativo, mod_professor |
| `historicos.dat` | Frequ�ncia e notas por turma | `historic_individual` | entrada_aluno, entrada_professor |
| `eventos.dat` | Eventos agendados | `EventoAgenda` | entrada_eventos, entrada_aluno |
| `produtos_cantina.dat` | Produtos � venda | `ProdutoCantina` | entrada_vendedor, servico_venda |

### **Inicializadores: A Ponte Entre Arquivo e Mem�ria**

O arquivo `inicializacao/inicializadores.cpp` cont�m fun��es que:
- **L�m** dados do arquivo para mem�ria (fread)
- **Escrevem** dados da mem�ria para arquivo (fwrite)

Exemplos (A IMPLEMENTAR):
```cpp
// ? LER do arquivo PARA mem�ria
Aluno carregar_aluno_por_id(int id) {
    FILE *f = fopen("database/registros_notas.dat", "rb");
    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, f);
    fclose(f);
    return aluno;  // Retorna struct em RAM
}

// ? ESCREVER da mem�ria PARA arquivo
void salvar_aluno(const Aluno &a) {
    FILE *f = fopen("database/registros_notas.dat", "ab");
    fwrite(&a, sizeof(Aluno), 1, f);
    fclose(f);
}
```

---

## 🔐 Sistema de Autenticação e Cadastro

### 🔑 **Credenciais de Teste**

Para testar o sistema, use:

| Campo | Valor | Descri��o |
|-------|-------|-----------|
| **ID** | `0000` | ID fixo para teste |
| **Senha** | `0000` | Senha para teste |
| **Senha Padr�o Usuarios** | `senha` | Padr�o para novos cadastros |

**IMPORTANTE:** A fun��o `validar_login()` em `mod_login_e_matricula/servico_login.cpp` atualmente est� **hardcoded** (engessada). Voc� DEVE implementar a valida��o real com banco de dados.

---

## 🔓 Como Fazer Login

### **Passo 1: Abra o programa**
```bash
# Em VS Code:
Ctrl + Shift + B  # Compila
# Ou execute: main.exe
```

### **Passo 2: Tela de Login**
```
┌──────────────────────────────┐
│ Digite seu ID: 0000          │
│ Digite sua senha: 0000       │
└──────────────────────────────┘
```

### **Passo 3: Resultado**
- Com os dados atuais, voc� ser� redirecionado para **ADMIN** (painel do administrador)
- Para testar outros cargos, edite `servico_login.cpp` (veja pr�xima se��o)

---

## 👤 Como Cadastrar e Testar Diferentes Tipos de Usuário

### **Op��o 1: Modificar servico_login.cpp para Teste R�pido**

**Arquivo:** `mod_login_e_matricula/servico_login.cpp`

**C�digo Atual (Hardcoded - Sempre ADMIN):**

```cpp
login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;

    // ? HARDCODED - SEMPRE RETORNA ADMIN
    info.status = VALIDO;
    info.chave_acesso = ADMIN;  // ? Sempre ADMIN
    
    info.info_basica.id = 1;
    info.info_basica.ativo = 1;
    std::strcpy(info.info_basica.nome, "Administrador Padrao");
    std::strcpy(info.info_basica.cpf, "00000000000");
    info.info_basica.categoria = ADMIN;
    std::strcpy(info.info_basica.especialidade, "Administrador");

    return info;
}
```

**Para Testar como ALUNO, Modifique para:**

```cpp
login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;

    // ? MODIFICADO PARA TESTAR
    info.status = VALIDO;
    info.chave_acesso = ALUNO;  // ?? MUDE AQUI
    
    info.info_basica.id = 1;
    info.info_basica.ativo = 1;
    std::strcpy(info.info_basica.nome, "Jo�o da Silva");
    std::strcpy(info.info_basica.cpf, "12345678901");
    info.info_basica.categoria = ALUNO;  // ?? E AQUI TAMB�M
    std::strcpy(info.info_basica.especialidade, "");

    return info;
}
```

### **Tabela de Configura��o R�pida:**

| Cargo | `chave_acesso` | `categoria` | `nome` | `especialidade` |
|-------|---|---|---|---|
| **ADMIN** | `ADMIN` | `ADMIN` | "Admin Teste" | "Administrador" |
| **ALUNO** | `ALUNO` | `ALUNO` | "Jo�o Silva" | "" |
| **PROFESSOR** | `PROFESSOR` | `PROFESSOR` | "Prof. Ana" | "Piano" |
| **VENDEDOR** | `VENDEDOR` | `VENDEDOR` | "Vendedor Teste" | "Vendedor" |
| **LOCADOR** | `LOCADOR` | `LOCADOR` | "Locador Teste" | "Locador" |

### **Passos para Testar um Cargo Diferente:**

1. **Abra:** `mod_login_e_matricula/servico_login.cpp`
2. **Procure** a fun��o `validar_login()`
3. **Altere** `info.chave_acesso = TIPO_DESEJADO`
4. **Altere tamb�m** `info.info_basica.categoria = TIPO_DESEJADO`
5. **Atualize** `nome` e outros campos conforme tabela acima
6. **Salve:** Ctrl + S
7. **Compile:** Ctrl + Shift + B
8. **Execute:** main.exe

---

### **Op��o 2: Cadastro de Novo Usu�rio (Quando Implementado)**

Quando a funcionalidade de cadastro estiver pronta:

1. Selecione **"Novo Cadastro"** no menu principal
2. Insira dados:
   - Nome completo
   - CPF
   - Email
   - Senha (padr�o � "senha" se deixar em branco)
3. Escolha o tipo:
   - `ALUNO` - Aluno/Discente
   - `PROFESSOR` - Professor/Docente
   - `ADMIN` - Administrador
   - `VENDEDOR` - Gerencia lanchonete
   - `LOCADOR` - Gerencia instrumentos
4. Confirme

📝 **AINDA NÃO IMPLEMENTADO** - Este módulo precisa ser criado

---

## 📋 O Que Precisa Ser Feito (Roadmap Completo)

### **Fase 1: Inicializadores ? CR�TICO - SEM ISSO NADA FUNCIONA**

Arquivo: `inicializacao/inicializadores.cpp`

**DEVE IMPLEMENTAR:**

```cpp
// ? FUN��ES DE LEITURA (arquivo ? mem�ria)
Identidade carregar_identidade_por_id(int id);
Aluno carregar_aluno_por_id(int id);
Professor carregar_professor_por_id(int id);
Turma carregar_turma_por_codigo(char *codigo);
EventoAgenda carregar_evento_por_id(int id);
ProdutoCantina* carregar_todos_produtos(int &total);
Instrumento* carregar_todos_instrumentos(int &total);

// ? FUN��ES DE ESCRITA (mem�ria ? arquivo)
void salvar_identidade(const Identidade &id);
void salvar_aluno(const Aluno &a);
void salvar_professor(const Professor &p);
void salvar_turma(const Turma &t);
void salvar_evento(const EventoAgenda &e);

// ? FUN��ES DE LISTAGEM
Identidade* carregar_todas_identidades(int &total);
Aluno* carregar_todos_alunos(int &total);
Professor* carregar_todos_professores(int &total);
Turma* carregar_todas_turmas(int &total);
```

**Por que � cr�tico?**
- ? Sem isso: Dados n�o persistem entre execu��es
- ? Sem isso: M�dulos n�o conseguem compartilhar dados
- ? Sem isso: Sistema n�o funciona

---

### **Fase 2: Valida��o Real em servico_login.cpp** ? IMPORTANTE

**Problema Atual:**
```cpp
// ? Sempre retorna ADMIN, ignora senha
info.chave_acesso = ADMIN;
```

**Solu��o:**
```cpp
// ? Busca no banco e valida
Identidade id = carregar_identidade_por_id(*id_usuario);

if (strcmp(id.senha, senha) == 0) {
    return criar_login_info_de_identidade(id);
} else {
    info.status = INVALIDO;
    return info;
}
```

---

### **Fase 3: Completar M�dulos Incompletos**

#### **mod_area_do_aluno/**

**Funcionando:**
- Menu principal funciona
- Fun��es `mostrarNotas()`, `mostrarExtratoLanchonete()` existem
- Sem erros de compila��o

**Faltando:**
- Carregar dados reais do banco via inicializadores
- Op��o 2 (Verificar Frequ�ncia) - fun��o n�o existe
- Op��o 3 (Visualizar Turmas) - fun��o n�o existe
- Op��o 5 (Visualizar Eventos) - fun��o n�o existe

**O QUE FAZER:**
```cpp
// Dentro de janela_aluno():
Aluno aluno = carregar_aluno_por_id(info.id);  // ⬅️ ADICIONAR
Emprestimo emprestimos[100];
int total = carregar_emprestimos(emprestimos);  // ⬅️ ADICIONAR

// Depois descomentar as funções:
case 1: mostrarNotas(aluno); break;  // Vai exibir dados reais
case 4: mostrarExtratoLanchonete(aluno); break;
```

#### **mod_area_do_professor/**

**Status:**
- Menu n�o funciona
- Op��es vazias
- Sem integra��o com turmas/notas/frequ�ncia

**O QUE FAZER:**
1. Carregar turmas do professor
2. Implementar lan�amento de notas
3. Implementar registro de frequ�ncia
4. Integrar com `mod_eventos/entrada_eventos.cpp`

#### **mod_adminitrativo/**

**Status:**
- Menu principal funciona
- Op��es sem funcionalidade
- Sem CRUD de usu�rios
- Sem CRUD de turmas

**O QUE FAZER:**
1. Criar usu�rios (chamar `entrada_cadastro.cpp`)
2. Editar usu�rios (atualizar em banco)
3. Deletar usu�rios (marcar como inativo)
4. Criar turmas
5. Editar turmas
6. Movimentar aluno entre turmas

#### **mod_eventos/**

**Status:**
- `entrada_eventos.cpp` vazio
- `servico_inscricao_evento.cpp` N�O EXISTE
- Sem cria��o/inscri��o de eventos

**O QUE FAZER:**
1. Criar `servico_inscricao_evento.cpp` - compartilhado
2. Implementar `entrada_eventos.cpp` com menu
3. Validar vagas dispon�veis
4. Registrar inscri��es no banco

#### **mod_instrumentos/**

**Status:**
- `entrada_locador.cpp` estrutura existe
- `servico_emprestimo.cpp` n�o implementado
- Sem CRUD de instrumentos
- Sem sistema de empr�stimos

**O QUE FAZER:**
1. Completar `servico_emprestimo.cpp` - compartilhado
2. Validar limite de 5 itens por pessoa
3. Registrar datas de empr�stimo/devolu��o
4. Controlar danos e perdas

#### **mod_lanchonete/**

**Status:**
- `entrada_vendedor.cpp` estrutura existe
- `servico_venda.cpp` incompleto
- Sem CRUD de produtos
- Sem controle de estoque

**O QUE FAZER:**
1. Completar `servico_venda.cpp` - compartilhado
2. Implementar CRUD de produtos
3. Validar quantidade em estoque
4. Atualizar saldo do usu�rio ap�s compra

---

## 🛠️ Como Implementar um Módulo (Passo a Passo)

### **Exemplo: Completar mod_area_do_aluno/**

#### **Passo 1: Entender headers.h**
```cpp
// Abra headers.h e procure por:
struct Identidade { ... }      // Dados do aluno
struct RegistroNotas { ... }   // Notas do aluno
struct Saldo_Cantina { ... }   // Cr�dito do aluno
struct Atraso_Instrumento { ... }  // Empr�stimos
```

#### **Passo 2: Implementar Inicializadores**
```cpp
// Em inicializacao/inicializadores.cpp, crie:
Aluno carregar_aluno_por_id(int id) {
    FILE *f = fopen("database/registros_notas.dat", "rb");
    Aluno aluno;
    // Buscar aluno com esse id
    fread(&aluno, sizeof(Aluno), 1, f);
    fclose(f);
    return aluno;
}
```

#### **Passo 3: Usar nos Serviços**
```cpp
// Em mod_area_do_aluno/entrada_aluno.cpp:
void janela_aluno(login_info info) {
    Aluno aluno = carregar_aluno_por_id(info.id);  // ⬅️ CARREGAR
    
    switch(escolha) {
        case 1:
            mostrarNotas(aluno);  // Já tem dados reais
            break;
    }
}
```

#### **Passo 4: Testar**
```
1. Compilar com Ctrl + Shift + B
2. Executar main.exe
3. Logar como ALUNO
4. Testar as funcionalidades
```

---

## 📌 Campos Pendentes de Implementação

### **EDITOR DE SENHA - AINDA N�O CRIADO** ?

A funcionalidade de **editar/mudar a senha** ainda precisa ser implementada.

**Onde deve existir:**
- `mod_login_e_matricula/entrada_cadastro.cpp` (usu�rio muda sua pr�pria senha)
- Ou perfil do usu�rio (cada um muda sua senha)

**C�digo sugerido:**
```cpp
void editar_senha(int id_usuario) {
    char senha_atual[20];
    char senha_nova[20];
    
    cout << "Digite sua senha atual: ";
    cin >> senha_atual;
    
    Identidade id = carregar_identidade_por_id(id_usuario);
    if (strcmp(id.senha, senha_atual) != 0) {
        cout << "? Senha incorreta!\n";
        return;
    }
    
    cout << "Digite a nova senha: ";
    cin >> senha_nova;
    
    strcpy(id.senha, senha_nova);
    salvar_identidade(id);
    
    cout << "? Senha atualizada!\n";
}
```

---

## 🔄 Fluxo Principal de Funcionamento

```
┌───────────────────────────────────────────────────────────────┐
│ 1. main.cpp inicia                                          │
└───────────────────────────────────────────────────────────────┘
                              ↓
                ┌──────────────────────────┐
                │ servico_login.cpp        │
                │ Autentica usuário        │
                │ Retorna person_role      │
                └──────────────────────────┘
                              ↓
        ┌─────────────────────────────────────────────────────────┐
        │  ADMIN  ALUNO  PROFESSOR  VENDEDOR  LOCADOR  EVENTOS   │
        │    ↓       ↓       ↓         ↓         ↓        ↓       │
        │ entrada_ entrada_ entrada_ entrada_ entrada_ entrada_ │
        │ admin   aluno   prof.    vendedor locador  eventos   │
        │    ↓       ↓       ↓         ↓         ↓        ↓       │
        └─────────────────────────────────────────────────────────┘
                              ↓
       ═════════════════════════════════════════════════════════════
       🔗 SERVIÇOS COMPARTILHADOS
       • servico_venda (todos compram)
       • servico_emprestimo (todos pegam instrumento)
       • servico_inscricao_evento (todos se inscrevem)
       ═════════════════════════════════════════════════════════════
```

---

## 📂 Estrutura de Diretórios Esperada

```
Projeto-2-Prog-2-2025.2/
📁 main.cpp                                 # Ponto de entrada
📁 headers.h                                # Estruturas centralizadas
📁 Readme.md                                # Este arquivo
📁 database/                                # Camada de persistência
   📄 identidades.dat
   📄 registros_notas.dat
   📄 saldos_cantina.dat
   📄 vendas_cantina.dat
   📄 atrasos_instrumentos.dat
   📄 (outros .dat)
📁 inicializacao/
   📄 inicializadores.cpp                   # CRÍTICO: Lê/escreve arquivo
   📄 inicializadores.h
📁 mod_login_e_matricula/
   📄 servico_login.cpp / .h                # Validação hardcoded
   📄 entrada_cadastro.cpp / .h             # Incompleto
   📄 servico_cadastros.cpp / .h
   📄 inicializadores_de_tabela.cpp / .h
📁 mod_adminitrativo/
   📄 entrada_admin.cpp / .h                # Incompleto
   📁 (bin/, obj/ - ignorar)
📁 mod_area_do_aluno/
   📄 entrada_aluno.cpp / .h                # 60% - falta dados
   📁 (bin/, obj/ - ignorar)
📁 mod_area_do_professor/
   📄 entrada_professor.cpp / .h            # 30% - muito incompleto
   📁 (bin/, obj/ - ignorar)
📁 mod_eventos/
   📄 entrada_eventos.cpp / .h              # 20% - vazio
   📄 (servico_inscricao_evento.cpp - não existe)
📁 mod_instrumentos/
   📄 entrada_locador.cpp / .h              # 40% - sem serviço
   📄 (servico_emprestimo.cpp - não existe)
📁 mod_lanchonete/
   📄 entrada_vendedor.cpp / .h             # 50% - serviço incompleto
   📄 (servico_venda.cpp - parcial)
```

---

## 📊 Estruturas de Dados Principais (headers.h)

### **Identidade**
```cpp
struct Identidade {
    int id;
    char nome[50];
    char cpf[12];
    char senha[20];  // PADR�O: "senha"
    char email[50];
    person_role categoria;  // ALUNO, PROFESSOR, ADMIN, etc
    char especialidade[30];  // Ex: "Piano", "Canto"
    int ativo;
};
```

### **Aluno** (em mem�ria)
```cpp
struct Aluno {
    int id;
    char nome[50];
    float notas[4];
    float credito;  // Cr�dito na lanchonete
    Compra compras[20];  // Hist�rico de compras
    int quantCompras;
    Instrumento instrumentos[10];
    int quantInstrumentos;
};
```

### **Class (Turma)**
```cpp
struct Class {
    char codigo[10];  // Ex: "PROG001"
    char nome[50];    // Ex: "Programa��o II"
    int ano_letivo;
    int professor_id;
    int alunos[50];   // IDs dos alunos
    int quant_alunos;
    // ... mais campos
};
```

### **RegistroNotas**
```cpp
struct RegistroNotas {
    int aluno_id;
    int turma_codigo;
    float notas[10];  // At� 10 avalia��es
    int quant_notas;
};
```

---

## 🔑 Autenticação: Login + Senha Padrão

### **Credenciais para Teste:**
```
ID:     0000
Senha:  0000
```

### **Senha Padr�o para Novos Usu�rios:**
```
"senha"
```

### **Como Funciona Agora:**
1. Usu�rio digita qualquer ID e senha
2. Fun��o `validar_login()` **ignora** tudo
3. Sempre retorna ADMIN
4. ? **Isso est� errado!**

### **Como Deveria Funcionar:**
1. Usu�rio digita ID e senha
2. Carregar identidade do banco: `Identidade id = carregar_identidade_por_id(id_usuario);`
3. Comparar senhas: `if (strcmp(id.senha, senha) == 0)`
4. Se correto: retornar dados do usu�rio
5. Se errado: retornar `status = INVALIDO`

---

## 🔨 Compilação e Execução

### **Via VS Code (Recomendado):**
```bash
Ctrl + Shift + B    # Compila (usa build task g++.exe)
# Depois:
./main.exe          # Ou clique em "Run"
```

### **Manual com g++:**
```bash
cd d:\PROJECTS\Projeto-2-Prog-2-2025.2
g++ -g main.cpp mod_login_e_matricula/servico_login.cpp \
    mod_login_e_matricula/entrada_cadastro.cpp \
    mod_area_do_aluno/entrada_aluno.cpp \
    ... (adicione todos os .cpp) \
    -o main.exe

./main.exe
```

---

## ⚙️ Observações Importantes

- **M�ximo 50 alunos por turma**
- **M�ximo 100 aulas por turma**
- **M�ximo 10 eventos por turma**
- **M�ximo 5 itens emprestados por pessoa**
- **M�ximo 4 hor�rios por semana**
- **Cada fun��o DEVE validar dados de entrada**
- **M�dulos devem ser independentes e reutiliz�veis**
- **SEMPRE consultar `headers.h` para estruturas de dados**
- **Nunca duplicar estruturas em m�ltiplos arquivos**

---

## 🛠️ Arquivo utilidades.cpp

**O que cont�m:**
- `void limpar_tela()` - Limpa console (multiplataforma)
- `const char* enumerado_para_texto(funcao_pessoa f)` - Converte enum para string
- `void generate_random_cpf(char *cpf_buffer)` - Gera CPF aleat�rio
- `void generate_random_name(char *name_buffer)` - Gera nome aleat�rio

---

## 🌐 Codificação de Caracteres (Acentos)

Para que acentos funcionem:

**VS Code (`settings.json`):**
```json
{
    "files.encoding": "iso88591",
    "files.autoGuessEncoding": false
}
```

**C�digo C++:**
```cpp
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese");
    // ... resto do c�digo
}
```

---

## 🚀 Próximos Passos

1. **📖 LEIA headers.h** - Entenda as estruturas
2. **⚙️ Implemente inicializadores** - Função crítica
3. **🔓 Teste o login** - Modifique para diferentes cargos
4. **💾 Complete mod_area_do_aluno** - Carregue dados do banco
5. **🔧 Implemente os outros módulos** - Siga o mesmo padrão

---

**Vers�o:** 2025.2  
**Data:** Fevereiro 2026  
**Linguagem:** C++  
**Status:** Em Desenvolvimento - Aguardando Implementa��o de Inicializadores

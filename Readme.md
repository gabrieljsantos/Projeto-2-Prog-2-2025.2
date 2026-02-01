# ?? Sistema de Gerenciamento Escolar - Projeto 2 (Prog 2 - 2025.2)

## ?? **LEIA O ARQUIVO headers.h ANTES DE TUDO** ??

**Este arquivo contém TODAS as estruturas de dados fundamentais do projeto. Você DEVE entender o fluxo e as estruturas nele definidas antes de implementar qualquer módulo ou função!**

?? **Abra agora:** [headers.h](headers.h)

---

## ?? **IMPORTANTE: Projetos Separados NÃO Funcionam**

Este projeto **NÃO pode ser compilado em partes isoladas** (como em CodeBlocks ou módulos separados). 

### **Por que?**
- Todos os módulos dependem de `headers.h` (estruturas centralizadas)
- Todos os módulos precisam acessar `database/` (camada de dados)
- As funções são chamadas via `servico_*` compartilhados
- Sem centralização, cada módulo teria suas próprias estruturas duplicadas

### **Exemplo de Funcionamento ERRADO (Separado):**

```
? CodeBlocks Projeto 1 (Aluno):
   ?? mod_area_do_aluno/
   ?? headers.h (CÓPIA 1)
   ?? database/

? CodeBlocks Projeto 2 (Professor):  
   ?? mod_area_do_professor/
   ?? headers.h (CÓPIA 2 - CONFLITA COM 1)
   ?? database/ (DUPLICADO)
   
RESULTADO: 
   ? Duas definições de headers.h
   ? Dois databases separados
   ? Um aluno não consegue comprar na cantina
   ? Dados fragmentados e inconsistentes
```

### **Exemplo de Funcionamento CORRETO (Centralizado):**

```
? Sistema Único em VS Code:
   ?? main.cpp (PONTO DE ENTRADA ÚNICO)
   ?? headers.h (ESTRUTURAS CENTRALIZADAS)
   ?? database/ (BANCO ÚNICO E COMPARTILHADO)
   ?? mod_login_e_matricula/
   ?? mod_area_do_aluno/
   ?? mod_area_do_professor/
   ?? mod_adminitrativo/
   ?? mod_eventos/
   ?? mod_instrumentos/
   ?? mod_lanchonete/

RESULTADO:
   ? Uma única definição de headers.h
   ? Um database compartilhado
   ? Um aluno pode logar ? comprar ? pegar instrumento
   ? Todos os dados sincronizados
```

**Não tente compilar módulos em CodeBlocks separadamente. Use APENAS VS Code com main.cpp como ponto de entrada único.**

---

## ?? Sobre o Projeto

Este é um **Sistema de Gerenciamento Escolar Modular** desenvolvido em C++. Funciona como um portal unificado tipo SIGAA, onde diferentes usuários (alunos, professores, administradores, funcionários) fazem login e acessam funcionalidades conforme seus cargos.

O sistema foi arquitetado em **módulos independentes** que se comunicam entre si, permitindo escalabilidade e manutenção facilitada.

---

## ??? Arquitetura: Relação Entre Módulos, Headers, Database e Inicializadores

### **Estrutura em 4 Camadas:**

```
??????????????????????????????????????????????????????????????????
?                    CAMADA 1: INTERFACE (Entrada)              ?
?  mod_login_e_matricula/ ? mod_area_do_aluno/ ? mod_.../ etc   ?
??????????????????????????????????????????????????????????????????
?                    CAMADA 2: SERVIÇOS (Lógica)                ?
?  servico_venda.cpp ? servico_emprestimo.cpp ? servico_login.cpp
??????????????????????????????????????????????????????????????????
?                    CAMADA 3: ESTRUTURAS (Dados em Memória)    ?
?                          headers.h                             ?
?         (Aluno, Professor, Turma, Evento, etc)                ?
??????????????????????????????????????????????????????????????????
?                CAMADA 4: PERSISTÊNCIA (Arquivo)               ?
?                          database/                             ?
?     (registros_notas.dat, saldos_cantina.dat, etc)            ?
??????????????????????????????????????????????????????????????????
```

### **Fluxo de Dados Completo:**

```
[Usuário Interage com Interface]
        ?
[entrada_aluno.cpp - CAMADA 1: INTERFACE]
        ?
[Chama servico_venda.cpp - CAMADA 2: LÓGICA]
        ?
[Usa estruturas em headers.h - CAMADA 3: MEMÓRIA RAM]
        ?
[Chama inicializadores em inicializacao/ - LEITURA/ESCRITA]
        ?
[Arquivo em database/ - CAMADA 4: PERSISTÊNCIA]
```

### **Explicação Detalhada de Cada Camada:**

#### **CAMADA 1: Interface (entrada_*.cpp)**
**O que faz:** Exibe menus, coleta entrada do usuário, chama serviços
- Exemplo: `mod_area_do_aluno/entrada_aluno.cpp`
- Responsabilidade: Menu do aluno, escolher opção
- NÃO calcula, apenas exibe e coleta dados

#### **CAMADA 2: Serviços (servico_*.cpp)**
**O que faz:** Implementa a lógica de negócio, processa dados, valida
- Exemplo: `mod_lanchonete/servico_venda.cpp`
- Responsabilidade: Validar compra, atualizar saldo, chamar inicializadores
- NÃO exibe (interface faz isso), apenas processa

#### **CAMADA 3: Estruturas (headers.h)**
**O que faz:** Define tipos de dados que vivem em RAM
- Exemplo: `struct Aluno { int id; float notas[4]; };`
- Responsabilidade: Definir campos, tipo de dado
- Existe APENAS enquanto o programa roda

#### **CAMADA 4: Persistência (database/)**
**O que faz:** Armazena dados em arquivo binário
- Exemplo: `database/registros_notas.dat`
- Responsabilidade: Guardar dados permanentemente
- Sobrevive ao encerramento do programa

---

## ?? Ciclo de Vida dos Dados: Exemplo Completo

### **Cenário: Aluno Comprando na Lanchonete**

```
???????????????????????????????????????????????????????????????
? 1?? STARTUP DO PROGRAMA                                      ?
???????????????????????????????????????????????????????????????
? database/saldos_cantina.dat (arquivo permanente)            ?
?    ?                                                         ?
? [fread() - inicializadores.cpp]                             ?
?    ?                                                         ?
? Aluno aluno; aluno.credito = 100.0; (em RAM)               ?
???????????????????????????????????????????????????????????????

???????????????????????????????????????????????????????????????
? 2?? USUÁRIO INTERAGE (CAMADA 1: INTERFACE)                  ?
???????????????????????????????????????????????????????????????
? entrada_aluno.cpp mostra menu:                              ?
?    "4 - Acessar Lanchonete"                                 ?
?    Usuário escolhe: comprar um refrigerante                ?
???????????????????????????????????????????????????????????????

???????????????????????????????????????????????????????????????
? 3?? LÓGICA DE NEGÓCIO (CAMADA 2: SERVIÇO)                   ?
???????????????????????????????????????????????????????????????
? servico_venda.cpp valida:                                  ?
?    ? Produto existe?                                        ?
?    ? Há estoque?                                            ?
?    ? Aluno tem saldo suficiente?                           ?
?    ? Se tudo ok: aluno.credito -= preco;                  ?
???????????????????????????????????????????????????????????????

???????????????????????????????????????????????????????????????
? 4?? ATUALIZAR ARQUIVO (PERSISTÊNCIA)                        ?
???????????????????????????????????????????????????????????????
? inicializadores.cpp chama:                                  ?
?    [fwrite() - salvar aluno atualizado]                    ?
?    ?                                                         ?
? database/saldos_cantina.dat (arquivo atualizado)           ?
?    Novo credito: 90.0 (persistido)                         ?
???????????????????????????????????????????????????????????????

???????????????????????????????????????????????????????????????
? 5?? CONFIRMAÇÃO AO USUÁRIO (CAMADA 1: INTERFACE)            ?
???????????????????????????????????????????????????????????????
? entrada_aluno.cpp exibe:                                    ?
?    "? Compra realizada!"                                    ?
?    "Saldo atual: R$ 90.00"                                  ?
???????????????????????????????????????????????????????????????

???????????????????????????????????????????????????????????????
? 6?? PRÓXIMO LOGIN                                            ?
???????????????????????????????????????????????????????????????
? Quando o aluno logar novamente:                             ?
?    [fread() - carrega credito = 90.0]                      ?
?    Dados persistiram! ?                                     ?
???????????????????????????????????????????????????????????????
```

---

## ?? Database: Estrutura de Arquivos

Cada arquivo em `database/` armazena um tipo específico de dado:

| Arquivo | Conteúdo | Estrutura em headers.h | Acessado Por |
|---------|----------|--------|-------------|
| `identidades.dat` | Usuários (ID, nome, CPF, senha) | `Identidade` | servico_login, entrada_cadastro |
| `registros_notas.dat` | Notas de alunos | `RegistroNotas` | entrada_aluno, servico_notas |
| `saldos_cantina.dat` | Crédito do aluno | `Saldo_Cantina` | entrada_aluno, servico_venda |
| `vendas_cantina.dat` | Histórico de compras | `Venda_Cantina` | entrada_aluno, servico_venda |
| `atrasos_instrumentos.dat` | Empréstimos ativos | `Atraso_Instrumento` | entrada_aluno, servico_emprestimo |
| `inventario_instrumentos.dat` | Instrumentos disponíveis | `Instrumento` | entrada_locador, servico_emprestimo |
| `turmas.dat` | Classes/disciplinas | `Class` | mod_adminitrativo, mod_professor |
| `historicos.dat` | Frequência e notas por turma | `historic_individual` | entrada_aluno, entrada_professor |
| `eventos.dat` | Eventos agendados | `EventoAgenda` | entrada_eventos, entrada_aluno |
| `produtos_cantina.dat` | Produtos à venda | `ProdutoCantina` | entrada_vendedor, servico_venda |

### **Inicializadores: A Ponte Entre Arquivo e Memória**

O arquivo `inicializacao/inicializadores.cpp` contém funções que:
- **Lêm** dados do arquivo para memória (fread)
- **Escrevem** dados da memória para arquivo (fwrite)

Exemplos (A IMPLEMENTAR):
```cpp
// ? LER do arquivo PARA memória
Aluno carregar_aluno_por_id(int id) {
    FILE *f = fopen("database/registros_notas.dat", "rb");
    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, f);
    fclose(f);
    return aluno;  // Retorna struct em RAM
}

// ? ESCREVER da memória PARA arquivo
void salvar_aluno(const Aluno &a) {
    FILE *f = fopen("database/registros_notas.dat", "ab");
    fwrite(&a, sizeof(Aluno), 1, f);
    fclose(f);
}
```

---

## ?? Sistema de Autenticação e Cadastro

### ?? **Credenciais de Teste**

Para testar o sistema, use:

| Campo | Valor | Descrição |
|-------|-------|-----------|
| **ID** | `0000` | ID fixo para teste |
| **Senha** | `0000` | Senha para teste |
| **Senha Padrão Usuarios** | `senha` | Padrão para novos cadastros |

**IMPORTANTE:** A função `validar_login()` em `mod_login_e_matricula/servico_login.cpp` atualmente está **hardcoded** (engessada). Você DEVE implementar a validação real com banco de dados.

---

## ?? Como Fazer Login

### **Passo 1: Abra o programa**
```bash
# Em VS Code:
Ctrl + Shift + B  # Compila
# Ou execute: main.exe
```

### **Passo 2: Tela de Login**
```
??????????????????????????????
Digite seu ID: 0000
Digite sua senha: 0000
??????????????????????????????
```

### **Passo 3: Resultado**
- Com os dados atuais, você será redirecionado para **ADMIN** (painel do administrador)
- Para testar outros cargos, edite `servico_login.cpp` (veja próxima seção)

---

## ?? Como Cadastrar e Testar Diferentes Tipos de Usuário

### **Opção 1: Modificar servico_login.cpp para Teste Rápido**

**Arquivo:** `mod_login_e_matricula/servico_login.cpp`

**Código Atual (Hardcoded - Sempre ADMIN):**

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
    std::strcpy(info.info_basica.nome, "João da Silva");
    std::strcpy(info.info_basica.cpf, "12345678901");
    info.info_basica.categoria = ALUNO;  // ?? E AQUI TAMBÉM
    std::strcpy(info.info_basica.especialidade, "");

    return info;
}
```

### **Tabela de Configuração Rápida:**

| Cargo | `chave_acesso` | `categoria` | `nome` | `especialidade` |
|-------|---|---|---|---|
| **ADMIN** | `ADMIN` | `ADMIN` | "Admin Teste" | "Administrador" |
| **ALUNO** | `ALUNO` | `ALUNO` | "João Silva" | "" |
| **PROFESSOR** | `PROFESSOR` | `PROFESSOR` | "Prof. Ana" | "Piano" |
| **VENDEDOR** | `VENDEDOR` | `VENDEDOR` | "Vendedor Teste" | "Vendedor" |
| **LOCADOR** | `LOCADOR` | `LOCADOR` | "Locador Teste" | "Locador" |

### **Passos para Testar um Cargo Diferente:**

1. **Abra:** `mod_login_e_matricula/servico_login.cpp`
2. **Procure** a função `validar_login()`
3. **Altere** `info.chave_acesso = TIPO_DESEJADO`
4. **Altere também** `info.info_basica.categoria = TIPO_DESEJADO`
5. **Atualize** `nome` e outros campos conforme tabela acima
6. **Salve:** Ctrl + S
7. **Compile:** Ctrl + Shift + B
8. **Execute:** main.exe

---

### **Opção 2: Cadastro de Novo Usuário (Quando Implementado)**

Quando a funcionalidade de cadastro estiver pronta:

1. Selecione **"Novo Cadastro"** no menu principal
2. Insira dados:
   - Nome completo
   - CPF
   - Email
   - Senha (padrão é "senha" se deixar em branco)
3. Escolha o tipo:
   - `ALUNO` - Aluno/Discente
   - `PROFESSOR` - Professor/Docente
   - `ADMIN` - Administrador
   - `VENDEDOR` - Gerencia lanchonete
   - `LOCADOR` - Gerencia instrumentos
4. Confirme

?? **AINDA NÃO IMPLEMENTADO** - Este módulo precisa ser criado

---

## ?? O Que Precisa Ser Feito (Roadmap Completo)

### **Fase 1: Inicializadores ? CRÍTICO - SEM ISSO NADA FUNCIONA**

Arquivo: `inicializacao/inicializadores.cpp`

**DEVE IMPLEMENTAR:**

```cpp
// ? FUNÇÕES DE LEITURA (arquivo ? memória)
Identidade carregar_identidade_por_id(int id);
Aluno carregar_aluno_por_id(int id);
Professor carregar_professor_por_id(int id);
Turma carregar_turma_por_codigo(char *codigo);
EventoAgenda carregar_evento_por_id(int id);
ProdutoCantina* carregar_todos_produtos(int &total);
Instrumento* carregar_todos_instrumentos(int &total);

// ? FUNÇÕES DE ESCRITA (memória ? arquivo)
void salvar_identidade(const Identidade &id);
void salvar_aluno(const Aluno &a);
void salvar_professor(const Professor &p);
void salvar_turma(const Turma &t);
void salvar_evento(const EventoAgenda &e);

// ? FUNÇÕES DE LISTAGEM
Identidade* carregar_todas_identidades(int &total);
Aluno* carregar_todos_alunos(int &total);
Professor* carregar_todos_professores(int &total);
Turma* carregar_todas_turmas(int &total);
```

**Por que é crítico?**
- ? Sem isso: Dados não persistem entre execuções
- ? Sem isso: Módulos não conseguem compartilhar dados
- ? Sem isso: Sistema não funciona

---

### **Fase 2: Validação Real em servico_login.cpp** ? IMPORTANTE

**Problema Atual:**
```cpp
// ? Sempre retorna ADMIN, ignora senha
info.chave_acesso = ADMIN;
```

**Solução:**
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

### **Fase 3: Completar Módulos Incompletos** ? EM PROGRESSO

#### **mod_area_do_aluno/ - 60% COMPLETO** ?

**Funcionando:**
- ? Menu principal funciona
- ? Funções `mostrarNotas()`, `mostrarExtratoLanchonete()` existem
- ? Sem erros de compilação

**Faltando:**
- ? Carregar dados reais do banco via inicializadores
- ? Opção 2 (Verificar Frequência) - função não existe
- ? Opção 3 (Visualizar Turmas) - função não existe
- ? Opção 5 (Visualizar Eventos) - função não existe

**O QUE FAZER:**
```cpp
// Dentro de janela_aluno():
Aluno aluno = carregar_aluno_por_id(info.id);  // ?? ADICIONAR
Emprestimo emprestimos[100];
int total = carregar_emprestimos(emprestimos);  // ?? ADICIONAR

// Depois descomentar as funções:
case 1: mostrarNotas(aluno); break;  // Vai exibir dados reais
case 4: mostrarExtratoLanchonete(aluno); break;
```

#### **mod_area_do_professor/ - 30% COMPLETO** ?

**Status:**
- ? Menu não funciona
- ? Opções vazias
- ? Sem integração com turmas/notas/frequência

**O QUE FAZER:**
1. Carregar turmas do professor
2. Implementar lançamento de notas
3. Implementar registro de frequência
4. Integrar com `mod_eventos/entrada_eventos.cpp`

#### **mod_adminitrativo/ - 40% COMPLETO** ?

**Status:**
- ? Menu principal funciona
- ? Opções sem funcionalidade
- ? Sem CRUD de usuários
- ? Sem CRUD de turmas

**O QUE FAZER:**
1. Criar usuários (chamar `entrada_cadastro.cpp`)
2. Editar usuários (atualizar em banco)
3. Deletar usuários (marcar como inativo)
4. Criar turmas
5. Editar turmas
6. Movimentar aluno entre turmas

#### **mod_eventos/ - 20% COMPLETO** ?

**Status:**
- ? `entrada_eventos.cpp` vazio
- ? `servico_inscricao_evento.cpp` NÃO EXISTE
- ? Sem criação/inscrição de eventos

**O QUE FAZER:**
1. Criar `servico_inscricao_evento.cpp` - compartilhado
2. Implementar `entrada_eventos.cpp` com menu
3. Validar vagas disponíveis
4. Registrar inscrições no banco

#### **mod_instrumentos/ - 40% COMPLETO** ?

**Status:**
- ? `entrada_locador.cpp` estrutura existe
- ? `servico_emprestimo.cpp` não implementado
- ? Sem CRUD de instrumentos
- ? Sem sistema de empréstimos

**O QUE FAZER:**
1. Completar `servico_emprestimo.cpp` - compartilhado
2. Validar limite de 5 itens por pessoa
3. Registrar datas de empréstimo/devolução
4. Controlar danos e perdas

#### **mod_lanchonete/ - 50% COMPLETO** ?

**Status:**
- ? `entrada_vendedor.cpp` estrutura existe
- ? `servico_venda.cpp` incompleto
- ? Sem CRUD de produtos
- ? Sem controle de estoque

**O QUE FAZER:**
1. Completar `servico_venda.cpp` - compartilhado
2. Implementar CRUD de produtos
3. Validar quantidade em estoque
4. Atualizar saldo do usuário após compra

---

## ?? Como Implementar um Módulo (Passo a Passo)

### **Exemplo: Completar mod_area_do_aluno/**

#### **Passo 1: Entender headers.h**
```cpp
// Abra headers.h e procure por:
struct Identidade { ... }      // Dados do aluno
struct RegistroNotas { ... }   // Notas do aluno
struct Saldo_Cantina { ... }   // Crédito do aluno
struct Atraso_Instrumento { ... }  // Empréstimos
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
    Aluno aluno = carregar_aluno_por_id(info.id);  // ?? CARREGAR
    
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

## ?? Campos Pendentes de Implementação

### **EDITOR DE SENHA - AINDA NÃO CRIADO** ?

A funcionalidade de **editar/mudar a senha** ainda precisa ser implementada.

**Onde deve existir:**
- `mod_login_e_matricula/entrada_cadastro.cpp` (usuário muda sua própria senha)
- Ou perfil do usuário (cada um muda sua senha)

**Código sugerido:**
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

## ?? Fluxo Principal de Funcionamento

```
???????????????????????????????????????????????????????????????
? 1. main.cpp inicia                                          ?
???????????????????????????????????????????????????????????????
                              ?
                ??????????????????????????????
                ? servico_login.cpp          ?
                ? Autentica usuário          ?
                ? Retorna person_role        ?
                ??????????????????????????????
                              ?
        ????????????????????????????????????????????????????????????????
        ?         ?           ?           ?             ?              ?
        ?         ?           ?           ?             ?              ?
     ADMIN    ALUNO      PROFESSOR     VENDEDOR      LOCADOR       EVENTOS
        ?         ?           ?           ?             ?              ?
        ?         ?           ?           ?             ?              ?
   entrada_   entrada_   entrada_   entrada_   entrada_           entrada_
   admin      aluno      professor  vendedor   locador            eventos
        ?         ?           ?           ?             ?              ?
        ?         ?????????????????????????????????????????????????????
        ?               ?           ?           ?             ?
        ?               ?           ?           ?             ?
       SERVIÇOS COMPARTILHADOS
       ? servico_venda (todos compram)
       ? servico_emprestimo (todos pegam instrumento)
       ? servico_inscricao_evento (todos se inscrevem)
```

---

## ?? Estrutura de Diretórios Esperada

```
Projeto-2-Prog-2-2025.2/
?? main.cpp                                 # Ponto de entrada
?? headers.h                                # Estruturas centralizadas
?? Readme.md                                # Este arquivo
?? database/                                # Camada de persistência
?  ?? identidades.dat
?  ?? registros_notas.dat
?  ?? saldos_cantina.dat
?  ?? vendas_cantina.dat
?  ?? atrasos_instrumentos.dat
?  ?? (outros .dat)
?? inicializacao/
?  ?? inicializadores.cpp                   # CRÍTICO: Lê/escreve arquivo
?  ?? inicializadores.h
?? mod_login_e_matricula/
?  ?? servico_login.cpp / .h                # ? Validação hardcoded
?  ?? entrada_cadastro.cpp / .h             # ? Incompleto
?  ?? servico_cadastros.cpp / .h
?  ?? inicializadores_de_tabela.cpp / .h
?? mod_adminitrativo/
?  ?? entrada_admin.cpp / .h                # ? Incompleto
?  ?? (bin/, obj/ - ignorar)
?? mod_area_do_aluno/
?  ?? entrada_aluno.cpp / .h                # ? 60% - falta dados
?  ?? (bin/, obj/ - ignorar)
?? mod_area_do_professor/
?  ?? entrada_professor.cpp / .h            # ? 30% - muito incompleto
?  ?? (bin/, obj/ - ignorar)
?? mod_eventos/
?  ?? entrada_eventos.cpp / .h              # ? 20% - vazio
?  ?? (servico_inscricao_evento.cpp - não existe)
?? mod_instrumentos/
?  ?? entrada_locador.cpp / .h              # ? 40% - sem serviço
?  ?? (servico_emprestimo.cpp - não existe)
?? mod_lanchonete/
   ?? entrada_vendedor.cpp / .h             # ? 50% - serviço incompleto
   ?? (servico_venda.cpp - parcial)
```

---

## ?? Estruturas de Dados Principais (headers.h)

### **Identidade**
```cpp
struct Identidade {
    int id;
    char nome[50];
    char cpf[12];
    char senha[20];  // PADRÃO: "senha"
    char email[50];
    person_role categoria;  // ALUNO, PROFESSOR, ADMIN, etc
    char especialidade[30];  // Ex: "Piano", "Canto"
    int ativo;
};
```

### **Aluno** (em memória)
```cpp
struct Aluno {
    int id;
    char nome[50];
    float notas[4];
    float credito;  // Crédito na lanchonete
    Compra compras[20];  // Histórico de compras
    int quantCompras;
    Instrumento instrumentos[10];
    int quantInstrumentos;
};
```

### **Class (Turma)**
```cpp
struct Class {
    char codigo[10];  // Ex: "PROG001"
    char nome[50];    // Ex: "Programação II"
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
    float notas[10];  // Até 10 avaliações
    int quant_notas;
};
```

---

## ?? Autenticação: Login + Senha Padrão

### **Credenciais para Teste:**
```
ID:     0000
Senha:  0000
```

### **Senha Padrão para Novos Usuários:**
```
"senha"
```

### **Como Funciona Agora:**
1. Usuário digita qualquer ID e senha
2. Função `validar_login()` **ignora** tudo
3. Sempre retorna ADMIN
4. ? **Isso está errado!**

### **Como Deveria Funcionar:**
1. Usuário digita ID e senha
2. Carregar identidade do banco: `Identidade id = carregar_identidade_por_id(id_usuario);`
3. Comparar senhas: `if (strcmp(id.senha, senha) == 0)`
4. Se correto: retornar dados do usuário
5. Se errado: retornar `status = INVALIDO`

---

## ?? Compilação e Execução

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

## ?? Observações Importantes

- **Máximo 50 alunos por turma**
- **Máximo 100 aulas por turma**
- **Máximo 10 eventos por turma**
- **Máximo 5 itens emprestados por pessoa**
- **Máximo 4 horários por semana**
- **Cada função DEVE validar dados de entrada**
- **Módulos devem ser independentes e reutilizáveis**
- **SEMPRE consultar `headers.h` para estruturas de dados**
- **Nunca duplicar estruturas em múltiplos arquivos**

---

## ?? Arquivo utilidades.cpp

**O que contém:**
- `void limpar_tela()` - Limpa console (multiplataforma)
- `const char* enumerado_para_texto(funcao_pessoa f)` - Converte enum para string
- `void generate_random_cpf(char *cpf_buffer)` - Gera CPF aleatório
- `void generate_random_name(char *name_buffer)` - Gera nome aleatório

---

## ?? Codificação de Caracteres (Acentos)

Para que acentos funcionem:

**VS Code (`settings.json`):**
```json
{
    "files.encoding": "iso88591",
    "files.autoGuessEncoding": false
}
```

**Código C++:**
```cpp
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese");
    // ... resto do código
}
```

---

## ?? Próximos Passos

1. **?? LEIA headers.h** - Entenda as estruturas
2. **?? Implemente inicializadores** - Função crítica
3. **?? Teste o login** - Modifique para diferentes cargos
4. **?? Complete mod_area_do_aluno** - Carregue dados do banco
5. **?? Implemente os outros módulos** - Siga o mesmo padrão

---

**Versão:** 2025.2  
**Data:** Fevereiro 2026  
**Linguagem:** C++  
**Status:** Em Desenvolvimento - Aguardando Implementação de Inicializadores

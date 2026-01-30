# ⚠️ LEIA O ARQUIVO headers.h ⚠️

Este arquivo contém todas as estruturas de dados fundamentais do projeto. Comece por lá!

##### ESSE MD FOI GERADO POR IA

##### CRIE A PASTA REFERENTE AO SEU MODULO, NOTE QUE TRÊS PASTAS FORAM CRIADAS
---

## 📋 Sobre o Projeto

Este é um **Sistema de Gerenciamento Escolar** desenvolvido em C++. Funciona como um portal unificado tipo SIGAA, onde diferentes usuários (alunos, professores, administradores, funcionários) fazem login e acessam funcionalidades conforme seus cargos.

---

## 🏗️ Estrutura do Projeto

### **Diretórios Principais:**

- **`administrative_sector/`** - Portal do Administrativo/Administrador
  - Interface e serviços para gestão administrativa
  - Gerenciamento de usuários, turmas, eventos

- **`student_area/`** - Portal do Discente/Aluno
  - Área específica para alunos acessarem suas informações

- **`teachers_area/`** - Portal do Docente/Professor
  - Área específica para professores
  - Gerenciamento de turmas e notas

- **`database/`** - Camada de Dados
  - Persistência em banco de dados
  - Leitura/escrita de informações

- **`bin/` e `obj/`** - Arquivos Compilados
  - Executáveis e objetos gerados pela compilação

---

## 👥 Tipos de Usuários (Cargos)

O sistema suporta os seguintes cargos com diferentes permissões:

1. **ALUNO** - Discente
2. **PROFESSOR** - Docente
3. **ADMIN** - Administrador
4. **VENDEDOR** - Gerencia cantina
5. **LOCADOR** - Gerencia empréstimos
6. **BIBLIOTECÁRIO** - Cuida da biblioteca e organiza livros

---

## 📦 Estruturas de Dados Principais (em `headers.h`)

### **Identidade**
Representa um usuário no sistema:
- ID, nome, CPF, senha, email
- Categoria/cargo
- Especialidade (para professores: Piano, Canto, etc.)

### **Class (Turma)**
Representa uma turma/disciplina:
- Código, ano letivo, nome (ex: "Violão I")
- Professor responsável e lista de alunos (até 50)
- Horários (até 4 por semana)
- Eventos (provas, atividades, etc.)
- Aulas ministradas (até 100)

### **historic_individual (Histórico)**
Registro acadêmico de um aluno em uma turma:
- Frequência (presença/ausência em cada aula)
- Notas obtidas (até 10 notas)
- Status final (ABERTO, APROVADO, REPROVADO)

### **RecursosIndividuais (Recursos)**
Gerencia recursos de um usuário:
- Saldo (para cantina)
- Itens emprestados (livros/instrumentos, máximo 5)
- Ativo/Inativo

### **ProdutoCantina**
Produtos vendidos na cantina:
- ID, nome, preço
- Quantidade em estoque

### **Biblioteca**
Livros/instrumentos da biblioteca:
- ID, título, quantidade total
- Controle de disponibilidade

### **EventoAgenda**
Eventos agendados na escola:
- Nome, data, vagas totais
- Lista de inscritos
- Status (agendado/cancelado)

### **RegistroNotas**
Registro de notas de um aluno:
- Aluno, turma, notas lançadas

---

## 🔐 Sistema de Autenticação

O login verifica o `person_role` (cargo) do usuário e libera acessos conforme:
- Portal do Discente (ALUNO)
- Portal do Docente (PROFESSOR)
- Portal do Admin (ADMIN)
- Portais específicos (VENDEDOR, LOCADOR, BIBLIOTECÁRIO)

---

## 📁 Arquivos Principais

- **`headers.h`** - Definições de todas as estruturas
- **`main.cpp`** - Arquivo principal
- **`administrative_sector/services.h`** e **`.cpp`** - Funções administrativas
- **`administrative_sector/interface.cpp`** - Interface do admin

---

## 🔧 Compilação

Use o build task configurado:
```bash
g++.exe -fdiagnostics-color=always -g ${file} -o ${fileDirname}\\${fileBasenameNoExtension}.exe
```

---

## 📝 Observações

- Máximo 50 alunos por turma
- Máximo 100 aulas por turma
- Máximo 10 eventos por turma
- Máximo 5 itens emprestados por pessoa
- Máximo 4 horários por semana

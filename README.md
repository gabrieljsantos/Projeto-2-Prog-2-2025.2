# 🎵 Escola de Música Benicasa

Projeto desenvolvido para a disciplina **Programação II**, do professor **Alcides Xavier Benicasa**.

Trata-se de um sistema de gestão para uma escola de música, desenvolvido em C++.

---

## 🛠 Ambiente de Desenvolvimento

- IDE: Code::Blocks  
- Compilador: MinGW64  
- Linguagem: C++  
- Interface: Biblioteca PDCurses  

O projeto deve ser executado no Code::Blocks utilizando o compilador MinGW64 corretamente configurado.

---

## 📌 Descrição do Sistema

O sistema realiza o gerenciamento completo de uma escola de música, sendo dividido nos seguintes módulos:

- Login e Matrícula
- Lanchonete
- Empréstimo de Instrumentos
- Administração
- Professores
- Eventos
- Alunos

Existe uma `main` responsável por integrar todos os módulos.

A comunicação entre os módulos é feita através da leitura e escrita de arquivos binários (.dat), garantindo persistência dos dados.

---

## 🔐 Módulo Login e Matrícula

Este módulo é responsável por:

- Cadastro de professores
- Cadastro de alunos
- Sistema de login
- Controle de status de professores

### 📌 Regras importantes:

- Professores só podem realizar login após um administrador alterar seu status para **ativo**.
- O sistema gerencia usuários por ID incremental.
- ID base inicial: **20260000**
- Cada novo cadastro incrementa +1:
  - 20260001
  - 20260002
  - 20260003
  - ...

### 👤 Acesso Administrador

- ID: 20260001  
- Senha: 1234567  

O sistema possui um mecanismo simples de criptografia de senhas.

---

## 🖥 Prints do Sistema

### Tela Inicial
<img width="1428" height="704" alt="Screenshot 2026-02-22 153846" src="https://github.com/user-attachments/assets/92ea72dd-e452-48a1-b2b4-023283e22298" />

### Login/Cadastro
<img width="1449" height="705" alt="Screenshot 2026-02-22 153859" src="https://github.com/user-attachments/assets/69fbc48e-1e62-4917-8c82-f26544655088" />

### Tela do aluno
<img width="1448" height="704" alt="Screenshot 2026-02-22 153956" src="https://github.com/user-attachments/assets/de0dd42e-a976-4658-ada4-f40c7da0af53" />

---

## 📂 Estrutura do Projeto

- main.cpp → Integração geral do sistema  
- Módulos separados por responsabilidade  
- Arquivos `.dat` → Persistência de dados em formato binário  

---

## 📖 Observações

Este projeto foi desenvolvido com foco em aplicação prática de conceitos de:

- Manipulação de Arquivos Binários
- Modularização
- Recursividade
- Estruturação de Sistemas em C++

---

Desenvolvido como atividade acadêmica para a disciplina de Programação II

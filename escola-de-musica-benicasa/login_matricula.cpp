#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "login_matricula.h"

namespace Login_mat {

//======================================================================
// Arquivos
//======================================================================
    // Feito por Luiz Felipe
    void inicializarArquivoUsuario() {
        if (verificarArquivoExistente(ARQUIVO_USUARIOS))
            return;

        Usuario usuarioVazio = inicializarUsuarioVazio();

        ofstream outUsuarios(ARQUIVO_USUARIOS, ios::out | ios::binary);

        if (outUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        for (int i = 0; i < 100; i++)
            outUsuarios.write((const char*)(&usuarioVazio), sizeof(Usuario));

        outUsuarios.close();
    }

    // Feito por Luiz Felipe
    void inicializarArquivoAluno() {
        if (verificarArquivoExistente(ARQUIVO_ALUNOS))
            return;

        Aluno alunoVazio = inicializarAlunoVazio();

        ofstream outAlunos(ARQUIVO_ALUNOS, ios::out | ios::binary);

        if (outAlunos.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        for (int i = 0; i < 100; i++)
            outAlunos.write((const char*)(&alunoVazio), sizeof(Aluno));

        outAlunos.close();
    }

    // Feito por Clara
    void inicializarArquivoProfessor() {
        if (verificarArquivoExistente(ARQUIVO_PROFESSORES))
            return;

        Professor professorVazio = inicializarProfessorVazio();

        ofstream outProfessores(ARQUIVO_PROFESSORES, ios::out | ios::binary);

        if (outProfessores.fail()) {
            cout << "A abertura do aquivo falhou!\n";
            exit(1);
        }

        for (int i = 0; i < 100; i++)
            outProfessores.write((const char*)(&professorVazio), sizeof(Professor));

        outProfessores.close();
    }

    // Feito por Luiz Felipe
    void inicializarArquivoAdmin() {
        if (verificarArquivoExistente(ARQUIVO_ADMINISTRADORES))
            return;

        Admin adminVazio = inicializarAdminVazio();

        ofstream outAdmin;
        outAdmin.open(ARQUIVO_ADMINISTRADORES, ios::out|ios::binary);

        if(outAdmin.fail()){
            cout << "A abertura do aquivo falhou!\n";
            exit(1);
        }

        for(int i=0; i<100; i++) {
            outAdmin.write((const char*)(&adminVazio), sizeof(Admin));
        }

        outAdmin.close();
    }

    void inicializarArquivos() {
        inicializarArquivoUsuario();
        inicializarArquivoAluno();
        inicializarArquivoProfessor();
        inicializarArquivoAdmin();
    }

    bool verificarArquivoExistente(string nomeArquivo) {
        ifstream teste(nomeArquivo, ios::binary);
        if (teste) {
            teste.close();
            return true;
        }

        return false;
    }

//======================================================================
// Cadastro
//======================================================================

    int realizarCadastro() {
        saida_menu saidaCategoria;
        int idNovoUsuario = 0;

        saidaCategoria = exibirSelecionarCategoria();

        switch (saidaCategoria.indice_da_opcao) {
            case 0: {
                idNovoUsuario = realizarCadastroAluno();
                break;
            }
            case 1: {
                idNovoUsuario = realizarCadastroProfessor();
                break;
            }
        }

        return idNovoUsuario;
    }

    //Feito por Jeanderson, auxiliado por Luiz Felipe
    int realizarCadastroAluno() {
        Aluno* alunoPtr = new Aluno;
        *alunoPtr = inicializarAlunoVazio();
        int novoId = gerarNovoId();
        (*alunoPtr).base.id = novoId;
        (*alunoPtr).base.ativo = true;

        string titulo = "CADASTRO ALUNO";

        saida_entrada_texto saidaInput = digiteNome(titulo);
        if (!saidaInput.confirmado) return 0;
        strcpy((*alunoPtr).base.nome, saidaInput.valor.c_str());

        saidaInput = digiteEmail(titulo);
        if (!saidaInput.confirmado) return 0;
        strcpy((*alunoPtr).base.email, saidaInput.valor.c_str());

        saidaInput = digiteSenha(titulo);
        if (!saidaInput.confirmado) return 0;
        strcpy((*alunoPtr).base.senha, saidaInput.valor.c_str());

        salvarAluno(*alunoPtr);

        delete alunoPtr;
        alunoPtr = nullptr;

        return novoId;
    }

    // Feito por Clara
    int realizarCadastroProfessor(){
        Professor novoProfessor = inicializarProfessorVazio();
        int novoId = gerarNovoId();
        novoProfessor.base.id = novoId;

        cout << "\n──────── CADASTRO PROFESSOR ────────\n";
        cout << "\nNome: ";
        cin.getline(novoProfessor.base.nome, 100);

        cout << "Email: ";
        cin.getline(novoProfessor.base.email, 100);

        cout << "Senha: ";
        cin.getline(novoProfessor.base.senha, 30);

        cout << "Disciplina: ";
        cin.getline(novoProfessor.disciplina, 50);

        salvarProfessor(novoProfessor);

        return novoId;
    }

    void realizarCadastroAdmin() {
        Usuario base = {20260001, true, "admin", "admin@gmail.com", "1234567", ADMINISTRADOR, false};

        Admin* adminPtr = new Admin;
        (*adminPtr).base = base;

        salvarAdmin(*adminPtr);

        delete adminPtr;
        adminPtr = nullptr;
    }


//======================================================================
// Login
//======================================================================
    bool realizarLogin(Usuario &usuario) {
        bool sucesso = false;
        Usuario** usuarios = new Usuario*[2];
        for (int i = 0; i < 2; i++) {
            usuarios[i] = new Usuario;
        }

        string titulo = "LOGIN";

        saida_entrada_texto saidaInput = digiteId(titulo);
        if (saidaInput.confirmado)
            (*usuarios[0]).id = stoi(saidaInput.valor);
        else
            (*usuarios[0]).id = 0;

        saidaInput = digiteSenha(titulo);
        if (saidaInput.confirmado)
            strcpy((*usuarios[0]).senha, saidaInput.valor.c_str());

        if (verificarUsuarioExistente((*usuarios[0]).id)) {

            *usuarios[1] = lerUsuario((*usuarios[0]).id);

            if (strcmp((*usuarios[0]).senha, (*usuarios[1]).senha) == 0 && (*usuarios[1]).ativo) {
                usuario = *usuarios[1];
                usuario.logado = true;
                sucesso = true;
            }
        }

        for (int i = 0; i < 2; i++)
            delete usuarios[i];

        delete[] usuarios;
        usuarios = nullptr;

        return sucesso;
    }


//======================================================================
// Salvar
//======================================================================

    void salvarUsuario(Usuario usuario) {
        int posicao = adquirirPosicaoDoId(usuario.id);

        fstream arquivo;

        arquivo.open(ARQUIVO_USUARIOS, ios::in | ios::out | ios::binary);

         if (arquivo.fail()){
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((posicao - 1) * sizeof(Usuario));
        arquivo.write((const char*)(&usuario), sizeof(Usuario));
        arquivo.close();
    }

    void salvarAluno(Aluno aluno) {
        salvarUsuario(aluno.base);
        int posicao = adquirirPosicaoDoId(aluno.base.id);

        fstream arquivo;

        arquivo.open(ARQUIVO_ALUNOS, ios::in | ios::out| ios::binary);

        if (arquivo.fail()) {
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((posicao - 1) * sizeof(Aluno));
        arquivo.write((const char*)(&aluno), sizeof(Aluno));
        arquivo.close();
    }

    void salvarProfessor(Professor professor) {
        salvarUsuario(professor.base);
        int posicao = adquirirPosicaoDoId(professor.base.id);

        fstream arquivo;

        arquivo.open(ARQUIVO_PROFESSORES, ios::in | ios::out| ios::binary);

        if (arquivo.fail()) {
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((posicao - 1) * sizeof(Professor));
        arquivo.write((const char*)(&professor), sizeof(Professor));
        arquivo.close();
    }

    void salvarAdmin(Admin admin) {
        salvarUsuario(admin.base);
        int posicao = adquirirPosicaoDoId(admin.base.id);

        fstream arquivo;

        arquivo.open(ARQUIVO_ADMINISTRADORES, ios::in | ios::out| ios::binary);

        if (arquivo.fail()) {
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((posicao - 1) * sizeof(Admin));
        arquivo.write((const char*)(&admin), sizeof(Admin));
        arquivo.close();
    }


//======================================================================
// Gerar novo ID
//======================================================================

    int gerarNovoId() {
        return verificarUltimoIdUsuario() + 1;
    }

    int adquirirPosicaoDoId(int id) {
        return id - 20260000;
    }

    //Feito por Luiz Felipe
    int verificarUltimoIdUsuario() {
        int ultimoId = 20260000;
        Usuario usuario;
        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        while (inUsuarios.read((char*)(&usuario), sizeof(Usuario)))
            if (usuario.id > ultimoId)
                ultimoId = usuario.id;

        inUsuarios.close();

        return ultimoId;
    }


//======================================================================
// Ler X (usuarios, alunos, professores ou admin)
//======================================================================
    // Feito por Luiz Felipe
    Usuario lerUsuario(int id) {
        Usuario usuario = inicializarUsuarioVazio();
        int posicao = adquirirPosicaoDoId(id);

        if (id > verificarUltimoIdUsuario())
            return usuario;

        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo de entrada falhou.\n";
            exit(1);
        }

        inUsuarios.seekg((posicao - 1) * sizeof(Usuario));
        inUsuarios.read((char*)(&usuario), sizeof(Usuario));
        inUsuarios.close();

        return usuario;
    }

    // Feito por Luiz Felipe
    Aluno lerAluno(int id) {
        Aluno aluno = inicializarAlunoVazio();
        int posicao = adquirirPosicaoDoId(id);

        ifstream inAlunos;
        inAlunos.open(ARQUIVO_ALUNOS, ios::in | ios::binary);

        if (inAlunos.fail()) {
            cout << "A abertura do arquivo de entrada falhou.\n";
            exit(1);
        }

        inAlunos.seekg((posicao - 1) * sizeof(Aluno));
        inAlunos.read((char*)(&aluno), sizeof(Aluno));
        inAlunos.close();

        return aluno;
    }

    // Feito por Clara
    Professor lerProfessor(int id) {
        Professor professor = inicializarProfessorVazio();
        int posicao = adquirirPosicaoDoId(id);

        ifstream inProfessores;
        inProfessores.open(ARQUIVO_PROFESSORES, ios::in | ios::binary);

        if(inProfessores.fail()){
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        inProfessores.seekg((posicao - 1)* sizeof(Professor));
        inProfessores.read((char*)(&professor), sizeof(Professor));
        inProfessores.close();

        return professor;
    }

//======================================================================
// Atualizar
//======================================================================
    void atualizar(int id, Professor professor) {
        if (!verificarUsuarioExistente(id)) {
            cout << "O usuário não existe";
            return;
        }

        professor.base.id = id;
        professor.base.categoria = PROFESSOR;

        salvarProfessor(professor);
        cout << "Usuário atualizado com sucesso!";
    }

    void atualizar(int id, Aluno aluno) {
        if (!verificarUsuarioExistente(id)) {
            cout << "O usuário não existe";
            return;
        }

        aluno.base.id = id;
        aluno.base.categoria = ALUNO;

        salvarAluno(aluno);
        cout << "Usuário atualizado com sucesso!";
    }


//======================================================================
// Verificar quantos X (usuarios, alunos, professores ou admin)
//======================================================================
    // Feito por Luiz Felipe
    int verificarQuantosUsuarios() {
        int idUltimoUsuario = verificarUltimoIdUsuario();
        return adquirirPosicaoDoId(idUltimoUsuario);
    }

    // Feito por Luiz Felipe
    int verificarQuantosAlunos() {
        int contador = 0;
        Aluno aluno;
        ifstream inAlunos;
        inAlunos.open(ARQUIVO_ALUNOS, ios::in | ios::binary);

        if (inAlunos.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        while (inAlunos.read((char*)(&aluno), sizeof(Aluno)))
            if (aluno.base.id != 20260000)
                contador++;

        inAlunos.close();

        return contador;
    }

    // Feito por Clara
    int verificarQuantosProfessores() {
        int contador = 0;
        Professor professor;
        ifstream inProfessores;
        inProfessores.open(ARQUIVO_PROFESSORES, ios::in | ios::binary);

        if(inProfessores.fail()){
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        while (inProfessores.read((char*)(&professor), sizeof(Professor))) {
            if(professor.base.id != 20260000){
                contador++;
            }
        }
        inProfessores.close();
        return contador;
    }


//======================================================================
// Verificar se usuario existe
//======================================================================
    //Feito por Jeanderson
    bool verificarUsuarioExistente(int id) {
        int posicao = adquirirPosicaoDoId(id);

        if (id > verificarUltimoIdUsuario() || id <= 0)
            return false;

        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary );

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            return false;
        }

        inUsuarios.seekg((posicao - 1) * sizeof(Usuario));

        Usuario verificaId = inicializarUsuarioVazio();

        if (inUsuarios.read((char *)&verificaId, sizeof(Usuario))){
            inUsuarios.close();
            return verificaId.id == id;
        }

        inUsuarios.close();
        return false;
    }

    //Feito por Jhones
    bool verificarUsuarioExistente(const char nome[100]) {
        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            return false;
        }

        Usuario u;
        while (inUsuarios.read((char*)(&u), sizeof(Usuario))) {
            // ignora registros vazios
            if (u.id != 20260000) {
                if (strncmp(u.nome, nome, 100) == 0) {
                    inUsuarios.close();
                    return true;
                }
            }
        }

        inUsuarios.close();
        return false;
    }



//======================================================================
// Inicializadores de variaveis
//======================================================================
    //Feito por Luiz Felipe
     Usuario inicializarUsuarioVazio() {
        Usuario usuarioVazio = {20260000, false, "", "", "", NENHUMA, false};
        return usuarioVazio;
    }

    // Feito por Luiz Felipe
    Aluno inicializarAlunoVazio() {
        Aluno alunoVazio;
        alunoVazio.base = inicializarUsuarioVazio();
        alunoVazio.base.categoria = ALUNO;
        alunoVazio.saldo = 0.0;
        alunoVazio.notas[0] = 0;
        alunoVazio.notas[1] = 0;
        alunoVazio.faltas = 0;
        alunoVazio.instrumento = 0;
        alunoVazio.idInstrumento = 0;
        alunoVazio.turma = 0;
        return alunoVazio;
    }

    // Feito por Clara
    Professor inicializarProfessorVazio(){
        Professor professorVazio;
        professorVazio.base = inicializarUsuarioVazio();
        professorVazio.base.categoria = PROFESSOR;
        professorVazio.saldo = 0.0;
        professorVazio.disciplina[0] = '\0';
        for (int i = 0; i < 5; i++) 
            professorVazio.turmas[i] = 0;
        return professorVazio;
    }

    //Feito por Luiz Felipe
    Admin inicializarAdminVazio() {
        Admin adminVazio;
        adminVazio.base = inicializarUsuarioVazio();
        return adminVazio;
    }

//======================================================================
// Interface Grafica
//======================================================================
    saida_menu exibirSelecionarCategoria() {
        const int qtdOpcoes = 2;
        string opcoes[qtdOpcoes] = {"Aluno", "Professor"};
        ConfigMenu config;
        config.titulo = "REALIZAR CADASTRO";
        config.caminho = "MENU DE ACESSO -> REALIZAR CADASTRO";
        return interface_para_menu(qtdOpcoes, opcoes, config);
    }

    saida_entrada_texto digiteId(string titulo) {
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.descricao = "Digite o seu ID";
        config.caminho = titulo + " -> ID";
        config.label = "ID: ";
        config.tipo_entrada = TIPO_NUMERO;
        config.tamanho_maximo = 8;
        return interface_para_entrada_texto(config);
    }

    saida_entrada_texto digiteNome(string titulo) {
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.descricao = "Digite o seu nome";
        config.caminho = titulo + " -> NOME";
        config.label = "Nome: ";
        config.tipo_entrada = TIPO_TEXTO;
        config.tamanho_maximo = 100;
        return interface_para_entrada_texto(config);
    }

    saida_entrada_texto digiteEmail(string titulo) {
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.descricao = "Digite o seu e-mail";
        config.caminho = titulo + " -> EMAIL";
        config.label = "Email: ";
        config.tipo_entrada = TIPO_EMAIL;
        config.tamanho_maximo = 100;
        return interface_para_entrada_texto(config);
    }

    saida_entrada_texto digiteSenha(string titulo) {
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.caminho = titulo + " -> SENHA";
        config.descricao = "Digite o sua senha";
        config.label = "Senha: ";
        config.tipo_entrada = TIPO_SENHA;
        config.tamanho_maximo = 30;
        return interface_para_entrada_texto(config);
    }

}

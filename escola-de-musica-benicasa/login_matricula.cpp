#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "login_matricula.h"
#include "headers.h"

namespace Login_mat {

//======================================================================
// Arquivos
//======================================================================
    // Feito por Luiz Felipe
    void inicializarArquivoUsuario() {
        Usuario usuarioVazio = inicializarUsuarioVazio();

        ofstream outUsuarios;
        outUsuarios.open(ARQUIVO_USUARIOS, ios::out | ios::binary);

        if (outUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        for (int i = 0; i < 100; i++)
            outUsuarios.write((const char *)(&usuarioVazio), sizeof(Usuario));

        outUsuarios.close();
    }

    // Feito por Luiz Felipe
    void inicializarArquivoAluno() {
        Aluno alunoVazio = inicializarAlunoVazio();

        ofstream outAlunos;
        outAlunos.open(ARQUIVO_ALUNOS, ios::out | ios::binary);

        if (outAlunos.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        for (int i = 0; i < 100; i++)
            outAlunos.write((const char *)(&alunoVazio), sizeof(Aluno));

        outAlunos.close();
    }

    // Feito por Clara
    void inicializarArquivoProfessor() {
        Professor professorVazio = inicializarProfessorVazio();

        ofstream outProfessores;
        outProfessores.open(ARQUIVO_PROFESSORES, ios::out|ios::binary);

        if(outProfessores.fail()){
            cout << "A abertura do aquivo falhou!\n";
            exit(1);
        }

        for(int i=0; i<100; i++) {
            outProfessores.write((const char*)(&professorVazio), sizeof(Professor));
        }

        outProfessores.close();
    }

    void inicializarArquivos() {
        inicializarArquivoUsuario();
        inicializarArquivoAluno();
        inicializarArquivoProfessor();
    }

//======================================================================
// Cadastro
//======================================================================

    void realizarCadastro() {
        int opcaoCategoria = -1;
        int idNovoUsuario;

        cout << "\n┌─────────── CATEGORIA ───────────┐\n";
        cout << "│                                 │\n";
        cout << "│ [0] Aluno                       │\n";
        cout << "│ [1] Professor                   │\n";
        cout << "│                                 │\n";
        cout << "└─────────────────────────────────┘\n";

        do {
            cout << "\nEscolha: ";
            cin >> opcaoCategoria;
            cin.ignore();
        } while (opcaoCategoria < 0 || opcaoCategoria > 2);

        switch (opcaoCategoria) {
            case 0: {
                idNovoUsuario = realizarCadastroAluno();
                break;
            }
            case 1: {
                idNovoUsuario = realizarCadastroProfessor();
                break;
            }
        }

        cout << "\n┌──────────── SUCESSO ─────────────┐\n";
        cout << "│                                  │\n";
        cout << "│ Cadastro realizada com sucesso!  │\n";
        cout << "│ ID: " << left << setw(29) << idNovoUsuario << "│\n";
        cout << "│                                  │\n";
        cout << "└──────────────────────────────────┘\n";
    }

    //Feito por Jeanderson, auxiliado por Luiz Felipe
    int realizarCadastroAluno() {
        Aluno* alunoPtr = new Aluno;
        *alunoPtr = inicializarAlunoVazio();
        int novoId = gerarNovoId();
        (*alunoPtr).base.id = novoId;

        cout << "\n───────── CADASTRO ALUNO ─────────\n";
        cout << "\nNome: ";
        cin.getline((*alunoPtr).base.nome, 100);

        cout << "Email: ";
        cin.getline((*alunoPtr).base.email, 100);

        cout << "Senha: ";
        cin.getline((*alunoPtr).base.senha, 30);

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


//======================================================================
// Login
//======================================================================
    bool realizarLogin(Usuario &usuario) {
        bool sucesso = false;
        Usuario** usuarios = new Usuario*[2];
        for (int i = 0; i < 2; i++) {
            usuarios[i] = new Usuario;
        }

        cout << "\n───────────── LOGIN ──────────────\n";
        cout << "\nID: ";
        cin >> (*usuarios[0]).id;
        cin.ignore();
        cout << "Senha: ";
        cin.getline((*usuarios[0]).senha, 30);

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
        fstream arquivo;

        arquivo.open(ARQUIVO_USUARIOS, ios::in | ios::out | ios::binary);

         if (arquivo.fail()){
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((usuario.id - 1) * sizeof(Usuario));
        arquivo.write((const char*)(&usuario), sizeof(Usuario));
        arquivo.close();
    }

    void salvarAluno(Aluno aluno) {
        salvarUsuario(aluno.base);

        fstream arquivo;

        arquivo.open(ARQUIVO_ALUNOS, ios::in | ios::out| ios::binary);

        if (arquivo.fail()) {
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((aluno.base.id - 1) * sizeof(Aluno));
        arquivo.write((const char*)(&aluno), sizeof(Aluno));
        arquivo.close();
    }

    void salvarProfessor(Professor professor) {
        salvarUsuario(professor.base);

        fstream arquivo;

        arquivo.open(ARQUIVO_PROFESSORES, ios::in | ios::out| ios::binary);

        if (arquivo.fail()) {
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        arquivo.seekp((professor.base.id - 1) * sizeof(Professor));
        arquivo.write((const char*)(&professor), sizeof(Professor));
        arquivo.close();
    }


//======================================================================
// Gerar novo ID
//======================================================================

    int gerarNovoId() {
        return verificarUltimoIdUsuario() + 1;
    }

    //Feito por Luiz Felipe
    int verificarUltimoIdUsuario() {
        int ultimoId = 0;
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

        if (id > verificarUltimoIdUsuario())
            return usuario;

        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary);

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo de entrada falhou.\n";
            exit(1);
        }

        inUsuarios.seekg((id - 1) * sizeof(Usuario));
        inUsuarios.read((char*)(&usuario), sizeof(Usuario));
        inUsuarios.close();

        return usuario;
    }

    // Feito por Luiz Felipe
    Aluno lerAluno(int id) {
        Aluno aluno = inicializarAlunoVazio();

        ifstream inAlunos;
        inAlunos.open(ARQUIVO_ALUNOS, ios::in | ios::binary);

        if (inAlunos.fail()) {
            cout << "A abertura do arquivo de entrada falhou.\n";
            exit(1);
        }

        inAlunos.seekg((id - 1) * sizeof(Aluno));
        inAlunos.read((char*)(&aluno), sizeof(Aluno));
        inAlunos.close();

        return aluno;
    }

    // Feito por Clara
    Professor lerProfessor(int id) {
        Professor professor = inicializarProfessorVazio();
        ifstream inProfessores;
        inProfessores.open(ARQUIVO_PROFESSORES, ios::in | ios::binary);

        if(inProfessores.fail()){
            cout << "A abertura do arquivo falhou!\n";
            exit(1);
        }

        inProfessores.seekg((id - 1)* sizeof(Professor));
        inProfessores.read((char*)(&professor), sizeof(Professor));
        inProfessores.close();

        return professor;
    }

//======================================================================
// Atualizar
//======================================================================
    void atualizar(int id, Professor professor){

        if (verificarUsuarioExistente(id) == true) {
            cout<< "usuário existente!";
        }
        else {
            cout<< "O usuário não existe";
            return;
        }
        professor.base.id = id;
        professor.base.categoria = PROFESSOR;

        salvarProfessor(professor);
        cout<< "Usuário atualizado com sucesso!";
    }


//======================================================================
// Verificar quantos X (usuarios, alunos, professores ou admin)
//======================================================================
    // Feito por Luiz Felipe
    int verificarQuantosUsuarios() {
        return verificarUltimoIdUsuario();
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
            if (aluno.base.id != 0)
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
            if(professor.base.id != 0){
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

        if (id > verificarUltimoIdUsuario() || id <= 0)
            return false;

        ifstream inUsuarios;
        inUsuarios.open(ARQUIVO_USUARIOS, ios::in | ios::binary );

        if (inUsuarios.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            return false;
        }

        inUsuarios.seekg((id - 1) * sizeof(Usuario));

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
            if (u.id != 0) {
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
        Usuario usuarioVazio = {0, true, "", "", "", NENHUMA, false};
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
        return alunoVazio;
    }

    // Feito por Clara
    Professor inicializarProfessorVazio(){
        Professor professorVazio;
        professorVazio.base = inicializarUsuarioVazio();
        professorVazio.base.categoria = PROFESSOR;
        professorVazio.saldo = 0.0;
        return professorVazio;
    }

}

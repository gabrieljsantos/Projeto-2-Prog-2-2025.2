#include <iostream>
#include <fstream>
#include <cstring>
#include "login_matricula.h"
#include "headers.h"

//Feito por Luiz Felipe
const char* ARQUIVO_USUARIOS = "usuarios.dat";
const char* ARQUIVO_ALUNOS = "alunos.dat";
const char* ARQUIVO_PROFESSORES = "professores.dat";
const char* ARQUIVO_ADMINISTRADORES = "administradores.dat";

namespace Login_mat {

//======================================================================
// Arquivos
//======================================================================
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

    void inicializarArquivos() {
        inicializarArquivoUsuario();
        inicializarArquivoAluno();
    }

//======================================================================
// Cadastro
//======================================================================

    //Feito por Jeanderson, auxiliado por Luiz Felipe
    int realizarCadastroAluno() {
        Aluno* alunoPtr = new Aluno;
        *alunoPtr = inicializarAlunoVazio();
        int novoId = verificarUltimoIdUsuario() + 1;
        (*alunoPtr).base.id = novoId;

        cout << "\nNome: ";
        cin.getline((*alunoPtr).base.nome, 100);

        cout << "Email: ";
        cin.getline((*alunoPtr).base.email, 100);

        cout << "Senha: ";
        cin.getline((*alunoPtr).base.senha, 30);

        fstream arquivoCadastro;
        //======================================================================
        // Cadastrando no arquivo de usuarios
        //======================================================================
        arquivoCadastro.open(ARQUIVO_USUARIOS, ios::in | ios::out | ios::binary);

        if (arquivoCadastro.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        arquivoCadastro.seekp((novoId - 1) * sizeof(Usuario));
        arquivoCadastro.write((const char *)(&(*alunoPtr).base), sizeof(Usuario));

        arquivoCadastro.close();

        //======================================================================
        // Sincronizando com o arquivo de alunos
        //======================================================================
        arquivoCadastro.open(ARQUIVO_ALUNOS, ios::in | ios::out | ios::binary);

        if (arquivoCadastro.fail()) {
            cout << "A abertura do arquivo falhou.\n";
            exit(1);
        }

        arquivoCadastro.seekp((novoId - 1) * sizeof(Aluno));
        arquivoCadastro.write((const char *)(&(*alunoPtr)), sizeof(Aluno));

        arquivoCadastro.close();

        delete alunoPtr;
        alunoPtr = NULL;

        return novoId;
    }

    int realizarCadastroProfessor() {
        // Implementar fun��o
        return 0;
    }

    int realizarCadastroAdmin() {
        // Implementar fun��o
        return 0;
    }

//======================================================================
// Login
//======================================================================
    bool realizarLogin(int id, const char senha[30], Usuario &usuario) {
        usuario = inicializarUsuarioVazio();

        if (verificarUsuarioExistente(id) == false)
            return false;

        Usuario usuarioDeTeste = lerUsuario(id);

        if (usuarioDeTeste.ativo == false)
            return false;

        if (strcmp(senha, usuarioDeTeste.senha) == 0) {
            usuario = usuarioDeTeste;
            usuario.logado = true;
            return true;
        }
        return false;
    }


//======================================================================
// Gerar novo ID
//======================================================================
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


//======================================================================
// Verificar se usuario existe
//======================================================================
    //Feito por Jhones
    bool verificarUsuarioExistente(const char nome[30]) {
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
                if (strncmp(u.nome, nome, 30) == 0) {
                    inUsuarios.close();
                    return true;
                }
            }
        }

        inUsuarios.close();
        return false;
    }

    //Feito por Jeanderson
    bool verificarUsuarioExistente(int id){
        fstream registroDeUsuariosNovos;
        registroDeUsuariosNovos.open(ARQUIVO_USUARIOS, ios:: in | ios:: binary );
        registroDeUsuariosNovos.seekg((id - 1) * sizeof(Usuario));

        Usuario verificaId;

        if (registroDeUsuariosNovos.read((char *)&verificaId, sizeof(Usuario))){
            registroDeUsuariosNovos.close();
            return verificaId.id == id;
        }
        registroDeUsuariosNovos.close();
        return false;
    }


//======================================================================
// Inicializadores de variaveis
//======================================================================
    //Feito por Luiz Felipe
     Usuario inicializarUsuarioVazio() {
        Usuario usuarioVazio = {0, true, "", "", "", NENHUMA, .0, false};
        return usuarioVazio;
    }

    // Feito por Luiz Felipe
    Aluno inicializarAlunoVazio() {
        Aluno alunoVazio;
        alunoVazio.base = inicializarUsuarioVazio();
        alunoVazio.base.categoria = ALUNO;
        alunoVazio.notas[0] = 0;
        alunoVazio.notas[1] = 0;
        alunoVazio.faltas = 0;
        return alunoVazio;
    }

}

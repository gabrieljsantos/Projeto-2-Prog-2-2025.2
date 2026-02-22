#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H

#include "headers.h"
#include "interface_grafica.h"

#define ARQUIVO_USUARIOS "usuarios.dat"
#define ARQUIVO_ALUNOS "alunos.dat"
#define ARQUIVO_PROFESSORES "professores.dat"
#define ARQUIVO_ADMINISTRADORES "administradores.dat"

using namespace std;

namespace Login_mat {

    void inicializarArquivoUsuario();
    void inicializarArquivoAluno();
    void inicializarArquivoProfessor();
    void inicializarArquivoAdmin();
    void inicializarArquivos();

    bool verificarArquivoExistente(string nomeArquivo);

    int realizarCadastro();
    int realizarCadastroAluno();
    int realizarCadastroProfessor();
    void realizarCadastroAdmin();

    bool realizarLogin(Usuario &usuario);

    void salvarUsuario(Usuario usuario);
    void salvarAluno(Aluno aluno);
    void salvarProfessor(Professor professor);
    void salvarAdmin(Admin admin);

    string encriptografarSenha(string senha, int index);
    string desencriptografarSenha(string senhaEncriptografada, int index);

    int gerarNovoId();
    int adquirirPosicaoDoId(int id);
    int verificarUltimoIdUsuario();

    Usuario lerUsuario(int id);
    Aluno lerAluno(int id);
    Professor lerProfessor(int id);

    void atualizar(int id, Aluno aluno);
    void atualizar(int id, Professor professor);

    int verificarQuantosUsuarios();
    int verificarQuantosAlunos();
    int verificarQuantosProfessores();

    bool verificarUsuarioExistente(int id);
    bool verificarUsuarioExistente(const char nome[100]);

    Usuario inicializarUsuarioVazio();
    Aluno inicializarAlunoVazio();
    Professor inicializarProfessorVazio();
    Admin inicializarAdminVazio();

        saida_menu exibirSelecionarCategoria();
        saida_entrada_texto digiteId(string titulo);
        saida_entrada_texto digiteNome(string titulo);
        saida_entrada_texto digiteEmail(string titulo);
        saida_entrada_texto digiteSenha(string titulo);
        saida_entrada_texto digiteDisciplina(string titulo);
}

#endif

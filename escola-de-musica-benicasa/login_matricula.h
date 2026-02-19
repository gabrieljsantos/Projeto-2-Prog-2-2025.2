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

    // Inicializadores de arquivos
    void inicializarArquivoUsuario();
    void inicializarArquivoAluno();
    void inicializarArquivoProfessor();
    void inicializarArquivoAdmin();
    void inicializarArquivos(); // Junção de todos inicializadores

    // Verificadores de arquivo
    bool verificarArquivoExistente(string nomeArquivo);

    // Cadastro
    int realizarCadastro();
    int realizarCadastroAluno();
    int realizarCadastroProfessor();
    void realizarCadastroAdmin();

    // Login
    bool realizarLogin(Usuario &usuario);

    // Salvar
    void salvarUsuario(Usuario usuario);
    void salvarAluno(Aluno aluno); // Salva e sincroniza com "usuarios.dat"
    void salvarProfessor(Professor professor); // Salva e sincroniza com "usuarios.dat"
    void salvarAdmin(Admin admin); // Salva e sincroniza com "usuarios.dat"

    // Gera novo ID baseado no arquivo "usuarios.dat"
    int gerarNovoId();
    int adquirirPosicaoDoId(int id);
    int verificarUltimoIdUsuario();

    // Ler X (usuario, aluno, professor)
    Usuario lerUsuario(int id);
    Aluno lerAluno(int id);
    Professor lerProfessor(int id);

    // Atualizar
    void atualizar(int id, Aluno aluno);
    void atualizar(int id, Professor professor);

    // Verificar quantas pessoas tem em cada arquivo
    int verificarQuantosUsuarios();
    int verificarQuantosAlunos();
    int verificarQuantosProfessores();

    // Verificação em "usuarios.dat" apenas
    bool verificarUsuarioExistente(int id);
    bool verificarUsuarioExistente(const char nome[100]);

    // Inicializadores de variaveis vazias
    Usuario inicializarUsuarioVazio();
    Aluno inicializarAlunoVazio();
    Professor inicializarProfessorVazio();
    Admin inicializarAdminVazio();

    //Interface grafica
      //Cadastro
        saida_menu exibirSelecionarCategoria();
        saida_entrada_texto digiteId(string titulo);
        saida_entrada_texto digiteNome(string titulo);
        saida_entrada_texto digiteEmail(string titulo);
        saida_entrada_texto digiteSenha(string titulo);
}

#endif

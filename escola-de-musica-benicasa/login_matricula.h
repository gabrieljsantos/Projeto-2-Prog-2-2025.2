#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H

#include "headers.h"

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
    void inicializarArquivos(); // Junção de todos inicializadores

    // Cadastro
    void realizarCadastro();
    int realizarCadastroAluno();
    int realizarCadastroProfessor();
    int realizarCadastroAdmin();

    // Login
    bool realizarLogin(Usuario &usuario);

    // Salvar
    void salvarUsuario(Usuario usuario);
    void salvarAluno(Aluno aluno); // Salva e sincroniza com "usuarios.dat"
    void salvarProfessor(Professor professor); // Salva e sincroniza com "usuarios.dat"

    // Gera novo ID baseado no arquivo "usuarios.dat"
    int gerarNovoId();
    int verificarUltimoIdUsuario();

    // Ler X (usuario, aluno, professor)
    Usuario lerUsuario(int id);
    Aluno lerAluno(int id);
    Professor lerProfessor(int id);

    // Atualizar
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
}

#endif

#ifndef LOGIN_MATRICULA_H
#define LOGIN_MATRICULA_H
#include <string>

#include "headers.h"

using namespace std;

namespace Login_mat {

    void inicializarArquivoUsuario();
    void inicializarArquivoAluno();
    void inicializarArquivoProfessor();
    void inicializarArquivoAdmin();
    void inicializarArquivos(); // Junção de todos inicializadores

    // Cadastro
    int realizarCadastroAluno();
    int realizarCadastroProfessor();
    int realizarCadastroAdmin();

    // Login
    bool realizarLogin(int id, const char senha[30], Usuario &usuario);

    // Gera novo ID baseado no arquivo "usuarios.txt"
    int verificarUltimoIdUsuario();

    // Verificar quantas pessoas tem em cada arquivo
    int verificarQuantosUsuarios();
    int verificarQuantosAlunos();
    int verificarQuantosProfessores();
    int verificarQuantosAdministradores();

    // Ler X (usuario, aluno, professor ou administrador)
    Usuario lerUsuario(int id);
    Aluno lerAluno(int id);
    Professor lerProfessor(int id);
    Admin lerAdministrador(int id);

    // Verificação em "usuarios.txt" apenas
    bool verificarUsuarioExistente(int id);
    bool verificarUsuarioExistente(const char nome[30]);

    // Sobrecarga da função atualizar, o parametro define qual o tipo de usuario atualizar
    // OBS.: Todos eles alteram o arquivo "usuarios.txt"
    void atualizar(int id, Aluno Aluno);
    void atualizar(int id, Professor Professor);
    void atualizar(int id, Admin novoAdmin);

    // Inicializadores de variaveis vazias
    Usuario inicializarUsuarioVazio();
    Aluno inicializarAlunoVazio();
    Professor inicializarProfessorVazio();
    Admin inicializarAdminVazio();
}

#endif

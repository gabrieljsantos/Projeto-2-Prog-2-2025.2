#include <iostream>
#include <string>
#include <iomanip>
#include "headers.h"
#include "login_matricula.h"
#include "admin.h"
#include "professor.h"
#include "aluno.h"
#include "eventos.h"
#include "instrumentos.h"
#include "lanchonete.h"

using namespace std;

void pausar();
void exibirCabecalhoSistema();
void exibirMenuLogin();
void exibirMenuAdministrador();
void exibirMenuProfessor();
void exibirMenuAluno();

int main() {
    Usuario usuario;
    int opcaoLogin;
    bool executando = true;

    Login_mat::inicializarArquivo();

    exibirCabecalhoSistema();

    while (executando) {
        if (!usuario.logado) {
            exibirMenuLogin();
            cin >> opcaoLogin;
            cin.ignore();

            switch (opcaoLogin) {
                case 1: {
                    int id;
                    char senha[30];

                    cout << "ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Senha: ";
                    cin.getline(senha, 30);

                    if (Login_mat::realizarLogin(id, senha, usuario)) {
                        cout << "\nBem-vindo(a), " << usuario.nome << "!" << endl;
                        pausar();
                    } else {
                        cout << "\nFalha no login." << endl;
                        pausar();
                    }
                    break;
                }
                case 2: {
                    Usuario novoUsuario = Login_mat::inicializarUsuarioVazio();

                    cout << "Nome: ";
                    cin.getline(novoUsuario.nome, 100);

                    cout << "Email: ";
                    cin.getline(novoUsuario.email, 100);

                    cout << "Senha: ";
                    cin.getline(novoUsuario.senha, 30);

                    cout << "Especialidade: ";
                    cin.getline(novoUsuario.especialidade, 50);

                    int opcaoCategoria;

                    cout << "Categoria:\n";
                    cout << "0 - Aluno\n";
                    cout << "1 - Professor\n";
                    cout << "2 - Administrador\n";
                    cout << "Opcao: ";
                    cin >> opcaoCategoria;

                    if (opcaoCategoria >= 0 && opcaoCategoria <= 2)
                        novoUsuario.categoria = static_cast<Funcao>(opcaoCategoria);
                    else
                        novoUsuario.categoria = NENHUMA;

                    novoUsuario.logado = false;

                    int idNovoUsuario = Login_mat::realizarMatricula(novoUsuario);

                    cout << "Matricula realizada com sucesso!\nID: " << idNovoUsuario << endl;
                    pausar();
                    break;
                }
                case 0:
                    executando = false;
                    break;
                default:
                    cout << "\nOpcao invalida!" << endl;
                    pausar();
            }
        } else {
            switch (usuario.categoria) {
                case ADMINISTRADOR: {
                    int opcao;
                    exibirMenuAdministrador();
                    cin >> opcao;
                    cin.ignore();

                    switch (opcao) {
                        case 1: Admin::menuCadastroUsuarios(); break;
                        case 2: Admin::menuGerenciarUsuarios(); break;
                        case 3: Admin::menuCadastroCursos(); break;
                        case 4: Admin::consultarRelatoriosAcademicos(); break;
                        case 5: Admin::menuCadastroEventos(); break;
                        case 6: Admin::autorizarEventos(); break;
                        case 7: Admin::listarTodosEventos(); break;
                        case 8: Admin::menuCadastroInstrumentos(); break;
                        case 9: Admin::liberarInstrumentos(); break;
                        case 10: Admin::consultarPendenciasInstrumentos(); break;
                        case 11: Lanchonete::menuCadastroProdutos(); break;
                        case 12: Lanchonete::adicionarCreditosUsuario(); break;
                        case 13: Lanchonete::consultarEstoque(); break;
                        case 14: Admin::gerarRelatorioFinanceiro(); break;
                        case 15: Admin::gerarRelatorioPatrimonial(); break;
                        case 16: Admin::realizarBackup(); break;
                        case 17: Admin::restaurarBackup(); break;
                        case 0: usuario.logado = false; break;
                        default: cout << "\nOpcao invalida!" << endl;
                    }
                    if (usuario.logado) pausar();
                    break;
                }
                case PROFESSOR: {
                    int opcao;
                    float notas[10];
                    int n = 2;
                    exibirMenuProfessor();
                    cin >> opcao;
                    cin.ignore();

                    switch (opcao) {
                        case 1: Professor::registrarNotas(usuario.id, notas, n); break;
                        case 2: Professor::registrarAvaliacoes(usuario.id); break;
                        case 3: Professor::consultarAlunosMatriculados(usuario.id); break;
                        case 4: Professor::calcularMediasTurma(usuario.id); break;
                        case 5: Professor::consultarDesempenhoAcademico(usuario.id); break;
                        case 6: Professor::consultarEventosDisponiveis(); break;
                        case 7: Professor::consultarInstrumentosDisponiveis(); break;
                        case 8: Lanchonete::consultarSaldo(usuario.id); break;
                        case 0: usuario.logado = false; break;
                        default: cout << "\nOpcao invalida!" << endl;
                    }
                    if (usuario.logado) pausar();
                    break;
                }
                case ALUNO: {
                    int opcao;
                    exibirMenuAluno();
                    cin >> opcao;
                    cin.ignore();

                    switch (opcao) {
                        case 1: Aluno::consultarNotas(usuario.id); break;
                        case 2: Aluno::consultarMedias(usuario.id); break;
                        case 3: Aluno::consultarSituacaoAcademica(usuario.id); break;
                        case 4: Aluno::visualizarEventosDisponiveis(); break;
                        case 5: Aluno::inscreverEmEvento(usuario.id); break;
                        case 6: Aluno::consultarMinhasInscricoes(usuario.id); break;
                        case 7: Aluno::visualizarInstrumentosDisponiveis(); break;
                        case 8: Aluno::solicitarEmprestimo(usuario.id); break;
                        case 9: Aluno::realizarDevolucao(usuario.id); break;
                        case 10: Aluno::consultarMeusEmprestimos(usuario.id); break;
                        case 11: Lanchonete::consultarSaldo(usuario.id); break;
                        case 12: Lanchonete::visualizarProdutos(); break;
                        case 13: Lanchonete::realizarCompra(usuario.id); break;
                        case 0: usuario.logado = false; break;
                        default: cout << "\nOpcao invalida!" << endl;
                    }
                    if (usuario.logado) pausar();
                    break;
                }
                default:
                    cout << "\nPerfil invalido!" << endl;
                    usuario.logado = false;
                    pausar();
            }
        }
    }

    return 0;
}

void pausar() {
    cout << "\nPressione ENTER para continuar..." << endl;
    cin.get();
}

void exibirCabecalhoSistema() {
    cout << "============================================" << endl;
    cout << "   SISTEMA DE GESTAO - ESCOLA DE MUSICA    " << endl;
    cout << "============================================" << endl;
}

void exibirMenuLogin() {
    cout << "\nMENU DE ACESSO" << endl;
    cout << "============================================" << endl;
    cout << "1. Fazer Login" << endl;
    cout << "2. Realizar Matricula" << endl;
    cout << "0. Sair do Sistema" << endl;
    cout << "Opcao: ";
}

void exibirMenuAdministrador() {
    system("cls || clear");
    cout << "AREA DO ADMINISTRADOR" << endl;
    cout << "============================================" << endl;
    cout << "1. Cadastrar Usuarios" << endl;
    cout << "2. Gerenciar Usuarios" << endl;
    cout << "3. Cadastrar Cursos" << endl;
    cout << "4. Relatorios Academicos" << endl;
    cout << "5. Cadastrar Eventos" << endl;
    cout << "6. Autorizar Eventos" << endl;
    cout << "7. Listar Todos os Eventos" << endl;
    cout << "8. Cadastrar Instrumentos" << endl;
    cout << "9. Liberar Instrumentos" << endl;
    cout << "10. Consultar Pendencias" << endl;
    cout << "11. Cadastrar Produtos" << endl;
    cout << "12. Adicionar Creditos" << endl;
    cout << "13. Consultar Estoque" << endl;
    cout << "14. Relatorio Financeiro" << endl;
    cout << "15. Relatorio Patrimonial" << endl;
    cout << "16. Realizar Backup" << endl;
    cout << "17. Restaurar Backup" << endl;
    cout << "0. Logout" << endl;
    cout << "Opcao: ";
}

void exibirMenuProfessor() {
    system("cls || clear");
    cout << "AREA DO PROFESSOR" << endl;
    cout << "============================================" << endl;
    cout << "1. Registrar Notas" << endl;
    cout << "2. Registrar Avaliacoes" << endl;
    cout << "3. Consultar Alunos Matriculados" << endl;
    cout << "4. Calcular Medias da Turma" << endl;
    cout << "5. Consultar Desempenho Academico" << endl;
    cout << "6. Consultar Eventos" << endl;
    cout << "7. Consultar Instrumentos" << endl;
    cout << "8. Consultar Saldo Lanchonete" << endl;
    cout << "0. Logout" << endl;
    cout << "Opcao: ";
}

void exibirMenuAluno() {
    system("cls || clear");
    cout << "AREA DO ALUNO" << endl;
    cout << "============================================" << endl;
    cout << "1. Consultar Notas" << endl;
    cout << "2. Consultar Medias" << endl;
    cout << "3. Consultar Situacao Academica" << endl;
    cout << "4. Ver Eventos Disponiveis" << endl;
    cout << "5. Inscrever-se em Evento" << endl;
    cout << "6. Minhas Inscricoes" << endl;
    cout << "7. Ver Instrumentos Disponiveis" << endl;
    cout << "8. Solicitar Emprestimo" << endl;
    cout << "9. Realizar Devolucao" << endl;
    cout << "10. Meus Emprestimos" << endl;
    cout << "11. Consultar Saldo" << endl;
    cout << "12. Ver Produtos" << endl;
    cout << "13. Realizar Compra" << endl;
    cout << "0. Logout" << endl;
    cout << "Opcao: ";
}

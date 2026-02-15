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

    Login_mat::inicializarArquivos();

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

                    cout << "\nID: ";
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
                    int opcaoCategoria = -1;
                    int idNovoUsuario;

                    cout << "\nCategoria:\n";
                    cout << "0 - Aluno\n";
                    cout << "1 - Professor\n";
                    cout << "2 - Administrador\n";

                    do {
                        cout << "Opcao: ";
                        cin >> opcaoCategoria;
                        cin.ignore();
                    } while (opcaoCategoria < 0 || opcaoCategoria > 2);

                    switch (opcaoCategoria) {
                        case 0: {
                            idNovoUsuario = Login_mat::realizarCadastroAluno();
                            break;
                        }
                        case 1: {
                            idNovoUsuario = Login_mat::realizarCadastroProfessor();
                            break;
                        }
                        case 2: {
                            idNovoUsuario = Login_mat::realizarCadastroAdmin();
                            break;
                        }
                    }

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
                        case 1: Modulo_admin::menuCadastroUsuarios(); break;
                        case 2: Modulo_admin::menuGerenciarUsuarios(); break;
                        case 3: Modulo_admin::menuCadastroCursos(); break;
                        case 4: Modulo_admin::consultarRelatoriosAcademicos(); break;
                        case 5: Modulo_admin::menuCadastroEventos(); break;
                        case 6: Modulo_admin::autorizarEventos(); break;
                        case 7: Modulo_admin::listarTodosEventos(); break;
                        case 8: Modulo_admin::menuCadastroInstrumentos(); break;
                        case 9: Modulo_admin::liberarInstrumentos(); break;
                        case 10: Modulo_admin::consultarPendenciasInstrumentos(); break;
                        case 11: Lanchonete::menuCadastroProdutos(); break;
                        case 12: Lanchonete::adicionarCreditosUsuario(); break;
                        case 13: Lanchonete::consultarEstoque(); break;
                        case 14: Modulo_admin::gerarRelatorioFinanceiro(); break;
                        case 15: Modulo_admin::gerarRelatorioPatrimonial(); break;
                        case 16: Modulo_admin::realizarBackup(); break;
                        case 17: Modulo_admin::restaurarBackup(); break;
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
                        case 1: Modulo_professor::registrarNotas(usuario.id, notas, n); break;
                        case 2: Modulo_professor::registrarAvaliacoes(usuario.id); break;
                        case 3: Modulo_professor::consultarAlunosMatriculados(usuario.id); break;
                        case 4: Modulo_professor::calcularMediasTurma(usuario.id); break;
                        case 5: Modulo_professor::consultarDesempenhoAcademico(usuario.id); break;
                        case 6: Modulo_professor::consultarEventosDisponiveis(); break;
                        case 7: Modulo_professor::consultarInstrumentosDisponiveis(); break;
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
                        case 1: Modulo_aluno::consultarNotas(usuario.id); break;
                        case 2: Modulo_aluno::consultarMedias(usuario.id); break;
                        case 3: Modulo_aluno::consultarSituacaoAcademica(usuario.id); break;
                        case 4: Modulo_aluno::visualizarEventosDisponiveis(); break;
                        case 5: Modulo_aluno::inscreverEmEvento(usuario.id); break;
                        case 6: Modulo_aluno::consultarMinhasInscricoes(usuario.id); break;
                        case 7: Modulo_aluno::visualizarInstrumentosDisponiveis(); break;
                        case 8: Modulo_aluno::solicitarEmprestimo(usuario.id); break;
                        case 9: Modulo_aluno::realizarDevolucao(usuario.id); break;
                        case 10: Modulo_aluno::consultarMeusEmprestimos(usuario.id); break;
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

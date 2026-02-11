#include <iostream>
#include <string>
#include "login_matricula.h"
#include "admin.h"
#include "professor.h"
#include "aluno.h"
#include "eventos.h"
#include "instrumentos.h"
#include "lanchonete.h"

using namespace std;



struct UsuarioLogado {
    int id;
    string nome;
    TipoUsuario tipoUsuario;
    bool logado;

    UsuarioLogado() : id(0), nome(""), tipoUsuario(ADMINISTRADOR), logado(false) {}
};

void pausar();
void exibirCabecalhoSistema();
void exibirMenuLogin();
void exibirMenuAdministrador();
void exibirMenuProfessor();
void exibirMenuAluno();

int main() {
    UsuarioLogado usuario;
    int opcaoLogin;
    bool executando = true;

    exibirCabecalhoSistema();

    while (executando) {
        if (!usuario.logado) {
            exibirMenuLogin();
            cin >> opcaoLogin;
            cin.ignore();

            switch (opcaoLogin) {
                case 1: {
                    int id = 0;
                    string nome = " ";
                    TipoUsuario tipoUsuario = ADMINISTRADOR; //Só pra testar por enquanto

                    if (realizarLogin(id, nome, tipoUsuario)) {
                        usuario.id = id;
                        usuario.nome = nome;
                        usuario.tipoUsuario = tipoUsuario;
                        usuario.logado = true;
                        cout << "\nBem-vindo(a), " << usuario.nome << "!" << endl;
                        pausar();
                    } else {
                        cout << "\nFalha no login." << endl;
                        pausar();
                    }
                    break;
                }
                case 2:
                    realizarMatricula();
                    pausar();
                    break;
                case 0:
                    executando = false;
                    break;
                default:
                    cout << "\nOpcao invalida!" << endl;
                    pausar();
            }
        } else {
            switch (usuario.tipoUsuario) {
                case ADMINISTRADOR: {
                    int opcao;
                    exibirMenuAdministrador();
                    cin >> opcao;
                    cin.ignore();

                    switch (opcao) {
                        case 1: printf("Teste/n"); break;
//                        case 1: menuCadastroUsuarios(); break;
//                        case 2: menuGerenciarUsuarios(); break;
//                        case 3: menuCadastroCursos(); break;
//                        case 4: consultarRelatoriosAcademicos(); break;
//                        case 5: menuCadastroEventos(); break;
//                        case 6: autorizarEventos(); break;
//                        case 7: listarTodosEventos(); break;
//                        case 8: menuCadastroInstrumentos(); break;
//                        case 9: liberarInstrumentos(); break;
//                        case 10: consultarPendenciasInstrumentos(); break;
//                        case 11: menuCadastroProdutos(); break;
//                        case 12: adicionarCreditosUsuario(); break;
//                        case 13: consultarEstoque(); break;
//                        case 14: gerarRelatorioFinanceiro(); break;
//                        case 15: gerarRelatorioPatrimonial(); break;
//                        case 16: realizarBackup(); break;
//                        case 17: restaurarBackup(); break;
                        case 0: usuario.logado = false; break;
                        default: cout << "\nOpcao invalida!" << endl;
                    }
                    if (usuario.logado) pausar();
                    break;
                }
                case PROFESSOR: {
                    int opcao;
                    exibirMenuProfessor();
                    cin >> opcao;
                    cin.ignore();

                    switch (opcao) {
//                        case 1: registrarNotas(usuario.id); break;
//                        case 2: registrarAvaliacoes(usuario.id); break;
//                        case 3: consultarAlunosMatriculados(usuario.id); break;
//                        case 4: calcularMediasTurma(usuario.id); break;
//                        case 5: consultarDesempenhoAcademico(usuario.id); break;
//                        case 6: consultarEventosDisponiveis(); break;
//                        case 7: consultarInstrumentosDisponiveis(); break;
//                        case 8: consultarSaldoLanchonete(usuario.id); break;
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
//                        case 1: consultarNotas(usuario.id); break;
//                        case 2: consultarMedias(usuario.id); break;
//                        case 3: consultarSituacaoAcademica(usuario.id); break;
//                        case 4: visualizarEventosDisponiveis(); break;
//                        case 5: inscreverEmEvento(usuario.id); break;
//                        case 6: consultarMinhasInscricoes(usuario.id); break;
//                        case 7: visualizarInstrumentosDisponiveis(); break;
//                        case 8: solicitarEmprestimo(usuario.id); break;
//                        case 9: realizarDevolucao(usuario.id); break;
//                        case 10: consultarMeusEmprestimos(usuario.id); break;
//                        case 11: consultarSaldo(usuario.id); break;
//                        case 12: visualizarProdutos(); break;
//                        case 13: realizarCompra(usuario.id); break;
//                        case 14: consultarExtrato(usuario.id); break;
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
    cout << "\nPressione ENTER para continuar...";
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
    cout << "2. Realizar Matricula (Novo Aluno)" << endl;
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
    cout << "14. Consultar Extrato" << endl;
    cout << "0. Logout" << endl;
    cout << "Opcao: ";
}

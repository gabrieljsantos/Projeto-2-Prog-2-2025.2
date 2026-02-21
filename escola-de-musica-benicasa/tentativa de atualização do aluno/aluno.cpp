#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "aluno.h"
#include "headers.h"

using namespace std;

namespace Modulo_aluno {
    // ===== FUNÇÃO AUXILIAR =====
    void limparBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ===== CARREGAR ALUNO POR ID =====
    Aluno carregarAluno(int idAluno) {
        ifstream arq("alunos.dat", ios::binary);

        if (!arq)
            throw runtime_error("Arquivo alunos.dat nao pode ser aberto!");

        Aluno a;

        while (arq.read((char*)&a, sizeof(Aluno))) {
            if (a.base.id == idAluno)
                return a;
        }

        throw runtime_error("Aluno nao encontrado!");
    }

    // ===== SALVAR ALUNO POR ID =====
    void salvarAluno(const Aluno &aluno) {
        fstream arq("alunos.dat", ios::binary | ios::in | ios::out);

        if (!arq) {
            ofstream novo("alunos.dat", ios::binary);
            novo.close();
            arq.open("alunos.dat", ios::binary | ios::in | ios::out);
        }

        if (!arq)
            throw runtime_error("Falha ao abrir alunos.dat apos criacao.");

        Aluno temp;

        while (arq.read((char*)&temp, sizeof(Aluno))) {
            if (temp.base.id == aluno.base.id) {
                arq.seekp(-sizeof(Aluno), ios::cur);
                arq.write((char*)&aluno, sizeof(Aluno));
                return;
            }
        }

        arq.clear();
        arq.seekp(0, ios::end);
        arq.write((char*)&aluno, sizeof(Aluno));
    }


    // ===== CONSULTAS ACADÊMICAS =====
    void consultarNotas(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "----------- MINHAS NOTAS -----------\n";
            cout << "Nota 1: " << aluno.notas[0] << endl;
            cout << "Nota 2: " << aluno.notas[1] << endl;
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void consultarMedias(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "----------- MINHA MEDIA -----------\n";
            cout << "Media: " << aluno.media << endl;
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void consultarSituacaoAcademica(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ SITUACAO ACADEMICA ------\n";

            if (aluno.media >= 7)
                cout << "Status: APROVADO\n";
            else if (aluno.media >= 5)
                cout << "Status: RECUPERACAO\n";
            else
                cout << "Status: REPROVADO\n";

            cout << "Media: " << aluno.media << " | Faltas: " << aluno.faltas << endl;
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }


    // ===== EVENTOS =====
    void visualizarEventosDisponiveis() {
        system("cls || clear");
        cout << "------ EVENTOS DISPONIVEIS ------\n";

        ifstream arq("eventos.dat", ios::binary);

        if (!arq) {
            cout << "Arquivo eventos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Evento e;
        bool encontrou = false;

        while (arq.read((char*)&e, sizeof(Evento))) {
            if (e.ativo == 1 && e.autorizado == 1) {
                cout << "\nID: " << e.id << " - " << e.nome << endl;
                cout << "Descricao: " << e.descricao << endl;
                cout << "Local: " << e.local << endl;
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum evento autorizado encontrado.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void inscreverEmEvento(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ INSCREVER EM EVENTO ------\n";

            int id;
            cout << "Digite o ID do evento: ";
            cin >> id;
            limparBuffer();

            fstream arq("eventos.dat", ios::binary | ios::in | ios::out);

            if (!arq) {
                cout << "Arquivo eventos.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            Evento e;
            bool encontrou = false;

            while (arq.read((char*)&e, sizeof(Evento))) {
                if (e.id == id && e.ativo == 1 && e.autorizado == 1) {
                    cout << "Inscricao no evento realizada com sucesso!\n";
                    encontrou = true;
                    break;
                }
            }

            if (!encontrou)
                cout << "Evento nao encontrado ou nao autorizado.\n";

            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void consultarMinhasInscricoes(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ MINHAS INSCRICOES ------\n";

            ifstream arq("eventos.dat", ios::binary);

            if (!arq) {
                cout << "Arquivo eventos.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            cout << "Inscrições do aluno: " << aluno.base.nome << endl;

            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }


    // ===== INSTRUMENTOS =====
    void visualizarInstrumentosDisponiveis() {
        system("cls || clear");
        cout << "------ INSTRUMENTOS DISPONIVEIS ------\n";

        ifstream arq("instrumentos.dat", ios::binary);

        if (!arq) {
            cout << "Arquivo instrumentos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Instrumento i;
        bool encontrou = false;

        while (arq.read((char*)&i, sizeof(Instrumento))) {
            if (i.ativo == 1 && i.disponivel && i.estoque > 0) {
                cout << "ID: " << i.id << " - " << i.nome 
                     << " (Disponivel: " << i.estoque << ")\n";
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum instrumento disponivel no momento.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void solicitarEmprestimo(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ SOLICITAR EMPRESTIMO ------\n";

            int id;
            cout << "Digite o ID do instrumento: ";
            cin >> id;
            limparBuffer();

            fstream arq("instrumentos.dat", ios::binary | ios::in | ios::out);

            if (!arq) {
                cout << "Arquivo instrumentos.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            Instrumento inst;
            bool encontrou = false;

            while (arq.read((char*)&inst, sizeof(Instrumento))) {
                if (inst.id == id && inst.ativo == 1 && inst.disponivel && inst.estoque > 0) {
                    // Reduz estoque
                    inst.estoque--;
                    if (inst.estoque == 0)
                        inst.disponivel = false;

                    arq.seekp(-sizeof(Instrumento), ios::cur);
                    arq.write((char*)&inst, sizeof(Instrumento));

                    // Registra empréstimo
                    ofstream emp("emprestimos.dat", ios::binary | ios::app);
                    Emprestimo e;
                    e.idAluno = idAluno;
                    e.idInstrumento = id;
                    strcpy(e.nome_Alu, aluno.base.nome);
                    strcpy(e.nome_In, inst.nome);
                    // Aqui você deveria preencher as datas, deixo como exemplo
                    strcpy(e.dataEmprestimo, "00/00/0000");
                    strcpy(e.dataPrevista, "00/00/0000");
                    emp.write((char*)&e, sizeof(Emprestimo));
                    emp.close();

                    cout << "Emprestimo realizado com sucesso!\n";
                    encontrou = true;
                    break;
                }
            }

            if (!encontrou)
                cout << "Instrumento nao encontrado ou indisponivel.\n";

            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void realizarDevolucao(int idAluno) {
        try {
            system("cls || clear");
            cout << "------ DEVOLVER INSTRUMENTO ------\n";

            int id;
            cout << "Digite o ID do instrumento para devolver: ";
            cin >> id;
            limparBuffer();

            // Procura empréstimo ativo
            fstream emp("emprestimos.dat", ios::binary | ios::in | ios::out);

            if (!emp) {
                cout << "Arquivo emprestimos.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            Emprestimo e;
            bool encontrou = false;

            while (emp.read((char*)&e, sizeof(Emprestimo))) {
                if (e.idInstrumento == id && e.idAluno == idAluno) {
                    // Atualiza instrumento
                    fstream arq("instrumentos.dat", ios::binary | ios::in | ios::out);

                    if (arq) {
                        Instrumento inst;

                        while (arq.read((char*)&inst, sizeof(Instrumento))) {
                            if (inst.id == id) {
                                inst.estoque++;
                                inst.disponivel = true;

                                arq.seekp(-sizeof(Instrumento), ios::cur);
                                arq.write((char*)&inst, sizeof(Instrumento));
                                break;
                            }
                        }

                        arq.close();
                    }

                    // Remove registro de empréstimo (marca como inativo ou deleta)
                    emp.seekp(-sizeof(Emprestimo), ios::cur);
                    e.idAluno = -1; // Marca como inativo
                    emp.write((char*)&e, sizeof(Emprestimo));

                    cout << "Devolucao realizada com sucesso!\n";
                    encontrou = true;
                    break;
                }
            }

            emp.close();

            if (!encontrou)
                cout << "Nenhum emprestimo encontrado para este instrumento.\n";

            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void consultarMeusEmprestimos(int idAluno) {
        try {
            system("cls || clear");
            cout << "------ MEUS EMPRESTIMOS ------\n";

            ifstream emp("emprestimos.dat", ios::binary);

            if (!emp) {
                cout << "Arquivo emprestimos.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            Emprestimo e;
            bool encontrou = false;

            while (emp.read((char*)&e, sizeof(Emprestimo))) {
                if (e.idAluno == idAluno && e.idAluno != -1) {
                    cout << "\nInstrumento: " << e.nome_In << endl;
                    cout << "Data Emprestimo: " << e.dataEmprestimo << endl;
                    cout << "Data Prevista para Devolucao: " << e.dataPrevista << endl;
                    encontrou = true;
                }
            }

            if (!encontrou)
                cout << "Voce nao possui instrumentos emprestados.\n";

            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    // ===== LANCHONETE =====
    void visualizarProdutos() {
        system("cls || clear");
        cout << "------ PRODUTOS DISPONIVEIS ------\n";

        ifstream arq("lanchonete.dat", ios::binary);

        if (!arq) {
            cout << "Arquivo lanchonete.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Produto p;
        bool encontrou = false;

        while (arq.read((char*)&p, sizeof(Produto))) {
            if (p.ativo) {
                cout << "ID: " << p.id << " - " << p.nome 
                     << " - R$ " << p.preco 
                     << " (Estoque: " << p.estoque << ")\n";
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum produto disponivel.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void comprarProduto(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ REALIZAR COMPRA ------\n";

            int id;
            cout << "Digite o ID do produto: ";
            cin >> id;
            limparBuffer();

            fstream arq("lanchonete.dat", ios::binary | ios::in | ios::out);

            if (!arq) {
                cout << "Arquivo lanchonete.dat nao encontrado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            Produto p;

            while (arq.read((char*)&p, sizeof(Produto))) {
                if (p.id == id && p.ativo) {
                    if (p.estoque <= 0) {
                        cout << "Produto sem estoque.\n";
                        cout << "\nPressione ENTER para voltar...";
                        cin.get();
                        return;
                    }

                    if (aluno.saldo >= p.preco) {
                        aluno.saldo -= p.preco;
                        p.estoque--;

                        // Atualiza aluno
                        salvarAluno(aluno);

                        // Atualiza produto
                        arq.seekp(-sizeof(Produto), ios::cur);
                        arq.write((char*)&p, sizeof(Produto));

                        cout << "Compra realizada com sucesso!\n";
                        cout << "Novo saldo: R$ " << aluno.saldo << endl;
                    } else {
                        cout << "Saldo insuficiente! Seu saldo: R$ " << aluno.saldo << endl;
                    }

                    cout << "\nPressione ENTER para voltar...";
                    cin.get();
                    return;
                }
            }

            cout << "Produto nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void consultarSaldo(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "----------- MEU SALDO -----------\n";
            cout << "Saldo atual: R$ " << aluno.saldo << endl;
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

    void solicitarCreditos(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            system("cls || clear");
            cout << "------ SOLICITAR CREDITOS ------\n";

            double valor;
            cout << "Digite o valor que deseja adicionar: R$ ";
            cin >> valor;
            limparBuffer();

            if (valor <= 0) {
                cout << "Valor invalido.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            aluno.saldo += valor;
            salvarAluno(aluno);

            cout << "Credito adicionado com sucesso!\n";
            cout << "Novo saldo: R$ " << aluno.saldo << endl;
            cout << "\nPressione ENTER para voltar...";
            cin.get();
        } catch (const exception &e) {
            cout << "Erro: " << e.what() << endl;
            cout << "Pressione ENTER para voltar...";
            cin.get();
        }
    }

}

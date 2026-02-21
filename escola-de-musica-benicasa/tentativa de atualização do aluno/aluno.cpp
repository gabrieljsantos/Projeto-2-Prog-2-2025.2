#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "aluno.h"
#include "headers.h"

using namespace std;

namespace Modulo_aluno
{

    //  CARREGAR ALUNO POR ID

    Aluno carregarAluno(int idAluno)
    {
        ifstream arq("alunos.dat", ios::binary);

        if (!arq)
            throw runtime_error("Arquivo alunos.dat nao pode ser aberto!");

        Aluno a;

        while (arq.read((char*)&a, sizeof(Aluno)))
        {
            if (a.base.id == idAluno)
                return a;
        }

        throw runtime_error("Aluno nao encontrado!");
    }

    //  SALVAR ALUNO POR ID

    void salvarAluno(const Aluno &aluno)
    {
        fstream arq("alunos.dat", ios::binary | ios::in | ios::out);

        if (!arq)
        {
            // cria arquivo
            ofstream novo("alunos.dat", ios::binary);
            novo.close();
            arq.open("alunos.dat", ios::binary | ios::in | ios::out);
        }

        if (!arq)
            throw runtime_error("Falha ao abrir alunos.dat apos criacao.");

        Aluno temp;

        while (arq.read((char*)&temp, sizeof(Aluno)))
        {
            if (temp.base.id == aluno.base.id)
            {
                arq.seekp(-sizeof(Aluno), ios::cur);
                arq.write((char*)&aluno, sizeof(Aluno));
                return;
            }
        }

        arq.clear();
        arq.seekp(0, ios::end);
        arq.write((char*)&aluno, sizeof(Aluno));
    }

    //  CONSULTAS ACADÊMICAS

    void consultarNotas(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "----------- MINHAS NOTAS -----------\n";

        cout << "Nota 1: " << aluno.notas[0] << endl;
        cout << "Nota 2: " << aluno.notas[1] << endl;

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void consultarMedias(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "----------- MINHA MEDIA -----------\n";

        cout << "Media: " << aluno.media << endl;

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void consultarSituacaoAcademica(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ SITUACAO ACADEMICA ------\n";

        if (aluno.media >= 7)
            cout << "Aprovado\n";
        else if (aluno.media >= 5)
            cout << "Recuperacao\n";
        else
            cout << "Reprovado\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    //  EVENTOS

    void visualizarEventosDisponiveis()
    {
        system("cls || clear");
        cout << "------ EVENTOS DISPONIVEIS ------\n";

        ifstream arq("eventos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo eventos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Evento e;
        bool encontrou = false;

        while (arq.read((char*)&e, sizeof(Evento)))
        {
            if (e.autorizado == 1)
            {
                cout << "ID: " << e.id << " - " << e.nome << endl;
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum evento autorizado encontrado.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void inscreverEmEvento(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ INSCREVER EM EVENTO ------\n";

        if (aluno.qtdEventos >= 20)
        {
            cout << "Limite de eventos atingido.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        int id;
        cout << "Digite o ID do evento: ";
        cin >> id;
        limparBuffer();

        ifstream arq("eventos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo eventos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Evento e;

        while (arq.read((char*)&e, sizeof(Evento)))
        {
            if (e.id == id && e.autorizado == 1)
            {
                aluno.eventos[aluno.qtdEventos++] = id;
                salvarAluno(aluno);

                cout << "Inscricao realizada com sucesso!\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }
        }

        cout << "Evento nao encontrado ou nao autorizado.\n";
        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void consultarMinhasInscricoes(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ MINHAS INSCRICOES ------\n";

        if (aluno.qtdEventos == 0)
        {
            cout << "Voce nao esta inscrito em nenhum evento.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        ifstream arq("eventos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo eventos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Evento e;
        bool encontrou = false;

        while (arq.read((char*)&e, sizeof(Evento)))
        {
            for (int i = 0; i < aluno.qtdEventos; i++)
            {
                if (e.id == aluno.eventos[i])
                {
                    cout << "Evento: " << e.nome << endl;
                    encontrou = true;
                }
            }
        }

        if (!encontrou)
            cout << "Nenhum dos eventos cadastrados foi encontrado no arquivo.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    //  INSTRUMENTOS

    void visualizarInstrumentosDisponiveis()
    {
        system("cls || clear");
        cout << "------ INSTRUMENTOS DISPONIVEIS ------\n";

        ifstream arq("instrumentos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo instrumentos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Instrumento i;
        bool encontrou = false;

        while (arq.read((char*)&i, sizeof(Instrumento)))
        {
            if (i.disponivel)
            {
                cout << "ID: " << i.id << " - " << i.nome << endl;
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum instrumento disponivel no momento.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void solicitarEmprestimo(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ SOLICITAR EMPRESTIMO ------\n";

        if (aluno.qtdInstrumentos >= 10)
        {
            cout << "Limite de instrumentos atingido.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        int id;
        cout << "Digite o ID do instrumento: ";
        cin >> id;
        limparBuffer();

        fstream arq("instrumentos.dat", ios::binary | ios::in | ios::out);

        if (!arq)
        {
            cout << "Arquivo instrumentos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }
        Instrumento i;

        while (arq.read((char*)&i, sizeof(Instrumento)))
        {
            if (i.id == id && i.disponivel)
            {
                i.disponivel = false;
                i.responsavel = idAluno;

                arq.seekp(-sizeof(Instrumento), ios::cur);
                arq.write((char*)&i, sizeof(Instrumento));

                aluno.instrumentos[aluno.qtdInstrumentos++] = id;
                salvarAluno(aluno);

                cout << "Emprestimo realizado com sucesso!\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }
        }

        cout << "Instrumento nao encontrado ou indisponivel.\n";
        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void realizarDevolucao(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ DEVOLVER INSTRUMENTO ------\n";

        if (aluno.qtdInstrumentos == 0)
        {
            cout << "Voce nao possui instrumentos emprestados.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        cout << "Instrumentos emprestados:\n";
        for (int i = 0; i < aluno.qtdInstrumentos; i++)
            cout << "- ID: " << aluno.instrumentos[i] << endl;

        int id;
        cout << "Digite o ID do instrumento para devolver: ";
        cin >> id;
        limparBuffer();

        fstream arq("instrumentos.dat", ios::binary | ios::in | ios::out);

        if (!arq)
        {
            cout << "Arquivo instrumentos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Instrumento inst;

        while (arq.read((char*)&inst, sizeof(Instrumento)))
        {
            if (inst.id == id && inst.responsavel == idAluno)
            {
                inst.disponivel = true;
                inst.responsavel = -1;

                arq.seekp(-sizeof(Instrumento), ios::cur);
                arq.write((char*)&inst, sizeof(Instrumento));

                for (int i = 0; i < aluno.qtdInstrumentos; i++)
                {
                    if (aluno.instrumentos[i] == id)
                    {
                        for (int j = i; j < aluno.qtdInstrumentos - 1; j++)
                            aluno.instrumentos[j] = aluno.instrumentos[j + 1];

                        aluno.qtdInstrumentos--;
                        break;
                    }
                }

                salvarAluno(aluno);

                cout << "Devolucao realizada com sucesso!\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }
        }

        cout << "Instrumento nao encontrado ou nao pertence a voce.\n";
        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void consultarMeusEmprestimos(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ MEUS EMPRESTIMOS ------\n";

        if (aluno.qtdInstrumentos == 0)
        {
            cout << "Voce nao possui instrumentos emprestados.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        ifstream arq("instrumentos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo instrumentos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Instrumento i;
        bool encontrou = false;

        while (arq.read((char*)&i, sizeof(Instrumento)))
        {
            for (int j = 0; j < aluno.qtdInstrumentos; j++)
            {
                if (i.id == aluno.instrumentos[j])
                {
                    cout << "Instrumento: " << i.nome << endl;
                    encontrou = true;
                }
            }
        }

        if (!encontrou)
            cout << "Nenhum dos instrumentos emprestados foi encontrado no arquivo.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    //  LANCHONETE

    void visualizarProdutos()
    {
        system("cls || clear");
        cout << "------ PRODUTOS DISPONIVEIS ------\n";

        ifstream arq("produtos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo produtos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Produto p;
        bool encontrou = false;

        while (arq.read((char*)&p, sizeof(Produto)))
        {
            cout << "ID: " << p.id << " - " << p.nome
                 << " - R$ " << p.preco << endl;
            encontrou = true;
        }

        if (!encontrou)
            cout << "Nenhum produto cadastrado.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void comprarProduto(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ REALIZAR COMPRA ------\n";

        int id;
        cout << "Digite o ID do produto: ";
        cin >> id;
        limparBuffer();

        ifstream arq("produtos.dat", ios::binary);

        if (!arq)
        {
            cout << "Arquivo produtos.dat nao encontrado.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        Produto p;

        while (arq.read((char*)&p, sizeof(Produto)))
        {
            if (p.id == id)
            {
                if (aluno.saldo >= p.preco)
                {
                    aluno.saldo -= p.preco;
                    salvarAluno(aluno);

                    cout << "Compra realizada com sucesso!\n";
                }

                else
                {
                    cout << "Saldo insuficiente.\n";
                }

                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }
        }

        cout << "Produto nao encontrado.\n";
        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    // SALDO

    void consultarSaldo(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "----------- MEU SALDO -----------\n";
        cout << "Saldo atual: R$ " << aluno.saldo << "\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void solicitarCreditos(int idAluno)
    {
        Aluno aluno = carregarAluno(idAluno);

        system("cls || clear");
        cout << "------ SOLICITAR CREDITOS ------\n";

        double valor;
        cout << "Digite o valor que deseja adicionar: R$ ";
        cin >> valor;
        limparBuffer();

        if (valor <= 0)
        {
            cout << "Valor invalido.\n";
            cin.get();
            return;
        }

        aluno.saldo += valor;
        salvarAluno(aluno);

        cout << "Credito adicionado com sucesso!\n";
        cout << "Novo saldo: R$ " << aluno.saldo << endl;

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

}

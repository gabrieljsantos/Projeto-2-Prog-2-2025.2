#ifndef EVENTOS_H
#define EVENTOS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

static const char* ARQUIVO_EVENTOS = "eventos.txt";


struct Horario
{
    int hora;
    int minuto;
};

struct Data
{
    int dia;
    int mes;
    int ano;
};

struct Evento
{
    int id;
    char nome[100];
    Horario horario;
    Data data;
    char local[100];
    char professorResponsavel[100];
    int vagasDisponiveis;
    char descricao[200];
    bool finalizado;
    bool aprovado;
    char nomeAluno[100][100];
    int idalunos[100];
    int totalinscritos;
};

extern vector<Evento> eventos;
extern int proximoID;



inline bool SalvarEventosNoArquivo(const vector<Evento>& eventos)
{
    ofstream arquivo(ARQUIVO_EVENTOS, ios::binary);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir arquivo.\n";
        return false;
    }
    arquivo.write(reinterpret_cast<const char*>(&proximoID), sizeof(proximoID));
    for (const Evento& e : eventos)
    {
        arquivo.write(reinterpret_cast<const char*>(&e), sizeof(Evento));
    }

    arquivo.close();
    return true;
}



inline void adicionar_evento() {
    Evento e;
    bool *validar = new bool(false);

    e.id = proximoID;
    proximoID++;
    e.finalizado = false;
    e.aprovado = false;

    cout << "\n\n============================================================" << endl;

    cout << "Nome do evento: ";
    cin.getline(e.nome, 100);

    cout << "Local: ";
    cin.getline(e.local, 100);

    cout << "Hora: ";
    cin >> e.horario.hora;
    while (cin.fail() || e.horario.hora > 23 || e.horario.hora < 0)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.horario.hora;
    }

    cout << "Minuto: ";
    cin >> e.horario.minuto;
    while (cin.fail() || e.horario.minuto > 59 || e.horario.minuto < 0)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.horario.minuto;
    }

    cin.ignore();

    cout << "Vagas: ";
    cin >> e.vagasDisponiveis;
    while (cin.fail() || e.vagasDisponiveis < 0)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.vagasDisponiveis;
    }
    cin.ignore();

    cout << "Professor: ";
    cin.getline(e.professorResponsavel, 100);

    cout << "Dia: ";
    cin >> e.data.dia;
    while (cin.fail() || e.data.dia > 31 || e.data.dia < 1)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.data.dia;
    }

    cout << "Mes: ";
    cin >> e.data.mes;
    while (cin.fail() || e.data.mes > 12 || e.data.mes < 1)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.data.mes;
    }

    cout << "Ano: ";
    cin >> e.data.ano;
    while (cin.fail() || e.data.ano > 2027 || e.data.ano < 2026)
    {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.data.ano;
    }
    cin.ignore();

    cout << "Descricao: ";
    cin.getline(e.descricao, 200);

    cout << "O ID do seu evento e: " << e.id;

    eventos.push_back(e);

    if (SalvarEventosNoArquivo(eventos))
        cout << "\n\nEvento cadastrado com sucesso!\n";
    else
        cout << "\n\nErro ao salvar evento.\n";
}

inline void CarregarEventos()
{
    ifstream arquivo(ARQUIVO_EVENTOS, ios::binary);

    if (!arquivo.is_open())
        return;

    eventos.clear();

    arquivo.read(reinterpret_cast<char*>(&proximoID), sizeof(proximoID));

    Evento e;
    while (arquivo.read(reinterpret_cast<char*>(&e), sizeof(Evento)))
    {
        eventos.push_back(e);
    }

    arquivo.close();
}


inline void editar_evento()
{
    CarregarEventos();

    cout << "\nDigite o valor de ID do seu evento: ";
    int id;
    cin >> id;
    cin.ignore();

    Evento* e = nullptr;

    for (auto& ev : eventos)
    {
        if (ev.id == id)
        {
            e = &ev;
            break;
        }
    }

    if (e == nullptr)
    {
        cout << "Evento nao encontrado.\n";
        return;
    }

    if (!e->aprovado)
    {
        cout << "Evento finalizado nao pode ser editado.\n";
        return;
    }

    int opcao = -1;

    while (opcao != 0)
    {
        cout << "\n1 - Nome\n";
        cout << "2 - Local\n";
        cout << "3 - Professor Ministrante\n";
        cout << "4 - Data\n";
        cout << "5 - Horario\n";
        cout << "6 - Numero de Vagas\n";
        cout << "7 - Descricao\n";
        cout << "0 - Para sair\n";
        cout << "Escolha qual campo voce deseja alterar: ";

        cin >> opcao;
        cin.ignore();

        switch (opcao)
        {
            case 1:
                cout << "\nDigite o novo nome do seu evento: ";
                cin.getline(e->nome, sizeof(e->nome));
                break;

            case 2:
                cout << "\nDigite o novo local do seu evento: ";
                cin.getline(e->local, sizeof(e->local));
                break;

            case 3:
                cout << "\nDigite o novo professor responsavel pelo seu evento: ";
                cin.getline(e->professorResponsavel, sizeof(e->professorResponsavel));
                break;

            case 4:
                cout << "\nNova data do evento:\n";
                cout << "Dia: ";
                cin >> e->data.dia;
                cout << "Mes: ";
                cin >> e->data.mes;
                cout << "Ano: ";
                cin >> e->data.ano;
                cin.ignore();
                break;

            case 5:
                cout << "\nNovo horario do evento:\n";
                cout << "Hora: ";
                cin >> e->horario.hora;
                cout << "Minuto: ";
                cin >> e->horario.minuto;
                cin.ignore();
                break;

            case 6:
                cout << "\nNova quantidade de vagas: ";
                cin >> e->vagasDisponiveis;

                if (e->vagasDisponiveis < e->totalinscritos)
                {
                    cout << "Vagas nao podem ser menores que inscritos.\n";
                    continue;
                }
                cin.ignore();
                break;

            case 7:
                cout << "\nNova descricao do evento: ";
                cin.getline(e->descricao, sizeof(e->descricao));
                break;

            case 0:
                cout << "Edicao finalizada.\n";
                break;

            default:
                cout << "Opcao invalida.\n";
        }
    }

    if (SalvarEventosNoArquivo(eventos))
        cout << "\nAlteracoes salvas com sucesso!\n";
    else
        cout << "\nErro ao salvar alteracoes.\n";
}

inline void finalizar_evento()
{
    CarregarEventos();

    cout << "\nDigite o ID do evento que deseja excluir: ";
    int id;
    cin >> id;
    cin.ignore();

    Evento* e = nullptr;

    for (auto& ev : eventos)
    {
        if (ev.id == id)
        {
            e = &ev;
            break;
        }
    }

    if (e == nullptr)
    {
        cout << "Evento nao encontrado.\n";
        return;
    }

    if (e->finalizado)
    {
        cout << "Evento finalizado nao pode ser excluido.\n";
        return;
    }


    e->finalizado = true;

    if (SalvarEventosNoArquivo(eventos))
        cout << "\nEvento excluido com sucesso!\n";
    else
        cout << "\nErro ao excluir evento.\n";
}

inline void listar_alunos_inscritos()
{
    int idevento;
    cout << "\nDigite o ID do evento: ";
    cin >> idevento;
    cin.ignore();

    Evento* eventoescolhido = nullptr;

    for (Evento &e : eventos)
    {
        if (e.id == idevento)
        {
            eventoescolhido = &e;
            break;
        }
    }

    if (eventoescolhido == nullptr)
    {
        cout << "Evento nao encontrado.\n";
        return;
    }

    if (eventoescolhido->totalinscritos == 0)
    {
        cout << "Nenhum aluno inscrito neste evento.\n";
        return;
    }

    cout << "\n===== Alunos inscritos no evento: "
         << eventoescolhido->nome << " =====\n";

    for (int i = 0; i < eventoescolhido->totalinscritos; i++)
    {
        cout << i + 1 << " - " << eventoescolhido->nomeAluno[i] << endl;
    }

    cout << "\nTotal de inscritos: " << eventoescolhido->totalinscritos << endl;
}

inline void listar_eventos(const vector<Evento>& eventos)
{
    CarregarEventos();
    int opcao;

    cout << "\n(1) Disponiveis";
    cout << "\n(2) Finzalizados";
    cout << "\n\nDeseja vizualizar os eventos disponiveis ou finalizados: ";
    cin >> opcao;
    cin.ignore();

    switch (opcao)
    {
        case 1:
            for (const Evento& e : eventos)
            {
                if(!e.finalizado && e.aprovado)
                {
                    cout << "============================================================" << endl;
                    cout << "Nome: " << e.nome << endl;
                    cout << "ID: " << e.id << endl;
                    cout << "Vagas Restantes: " << e.vagasDisponiveis << endl;
                    cout << "Professor Responsavel: " << e.professorResponsavel << endl;
                    cout << "Data: " << e.data.dia << "/" << e.data.mes << "/" << e.data.ano << endl;
                    cout << "Horario: " << e.horario.hora << ":" << e.horario.minuto << endl;
                    cout << "Descricao: " << e.descricao << endl;
                    cout << endl;
                }
            }
        case 2:
            for (const Evento& e : eventos)
            {
                if(e.finalizado)
                {
                    cout << "============================================================" << endl;
                    cout << "Nome: " << e.nome << endl;
                    cout << "ID: " << e.id << endl;
                    cout << "Vagas Restantes: " << e.vagasDisponiveis << endl;
                    cout << "Professor Responsavel: " << e.professorResponsavel << endl;
                    cout << "Data: " << e.data.dia << "/" << e.data.mes << "/" << e.data.ano << endl;
                    cout << "Horario: " << e.horario.hora << ":" << e.horario.minuto << endl;
                    cout << "Descricao: " << e.descricao << endl;
                    cout << endl;
                }
            }
             break;

    }
}

inline void inscrever_aluno()
{
    CarregarEventos();

    int idEvento;
    cout << "\nDigite o ID do evento: ";
    cin >> idEvento;


    Evento* e = nullptr;

    for (auto& ev : eventos)
    {
        if (ev.id == idEvento)
        {
            e = &ev;
            break;
        }
    }

    if (e == nullptr)
    {
        cout << "Evento nao encontrado.\n";
        return;
    }

    if (e->finalizado)
    {
        cout << "Evento finalizado.\n";
        return;
    }

    if (e->vagasDisponiveis <= 0)
    {
        cout << "Nao ha vagas disponiveis.\n";
        return;
    }

    int idAluno;
    cout << "Digite o ID do aluno: ";
    cin >> idAluno;

    for (int i = 0; i < e->totalinscritos; i++)
    {
        if (e->idalunos[i] == idAluno)
        {
            cout << "Aluno ja inscrito neste evento.\n";
            return;
        }
    }

    char nomeAluno[100];
    cout << "Digite o nome do aluno: ";
    cin.getline(nomeAluno, 100);


    e->idalunos[e->totalinscritos] = idAluno;
    strcpy(e->nomeAluno[e->totalinscritos], nomeAluno);

    e->totalinscritos++;
    e->vagasDisponiveis--;

    if (SalvarEventosNoArquivo(eventos))
        cout << "Aluno inscrito com sucesso!\n";
    else
        cout << "Erro ao salvar inscricao.\n";
}



#endif

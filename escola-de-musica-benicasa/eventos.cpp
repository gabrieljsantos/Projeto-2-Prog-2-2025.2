#include "eventos.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

const char* ARQUIVO_EVENTOS = "eventos.txt";

vector<Evento> eventos;
int proximoID = 1;

bool SalvarEventosNoArquivo(const vector<Evento>& eventos) {
    ofstream arquivo(ARQUIVO_EVENTOS, ios::binary);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo.\n";
        return false;
    }
    arquivo.write(reinterpret_cast<const char*>(&proximoID), sizeof(proximoID));
    for (const Evento& e : eventos) {
        arquivo.write(reinterpret_cast<const char*>(&e), sizeof(Evento));
    }

    arquivo.close();
    return true;
}

void CarregarEventos() {
    ifstream arquivo(ARQUIVO_EVENTOS, ios::binary);

    if (!arquivo.is_open())
        return;

    eventos.clear();

    arquivo.read(reinterpret_cast<char*>(&proximoID), sizeof(proximoID));

    Evento e;
    while (arquivo.read(reinterpret_cast<char*>(&e), sizeof(Evento))) {
        eventos.push_back(e);
    }

    arquivo.close();
}

void adicionar_evento() {
    Evento e;
    
    // Mantendo a sua alocacao dinamica original
    bool *validar = new bool(false); 

    e.id = proximoID;
    proximoID++;
    e.finalizado = false;
    e.totalinscritos = 0; // Garantindo que inicie em 0

    cout << "\n\n============================================================" << endl;

    cout << "Nome do evento: ";
    cin.getline(e.nome, 100);

    cout << "Local: ";
    cin.getline(e.local, 100);

    cout << "Hora: ";
    cin >> e.horario.hora;
    while (cin.fail() || e.horario.hora > 23 || e.horario.hora < 0) {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.horario.hora;
    }

    cout << "Minuto: ";
    cin >> e.horario.minuto;
    while (cin.fail() || e.horario.minuto > 59 || e.horario.minuto < 0) {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.horario.minuto;
    }

    cin.ignore();

    cout << "Vagas: ";
    cin >> e.vagasDisponiveis;
    while (cin.fail() || e.vagasDisponiveis < 0) {
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
    while (cin.fail() || e.data.dia > 31 || e.data.dia < 1) {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.data.dia;
    }

    cout << "Mes: ";
    cin >> e.data.mes;
    while (cin.fail() || e.data.mes > 12 || e.data.mes < 1) {
        cin.clear();
        cin.ignore();
        cout << "Digite um valor valido: ";
        cin >> e.data.mes;
    }

    cout << "Ano: ";
    cin >> e.data.ano;
    while (cin.fail() || e.data.ano > 2027 || e.data.ano < 2026) {
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
        
    // CORRECAO: Liberando a memoria que foi alocada dinamicamente com 'new' para evitar Memory Leak
    delete validar; 
}

void editar_evento (vector<Evento>& eventos) {
    cout << "\nDigite o valor de ID do seu evento: ";
    
    // Mantendo a sua alocacao original com ponteiros
    int *id = new int;
    cin >> *id;
    cin.ignore();

    Evento *e = nullptr;

    for (auto& ev : eventos) {
        if (ev.id == *id) {
            e = &ev;
            break;
        }
    }

    if (e == nullptr) {
        cout << "Evento nao encontrado.\n";
        delete id; // Corrigindo vazamento caso saia mais cedo
        return;
    }

    int *opcao = new int (-1);
    while (*opcao != 0) {
        cout << "\n1 - Nome" << endl;
        cout << "2 - Local" << endl;
        cout << "3 - Professor Ministrante" << endl;
        cout << "4 - Data" << endl;
        cout << "5 - Horario" << endl;
        cout << "6 - Numero de Vagas" << endl;
        cout << "7 - Descricao" << endl;
        cout << "0 - Para sair";
        cout << "\nEscolha qual campo voce deseja alterar: ";
        cin >> *opcao;
        cin.ignore();

        switch (*opcao) {
        case 1:
            cout << "\nDigite o novo nome do seu evento: ";
            cin.getline(e->nome,sizeof(e->nome));
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
            cout << "\nQual sera a novo data do seu evento: ";
            cout << "\nDia: ";
            cin >> e->data.dia;
            cout << "\nMes: ";
            cin >> e->data.mes;
            cout << "\nAno: ";
            cin >> e->data.ano;
            cin.ignore();
            break;
        case 5:
            cout << "\nQual sera o novo horario do seu evento: ";
            cout << "\nHoras: ";
            cin >> e->horario.hora;
            cout << "\nMinutos: ";
            cin >> e->horario.minuto;
            cin.ignore();
            break;
        case 6:
            cout << "\nQuantas vagas deseja que hajam agora em seu evento: ";
            cin >> e->vagasDisponiveis;
            if (e->vagasDisponiveis < e->totalinscritos) {
                cout << "Vagas nao podem ser menores que inscritos\n";
                continue;
            }
            break;
        case 7:
            cout << "\nEscreva aqui a sua nova descricao: ";
            cin.getline(e->descricao, sizeof(e->descricao));
            break;
        case 0:
            cout << "Edicao Finalizada!";
            break;
        default:
            cout << "Opcao Invalida";
        }
    }
    
    // Voce fez certinho aqui: limpando a memoria alocada no final da funcao!
    delete id;
    delete opcao;

    if (SalvarEventosNoArquivo(eventos))
        cout << "\nAlteracoes salvas com sucesso!";
    else
        cout << "\nErro ao salvar alteracoes.";
}

void excluir_evento(int id_excluir) {
    bool achou = false;
    for (size_t i = 0; i < eventos.size(); i++) {
        if (eventos[i].id == id_excluir) {
            eventos.erase(eventos.begin() + i);
            achou = true;
            break;
        }
    }

    if (achou) {
        if (SalvarEventosNoArquivo(eventos))
            cout << "Evento excluido com sucesso!\n";
    } else {
        cout << "Evento nao encontrado!\n";
    }
}

void listar_alunos_inscritos() {
    int idevento;
    cout << "\nDigite o ID do evento: ";
    cin >> idevento;
    cin.ignore();

    Evento* eventoescolhido = nullptr;

    for (Evento &e : eventos) {
        if (e.id == idevento) {
            eventoescolhido = &e;
            break;
        }
    }

    if (eventoescolhido == nullptr) {
        cout << "Evento nao encontrado.\n";
        return;
    }

    if (eventoescolhido->totalinscritos == 0) {
        cout << "Nenhum aluno inscrito neste evento.\n";
        return;
    }

    cout << "\n===== Alunos inscritos no evento: "
         << eventoescolhido->nome << " =====\n";

    for (int i = 0; i < eventoescolhido->totalinscritos; i++) {
        cout << i + 1 << " - " << eventoescolhido->alunos[i] << endl;
    }

    cout << "\nTotal de inscritos: " << eventoescolhido->totalinscritos << endl;
}

void listar_eventos(const vector<Evento>& eventos) {
    int opcao;

    cout << "\n(1) Disponiveis";
    cout << "\n(2) Finalizados";
    cout << "\n\nDeseja visualizar os eventos disponiveis ou finalizados: ";
    cin >> opcao;
    cin.ignore();

    switch (opcao) {
        case 1:
            for (const Evento& e : eventos) {
                if(!e.finalizado) {
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
            break; // Faltava um break aqui no seu original
        case 2:
            for (const Evento& e : eventos) {
                if(e.finalizado) {
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

void inscrever_alunos() {
    int idevento;
    cout << "\nDigite o ID do evento: ";
    cin >> idevento;
    cin.ignore();

    Evento* eventoescolhido = nullptr;

    for (Evento &e : eventos) {
        if (e.id == idevento && !e.finalizado) {
            eventoescolhido = &e;
            break;
        }
    }

    if (eventoescolhido == nullptr) {
        cout << "Evento nao encontrado\n";
        return;
    }

    if (eventoescolhido->vagasDisponiveis <= 0) {
        cout << "Nao ha vagas disponiveis\n";
        return;
    }

    if (eventoescolhido->totalinscritos >= 100) {
        cout << "Limite maximo de alunos atingido.\n";
        return;
    }

    cout << "Nome do aluno: ";
    cin.getline(eventoescolhido->alunos[eventoescolhido->totalinscritos],100);

    eventoescolhido->totalinscritos++;
    eventoescolhido->vagasDisponiveis--;

    SalvarEventosNoArquivo(eventos);

    cout << "Aluno inscrito com sucesso!\n";
}

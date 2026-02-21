#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstring>
#include <vector>
#include "aluno.h"
#include "headers.h"
#include "interface_grafica.h"
#include "instrumentos.h"
#include "eventos.h"

using namespace std;

// Declarações extern do módulo eventos
extern vector<Evento> eventos;
extern void CarregarEventos();
extern bool SalvarEventosNoArquivo(const vector<Evento>& eventos);

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

    // ===== FUNÇÕES AUXILIARES PARA TURMA =====
    static bool lerTurmaById(int turmaId, Turma &turma) {
        ifstream file("turmas.dat", ios::binary);
        if (!file)
            return false;

        while (file.read((char *)&turma, sizeof(Turma))) {
            if (turma.id == turmaId) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    static Aluno* procurarAlunoNaTurma(Turma &turma, int idAluno) {
        for (int i = 0; i < MAX_ALUNOS; i++) {
            if (turma.alunos[i].base.id == idAluno)
                return &turma.alunos[i];
        }
        return nullptr;
    }


    // ===== CONSULTAS ACADÊMICAS =====
    void consultarNotas(int idAluno) {
        try {
            cout << "[DEBUG] consultarNotas: Iniciando consulta de notas - ID=" << idAluno << endl;
            Aluno aluno = carregarAluno(idAluno);
            cout << "[DEBUG] consultarNotas: Aluno carregado - " << aluno.base.nome << endl;

            // Procurar iterativamente por todas as turmas
            cout << "[DEBUG] consultarNotas: Abrindo arquivo turmas.dat" << endl;
            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                cout << "[DEBUG] consultarNotas: ERRO ao abrir turmas.dat" << endl;
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;
            int turmaCount = 0;

            while (file.read((char*)&turma, sizeof(Turma))) {
                turmaCount++;
                cout << "[DEBUG] consultarNotas: Lendo turma " << turmaCount << " - ID=" << turma.id << " Nome=" << turma.nome << endl;
                // Procurar o aluno nesta turma
                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    cout << "[DEBUG] consultarNotas: ENCONTRADO! Aluno na turma " << turma.nome << endl;
                    cout << "[DEBUG] consultarNotas: Notas - N1=" << alunoNaTurma->notas[0] << " N2=" << alunoNaTurma->notas[1] << endl;
                    file.close();

                    // Montar topicos para mostrar notas
                    TopicDetalhes topicos[3];
                    int numTopicos = 0;

                    topicos[numTopicos].titulo = "Turma";
                    topicos[numTopicos].descricao = turma.nome;
                    numTopicos++;

                    topicos[numTopicos].titulo = "Avaliacao 1";
                    topicos[numTopicos].descricao = to_string(alunoNaTurma->notas[0]);
                    numTopicos++;

                    topicos[numTopicos].titulo = "Avaliacao 2";
                    topicos[numTopicos].descricao = to_string(alunoNaTurma->notas[1]);
                    numTopicos++;

                    ConfigDetalhes configDet;
                    configDet.titulo = "MINHAS NOTAS";
                    configDet.descricao = "Aluno: " + string(alunoNaTurma->base.nome);
                    configDet.cores.cor_borda = 6;

                    cout << "[DEBUG] consultarNotas: Exibindo interface grafica" << endl;
                    mostrar_detalhes(topicos, numTopicos, configDet);
                    cout << "[DEBUG] consultarNotas: Interface exibida, aguardando pausa" << endl;
                    system("pause");
                    return;
                }
            }

            file.close();
            cout << "[DEBUG] consultarNotas: Total de turmas lidas = " << turmaCount << endl;
            if (!encontrado) {
                cout << "[DEBUG] consultarNotas: Aluno NAO encontrado em nenhuma turma!" << endl;
                mostrar_caixa_informacao("ERRO", "Aluno nao encontrado em nenhuma turma!");
            }

        } catch (const exception &e) {
            cout << "[DEBUG] consultarNotas: EXCECAO CAPTURADA - " << e.what() << endl;
            mostrar_caixa_informacao("ERRO", e.what());
        }
    }

    void consultarMedias(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            // Procurar iterativamente por todas as turmas
            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;

            while (file.read((char*)&turma, sizeof(Turma))) {
                // Procurar o aluno nesta turma
                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    file.close();

                    // Montar topicos para mostrar media
                    TopicDetalhes topicos[2];
                    int numTopicos = 0;

                    topicos[numTopicos].titulo = "Turma";
                    topicos[numTopicos].descricao = turma.nome;
                    numTopicos++;

                    topicos[numTopicos].titulo = "Media Geral";
                    topicos[numTopicos].descricao = to_string(alunoNaTurma->media);
                    numTopicos++;

                    ConfigDetalhes configDet;
                    configDet.titulo = "MINHAS MEDIAS";
                    configDet.descricao = "Aluno: " + string(alunoNaTurma->base.nome);
                    configDet.cores.cor_borda = 6;

                    mostrar_detalhes(topicos, numTopicos, configDet);
                    system("pause");
                    return;
                }
            }

            file.close();
            if (!encontrado) {
                mostrar_caixa_informacao("ERRO", "Aluno nao encontrado em nenhuma turma!");
            }

        } catch (const exception &e) {
            mostrar_caixa_informacao("ERRO", e.what());
        }
    }

    void consultarSituacaoAcademica(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            // Procurar iterativamente por todas as turmas
            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;

            while (file.read((char*)&turma, sizeof(Turma))) {
                // Procurar o aluno nesta turma
                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    file.close();

                    // Determinar status academico
                    string status;
                    if (alunoNaTurma->faltas > 10)
                        status = "REPROVADO POR FALTA";
                    else if (alunoNaTurma->media >= 7.0)
                        status = "APROVADO";
                    else if (alunoNaTurma->media >= 5.0)
                        status = "RECUPERACAO";
                    else
                        status = "REPROVADO";

                    // Montar topicos para mostrar situação acadêmica
                    TopicDetalhes topicos[5];
                    int numTopicos = 0;

                    topicos[numTopicos].titulo = "Turma";
                    topicos[numTopicos].descricao = turma.nome;
                    numTopicos++;

                    topicos[numTopicos].titulo = "Status Academico";
                    topicos[numTopicos].descricao = status;
                    numTopicos++;

                    topicos[numTopicos].titulo = "Media Geral";
                    topicos[numTopicos].descricao = to_string(alunoNaTurma->media);
                    numTopicos++;

                    topicos[numTopicos].titulo = "Faltas";
                    topicos[numTopicos].descricao = to_string(alunoNaTurma->faltas) + " / 10 maximas";
                    numTopicos++;

                    topicos[numTopicos].titulo = "Observacoes";
                    if (alunoNaTurma->faltas > 10)
                        topicos[numTopicos].descricao = "Aluno reprovado por excesso de faltas";
                    else if (alunoNaTurma->media < 5.0)
                        topicos[numTopicos].descricao = "Desempenho critico - media insuficiente";
                    else if (alunoNaTurma->media < 7.0)
                        topicos[numTopicos].descricao = "Aluno em recuperacao - pode melhorar";
                    else
                        topicos[numTopicos].descricao = "Aluno em bom desempenho academico";
                    numTopicos++;

                    ConfigDetalhes configDet;
                    configDet.titulo = "SITUACAO ACADEMICA";
                    configDet.descricao = "Aluno: " + string(alunoNaTurma->base.nome);
                    configDet.cores.cor_borda = 6;

                    mostrar_detalhes(topicos, numTopicos, configDet);
                    system("pause");
                    return;
                }
            }

            file.close();
            if (!encontrado) {
                mostrar_caixa_informacao("ERRO", "Aluno nao encontrado em nenhuma turma!");
            }

        } catch (const exception &e) {
            mostrar_caixa_informacao("ERRO", e.what());
        }
    }


    // ===== EVENTOS =====
    void visualizarEventosDisponiveis() {
        system("cls || clear");
        cout << "------ EVENTOS DISPONIVEIS ------\n\n";

        // Carregar eventos
        CarregarEventos();

        if (eventos.empty()) {
            cout << "Nenhum evento disponivel no sistema.\n";
            cout << "\nPressione ENTER para voltar...";
            cin.get();
            return;
        }

        bool encontrou = false;

        for (const Evento &e : eventos) {
            if (e.ativo == 1 && e.autorizado == 1 && !e.finalizado) {
                cout << "[ID: " << e.id << "] " << e.nome << endl;
                cout << "  Descricao: " << e.descricao << endl;
                cout << "  Local: " << e.local << endl;
                cout << "  Data: " << e.data.dia << "/" << e.data.mes << "/" 
                     << e.data.ano << " - " << e.horario.hora << ":" 
                     << (e.horario.minuto < 10 ? "0" : "") << e.horario.minuto << endl;
                cout << "  Professor: " << e.professorResponsavel << endl;
                cout << "  Vagas disponiveis: " << e.vagasDisponiveis << endl;
                cout << "  Total de inscritos: " << e.totalinscritos << endl << endl;
                encontrou = true;
            }
        }

        if (!encontrou)
            cout << "Nenhum evento autorizado e ativo encontrado.\n";

        cout << "\nPressione ENTER para voltar...";
        cin.get();
    }

    void inscreverEmEvento(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);
            
            system("cls || clear");
            cout << "------ INSCREVER EM EVENTO ------\n";

            // Carregar eventos
            CarregarEventos();

            if (eventos.empty()) {
                cout << "Nenhum evento disponivel.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            int id;
            cout << "Digite o ID do evento: ";
            cin >> id;
            limparBuffer();

            Evento* eventoEncontrado = nullptr;

            for (Evento &e : eventos) {
                if (e.id == id && e.ativo == 1 && e.autorizado == 1 && !e.finalizado) {
                    eventoEncontrado = &e;
                    break;
                }
            }

            if (eventoEncontrado == nullptr) {
                cout << "Evento nao encontrado, nao autorizado ou ja finalizado.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            // Verificar vagas
            if (eventoEncontrado->vagasDisponiveis <= 0) {
                cout << "Nao ha vagas disponiveis para este evento!\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            // Verificar se ja esta inscrito
            for (int i = 0; i < eventoEncontrado->totalinscritos; i++) {
                if (strcmp(eventoEncontrado->alunos[i], aluno.base.nome) == 0) {
                    cout << "Voce ja esta inscrito neste evento!\n";
                    cout << "\nPressione ENTER para voltar...";
                    cin.get();
                    return;
                }
            }

            // Adicionar inscricao
            strncpy(eventoEncontrado->alunos[eventoEncontrado->totalinscritos], 
                    aluno.base.nome, 99);
            eventoEncontrado->alunos[eventoEncontrado->totalinscritos][99] = '\0';
            
            eventoEncontrado->totalinscritos++;
            eventoEncontrado->vagasDisponiveis--;

            // Salvar eventos
            if (SalvarEventosNoArquivo(eventos)) {
                cout << "Inscricao no evento realizada com sucesso!\n";
                cout << "Vagas restantes: " << eventoEncontrado->vagasDisponiveis << endl;
            } else {
                cout << "Erro ao salvar inscricao.\n";
            }

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
            cout << "Aluno: " << aluno.base.nome << endl << endl;

            // Carregar eventos
            CarregarEventos();

            if (eventos.empty()) {
                cout << "Nenhum evento disponivel no sistema.\n";
                cout << "\nPressione ENTER para voltar...";
                cin.get();
                return;
            }

            bool encontrouInscricao = false;

            for (const Evento &e : eventos) {
                // Procurar se o aluno esta inscrito neste evento
                for (int i = 0; i < e.totalinscritos; i++) {
                    if (strcmp(e.alunos[i], aluno.base.nome) == 0) {
                        encontrouInscricao = true;
                        cout << "\n[ID: " << e.id << "] " << e.nome << endl;
                        cout << "  Data: " << e.data.dia << "/" << e.data.mes << "/" 
                             << e.data.ano << " - " << e.horario.hora << ":" 
                             << (e.horario.minuto < 10 ? "0" : "") << e.horario.minuto << endl;
                        cout << "  Local: " << e.local << endl;
                        cout << "  Professor: " << e.professorResponsavel << endl;
                        cout << "  Total de inscritos: " << e.totalinscritos << endl;
                        break;
                    }
                }
            }

            if (!encontrouInscricao) {
                cout << "Voce nao esta inscrito em nenhum evento.\n";
            }

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
        listarInstrumentosDisponiveis();
    }

    void solicitarEmprestimo(int idAluno) {
        realizarEmprestimo(idAluno);
    }

    void realizarDevolucao(int idAluno) {
        ::realizarDevolucao(idAluno);
    }

    void consultarMeusEmprestimos(int idAluno) {
        listarMeusEmprestimos(idAluno);
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

// ===== PONTO DE ENTRADA DO MENU ALUNO =====
void abrir_menu_aluno(Usuario* usuario) {
    constexpr int qtdOpcoes = 14;
    string opcoes[qtdOpcoes] = {
        "Consultar Notas",
        "Consultar Medias",
        "Consultar Situacao Academica",
        "Ver Eventos Disponiveis",
        "Inscrever-se em Evento",
        "Minhas Inscricoes",
        "Ver Instrumentos Disponiveis",
        "Solicitar Emprestimo",
        "Devolver Instrumento",
        "Meus Emprestimos",
        "Consultar Saldo Lanchonete",
        "Ver Produtos Lanchonete",
        "Realizar Compra",
        "Logout"
    };
    
    bool emMenuAluno = true;
    
    while (emMenuAluno) {
        ConfigMenu config;
        config.titulo = "AREA DO ALUNO";
        config.descricao = "Bem-vindo(a), " + string(usuario->nome) + "!";
        
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
        
        switch (resultado.indice_da_opcao) {
            case 0: Modulo_aluno::consultarNotas(usuario->id); break;
            case 1: Modulo_aluno::consultarMedias(usuario->id); break;
            case 2: Modulo_aluno::consultarSituacaoAcademica(usuario->id); break;
            case 3: Modulo_aluno::visualizarEventosDisponiveis(); break;
            case 4: Modulo_aluno::inscreverEmEvento(usuario->id); break;
            case 5: Modulo_aluno::consultarMinhasInscricoes(usuario->id); break;
            case 6: Modulo_aluno::visualizarInstrumentosDisponiveis(); break;
            case 7: Modulo_aluno::solicitarEmprestimo(usuario->id); break;
            case 8: Modulo_aluno::realizarDevolucao(usuario->id); break;
            case 9: Modulo_aluno::consultarMeusEmprestimos(usuario->id); break;
            case 10: Modulo_aluno::consultarSaldo(usuario->id); break;
            case 11: Modulo_aluno::visualizarProdutos(); break;
            case 12: Modulo_aluno::comprarProduto(usuario->id); break;
            case 13:
                usuario->logado = false;
                emMenuAluno = false;
                break;
        }
    }
}

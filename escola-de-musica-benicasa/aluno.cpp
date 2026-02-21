#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstring>
#include <vector>
#include "aluno.h"
#include "headers.h"
#include "lanchonete.h"
#include "interface_grafica.h"
#include "instrumentos.h"
#include "eventos.h"

using namespace std;

extern vector<Evento> eventos;
extern void CarregarEventos();
extern bool SalvarEventosNoArquivo(const vector<Evento>& eventos);

namespace Modulo_aluno {

    void limparBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

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

    void consultarNotas(int idAluno) {
        try {

            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;
            int turmaCount = 0;

            while (file.read((char*)&turma, sizeof(Turma))) {
                turmaCount++;

                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    file.close();

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

                    mostrar_detalhes(topicos, numTopicos, configDet);
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

    void consultarMedias(int idAluno) {
        try {

            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;

            while (file.read((char*)&turma, sizeof(Turma))) {

                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    file.close();

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

            ifstream file("turmas.dat", ios::binary);
            if (!file) {
                mostrar_caixa_informacao("ERRO", "Arquivo turmas.dat nao encontrado!");
                return;
            }

            Turma turma;
            bool encontrado = false;

            while (file.read((char*)&turma, sizeof(Turma))) {

                Aluno *alunoNaTurma = procurarAlunoNaTurma(turma, idAluno);
                if (alunoNaTurma != nullptr) {
                    encontrado = true;
                    file.close();

                    string status;
                    if (alunoNaTurma->faltas > 10)
                        status = "REPROVADO POR FALTA";
                    else if (alunoNaTurma->media >= 7.0)
                        status = "APROVADO";
                    else if (alunoNaTurma->media >= 5.0)
                        status = "RECUPERACAO";
                    else
                        status = "REPROVADO";

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

    void visualizarEventosDisponiveis() {

        CarregarEventos();

        if (eventos.empty()) {
            mostrar_caixa_informacao("INFO", "Nenhum evento disponivel no sistema.");
            return;
        }

        bool encontrou = false;

        vector<TopicDetalhes> topicos_eventos;
        for (const Evento &e : eventos) {
            if (e.ativo == 1 && e.autorizado == 1 && !e.finalizado) {
                TopicDetalhes t;
                t.titulo = e.nome;
                string info = string("Local: ") + e.local + "\n";
                info += "Data: " + to_string(e.data.dia) + "/" + to_string(e.data.mes) + "/" + to_string(e.data.ano);
                info += " - " + to_string(e.horario.hora) + ":" + (e.horario.minuto < 10 ? "0" : "") + to_string(e.horario.minuto);
                info += "\nProfessor: " + string(e.professorResponsavel);
                info += "\nVagas: " + to_string(e.vagasDisponiveis) + " | Inscritos: " + to_string(e.totalinscritos);
                t.descricao = info;
                topicos_eventos.push_back(t);
                encontrou = true;
            }
        }

        if (!encontrou) {
            mostrar_caixa_informacao("INFO", "Nenhum evento autorizado e ativo encontrado.");
        } else {
            ConfigDetalhes configDet;
            configDet.titulo = "EVENTOS DISPONIVEIS";
            configDet.descricao = "Eventos cadastrados no sistema";
            configDet.cores.cor_borda = 6;
            
            TopicDetalhes* topicos_arr = new TopicDetalhes[topicos_eventos.size()];
            for (size_t i = 0; i < topicos_eventos.size(); i++) {
                topicos_arr[i] = topicos_eventos[i];
            }
            mostrar_detalhes(topicos_arr, topicos_eventos.size(), configDet);
            delete[] topicos_arr;
        }
    }

    void inscreverEmEvento(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            CarregarEventos();

            if (eventos.empty()) {
                mostrar_caixa_informacao("INFO", "Nenhum evento disponivel.");
                return;
            }

            int id;
            ConfigEntradaTexto configId;
            configId.titulo = "INSCREVER EM EVENTO";
            configId.label = "ID do evento: ";
            configId.tipo_entrada = TIPO_NUMERO;
            saida_entrada_texto resId = interface_para_entrada_texto(configId);
            if (!resId.confirmado) return;
            id = stoi(resId.valor);

            Evento* eventoEncontrado = nullptr;

            for (Evento &e : eventos) {
                if (e.id == id && e.ativo == 1 && e.autorizado == 1 && !e.finalizado) {
                    eventoEncontrado = &e;
                    break;
                }
            }

            if (eventoEncontrado == nullptr) {
                mostrar_caixa_informacao("ERRO", "Evento nao encontrado, nao autorizado ou ja finalizado.");
                return;
            }

            if (eventoEncontrado->vagasDisponiveis <= 0) {
                mostrar_caixa_informacao("ERRO", "Nao ha vagas disponiveis para este evento!");
                return;
            }

            for (int i = 0; i < eventoEncontrado->totalinscritos; i++) {
                if (strcmp(eventoEncontrado->alunos[i], aluno.base.nome) == 0) {
                    mostrar_caixa_informacao("ERRO", "Voce ja esta inscrito neste evento!");
                    return;
                }
            }

            strncpy(eventoEncontrado->alunos[eventoEncontrado->totalinscritos], 
                    aluno.base.nome, 99);
            eventoEncontrado->alunos[eventoEncontrado->totalinscritos][99] = '\0';
            
            eventoEncontrado->totalinscritos++;
            eventoEncontrado->vagasDisponiveis--;

            if (SalvarEventosNoArquivo(eventos)) {
                mostrar_caixa_informacao("SUCESSO", "Inscricao no evento realizada com sucesso!");
            } else {
                mostrar_caixa_informacao("ERRO", "Erro ao salvar inscricao.");
            }
        } catch (const exception &e) {
            mostrar_caixa_informacao("ERRO", e.what());
        }
    }

    void consultarMinhasInscricoes(int idAluno) {
        try {
            Aluno aluno = carregarAluno(idAluno);

            CarregarEventos();

            if (eventos.empty()) {
                mostrar_caixa_informacao("INFO", "Nenhum evento disponivel no sistema.");
                return;
            }

            bool encontrouInscricao = false;

            vector<TopicDetalhes> topicos_inscricoes;
            for (const Evento &e : eventos) {

                for (int i = 0; i < e.totalinscritos; i++) {
                    if (strcmp(e.alunos[i], aluno.base.nome) == 0) {
                        encontrouInscricao = true;
                        TopicDetalhes t;
                        t.titulo = e.nome;
                        string info = string("Data: ") + to_string(e.data.dia) + "/" + to_string(e.data.mes) + "/" + to_string(e.data.ano);
                        info += " - " + to_string(e.horario.hora) + ":" + (e.horario.minuto < 10 ? "0" : "") + to_string(e.horario.minuto);
                        info += "\nLocal: " + string(e.local);
                        info += "\nProfessor: " + string(e.professorResponsavel);
                        info += "\nInscritos: " + to_string(e.totalinscritos);
                        t.descricao = info;
                        topicos_inscricoes.push_back(t);
                        break;
                    }
                }
            }

            if (!encontrouInscricao) {
                mostrar_caixa_informacao("INFO", "Voce nao esta inscrito em nenhum evento.");
            } else {
                ConfigDetalhes configDet;
                configDet.titulo = "MINHAS INSCRICOES";
                configDet.descricao = "Aluno: " + string(aluno.base.nome);
                configDet.cores.cor_borda = 6;
                
                TopicDetalhes* topicos_arr = new TopicDetalhes[topicos_inscricoes.size()];
                for (size_t i = 0; i < topicos_inscricoes.size(); i++) {
                    topicos_arr[i] = topicos_inscricoes[i];
                }
                mostrar_detalhes(topicos_arr, topicos_inscricoes.size(), configDet);
                delete[] topicos_arr;
            }
        } catch (const exception &e) {
            mostrar_caixa_informacao("ERRO", e.what());
        }
    }

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
}

void abrir_menu_aluno(Usuario* usuario) {
    constexpr int qtdOpcoes = 15;
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
        "Realizar Compra Lanchonete",
        "Solicitar Creditos Lanchonete",
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
            case 10: Lanchonete::consultarSaldo(usuario->id); break;
            case 11: Lanchonete::visualizarProdutos(); break;
            case 12: Lanchonete::realizarCompra(usuario->id); break;
            case 13: Lanchonete::solicitarCreditosUsuario(usuario->id); break;
            case 14:
                usuario->logado = false;
                emMenuAluno = false;
                break;
        }
    }
}

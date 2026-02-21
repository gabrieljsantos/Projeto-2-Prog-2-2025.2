#include "eventos.h"
#include "interface_grafica.h"
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
    
    e.id = proximoID;
    proximoID++;
    e.finalizado = false;
    e.totalinscritos = 0;

    // ─── Nome do evento ────────────────────────────────────
    ConfigEntradaTexto configNome;
    configNome.titulo = "CADASTRAR NOVO EVENTO";
    configNome.descricao = "Preencha os campos para criar um novo evento";
    configNome.label = "Nome do evento: ";
    configNome.tamanho_maximo = 99;
    configNome.tipo_entrada = TIPO_TEXTO;

    saida_entrada_texto resNome = interface_para_entrada_texto(configNome);
    if (!resNome.confirmado) return;
    strncpy(e.nome, resNome.valor.c_str(), 99);
    e.nome[99] = '\0';

    // ─── Local do evento ───────────────────────────────────
    ConfigEntradaTexto configLocal;
    configLocal.titulo = "CADASTRAR NOVO EVENTO";
    configLocal.descricao = "Evento: " + resNome.valor;
    configLocal.label = "Local: ";
    configLocal.tamanho_maximo = 99;
    configLocal.tipo_entrada = TIPO_TEXTO;

    saida_entrada_texto resLocal = interface_para_entrada_texto(configLocal);
    if (!resLocal.confirmado) return;
    strncpy(e.local, resLocal.valor.c_str(), 99);
    e.local[99] = '\0';

    // ─── Hora ──────────────────────────────────────────────
    ConfigEntradaTexto configHora;
    configHora.titulo = "CADASTRAR NOVO EVENTO";
    configHora.descricao = "Evento: " + resNome.valor + " | Local: " + resLocal.valor;
    configHora.label = "Hora (0-23): ";
    configHora.tamanho_maximo = 2;
    configHora.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resHora = interface_para_entrada_texto(configHora);
    if (!resHora.confirmado) return;
    e.horario.hora = atoi(resHora.valor.c_str());
    if (e.horario.hora < 0 || e.horario.hora > 23) {
        mostrar_caixa_informacao("ERRO", "Hora inválida! Digite um valor entre 0 e 23.");
        return;
    }

    // ─── Minuto ────────────────────────────────────────────
    ConfigEntradaTexto configMinuto;
    configMinuto.titulo = "CADASTRAR NOVO EVENTO";
    configMinuto.descricao = "Evento: " + resNome.valor + " | Horário: " + resHora.valor + ":00";
    configMinuto.label = "Minuto (0-59): ";
    configMinuto.tamanho_maximo = 2;
    configMinuto.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resMinuto = interface_para_entrada_texto(configMinuto);
    if (!resMinuto.confirmado) return;
    e.horario.minuto = atoi(resMinuto.valor.c_str());
    if (e.horario.minuto < 0 || e.horario.minuto > 59) {
        mostrar_caixa_informacao("ERRO", "Minuto inválido! Digite um valor entre 0 e 59.");
        return;
    }

    // ─── Vagas ─────────────────────────────────────────────
    ConfigEntradaTexto configVagas;
    configVagas.titulo = "CADASTRAR NOVO EVENTO";
    configVagas.descricao = "Evento: " + resNome.valor;
    configVagas.label = "Número de vagas: ";
    configVagas.tamanho_maximo = 3;
    configVagas.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resVagas = interface_para_entrada_texto(configVagas);
    if (!resVagas.confirmado) return;
    e.vagasDisponiveis = atoi(resVagas.valor.c_str());
    if (e.vagasDisponiveis < 0) {
        mostrar_caixa_informacao("ERRO", "Vagas não podem ser negativas!");
        return;
    }

    // ─── Professor responsável ────────────────────────────
    ConfigEntradaTexto configProf;
    configProf.titulo = "CADASTRAR NOVO EVENTO";
    configProf.descricao = "Evento: " + resNome.valor + " | Vagas: " + resVagas.valor;
    configProf.label = "Professor responsável: ";
    configProf.tamanho_maximo = 99;
    configProf.tipo_entrada = TIPO_TEXTO;

    saida_entrada_texto resProf = interface_para_entrada_texto(configProf);
    if (!resProf.confirmado) return;
    strncpy(e.professorResponsavel, resProf.valor.c_str(), 99);
    e.professorResponsavel[99] = '\0';

    // ─── Data (Dia) ────────────────────────────────────────
    ConfigEntradaTexto configDia;
    configDia.titulo = "CADASTRAR NOVO EVENTO";
    configDia.descricao = "Evento: " + resNome.valor;
    configDia.label = "Dia (1-31): ";
    configDia.tamanho_maximo = 2;
    configDia.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resDia = interface_para_entrada_texto(configDia);
    if (!resDia.confirmado) return;
    e.data.dia = atoi(resDia.valor.c_str());
    if (e.data.dia < 1 || e.data.dia > 31) {
        mostrar_caixa_informacao("ERRO", "Dia inválido! Digite um valor entre 1 e 31.");
        return;
    }

    // ─── Data (Mês) ────────────────────────────────────────
    ConfigEntradaTexto configMes;
    configMes.titulo = "CADASTRAR NOVO EVENTO";
    configMes.descricao = "Evento: " + resNome.valor + " | Dia: " + resDia.valor;
    configMes.label = "Mês (1-12): ";
    configMes.tamanho_maximo = 2;
    configMes.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resMes = interface_para_entrada_texto(configMes);
    if (!resMes.confirmado) return;
    e.data.mes = atoi(resMes.valor.c_str());
    if (e.data.mes < 1 || e.data.mes > 12) {
        mostrar_caixa_informacao("ERRO", "Mês inválido! Digite um valor entre 1 e 12.");
        return;
    }

    // ─── Data (Ano) ────────────────────────────────────────
    ConfigEntradaTexto configAno;
    configAno.titulo = "CADASTRAR NOVO EVENTO";
    configAno.descricao = "Evento: " + resNome.valor + " | Data: " + resDia.valor + "/" + resMes.valor;
    configAno.label = "Ano (2026-2027): ";
    configAno.tamanho_maximo = 4;
    configAno.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resAno = interface_para_entrada_texto(configAno);
    if (!resAno.confirmado) return;
    e.data.ano = atoi(resAno.valor.c_str());
    if (e.data.ano < 2026 || e.data.ano > 2027) {
        mostrar_caixa_informacao("ERRO", "Ano inválido! Digite um valor entre 2026 e 2027.");
        return;
    }

    // ─── Descrição (GRANDE - até 200 caracteres) ──────────
    ConfigEntradaTexto configDesc;
    configDesc.titulo = "CADASTRAR NOVO EVENTO";
    configDesc.descricao = "Evento: " + resNome.valor + " | Data: " + resDia.valor + "/" + resMes.valor + "/" + resAno.valor;
    configDesc.label = "Descrição: ";
    configDesc.tamanho_maximo = 200;
    configDesc.tipo_entrada = TIPO_TEXTO;

    saida_entrada_texto resDesc = interface_para_entrada_texto_grande(configDesc);
    if (!resDesc.confirmado) return;
    strncpy(e.descricao, resDesc.valor.c_str(), 199);
    e.descricao[199] = '\0';

    eventos.push_back(e);

    if (SalvarEventosNoArquivo(eventos)) {
        mostrar_caixa_informacao("SUCESSO", 
            "Evento cadastrado com sucesso!\nID do evento: " + to_string(e.id));
    } else {
        mostrar_caixa_informacao("ERRO", "Erro ao salvar o evento.");
    }
}

void editar_evento(vector<Evento>& eventos) {
    ConfigEntradaTexto configBusca;
    configBusca.titulo = "EDITAR EVENTO";
    configBusca.descricao = "Buscar evento no sistema";
    configBusca.label = "ID do evento: ";
    configBusca.tamanho_maximo = 10;
    configBusca.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resBusca = interface_para_entrada_texto(configBusca);
    if (!resBusca.confirmado) return;
    
    int id = atoi(resBusca.valor.c_str());

    Evento *e = nullptr;

    for (auto& ev : eventos) {
        if (ev.id == id) {
            e = &ev;
            break;
        }
    }

    if (e == nullptr) {
        mostrar_caixa_informacao("ERRO", "Evento não encontrado.");
        return;
    }

    bool continuar_editando = true;
    while (continuar_editando) {
        constexpr int qtdOpcoes = 8;
        string opcoes[qtdOpcoes] = {
            "Editar Nome",
            "Editar Local",
            "Editar Professor Responsável",
            "Editar Data",
            "Editar Horário",
            "Editar Número de Vagas",
            "Editar Descrição",
            "Voltar"
        };

        ConfigMenu configMenu;
        configMenu.titulo = "EDITAR EVENTO";
        configMenu.descricao = "Editando: " + string(e->nome) + " (ID: " + to_string(e->id) + ")";
        
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, configMenu);
        
        switch (resultado.indice_da_opcao) {
            case 0: { // Nome
                ConfigEntradaTexto config;
                config.titulo = "EDITAR EVENTO - NOME";
                config.descricao = "Evento: " + string(e->nome);
                config.label = "Novo nome: ";
                config.valor_inicial = string(e->nome);
                config.tamanho_maximo = 99;
                config.tipo_entrada = TIPO_TEXTO;

                saida_entrada_texto res = interface_para_entrada_texto(config);
                if (res.confirmado) {
                    strncpy(e->nome, res.valor.c_str(), 99);
                    e->nome[99] = '\0';
                    mostrar_caixa_informacao("SUCESSO", "Nome atualizado!");
                }
                break;
            }
            case 1: { // Local
                ConfigEntradaTexto config;
                config.titulo = "EDITAR EVENTO - LOCAL";
                config.descricao = "Evento: " + string(e->nome);
                config.label = "Novo local: ";
                config.valor_inicial = string(e->local);
                config.tamanho_maximo = 99;
                config.tipo_entrada = TIPO_TEXTO;

                saida_entrada_texto res = interface_para_entrada_texto(config);
                if (res.confirmado) {
                    strncpy(e->local, res.valor.c_str(), 99);
                    e->local[99] = '\0';
                    mostrar_caixa_informacao("SUCESSO", "Local atualizado!");
                }
                break;
            }
            case 2: { // Professor
                ConfigEntradaTexto config;
                config.titulo = "EDITAR EVENTO - PROFESSOR";
                config.descricao = "Evento: " + string(e->nome);
                config.label = "Novo professor: ";
                config.valor_inicial = string(e->professorResponsavel);
                config.tamanho_maximo = 99;
                config.tipo_entrada = TIPO_TEXTO;

                saida_entrada_texto res = interface_para_entrada_texto(config);
                if (res.confirmado) {
                    strncpy(e->professorResponsavel, res.valor.c_str(), 99);
                    e->professorResponsavel[99] = '\0';
                    mostrar_caixa_informacao("SUCESSO", "Professor atualizado!");
                }
                break;
            }
            case 3: { // Data
                ConfigEntradaTexto configDia;
                configDia.titulo = "EDITAR EVENTO - DATA";
                configDia.descricao = "Evento: " + string(e->nome);
                configDia.label = "Novo dia (1-31): ";
                configDia.valor_inicial = to_string(e->data.dia);
                configDia.tamanho_maximo = 2;
                configDia.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resDia = interface_para_entrada_texto(configDia);
                if (!resDia.confirmado) break;
                
                int dia = atoi(resDia.valor.c_str());
                if (dia < 1 || dia > 31) {
                    mostrar_caixa_informacao("ERRO", "Dia inválido!");
                    break;
                }
                e->data.dia = dia;

                ConfigEntradaTexto configMes;
                configMes.titulo = "EDITAR EVENTO - DATA";
                configMes.descricao = "Evento: " + string(e->nome) + " | Dia: " + resDia.valor;
                configMes.label = "Novo mês (1-12): ";
                configMes.valor_inicial = to_string(e->data.mes);
                configMes.tamanho_maximo = 2;
                configMes.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resMes = interface_para_entrada_texto(configMes);
                if (!resMes.confirmado) break;
                
                int mes = atoi(resMes.valor.c_str());
                if (mes < 1 || mes > 12) {
                    mostrar_caixa_informacao("ERRO", "Mês inválido!");
                    break;
                }
                e->data.mes = mes;

                ConfigEntradaTexto configAno;
                configAno.titulo = "EDITAR EVENTO - DATA";
                configAno.descricao = "Evento: " + string(e->nome) + " | Data: " + resDia.valor + "/" + resMes.valor;
                configAno.label = "Novo ano (2026-2027): ";
                configAno.valor_inicial = to_string(e->data.ano);
                configAno.tamanho_maximo = 4;
                configAno.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resAno = interface_para_entrada_texto(configAno);
                if (!resAno.confirmado) break;
                
                int ano = atoi(resAno.valor.c_str());
                if (ano < 2026 || ano > 2027) {
                    mostrar_caixa_informacao("ERRO", "Ano inválido!");
                    break;
                }
                e->data.ano = ano;
                mostrar_caixa_informacao("SUCESSO", "Data atualizada!");
                break;
            }
            case 4: { // Horário
                ConfigEntradaTexto configHora;
                configHora.titulo = "EDITAR EVENTO - HORÁRIO";
                configHora.descricao = "Evento: " + string(e->nome);
                configHora.label = "Nova hora (0-23): ";
                configHora.valor_inicial = to_string(e->horario.hora);
                configHora.tamanho_maximo = 2;
                configHora.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resHora = interface_para_entrada_texto(configHora);
                if (!resHora.confirmado) break;
                
                int hora = atoi(resHora.valor.c_str());
                if (hora < 0 || hora > 23) {
                    mostrar_caixa_informacao("ERRO", "Hora inválida!");
                    break;
                }
                e->horario.hora = hora;

                ConfigEntradaTexto configMinuto;
                configMinuto.titulo = "EDITAR EVENTO - HORÁRIO";
                configMinuto.descricao = "Evento: " + string(e->nome) + " | Hora: " + resHora.valor;
                configMinuto.label = "Novo minuto (0-59): ";
                configMinuto.valor_inicial = to_string(e->horario.minuto);
                configMinuto.tamanho_maximo = 2;
                configMinuto.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resMinuto = interface_para_entrada_texto(configMinuto);
                if (!resMinuto.confirmado) break;
                
                int minuto = atoi(resMinuto.valor.c_str());
                if (minuto < 0 || minuto > 59) {
                    mostrar_caixa_informacao("ERRO", "Minuto inválido!");
                    break;
                }
                e->horario.minuto = minuto;
                mostrar_caixa_informacao("SUCESSO", "Horário atualizado!");
                break;
            }
            case 5: { // Vagas
                ConfigEntradaTexto config;
                config.titulo = "EDITAR EVENTO - VAGAS";
                config.descricao = "Evento: " + string(e->nome) + " | Inscritos: " + to_string(e->totalinscritos);
                config.label = "Novo número de vagas: ";
                config.valor_inicial = to_string(e->vagasDisponiveis);
                config.tamanho_maximo = 3;
                config.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto res = interface_para_entrada_texto(config);
                if (res.confirmado) {
                    int vagas = atoi(res.valor.c_str());
                    if (vagas < e->totalinscritos) {
                        mostrar_caixa_informacao("ERRO", "Vagas não podem ser menores que inscritos!");
                        break;
                    }
                    e->vagasDisponiveis = vagas;
                    mostrar_caixa_informacao("SUCESSO", "Vagas atualizadas!");
                }
                break;
            }
            case 6: { // Descrição
                ConfigEntradaTexto config;
                config.titulo = "EDITAR EVENTO - DESCRIÇÃO";
                config.descricao = "Evento: " + string(e->nome);
                config.label = "Nova descrição: ";
                config.valor_inicial = string(e->descricao);
                config.tamanho_maximo = 200;
                config.tipo_entrada = TIPO_TEXTO;

                saida_entrada_texto res = interface_para_entrada_texto_grande(config);
                if (res.confirmado) {
                    strncpy(e->descricao, res.valor.c_str(), 199);
                    e->descricao[199] = '\0';
                    mostrar_caixa_informacao("SUCESSO", "Descrição atualizada!");
                }
                break;
            }
            case 7: { // Voltar
                continuar_editando = false;
                break;
            }
        }
    }

    if (SalvarEventosNoArquivo(eventos))
        mostrar_caixa_informacao("SUCESSO", "Alterações salvas com sucesso!");
    else
        mostrar_caixa_informacao("ERRO", "Erro ao salvar alterações.");
}

void excluir_evento(int id_excluir) {
    bool achou = false;
    Evento evento_removido;
    
    for (size_t i = 0; i < eventos.size(); i++) {
        if (eventos[i].id == id_excluir) {
            evento_removido = eventos[i];
            eventos.erase(eventos.begin() + i);
            achou = true;
            break;
        }
    }

    if (achou) {
        if (SalvarEventosNoArquivo(eventos)) {
            mostrar_caixa_informacao("SUCESSO", 
                "Evento '" + string(evento_removido.nome) + "' excluído com sucesso!");
        } else {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar alterações.");
        }
    } else {
        mostrar_caixa_informacao("ERRO", "Evento não encontrado!");
    }
}

void finalizar_evento() {
    ConfigEntradaTexto configBusca;
    configBusca.titulo = "FINALIZAR EVENTO";
    configBusca.descricao = "Marcar evento como finalizado";
    configBusca.label = "ID do evento: ";
    configBusca.tamanho_maximo = 10;
    configBusca.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resBusca = interface_para_entrada_texto(configBusca);
    if (!resBusca.confirmado) return;
    
    int id = atoi(resBusca.valor.c_str());

    Evento* e = nullptr;

    for (auto& ev : eventos) {
        if (ev.id == id) {
            e = &ev;
            break;
        }
    }

    if (e == nullptr) {
        mostrar_caixa_informacao("ERRO", "Evento não encontrado.");
        return;
    }

    if (e->finalizado) {
        mostrar_caixa_informacao("AVISO", "Este evento já foi finalizado!");
        return;
    }

    e->finalizado = true;

    if (SalvarEventosNoArquivo(eventos)) {
        mostrar_caixa_informacao("SUCESSO", 
            "Evento '" + string(e->nome) + "' finalizado com sucesso!");
    } else {
        mostrar_caixa_informacao("ERRO", "Erro ao salvar alterações.");
    }
}

void listar_alunos_inscritos() {
    ConfigEntradaTexto config;
    config.titulo = "LISTAR ALUNOS INSCRITOS";
    config.descricao = "Buscar evento no sistema";
    config.label = "ID do evento: ";
    config.tamanho_maximo = 10;
    config.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto res = interface_para_entrada_texto(config);
    if (!res.confirmado) return;
    
    int idevento = atoi(res.valor.c_str());

    Evento* eventoescolhido = nullptr;

    for (Evento &e : eventos) {
        if (e.id == idevento) {
            eventoescolhido = &e;
            break;
        }
    }

    if (eventoescolhido == nullptr) {
        mostrar_caixa_informacao("ERRO", "Evento não encontrado.");
        return;
    }

    if (eventoescolhido->totalinscritos == 0) {
        mostrar_caixa_informacao("INFO", "Nenhum aluno inscrito neste evento.");
        return;
    }

    string alunos_listados = "Evento: " + string(eventoescolhido->nome) + "\n\n";
    for (int i = 0; i < eventoescolhido->totalinscritos; i++) {
        alunos_listados += to_string(i + 1) + " - " + string(eventoescolhido->alunos[i]) + "\n";
    }
    alunos_listados += "\nTotal de inscritos: " + to_string(eventoescolhido->totalinscritos);

    mostrar_caixa_informacao("ALUNOS INSCRITOS", alunos_listados);
}

void listar_eventos(const vector<Evento>& eventos) {
    constexpr int qtdOpcoes = 2;
    string opcoes[qtdOpcoes] = {
        "Eventos Disponíveis",
        "Eventos Finalizados"
    };

    ConfigMenu configMenu;
    configMenu.titulo = "LISTAR EVENTOS";
    configMenu.descricao = "Escolha qual tipo de evento deseja visualizar";

    saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, configMenu);

    vector<Evento> eventos_filtrados;
    bool selecionou_disponiveis = (resultado.indice_da_opcao == 0);

    for (const Evento& e : eventos) {
        if ((selecionou_disponiveis && !e.finalizado) || 
            (!selecionou_disponiveis && e.finalizado)) {
            eventos_filtrados.push_back(e);
        }
    }

    if (eventos_filtrados.empty()) {
        string tipo = selecionou_disponiveis ? "disponíveis" : "finalizados";
        mostrar_caixa_informacao("INFO", "Nenhum evento " + tipo + " no sistema.");
        return;
    }

    int num_linhas = eventos_filtrados.size();
    constexpr int num_colunas = 6;
    
    string** matriz = new string*[num_linhas];
    for (int i = 0; i < num_linhas; i++) {
        matriz[i] = new string[num_colunas];
    }

    for (int i = 0; i < num_linhas; i++) {
        const Evento& e = eventos_filtrados[i];
        
        matriz[i][0] = to_string(e.id);
        matriz[i][1] = string(e.nome);
        matriz[i][2] = to_string(e.vagasDisponiveis);
        matriz[i][3] = string(e.professorResponsavel);
        matriz[i][4] = to_string(e.data.dia) + "/" + to_string(e.data.mes) + "/" + to_string(e.data.ano);
        matriz[i][5] = to_string(e.horario.hora) + ":" + (e.horario.minuto < 10 ? "0" : "") + to_string(e.horario.minuto);
    }

    string titulos[num_colunas] = {
        "ID",
        "Nome",
        "Vagas",
        "Professor",
        "Data",
        "Horário"
    };

    int larguras[num_colunas] = {4, 25, 6, 18, 12, 8};

    ConfigTabela configTabela;
    configTabela.titulo = selecionou_disponiveis ? "EVENTOS DISPONÍVEIS" : "EVENTOS FINALIZADOS";
    configTabela.descricao = "Total: " + to_string(num_linhas);
    configTabela.larguras_colunas = larguras;
    configTabela.num_colunas = num_colunas;
    configTabela.max_colunas = num_colunas;

    saida_tabela resultado_tabela = interface_para_tabela(num_linhas, num_colunas, 
                                                          const_cast<const string**>(matriz), 
                                                          titulos, 0, configTabela);

    if (resultado_tabela.indice_linha >= 0 && resultado_tabela.indice_linha < num_linhas) {
        const Evento& e = eventos_filtrados[resultado_tabela.indice_linha];
        
        TopicDetalhes topicos[9];
        topicos[0].titulo = "ID";
        topicos[0].descricao = to_string(e.id);
        
        topicos[1].titulo = "Nome";
        topicos[1].descricao = string(e.nome);
        
        topicos[2].titulo = "Local";
        topicos[2].descricao = string(e.local);
        
        topicos[3].titulo = "Professor Responsável";
        topicos[3].descricao = string(e.professorResponsavel);
        
        topicos[4].titulo = "Data";
        topicos[4].descricao = to_string(e.data.dia) + "/" + to_string(e.data.mes) + "/" + to_string(e.data.ano);
        
        topicos[5].titulo = "Horário";
        topicos[5].descricao = to_string(e.horario.hora) + ":" + (e.horario.minuto < 10 ? "0" : "") + to_string(e.horario.minuto);
        
        topicos[6].titulo = "Vagas Disponíveis";
        topicos[6].descricao = to_string(e.vagasDisponiveis);
        
        topicos[7].titulo = "Total de Inscritos";
        topicos[7].descricao = to_string(e.totalinscritos);
        
        topicos[8].titulo = "Descrição";
        topicos[8].descricao = string(e.descricao);

        ConfigDetalhes configDetalhes;
        configDetalhes.titulo = "DETALHES DO EVENTO";
        configDetalhes.descricao = "Evento ID " + to_string(e.id);
        configDetalhes.topicos_por_pagina = 5;

        mostrar_detalhes(topicos, 9, configDetalhes);
    }

    for (int i = 0; i < num_linhas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void inscrever_alunos() {
    ConfigEntradaTexto configBusca;
    configBusca.titulo = "INSCREVER ALUNO EM EVENTO";
    configBusca.descricao = "Buscar evento no sistema";
    configBusca.label = "ID do evento: ";
    configBusca.tamanho_maximo = 10;
    configBusca.tipo_entrada = TIPO_NUMERO;

    saida_entrada_texto resBusca = interface_para_entrada_texto(configBusca);
    if (!resBusca.confirmado) return;
    
    int idevento = atoi(resBusca.valor.c_str());

    Evento* eventoescolhido = nullptr;

    for (Evento &e : eventos) {
        if (e.id == idevento && !e.finalizado) {
            eventoescolhido = &e;
            break;
        }
    }

    if (eventoescolhido == nullptr) {
        mostrar_caixa_informacao("ERRO", "Evento não encontrado ou já finalizado.");
        return;
    }

    if (eventoescolhido->vagasDisponiveis <= 0) {
        mostrar_caixa_informacao("AVISO", "Não há vagas disponíveis neste evento.");
        return;
    }

    if (eventoescolhido->totalinscritos >= 100) {
        mostrar_caixa_informacao("ERRO", "Limite máximo de alunos atingido.");
        return;
    }

    ConfigEntradaTexto configAluno;
    configAluno.titulo = "INSCREVER ALUNO EM EVENTO";
    configAluno.descricao = "Evento: " + string(eventoescolhido->nome) + 
                            " | Vagas disponíveis: " + to_string(eventoescolhido->vagasDisponiveis);
    configAluno.label = "Nome do aluno: ";
    configAluno.tamanho_maximo = 99;
    configAluno.tipo_entrada = TIPO_TEXTO;

    saida_entrada_texto resAluno = interface_para_entrada_texto(configAluno);
    if (!resAluno.confirmado) return;

    strncpy(eventoescolhido->alunos[eventoescolhido->totalinscritos], 
            resAluno.valor.c_str(), 99);
    eventoescolhido->alunos[eventoescolhido->totalinscritos][99] = '\0';

    eventoescolhido->totalinscritos++;
    eventoescolhido->vagasDisponiveis--;

    if (SalvarEventosNoArquivo(eventos)) {
        mostrar_caixa_informacao("SUCESSO", 
            "Aluno inscrito com sucesso!\nVagas restantes: " + 
            to_string(eventoescolhido->vagasDisponiveis));
    } else {
        mostrar_caixa_informacao("ERRO", "Erro ao salvar inscrição.");
    }
}

void menu_eventos() {
    bool continuar = true;

    CarregarEventos();

    constexpr int qtdOpcoes = 7;
    string opcoes[qtdOpcoes] = {
        "Cadastrar Evento",
        "Editar Evento",
        "Listar Eventos",
        "Listar Alunos Inscritos",
        "Inscrever Aluno em Evento",
        "Finalizar Evento",
        "Voltar"
    };

    while (continuar) {
        ConfigMenu config;
        config.titulo = "GERENCIAMENTO DE EVENTOS";
        config.descricao = "Total de eventos: " + to_string(eventos.size());
        
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
        
        switch (resultado.indice_da_opcao) {
            case 0:
                adicionar_evento();
                break;
            case 1:
                editar_evento(eventos);
                break;
            case 2:
                listar_eventos(eventos);
                break;
            case 3:
                listar_alunos_inscritos();
                break;
            case 4:
                inscrever_alunos();
                break;
            case 5:
                finalizar_evento();
                break;
            case 6:
                continuar = false;
                break;
            default:
                continuar = false;
                break;
        }
    }
}

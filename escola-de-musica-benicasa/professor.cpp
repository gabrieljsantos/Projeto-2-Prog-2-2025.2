#include "professor.h"
#include "login_matricula.h"
#include "headers.h"
#include "interface_grafica.h"
#include "lanchonete.h"
#include "eventos.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

#define MAX_TURMAS 30
#define MAX_ALUNOS 20
#define MAX_TURMAS_PROF 5
#define MEDIA_MINIMA 7.0
#define FALTAS_MAXIMAS 10

namespace Calculos
{
    float somaRecursivaNotas(float notas[], int n)
    {
        if (n == 0)
            return 0;
        return notas[n - 1] + somaRecursivaNotas(notas, n - 1);
    }

    float calcularMedia(float notas[], int n)
    {
        if (n == 0)
            return 0;
        float media = somaRecursivaNotas(notas, n) / n;
        return media < 0 ? 0 : media;
    }
}

bool turmaPermitida(int id_prof, int turma_index) // 20260001
{
    if (turma_index < 1 || turma_index > MAX_TURMAS)
        return false;

    ifstream file("turmas.dat", ios::binary);
    if (!file)
        return false;

    file.seekg((turma_index - 1) * sizeof(Turma));
    Turma turma;
    file.read((char *)&turma, sizeof(Turma));
    file.close();

    if (turma.id_prof != id_prof)
        return false;

    Professor prof = Login_mat::lerProfessor(id_prof);

    for (int i = 0; i < MAX_TURMAS_PROF; i++)
        if (prof.turmas[i] == turma.id)
            return true;

    return false;
}

namespace ModuloProfessor
{
    static bool lerTurma(int index, Turma &turma)
    {
        ifstream file("turmas.dat", ios::binary);
        if (!file)
            return false;

        file.seekg((index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        file.close();
        return true;
    }

    // ============================
    // Seletor grafico de turma
    // Retorna o indice da turma (campo id) ou -1 se cancelado
    // ============================
    int selecionarTurma(int id_prof)
    {
        Professor prof = Login_mat::lerProfessor(id_prof);

        // Contar turmas validas
        int total = 0;
        Turma turmasProf[MAX_TURMAS_PROF];
        int indicesTurma[MAX_TURMAS_PROF];

        for (int t = 0; t < MAX_TURMAS_PROF; t++)
        {
            if (prof.turmas[t] == 0)
                continue;

            Turma turma;
            if (!lerTurma(prof.turmas[t], turma))
                continue;

            turmasProf[total] = turma;
            indicesTurma[total] = prof.turmas[t];
            total++;
        }

        if (total == 0)
        {
            mostrar_caixa_informacao("INFO", "Voce nao possui turmas atribuidas.");
            return -1;
        }

        // Montar tabela
        string dados[MAX_TURMAS_PROF][3];
        const string* dados_ptr[MAX_TURMAS_PROF];

        for (int i = 0; i < total; i++)
        {
            dados[i][0] = to_string(indicesTurma[i]);
            dados[i][1] = turmasProf[i].nome;
            dados[i][2] = to_string(turmasProf[i].qtdAlunos) + " aluno(s)";
            dados_ptr[i] = dados[i];
        }

        string titulos[3] = {"Numero", "Nome da Turma", "Alunos"};

        ConfigTabela configTab;
        configTab.titulo = "SELECIONAR TURMA";
        configTab.descricao = "Escolha a turma desejada:";
        int larguras[] = {10, 30, 15};
        configTab.larguras_colunas = larguras;
        configTab.num_colunas = 3;

        saida_tabela resultado = interface_para_tabela(total, 3, (const string**)dados_ptr, titulos, 0, configTab);

        if (resultado.indice_linha == -1)
            return -1;

        return indicesTurma[resultado.indice_linha];
    }

    // ============================
    // Seletor grafico de aluno dentro de uma turma
    // Retorna o indice do aluno NO ARRAY da turma, ou -1 se cancelado
    // ============================
    int selecionarAluno(Turma &turma)
    {
        // Contar alunos validos
        int total = 0;
        int indicesAluno[MAX_ALUNOS];

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id != 0)
            {
                indicesAluno[total] = i;
                total++;
            }
        }

        if (total == 0)
        {
            mostrar_caixa_informacao("INFO", "Nenhum aluno nesta turma.");
            return -1;
        }

        // Montar tabela
        string dados[MAX_ALUNOS][4];
        const string* dados_ptr[MAX_ALUNOS];

        for (int i = 0; i < total; i++)
        {
            int idx = indicesAluno[i];
            dados[i][0] = to_string(turma.alunos[idx].base.id);
            dados[i][1] = turma.alunos[idx].base.nome;
            dados[i][2] = to_string(turma.alunos[idx].media);
            dados[i][3] = to_string(turma.alunos[idx].faltas);
            dados_ptr[i] = dados[i];
        }

        string titulos[4] = {"ID", "Nome", "Media", "Faltas"};

        ConfigTabela configTab;
        configTab.titulo = "SELECIONAR ALUNO";
        configTab.descricao = "Turma: " + string(turma.nome);
        int larguras[] = {10, 30, 10, 10};
        configTab.larguras_colunas = larguras;
        configTab.num_colunas = 4;

        saida_tabela resultado = interface_para_tabela(total, 4, (const string**)dados_ptr, titulos, 0, configTab);

        if (resultado.indice_linha == -1)
            return -1;

        return indicesAluno[resultado.indice_linha];
    }

    void registrarNotas(int id_prof)
    {
        cout << "[DEBUG] registrarNotas: Iniciando registro de notas" << endl;
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        cout << "[DEBUG] registrarNotas: Turma selecionada - ID=" << turma_index << endl;
        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        cout << "[DEBUG] registrarNotas: Turma carregada - " << turma.nome << endl;

        int aluno_idx = selecionarAluno(turma);
        if (aluno_idx == -1)
        {
            file.close();
            return;
        }

        cout << "[DEBUG] registrarNotas: Aluno selecionado - " << turma.alunos[aluno_idx].base.nome << endl;
        float notas[2];
        for (int j = 0; j < 2; j++)
        {
            ConfigEntradaTexto configNota;
            configNota.titulo = "REGISTRAR NOTAS";
            configNota.descricao = "Aluno: " + string(turma.alunos[aluno_idx].base.nome);
            configNota.label = "Nota " + to_string(j + 1) + ": ";
            configNota.tipo_entrada = TIPO_NUMERO;

            saida_entrada_texto resultado = interface_para_entrada_texto(configNota);
            if (!resultado.confirmado)
            {
                file.close();
                return;
            }
            notas[j] = stof(resultado.valor);
            turma.alunos[aluno_idx].notas[j] = notas[j];
            cout << "[DEBUG] registrarNotas: Nota " << (j + 1) << " = " << notas[j] << endl;
        }

        turma.alunos[aluno_idx].media = Calculos::calcularMedia(notas, 2);
        cout << "[DEBUG] registrarNotas: Media calculada = " << turma.alunos[aluno_idx].media << endl;

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        cout << "[DEBUG] registrarNotas: Notas salvas no arquivo" << endl;

        mostrar_caixa_informacao("SUCESSO", "Notas registradas com sucesso!");
        system("pause");
    }

    void registrarNotasTodo(int id_prof)
    {
        cout << "[DEBUG] registrarNotasTodo: Iniciando registro de notas para toda a turma" << endl;
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        cout << "[DEBUG] registrarNotasTodo: Turma selecionada - ID=" << turma_index << endl;
        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        cout << "[DEBUG] registrarNotasTodo: Turma carregada - " << turma.nome << endl;

        int alunosProcessados = 0;
        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            cout << "[DEBUG] registrarNotasTodo: Processando aluno - " << (*aluno).base.nome << endl;
            float notas[2];
            for (int j = 0; j < 2; j++)
            {
                ConfigEntradaTexto configNota;
                configNota.titulo = "REGISTRAR NOTAS - TURMA";
                configNota.descricao = "Aluno: " + string((*aluno).base.nome);
                configNota.label = "Nota " + to_string(j + 1) + ": ";
                configNota.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resultado = interface_para_entrada_texto(configNota);
                if (!resultado.confirmado)
                {
                    file.close();
                    return;
                }
                notas[j] = stof(resultado.valor);
                cout << "[DEBUG] registrarNotasTodo: Nota " << (j + 1) << " = " << notas[j] << endl;
            }

            (*aluno).notas[0] = notas[0];
            (*aluno).notas[1] = notas[1];
            (*aluno).media = Calculos::calcularMedia(notas, 2);
            cout << "[DEBUG] registrarNotasTodo: Media = " << (*aluno).media << endl;
            alunosProcessados++;
        }

        cout << "[DEBUG] registrarNotasTodo: Total de alunos processados = " << alunosProcessados << endl;
        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        cout << "[DEBUG] registrarNotasTodo: Dados salvos no arquivo" << endl;

        mostrar_caixa_informacao("SUCESSO", "Notas registradas para toda a turma!");
        system("pause");
    }

    void consultarAlunosMatriculados(int id_prof)
    {
        Professor *prof = new Professor;
        *prof = Login_mat::lerProfessor(id_prof);

        // Primeiro, selecionar a turma via interface grafica
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
        {
            delete prof;
            return;
        }

        Turma turma;
        if (!lerTurma(turma_index, turma))
        {
            delete prof;
            return;
        }

        // Montar tabela de alunos
        int totalAlunos = 0;
        string dadosAlunos[MAX_ALUNOS][6];
        const string* dadosAlunos_ptr[MAX_ALUNOS];

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            dadosAlunos[totalAlunos][0] = to_string((*aluno).base.id);
            dadosAlunos[totalAlunos][1] = (*aluno).base.nome;
            dadosAlunos[totalAlunos][2] = to_string((*aluno).notas[0]);
            dadosAlunos[totalAlunos][3] = to_string((*aluno).notas[1]);
            dadosAlunos[totalAlunos][4] = to_string((*aluno).media);
            dadosAlunos[totalAlunos][5] = to_string((*aluno).faltas);
            dadosAlunos_ptr[totalAlunos] = dadosAlunos[totalAlunos];
            totalAlunos++;
        }

        if (totalAlunos == 0)
        {
            mostrar_caixa_informacao("INFO", "Nenhum aluno matriculado nesta turma.");
        }
        else
        {
            string titulosAlunos[6] = {"ID", "Nome", "N1", "N2", "Media", "Faltas"};

            ConfigTabela configTab;
            configTab.titulo = "ALUNOS MATRICULADOS";
            configTab.descricao = "Turma: " + string(turma.nome);
            int larguras[] = {8, 25, 8, 8, 8, 8};
            configTab.larguras_colunas = larguras;
            configTab.num_colunas = 6;

            interface_para_tabela(totalAlunos, 6, (const string**)dadosAlunos_ptr, titulosAlunos, 0, configTab);
        }

        delete prof;
    }

    void consultarDesempenhoAcademico(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        Turma turma;
        if (!lerTurma(turma_index, turma))
            return;

        Aluno **criticos = new Aluno *[MAX_ALUNOS];
        int qtd = 0;

        int aprovados = 0, reprovadosNota = 0, reprovadosFalta = 0;
        float soma = 0;
        int total = 0;

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            total++;
            soma += (*aluno).media;

            if ((*aluno).faltas > FALTAS_MAXIMAS)
            {
                reprovadosFalta++;
                criticos[qtd++] = aluno;
            }
            else if ((*aluno).media >= MEDIA_MINIMA)
                aprovados++;
            else
            {
                reprovadosNota++;
                criticos[qtd++] = aluno;
            }
        }

        if (total == 0)
        {
            mostrar_caixa_informacao("INFO", "Nenhum aluno nesta turma.");
            delete[] criticos;
            return;
        }

        // Montar topicos para mostrar_detalhes
        int numTopicos = 0;
        TopicDetalhes topicos[MAX_ALUNOS + 4]; // resumo + alunos criticos

        // Resumo geral
        topicos[numTopicos].titulo = "Media da Turma";
        topicos[numTopicos].descricao = to_string(soma / total);
        numTopicos++;

        topicos[numTopicos].titulo = "Aprovados";
        topicos[numTopicos].descricao = to_string(aprovados) + " aluno(s)";
        numTopicos++;

        topicos[numTopicos].titulo = "Reprovados por Nota";
        topicos[numTopicos].descricao = to_string(reprovadosNota) + " aluno(s)";
        numTopicos++;

        topicos[numTopicos].titulo = "Reprovados por Falta";
        topicos[numTopicos].descricao = to_string(reprovadosFalta) + " aluno(s)";
        numTopicos++;

        if (qtd > 0)
        {
            for (int i = 0; i < qtd; i++)
            {
                topicos[numTopicos].titulo = "[CRITICO] " + string((*criticos[i]).base.nome);
                topicos[numTopicos].descricao = "Media: " + to_string((*criticos[i]).media)
                                              + " | Faltas: " + to_string((*criticos[i]).faltas);
                numTopicos++;
            }
        }

        ConfigDetalhes configDet;
        configDet.titulo = "DESEMPENHO ACADEMICO";
        configDet.descricao = "Turma: " + string(turma.nome);
        configDet.cores.cor_borda = 6; // Par de cor igual ao fundo (COLOR_BLACK)

        mostrar_detalhes(topicos, numTopicos, configDet);

        delete[] criticos;
    }

    void registrarFaltas(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            ConfigBotoes configBotao;
            configBotao.titulo = "REGISTRAR FALTAS";
            configBotao.descricao = "Turma: " + string(turma.nome);
            configBotao.pergunta = string((*aluno).base.nome) + " faltou?";
            configBotao.botoes[0] = {'S', "Sim", 1};
            configBotao.botoes[1] = {'N', "Nao", 0};
            configBotao.numero_botoes = 2;

            saida_botoes resultado = interface_para_botoes(configBotao);

            if (!resultado.confirmado)
            {
                file.close();
                return;
            }

            if (resultado.valor_retorno == 1)
                (*aluno).faltas++;
        }

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();

        mostrar_caixa_informacao("SUCESSO", "Faltas registradas com sucesso!");
    }

    void consultarTurma(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        Turma turma;

        ifstream file("turmas.dat", ios::binary);
        if (!file)
        {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir arquivo.");
            return;
        }

        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        file.close();

        // Montar tabela de alunos da turma
        int totalAlunos = 0;
        string dadosAlunos[MAX_ALUNOS][4];
        const string* dadosAlunos_ptr[MAX_ALUNOS];

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == 0)
                continue;

            dadosAlunos[totalAlunos][0] = to_string(turma.alunos[i].base.id);
            dadosAlunos[totalAlunos][1] = turma.alunos[i].base.nome;
            dadosAlunos[totalAlunos][2] = to_string(turma.alunos[i].media);
            dadosAlunos[totalAlunos][3] = to_string(turma.alunos[i].faltas);
            dadosAlunos_ptr[totalAlunos] = dadosAlunos[totalAlunos];
            totalAlunos++;
        }

        if (totalAlunos > 0)
        {
            string titulosAlunos[4] = {"ID", "Nome", "Media", "Faltas"};

            ConfigTabela configTab;
            configTab.titulo = "TURMA: " + string(turma.nome);
            configTab.descricao = "Professor ID: " + to_string(turma.id_prof) + " | Alunos: " + to_string(totalAlunos);
            int larguras[] = {10, 30, 10, 10};
            configTab.larguras_colunas = larguras;
            configTab.num_colunas = 4;

            interface_para_tabela(totalAlunos, 4, (const string**)dadosAlunos_ptr, titulosAlunos, 0, configTab);
        }
        else
        {
            mostrar_caixa_informacao("INFO", "Turma " + string(turma.nome) + " nao possui alunos.");
        }

        // Mostrar historico de avaliacoes
        if (turma.qtdAvaliacoes > 0)
        {
            TopicDetalhes topicos[MAX_AVALIACOES];
            for (int i = 0; i < turma.qtdAvaliacoes; i++)
            {
                topicos[i].titulo = string(turma.avaliacoes[i].data);
                topicos[i].descricao = string(turma.avaliacoes[i].descricao);
            }

            ConfigDetalhes configDet;
            configDet.titulo = "HISTORICO DE AVALIACOES";
            configDet.descricao = "Turma: " + string(turma.nome);
            configDet.cores.cor_borda = 6; // Par de cor igual ao fundo (COLOR_BLACK)

            mostrar_detalhes(topicos, turma.qtdAvaliacoes, configDet);
        }
        else
        {
            mostrar_caixa_informacao("AVALIACOES", "Nenhuma avaliacao registrada para esta turma.");
        }
    }

    void registrarAvaliacao(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1)
            return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
        {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir arquivo.");
            return;
        }

        Turma turma;

        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        if (turma.qtdAvaliacoes >= MAX_AVALIACOES)
        {
            mostrar_caixa_informacao("INFO", "Limite de avaliacoes atingido.");
            file.close();
            return;
        }

        Avaliacao *av = &turma.avaliacoes[turma.qtdAvaliacoes];

        // Data
        ConfigEntradaTexto configData;
        configData.titulo = "REGISTRAR AVALIACAO";
        configData.descricao = "Turma: " + string(turma.nome);
        configData.label = "Data (dd/mm/aaaa): ";
        configData.tamanho_maximo = 11;

        saida_entrada_texto resData = interface_para_entrada_texto(configData);
        if (!resData.confirmado)
        {
            file.close();
            return;
        }

        strncpy(av->data, resData.valor.c_str(), 11);
        av->data[11] = '\0';

        // Descricao
        ConfigEntradaTexto configDesc;
        configDesc.titulo = "REGISTRAR AVALIACAO";
        configDesc.descricao = "Turma: " + string(turma.nome) + " | Data: " + resData.valor;
        configDesc.label = "Descricao da avaliacao: ";
        configDesc.tamanho_maximo = 99;

        saida_entrada_texto resDesc = interface_para_entrada_texto(configDesc);
        if (!resDesc.confirmado)
        {
            file.close();
            return;
        }
        /*
        strncpy siginica :
- Copia no máximo o número especificado de caracteres de uma string para outra.
- Se a string de origem for menor que o número especificado, a função preencherá o restante do destino com caracteres nulos ('\0').
- Garante que a string de destino seja sempre terminada com um caractere nulo, evitando problemas de leitura além do limite da string.
        */
        strncpy(av->descricao, resDesc.valor.c_str(), 99);
        av->descricao[99] = '\0';

        turma.qtdAvaliacoes++;

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();

        mostrar_caixa_informacao("SUCESSO", "Avaliacao registrada com sucesso!");
    }
}

void abrir_menu_professor(Usuario* usuario) {
    constexpr int qtdOpcoes = 13;
    string opcoes[qtdOpcoes] = {
        "Registrar Notas",
        "Registrar Avaliacoes",
        "Consultar Alunos Matriculados",
        "Calcular Medias da Turma",
        "Consultar Desempenho Academico",
        "Consultar Turma",
        "Registrar Faltas",
        "Consultar Saldo Lanchonete",
        "Ver Produtos Lanchonete",
        "Realizar Compra Lanchonete",
        "Solicitar Creditos Lanchonete",
        "Cadastrar Evento",
        "Logout"
    };
    
    bool emMenuProfessor = true;
    
    while (emMenuProfessor) {
        ConfigMenu config;
        config.titulo = "AREA DO PROFESSOR";
        config.descricao = "Bem-vindo(a), " + string(usuario->nome) + "!";
        
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
        
        switch (resultado.indice_da_opcao) {
            case 0: ModuloProfessor::registrarNotas(usuario->id); break;
            case 1: ModuloProfessor::registrarAvaliacao(usuario->id); break;
            case 2: ModuloProfessor::consultarAlunosMatriculados(usuario->id); break;
            case 3: ModuloProfessor::registrarNotasTodo(usuario->id); break;
            case 4: ModuloProfessor::consultarDesempenhoAcademico(usuario->id); break;
            case 5: ModuloProfessor::consultarTurma(usuario->id); break;
            case 6: ModuloProfessor::registrarFaltas(usuario->id); break;
            case 7: Lanchonete::consultarSaldo(usuario->id); break;
            case 8: Lanchonete::visualizarProdutos(); break;
            case 9: Lanchonete::realizarCompra(usuario->id); break;
            case 10: Lanchonete::solicitarCreditosUsuario(usuario->id); break;
            case 11: adicionar_evento(); break;
            case 12:
                usuario->logado = false;
                emMenuProfessor = false;
                break;
        }
    }
}

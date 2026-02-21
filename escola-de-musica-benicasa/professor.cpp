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

bool turmaPermitida(int id_prof, int turma_index)
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

    int selecionarTurma(int id_prof)
    {
        Professor prof = Login_mat::lerProfessor(id_prof);
        int total = 0;
        Turma turmasProf[MAX_TURMAS_PROF];
        int indicesTurma[MAX_TURMAS_PROF];

        for (int t = 0; t < MAX_TURMAS_PROF; t++)
        {
            if (prof.turmas[t] == 0) continue;
            Turma turma;
            if (!lerTurma(prof.turmas[t], turma)) continue;
            turmasProf[total] = turma;
            indicesTurma[total] = prof.turmas[t];
            total++;
        }

        if (total == 0)
        {
            mostrar_caixa_informacao("INFO", "Voce nao possui turmas atribuidas.");
            return -1;
        }

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
        return (resultado.indice_linha == -1) ? -1 : indicesTurma[resultado.indice_linha];
    }

    int selecionarAluno(Turma &turma)
    {
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
        return (resultado.indice_linha == -1) ? -1 : indicesAluno[resultado.indice_linha];
    }

    void registrarNotas(int id_prof, int turma_index, int id_aluno)
    {
        if (!turmaPermitida(id_prof, turma_index))
        {
            mostrar_caixa_informacao("ERRO", "Turma nao pertence ao professor.");
            return;
        }

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file) return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == id_aluno)
            {
                float notas[2];
                for (int j = 0; j < 2; j++)
                {
                    ConfigEntradaTexto configNota;
                    configNota.titulo = "REGISTRAR NOTA";
                    configNota.descricao = "Aluno: " + string(turma.alunos[i].base.nome);
                    configNota.label = "Nota " + to_string(j + 1) + ": ";
                    configNota.tipo_entrada = TIPO_NUMERO;

                    saida_entrada_texto res = interface_para_entrada_texto(configNota);
                    if (!res.confirmado) { file.close(); return; }
                    notas[j] = stof(res.valor);
                    turma.alunos[i].notas[j] = notas[j];
                }

                turma.alunos[i].media = Calculos::calcularMedia(notas, 2);
                file.seekp((turma_index - 1) * sizeof(Turma));
                file.write((char *)&turma, sizeof(Turma));
                file.close();
                mostrar_caixa_informacao("SUCESSO", "Notas registradas!");
                return;
            }
        }
        mostrar_caixa_informacao("ERRO", "Aluno nao encontrado.");
        file.close();
    }

    void registrarNotas(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file) return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        int aluno_idx = selecionarAluno(turma);
        if (aluno_idx == -1) { file.close(); return; }

        float notas[2];
        for (int j = 0; j < 2; j++)
        {
            ConfigEntradaTexto configNota;
            configNota.titulo = "REGISTRAR NOTAS";
            configNota.descricao = "Aluno: " + string(turma.alunos[aluno_idx].base.nome);
            configNota.label = "Nota " + to_string(j + 1) + ": ";
            configNota.tipo_entrada = TIPO_NUMERO;

            saida_entrada_texto resultado = interface_para_entrada_texto(configNota);
            if (!resultado.confirmado) { file.close(); return; }
            notas[j] = stof(resultado.valor);
            turma.alunos[aluno_idx].notas[j] = notas[j];
        }

        turma.alunos[aluno_idx].media = Calculos::calcularMedia(notas, 2);
        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        mostrar_caixa_informacao("SUCESSO", "Notas registradas com sucesso!");
    }

    void registrarNotasTodo(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file) return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == 0) continue;

            float notas[2];
            for (int j = 0; j < 2; j++)
            {
                ConfigEntradaTexto configNota;
                configNota.titulo = "REGISTRAR NOTAS - TURMA";
                configNota.descricao = "Aluno: " + string(turma.alunos[i].base.nome);
                configNota.label = "Nota " + to_string(j + 1) + ": ";
                configNota.tipo_entrada = TIPO_NUMERO;

                saida_entrada_texto resultado = interface_para_entrada_texto(configNota);
                if (!resultado.confirmado) { file.close(); return; }
                notas[j] = stof(resultado.valor);
                turma.alunos[i].notas[j] = notas[j];
            }
            turma.alunos[i].media = Calculos::calcularMedia(notas, 2);
        }

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        mostrar_caixa_informacao("SUCESSO", "Notas da turma registradas!");
    }

    void consultarAlunosMatriculados(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        Turma turma;
        if (!lerTurma(turma_index, turma)) return;

        int totalAlunos = 0;
        string dadosAlunos[MAX_ALUNOS][6];
        const string* dadosAlunos_ptr[MAX_ALUNOS];

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == 0) continue;
            dadosAlunos[totalAlunos][0] = to_string(turma.alunos[i].base.id);
            dadosAlunos[totalAlunos][1] = turma.alunos[i].base.nome;
            dadosAlunos[totalAlunos][2] = to_string(turma.alunos[i].notas[0]);
            dadosAlunos[totalAlunos][3] = to_string(turma.alunos[i].notas[1]);
            dadosAlunos[totalAlunos][4] = to_string(turma.alunos[i].media);
            dadosAlunos[totalAlunos][5] = to_string(turma.alunos[i].faltas);
            dadosAlunos_ptr[totalAlunos] = dadosAlunos[totalAlunos];
            totalAlunos++;
        }

        if (totalAlunos == 0) mostrar_caixa_informacao("INFO", "Nenhum aluno matriculado.");
        else {
            string titulos[6] = {"ID", "Nome", "N1", "N2", "Media", "Faltas"};
            ConfigTabela configTab;
            configTab.titulo = "ALUNOS MATRICULADOS";
            configTab.descricao = "Turma: " + string(turma.nome);
            int larguras[] = {8, 25, 8, 8, 8, 8};
            configTab.larguras_colunas = larguras;
            configTab.num_colunas = 6;
            interface_para_tabela(totalAlunos, 6, (const string**)dadosAlunos_ptr, titulos, 0, configTab);
        }
    }

    void consultarDesempenhoAcademico(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        Turma turma;
        if (!lerTurma(turma_index, turma)) return;

        int aprovados = 0, reprovadosNota = 0, reprovadosFalta = 0, total = 0;
        float soma = 0;
        TopicDetalhes topicos[MAX_ALUNOS + 4];
        int numTopicos = 0;

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == 0) continue;
            total++;
            soma += turma.alunos[i].media;

            if (turma.alunos[i].faltas > FALTAS_MAXIMAS) reprovadosFalta++;
            else if (turma.alunos[i].media >= MEDIA_MINIMA) aprovados++;
            else reprovadosNota++;
        }

        if (total == 0) { mostrar_caixa_informacao("INFO", "Sem alunos."); return; }

        topicos[numTopicos++] = {"Media da Turma", to_string(soma / total)};
        topicos[numTopicos++] = {"Aprovados", to_string(aprovados)};
        topicos[numTopicos++] = {"Reprovados (Nota)", to_string(reprovadosNota)};
        topicos[numTopicos++] = {"Reprovados (Falta)", to_string(reprovadosFalta)};

        ConfigDetalhes configDet;
        configDet.titulo = "DESEMPENHO";
        configDet.cores.cor_borda = 6;
        mostrar_detalhes(topicos, numTopicos, configDet);
    }

    void registrarFaltas(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file) return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == 0) continue;

            ConfigBotoes config;
            config.titulo = "FALTAS";
            config.pergunta = string(turma.alunos[i].base.nome) + " faltou?";
            config.botoes[0] = {'S', "Sim", 1};
            config.botoes[1] = {'N', "Nao", 0};
            config.numero_botoes = 2;

            saida_botoes res = interface_para_botoes(config);
            if (!res.confirmado) { file.close(); return; }
            if (res.valor_retorno == 1) turma.alunos[i].faltas++;
        }

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        mostrar_caixa_informacao("SUCESSO", "Faltas registradas!");
    }

    void consultarTurma(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        Turma turma;
        if (!lerTurma(turma_index, turma)) return;

        if (turma.qtdAvaliacoes > 0)
        {
            TopicDetalhes topicos[MAX_AVALIACOES];
            for (int i = 0; i < turma.qtdAvaliacoes; i++)
            {
                topicos[i].titulo = string(turma.avaliacoes[i].data);
                topicos[i].descricao = string(turma.avaliacoes[i].descricao);
            }
            ConfigDetalhes configDet;
            configDet.titulo = "AVALIACOES: " + string(turma.nome);
            mostrar_detalhes(topicos, turma.qtdAvaliacoes, configDet);
        }
        else mostrar_caixa_informacao("INFO", "Sem avaliacoes.");
    }

    void registrarAvaliacao(int id_prof)
    {
        int turma_index = selecionarTurma(id_prof);
        if (turma_index == -1) return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file) return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        if (turma.qtdAvaliacoes >= MAX_AVALIACOES)
        {
            mostrar_caixa_informacao("LIMITE", "Limite de avaliacoes atingido.");
            file.close();
            return;
        }

        Avaliacao *av = &turma.avaliacoes[turma.qtdAvaliacoes];

        ConfigEntradaTexto configData;
        configData.label = "Data (dd/mm/aaaa): ";
        saida_entrada_texto resData = interface_para_entrada_texto(configData);
        if (!resData.confirmado) { file.close(); return; }
        strncpy(av->data, resData.valor.c_str(), 11);
        av->data[11] = '\0';

        ConfigEntradaTexto configDesc;
        configDesc.label = "Descricao: ";
        saida_entrada_texto resDesc = interface_para_entrada_texto(configDesc);
        if (!resDesc.confirmado) { file.close(); return; }
        strncpy(av->descricao, resDesc.valor.c_str(), 99);
        av->descricao[99] = '\0';

        turma.qtdAvaliacoes++;
        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();
        mostrar_caixa_informacao("SUCESSO", "Avaliacao registrada!");
    }
}

void abrir_menu_professor(Usuario* usuario) {
    constexpr int qtdOpcoes = 12;
    string opcoes[qtdOpcoes] = {
        "Registrar Notas", "Registrar Avaliacoes", "Consultar Alunos Matriculados",
        "Consultar Desempenho Academico", "Consultar Turma",
        "Registrar Faltas", "Consultar Saldo Lanchonete", "Ver Produtos Lanchonete",
        "Realizar Compra Lanchonete", "Solicitar Creditos Lanchonete", "Cadastrar Evento", "Logout"
    };
    
    while (true) {
        ConfigMenu config;
        config.titulo = "AREA DO PROFESSOR";
        config.descricao = "Bem-vindo(a), " + string(usuario->nome) + "!";
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
        
        switch (resultado.indice_da_opcao) {
            case 0: ModuloProfessor::registrarNotas(usuario->id); break;
            case 1: ModuloProfessor::registrarAvaliacao(usuario->id); break;
            case 2: ModuloProfessor::consultarAlunosMatriculados(usuario->id); break;
            case 3: ModuloProfessor::consultarDesempenhoAcademico(usuario->id); break;
            case 4: ModuloProfessor::consultarTurma(usuario->id); break;
            case 5: ModuloProfessor::registrarFaltas(usuario->id); break;
            case 6: Lanchonete::consultarSaldo(usuario->id); break;
            case 7: Lanchonete::visualizarProdutos(); break;
            case 8: Lanchonete::realizarCompra(usuario->id); break;
            case 9: Lanchonete::solicitarCreditosUsuario(usuario->id); break;
            case 10: adicionar_evento(); break;
            case 11: usuario->logado = false; return;
        }
    }
}
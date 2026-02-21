#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include "admin.h"
#include "headers.h"
#include "interface_grafica.h"
#include "eventos.h"
#include "instrumentos.h"
#include "lanchonete.h"
#include "login_matricula.h"

using namespace std;

// =====================================================================
// SEÇÃO: FUNÇÕES AUXILIARES GERAIS
// =====================================================================


void openFile(std::fstream &f, const std::string Nome){
    f.open(Nome, std::ios::in | std::ios::out | std::ios::binary);

    if (!f)
    {
        f.open(Nome, std::ios::out | std::ios::binary);
        f.close();
        f.open(Nome, std::ios::in | std::ios::out | std::ios::binary);
    }
}

// =====================================================================
// SEÇÃO: FUNÇÕES DE BUSCA
// =====================================================================

Professor buscaProf(std::fstream &file,int buscaId){
    Professor prof{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Professor);
    int posicao = buscaId - 20260001;

    if(posicao < 0 || posicao >= total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return prof;
    }
    file.seekg((buscaId - 20260001) * sizeof(Professor));
    file.read((char*)&prof,sizeof(Professor));
    if(prof.base.id == buscaId&&prof.base.ativo){
        return prof;
    }
    else{
        return prof;
    }
}

Disciplina buscaDisciplina(std::fstream &file, int buscaId){
    Disciplina disc{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Disciplina);
    if (buscaId <= 0 || buscaId > total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return disc;
    }
    file.seekg((buscaId - 1) * sizeof(Disciplina));
    file.read((char*)&disc, sizeof(Disciplina));
    if (disc.id == buscaId){
        return disc;
    }
    else{
        return disc;
    }
}

Aluno buscaAluno(std::fstream &file, int buscaId){
    Aluno aluno{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Aluno);
    int posicao = buscaId - 20260001;
    
    if(posicao < 0 || posicao >= total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return aluno;
    }
    file.seekg((buscaId - 20260001) * sizeof(Aluno));
    file.read((char*)&aluno, sizeof(Aluno));
    if(aluno.base.id == buscaId && aluno.base.ativo){
        return aluno;
    }
    else{
        return aluno;
    }
}





int gerarNovoId(std::fstream &file, size_t tamanhoStruct){
    file.clear();
    file.seekg(0, std::ios::end);
    std::streampos bytes = file.tellg();
    int totalRegistros = bytes / tamanhoStruct;
    return totalRegistros + 1;
}

// =====================================================================
// SEÇÃO: FUNÇÕES DE CONSULTA E RELATÓRIOS
// =====================================================================

void consultarPendenciasInstrumentos() {
    constexpr int Quantidades_opcoes = 3;
    bool continuar = true;
    string filtroAluno = "";

    while (continuar) {
        string opcoes[Quantidades_opcoes] = {
            "Listar Todas as Pendencias",
            "Filtrar por Aluno",
            "Voltar"
        };
        
        ConfigMenu config;
        config.titulo = "Consultar Pendencias de Instrumentos";   
        config.descricao = "Selecione uma opcao para gerenciar as pendencias.";
        saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
        
        std::fstream fileEmprestimo;
        std::fstream fileAlunos;
        
        switch (resultado.indice_da_opcao) {
            case 0: { // Listar todas as pendências
                openFile(fileEmprestimo, "emprestimos.dat");
                openFile(fileAlunos, "alunos.dat");
                
                string dados[100][6];
                int contador = 0;
                
                Emprestimo emp;
                Aluno user;
                
                fileEmprestimo.clear();
                fileEmprestimo.seekg(0);
                
                while(fileEmprestimo.read((char*)&emp, sizeof(Emprestimo)) && contador < 100) {
                    fileAlunos.clear();
                    fileAlunos.seekg((emp.idAluno - 20260001) * sizeof(Aluno));
                    
                    if(!fileAlunos.read((char*)&user, sizeof(Aluno)))
                        continue;
                    
                    if(user.base.categoria == ALUNO && user.base.ativo) {
                        dados[contador][0] = to_string(emp.idEmprestimo);
                        dados[contador][1] = user.base.nome;
                        dados[contador][2] = emp.nome_In;
                        dados[contador][3] = emp.dataEmprestimo;
                        dados[contador][4] = emp.dataPrevista;
                        dados[contador][5] = user.base.email;
                        contador++;
                    }
                }
                
                fileEmprestimo.clear();
                fileAlunos.clear();
                
                if (contador == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma pendencia encontrada!");
                    fileEmprestimo.close();
                    fileAlunos.close();
                    break;
                }
                
                string titulos[6] = {"ID Emp", "Aluno", "Instrumento", "Data Emp", "Data Prev", "Email"};
                const string* dados_ptr[100];
                for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Pendencias de Instrumentos";
                saida_tabela selecionada = interface_para_tabela(contador, 6, dados_ptr, titulos, 0, configTab);
                
                if (selecionada.indice_linha != -1) {
                    string detalhes = "ID Emprestimo: " + dados[selecionada.indice_linha][0] + 
                                    "\nAluno: " + dados[selecionada.indice_linha][1] + 
                                    "\nInstrumento: " + dados[selecionada.indice_linha][2] + 
                                    "\nData Emprestimo: " + dados[selecionada.indice_linha][3] + 
                                    "\nData Prevista Devolucao: " + dados[selecionada.indice_linha][4];
                    mostrar_caixa_informacao("DETALHES", detalhes);
                }
                
                fileEmprestimo.close();
                fileAlunos.close();
                break;
            }
            
            case 1: { // Filtrar por aluno
                ConfigEntradaTexto configEntrada;
                configEntrada.titulo = "Filtrar por Aluno";
                configEntrada.label = "Digite o ID do Aluno: ";
                configEntrada.tipo_entrada = TIPO_NUMERO;
                saida_entrada_texto resultado_entrada = interface_para_entrada_texto(configEntrada);
                
                int idAluno = 0;
                if (resultado_entrada.confirmado) {
                    idAluno = stoi(resultado_entrada.valor);
                } else {
                    break;
                }
                
                openFile(fileEmprestimo, "emprestimos.dat");
                openFile(fileAlunos, "alunos.dat");
                
                string dados[100][6];
                int contador = 0;
                
                Emprestimo emp;
                Aluno user;
                
                fileEmprestimo.clear();
                fileEmprestimo.seekg(0);
                
                while(fileEmprestimo.read((char*)&emp, sizeof(Emprestimo)) && contador < 100) {
                    if (emp.idAluno != idAluno)
                        continue;
                    
                    fileAlunos.clear();
                    fileAlunos.seekg((emp.idAluno - 20260001) * sizeof(Aluno));
                    
                    if(!fileAlunos.read((char*)&user, sizeof(Aluno)))
                        continue;
                    
                    if(user.base.categoria == ALUNO && user.base.ativo) {
                        dados[contador][0] = to_string(emp.idEmprestimo);
                        dados[contador][1] = user.base.nome;
                        dados[contador][2] = emp.nome_In;
                        dados[contador][3] = emp.dataEmprestimo;
                        dados[contador][4] = emp.dataPrevista;
                        dados[contador][5] = user.base.email;
                        contador++;
                    }
                }
                
                fileEmprestimo.clear();
                fileAlunos.clear();
                
                if (contador == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma pendencia encontrada para este aluno!");
                    fileEmprestimo.close();
                    fileAlunos.close();
                    break;
                }
                
                string titulos[6] = {"ID Emp", "Aluno", "Instrumento", "Data Emp", "Data Prev", "Email"};
                const string* dados_ptr[100];
                for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Pendencias do Aluno";
                saida_tabela selecionada = interface_para_tabela(contador, 6, dados_ptr, titulos, 0, configTab);
                
                if (selecionada.indice_linha != -1) {
                    string detalhes = "ID Emprestimo: " + dados[selecionada.indice_linha][0] + 
                                    "\nAluno: " + dados[selecionada.indice_linha][1] + 
                                    "\nInstrumento: " + dados[selecionada.indice_linha][2] + 
                                    "\nData Emprestimo: " + dados[selecionada.indice_linha][3] + 
                                    "\nData Prevista Devolucao: " + dados[selecionada.indice_linha][4];
                    mostrar_caixa_informacao("DETALHES", detalhes);
                }
                
                fileEmprestimo.close();
                fileAlunos.close();
                break;
            }
            
            case 2: { // Voltar
                continuar = false;
                break;
            }
        }
    }
}


// =====================================================================
// NAMESPACE: MÓDULO DE ADMINISTRAÇÃO (mod_ADM)
// Contém todos os menus e funções do painel administrativo
// =====================================================================

namespace mod_ADM {

    // ----- FUNÇÕES AUXILIARES DE LISTAGEM -----

    void atualizar_estado_de_usuario(int id_usuario, Funcao tipo_usuario, string estado) {
        bool novo_estado = (estado == "Ativo") ? true : false;

        switch (tipo_usuario) {
            case ALUNO: {
                Aluno aluno = Login_mat::lerAluno(id_usuario);
                if (aluno.base.id == 0)
                    return;
                aluno.base.ativo = novo_estado;
                Login_mat::salvarAluno(aluno);
                break;
            }
            case PROFESSOR: {
                Professor professor = Login_mat::lerProfessor(id_usuario);
                if (professor.base.id == 0)
                    return;
                professor.base.ativo = novo_estado;
                Login_mat::salvarProfessor(professor);
                break;
            }
            case ADMINISTRADOR: {
                Admin admin = Login_mat::inicializarAdminVazio();
                int posicao = Login_mat::adquirirPosicaoDoId(id_usuario);
                
                ifstream inAdmin(ARQUIVO_ADMINISTRADORES, ios::in | ios::binary);
                if (inAdmin.fail()) {
                    return;
                }
                
                inAdmin.seekg((posicao - 1) * sizeof(Admin));
                inAdmin.read((char*)(&admin), sizeof(Admin));
                inAdmin.close();
                
                if (admin.base.id == 0)
                    return;
                
                admin.base.ativo = novo_estado;
                Login_mat::salvarAdmin(admin);
                break;
            }
            default:
                return;
        }
    }

    void gerenciar_usuario_menu(int idUsuario) {
        // Buscar usuario nos alunos e professores
        bool encontrado = false;
        string nome_usuario = "";
        string email_usuario = "";
        string status_usuario = "";
        Funcao tipo_encontrado = ALUNO;
        
        // Buscar nos alunos e professores
        Aluno aluno = Login_mat::lerAluno(idUsuario);
        if (aluno.base.id == idUsuario) {
            encontrado = true;
            nome_usuario = string(aluno.base.nome);
            email_usuario = string(aluno.base.email);
            status_usuario = aluno.base.ativo ? "Ativo" : "Inativo";
            tipo_encontrado = ALUNO;
        } else {
            Professor prof = Login_mat::lerProfessor(idUsuario);
            if (prof.base.id == idUsuario) {
                encontrado = true;
                nome_usuario = string(prof.base.nome);
                email_usuario = string(prof.base.email);
                status_usuario = prof.base.ativo ? "Ativo" : "Inativo";
                tipo_encontrado = PROFESSOR;
            }
        }
        
        if (!encontrado) {
            mostrar_caixa_informacao("ERRO", "Usuario nao encontrado!");
            return;
        }
        
        // Exibir menu de gerenciamento
        ConfigBotoes configBotoes;
        configBotoes.titulo = "Gerenciar Usuario";
        configBotoes.descricao = "ID: " + to_string(idUsuario) + " | Nome: " + nome_usuario + " | Email: " + email_usuario + " | Status: " + status_usuario;
        configBotoes.botoes[0].label = "Ativa";
        configBotoes.botoes[0].tecla = 'A';
        configBotoes.botoes[0].valor_retorno = 1;
        configBotoes.botoes[1].label = "Desativa";
        configBotoes.botoes[1].tecla = 'D';
        configBotoes.botoes[1].valor_retorno = 0;
        configBotoes.botoes[2].label = "Modifica";
        configBotoes.botoes[2].tecla = 'M';
        configBotoes.botoes[2].valor_retorno = 2;
        configBotoes.botoes[3].label = "Sobre";
        configBotoes.botoes[3].tecla = 'S';
        configBotoes.botoes[3].valor_retorno = 3;
        configBotoes.numero_botoes = 4;
        saida_botoes acao_sob_usuario = interface_para_botoes(configBotoes);
        
        if (acao_sob_usuario.confirmado) {
            if (acao_sob_usuario.valor_retorno == 1) {
                // Ativar usuario
                atualizar_estado_de_usuario(idUsuario, tipo_encontrado, "Ativo");
                mostrar_caixa_informacao("SUCESSO", "Usuario ativado com sucesso!");
            }
            if (acao_sob_usuario.valor_retorno == 0) {
                // Desativar usuario
                atualizar_estado_de_usuario(idUsuario, tipo_encontrado, "Inativo");
                mostrar_caixa_informacao("SUCESSO", "Usuario desativado com sucesso!");
            }
            // valor_retorno == 2: Modifica (sem implementação)
            // valor_retorno == 3: Sobre (sem implementação)
        }
    }

    int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, string dados[100][6]){
            int contador = 0;
            
            switch (tipo_usuario)
            {
            case ALUNO: {
                std::fstream file;
                openFile(file, "alunos.dat");
                Aluno aluno;
                file.seekg(0);
                while(file.read((char*)&aluno, sizeof(Aluno)) && contador < 100) {
                    if ((aluno.base.id > USUARIO_VAZIO_ID) && (ativo == 2 || aluno.base.ativo == (ativo == 1))) {
                        dados[contador][0] = to_string(aluno.base.id); 
                        dados[contador][1] = aluno.base.nome;
                        dados[contador][2] = aluno.base.email;
                        dados[contador][3] = to_string(aluno.base.categoria);
                        dados[contador][4] = aluno.base.ativo ? "Ativo" : "Inativo";
                        contador++;
                    }
                }
                file.close();
                break;
            }
            case PROFESSOR: {
                std::fstream file;
                openFile(file, "professores.dat");
                Professor prof;
                file.seekg(0);
                while(file.read((char*)&prof, sizeof(Professor)) && contador < 100) {
                    if ((prof.base.id > USUARIO_VAZIO_ID) && (ativo == 2 || prof.base.ativo == (ativo == 1))) {
                        dados[contador][0] = to_string(prof.base.id); 
                        dados[contador][1] = prof.base.nome;
                        dados[contador][2] = prof.base.email;
                        dados[contador][3] = to_string(prof.base.categoria);
                        dados[contador][4] = prof.base.ativo ? "Ativo" : "Inativo";
                        contador++;
                    }
                }
                file.close();
                break;
            }

            default:
                break;
            }
            return contador;
    }

    int listar_disciplinas_especificas(int ativo, string dados[100][6]){
        int contador = 0;
        std::fstream file;
        openFile(file, "disciplinas.dat");
        Disciplina disciplina;
        
        file.seekg(0);
        while(file.read((char*)&disciplina, sizeof(Disciplina)) && contador < 100) {
            if(disciplina.id != 0 && (disciplina.ativo == ativo || ativo == 2)) {
                dados[contador][0] = to_string(disciplina.id);
                dados[contador][1] = disciplina.nome;
                dados[contador][2] = to_string(disciplina.cargaHoraria);
                dados[contador][3] = disciplina.ativo ? "Ativa" : "Inativa";
                dados[contador][4] = "";
                contador++;
            }
        }
        file.close();
        return contador;
    }

    int listar_disciplinas_avancado(int ativo, int comProfessor, string dados[100][6]){
        // ativo: 0=inativo, 1=ativo, 2=ambos
        // comProfessor: 0=sem professor, 1=com professor, 2=ambos
        int contador = 0;
        std::fstream file;
        std::fstream fileProf;
        openFile(file, "disciplinas.dat");
        openFile(fileProf, "professores.dat");
        Disciplina disciplina;
        
        file.seekg(0);
        while(file.read((char*)&disciplina, sizeof(Disciplina)) && contador < 100) {
            if(disciplina.id == 0) continue;
            if(ativo != 2 && disciplina.ativo != (ativo == 1)) continue;
            
            // Verificar se disciplina tem professor vinculado
            bool temProfessor = false;
            Professor prof;
            fileProf.seekg(0);
            while(fileProf.read((char*)&prof, sizeof(Professor))) {
                if(prof.base.id > 0 && strcmp(prof.disciplina, disciplina.nome) == 0) {
                    temProfessor = true;
                    break;
                }
            }
            fileProf.clear();
            
            // Aplicar filtro de professor
            if(comProfessor == 0 && temProfessor) continue;  // Quer sem professor, mas tem
            if(comProfessor == 1 && !temProfessor) continue; // Quer com professor, mas não tem
            
            dados[contador][0] = to_string(disciplina.id);
            dados[contador][1] = disciplina.nome;
            dados[contador][2] = to_string(disciplina.cargaHoraria);
            dados[contador][3] = disciplina.ativo ? "Ativa" : "Inativa";
            dados[contador][4] = temProfessor ? "Sim" : "Nao";
            contador++;
        }
        file.close();
        fileProf.close();
        return contador;
    }

    void gerenciar_disciplina_menu(int idDisciplina) {
        bool continuar_gerenciando = true;
        
        while(continuar_gerenciando) {
            // Recarregar disciplina a cada iteração
            std::fstream file;
            openFile(file, "disciplinas.dat");
            Disciplina disciplina = buscaDisciplina(file, idDisciplina);
            file.close();
            
            if (disciplina.id == 0) {
                mostrar_caixa_informacao("ERRO", "Disciplina nao encontrada!");
                return;
            }
            
            // Buscar professor vinculado a esta disciplina
            string nomeProfVinculado = "Nenhum";
            {
                std::fstream fileProfs;
                openFile(fileProfs, "professores.dat");
                Professor profTemp;
                fileProfs.seekg(0);
                while(fileProfs.read((char*)&profTemp, sizeof(Professor))) {
                    if(profTemp.base.id > 0 && strcmp(profTemp.disciplina, disciplina.nome) == 0) {
                        nomeProfVinculado = profTemp.base.nome;
                        break;
                    }
                }
                fileProfs.close();
            }

            // Mostrar menu de gerenciamento
            ConfigBotoes configBotoes;
            configBotoes.titulo = "Gerenciar Disciplina";
            configBotoes.descricao = "ID: " + to_string(idDisciplina) + " | Nome: " + string(disciplina.nome) + 
                                     " | Carga Horaria: " + to_string(disciplina.cargaHoraria) + "h" +
                                     " | Status: " + (disciplina.ativo ? "Ativa" : "Inativa") +
                                     " | Professor: " + nomeProfVinculado;
            configBotoes.botoes[0].label = "Ativar";
            configBotoes.botoes[0].tecla = 'A';
            configBotoes.botoes[0].valor_retorno = 1;
            configBotoes.botoes[1].label = "Desativar";
            configBotoes.botoes[1].tecla = 'D';
            configBotoes.botoes[1].valor_retorno = 0;
            configBotoes.botoes[2].label = "Vincular Prof.";
            configBotoes.botoes[2].tecla = 'V';
            configBotoes.botoes[2].valor_retorno = 5;
            configBotoes.botoes[3].label = "Remover Prof.";
            configBotoes.botoes[3].tecla = 'R';
            configBotoes.botoes[3].valor_retorno = 6;
            configBotoes.botoes[4].label = "Excluir";
            configBotoes.botoes[4].tecla = 'E';
            configBotoes.botoes[4].valor_retorno = 3;
            configBotoes.botoes[5].label = "Cancelar";
            configBotoes.botoes[5].tecla = 'C';
            configBotoes.botoes[5].valor_retorno = 4;
            configBotoes.numero_botoes = 6;
            
            saida_botoes acao = interface_para_botoes(configBotoes);
            
            if (!acao.confirmado) return;
            
            if (acao.valor_retorno == 5) {
                // Vincular professor a disciplina
                vincular_professor_disciplina(idDisciplina, string(disciplina.nome));
            }
            else if (acao.valor_retorno == 6) {
                // Remover vinculo professor da disciplina
                remover_vinculo_professor_disciplina(string(disciplina.nome));
            }
            else {
                openFile(file, "disciplinas.dat");
                
                if (acao.valor_retorno == 1) {
                    // Ativar disciplina
                    disciplina.ativo = 1;
                    file.seekp((idDisciplina - 1) * sizeof(Disciplina));
                    file.write((char*)&disciplina, sizeof(Disciplina));
                    file.close();
                    mostrar_caixa_informacao("SUCESSO", "Disciplina ativada com sucesso!");
                }
                else if (acao.valor_retorno == 0) {
                    // Desativar disciplina
                    disciplina.ativo = 0;
                    file.seekp((idDisciplina - 1) * sizeof(Disciplina));
                    file.write((char*)&disciplina, sizeof(Disciplina));
                    file.close();
                    mostrar_caixa_informacao("SUCESSO", "Disciplina desativada com sucesso!");
                }
                else if (acao.valor_retorno == 3) {
                    // Excluir disciplina
                    disciplina.id = 0;
                    file.seekp((idDisciplina - 1) * sizeof(Disciplina));
                    file.write((char*)&disciplina, sizeof(Disciplina));
                    file.close();
                    mostrar_caixa_informacao("SUCESSO", "Disciplina excluida com sucesso!");
                    continuar_gerenciando = false;
                }
                else if (acao.valor_retorno == 4) {
                    // Cancelar/Sair
                    file.close();
                    continuar_gerenciando = false;
                }
            }
        }
    }

    void vincular_professor_disciplina(int idDisciplina, const string &nomeDisciplina) {
        constexpr int Opcoes_vincular = 3;
        string opcoes_vincular[Opcoes_vincular] = {
            "Selecionar de Tabela",
            "Digitar ID",
            "Cancelar"
        };
        
        ConfigMenu configVincular;
        configVincular.titulo = "Vincular Professor";
        configVincular.descricao = "Selecione como deseja vincular um professor a disciplina.";
        saida_menu resultadoVincular = interface_para_menu(Opcoes_vincular, opcoes_vincular, configVincular);
        
        if (resultadoVincular.indice_da_opcao == 0) {
            // Opção 1: Selecionar professor de uma tabela
            vincular_professor_por_tabela(idDisciplina, nomeDisciplina);
        }
        else if (resultadoVincular.indice_da_opcao == 1) {
            // Opção 2: Digitar ID do professor
            vincular_professor_por_id(idDisciplina, nomeDisciplina);
        }
    }

    void vincular_professor_por_tabela(int idDisciplina, const string &nomeDisciplina) {
        // Carregar todos os professores ativos em uma tabela
        string dados[100][4];
        int total = 0;
        std::fstream fileProfs;
        openFile(fileProfs, "professores.dat");
        Professor profAtivo;
        
        fileProfs.seekg(0);
        while(fileProfs.read((char*)&profAtivo, sizeof(Professor)) && total < 100) {
            if(profAtivo.base.id > 0 && profAtivo.base.ativo) {
                dados[total][0] = to_string(profAtivo.base.id);
                dados[total][1] = profAtivo.base.nome;
                dados[total][2] = profAtivo.base.email;
                dados[total][3] = profAtivo.disciplina[0] != '\0' ? string(profAtivo.disciplina) : "Nenhuma";
                total++;
            }
        }
        fileProfs.close();
        
        if (total == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum professor ativo disponivel para vincular.");
            return;
        }
        
        string titulos[4] = {"ID", "Nome", "Email", "Disc. Atual"};
        const string* dados_ptr[100];
        for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Selecionar Professor - " + nomeDisciplina;
        saida_tabela prof_selecionado = interface_para_tabela(total, 4, dados_ptr, titulos, 0, configTab);
        
        if (prof_selecionado.indice_linha != -1) {
            int idProfessor = stoi(dados[prof_selecionado.indice_linha][0]);
            openFile(fileProfs, "professores.dat");
            Professor profVincular = buscaProf(fileProfs, idProfessor);
            fileProfs.close();
            
            if(profVincular.base.id > 0) {
                strncpy(profVincular.disciplina, nomeDisciplina.c_str(), 49);
                profVincular.disciplina[49] = '\0';
                
                openFile(fileProfs, "professores.dat");
                fileProfs.seekp((idProfessor - 20260001) * sizeof(Professor));
                fileProfs.write((char*)&profVincular, sizeof(Professor));
                fileProfs.close();
                
                mostrar_caixa_informacao("SUCESSO", "Professor " + string(profVincular.base.nome) + 
                                      " vinculado com sucesso!");
            }
        }
    }

    void vincular_professor_por_id(int idDisciplina, const string &nomeDisciplina) {
        ConfigEntradaTexto configProfId;
        configProfId.titulo = "Vincular Professor por ID";
        configProfId.label = "Digite o ID do Professor: ";
        configProfId.tipo_entrada = TIPO_NUMERO;
        configProfId.descricao = "Digite o ID do professor ativo que deseja vincular.";
        saida_entrada_texto resultProfId = interface_para_entrada_texto(configProfId);
        
        if (!resultProfId.confirmado) return;
        
        int idProfessor = stoi(resultProfId.valor);
        std::fstream fileProfs;
        openFile(fileProfs, "professores.dat");
        Professor profVincular = buscaProf(fileProfs, idProfessor);
        fileProfs.close();
        
        if(profVincular.base.id == 0 || !profVincular.base.ativo) {
            mostrar_caixa_informacao("ERRO", "Professor nao encontrado ou inativo!");
            return;
        }
        
        strncpy(profVincular.disciplina, nomeDisciplina.c_str(), 49);
        profVincular.disciplina[49] = '\0';
        
        openFile(fileProfs, "professores.dat");
        fileProfs.seekp((idProfessor - 20260001) * sizeof(Professor));
        fileProfs.write((char*)&profVincular, sizeof(Professor));
        fileProfs.close();
        
        mostrar_caixa_informacao("SUCESSO", "Professor " + string(profVincular.base.nome) + 
                              " vinculado com sucesso!");
    }

    void remover_vinculo_professor_disciplina(const string &nomeDisciplina) {
        // Encontrar o professor vinculado a esta disciplina e remover o vínculo
        std::fstream fileProfs;
        openFile(fileProfs, "professores.dat");
        
        Professor prof;
        int index = 0;
        bool encontrado = false;
        
        fileProfs.seekg(0);
        while(fileProfs.read((char*)&prof, sizeof(Professor))) {
            if(prof.base.id > 0 && strcmp(prof.disciplina, nomeDisciplina.c_str()) == 0) {
                // Encontrou o professor vinculado
                prof.disciplina[0] = '\0';  // Limpar o vínculo
                
                fileProfs.seekp(index * sizeof(Professor));
                fileProfs.write((char*)&prof, sizeof(Professor));
                encontrado = true;
                
                mostrar_caixa_informacao("SUCESSO", "Vínculo do professor " + 
                                      string(prof.base.nome) + " removido com sucesso!");
                break;
            }
            index++;
        }
        fileProfs.close();
        
        if(!encontrado) {
            mostrar_caixa_informacao("INFO", "Nenhum professor vinculado a esta disciplina.");
        }
    }





    int listar_instrumentos_especificos(int autorizado, string dados[100][6]){
        int contador = 0;
        std::fstream file;
        openFile(file, "instrumentos.dat");
        Instrumento instrumento;
        
        file.seekg(0);
        while(file.read((char*)&instrumento, sizeof(Instrumento)) && contador < 100) {
            if(instrumento.id != 0 && instrumento.ativo && (instrumento.autorizado == autorizado || autorizado == 2)) {
                dados[contador][0] = to_string(instrumento.id);
                dados[contador][1] = instrumento.nome;
                dados[contador][2] = to_string(instrumento.estoque);
                dados[contador][3] = instrumento.autorizado ? "Autorizado" : "Pendente";
                dados[contador][4] = instrumento.ativo ? "Ativo" : "Inativo";
                dados[contador][5] = instrumento.disponivel ? "Disp" : "Indisp";
                contador++;
            }
        }
        file.close();
        return contador;
    }

    void gerenciar_instrumento_menu(int idInstrumento) {
        std::fstream file;
        openFile(file, "instrumentos.dat");
        Instrumento instrumento;
        file.seekg((idInstrumento - 1) * sizeof(Instrumento));
        file.read((char*)&instrumento, sizeof(Instrumento));
        file.close();
        
        if (instrumento.id == 0) {
            mostrar_caixa_informacao("ERRO", "Instrumento nao encontrado!");
            return;
        }
        
        ConfigBotoes configBotoes;
        configBotoes.titulo = "Gerenciar Instrumento";
        configBotoes.descricao = "ID: " + to_string(idInstrumento) + " | Nome: " + string(instrumento.nome) + 
                                 " | Estoque: " + to_string(instrumento.estoque) + 
                                 " | Status: " + (instrumento.autorizado ? "Autorizado" : "Pendente");
        configBotoes.botoes[0].label = "Autorizar";
        configBotoes.botoes[0].tecla = 'A';
        configBotoes.botoes[0].valor_retorno = 1;
        configBotoes.botoes[1].label = "Desautorizar";
        configBotoes.botoes[1].tecla = 'D';
        configBotoes.botoes[1].valor_retorno = 0;
        configBotoes.botoes[2].label = "Inativar";
        configBotoes.botoes[2].tecla = 'I';
        configBotoes.botoes[2].valor_retorno = 2;
        configBotoes.botoes[3].label = "Cancelar";
        configBotoes.botoes[3].tecla = 'C';
        configBotoes.botoes[3].valor_retorno = 3;
        configBotoes.numero_botoes = 4;
        saida_botoes acao = interface_para_botoes(configBotoes);
        
        if (!acao.confirmado) return;
        
        openFile(file, "instrumentos.dat");
        
        if (acao.valor_retorno == 1) {
            instrumento.autorizado = 1;
            file.seekp((idInstrumento - 1) * sizeof(Instrumento));
            file.write((char*)&instrumento, sizeof(Instrumento));
            mostrar_caixa_informacao("SUCESSO", "Instrumento autorizado com sucesso!");
        }
        else if (acao.valor_retorno == 0) {
            instrumento.autorizado = 0;
            file.seekp((idInstrumento - 1) * sizeof(Instrumento));
            file.write((char*)&instrumento, sizeof(Instrumento));
            mostrar_caixa_informacao("SUCESSO", "Instrumento desautorizado com sucesso!");
        }
        else if (acao.valor_retorno == 2) {
            instrumento.ativo = 0;
            file.seekp((idInstrumento - 1) * sizeof(Instrumento));
            file.write((char*)&instrumento, sizeof(Instrumento));
            mostrar_caixa_informacao("SUCESSO", "Instrumento inativado com sucesso!");
        }
        
        file.close();
    }

    // ----- MENUS PRINCIPAIS -----

    void menuCadastroUsuarios() {
        constexpr int Quantidades_opcoes = 3;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Cadastrar Aluno",
                "Cadastrar Professor",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Cadastro de Usuarios";   
            config.descricao = "Selecione uma opcao para cadastrar novos usuarios.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0: { // Cadastrar Aluno
                    int novoId = Login_mat::realizarCadastroAluno();
                    if (novoId > 0) {
                        mostrar_caixa_informacao("SUCESSO", "Aluno cadastrado com sucesso!\nID: " + to_string(novoId));
                    }
                    break;
                }
                
                case 1: { // Cadastrar Professor
                    int novoId = Login_mat::realizarCadastroProfessor();
                    if (novoId > 0) {
                        mostrar_caixa_informacao("SUCESSO", "Professor cadastrado com sucesso!\nID: " + to_string(novoId));
                    }
                    break;
                }
                
                case 2: { // Voltar
                    continuar = false;
                    break;
                }
            }
        }
    }

    void menuGerenciarUsuarios(){

        constexpr int Quantidades_opcoes = 5;
        string texto_do_tipo_de_usuario;
        string tipo_usuario_str = "Ambos"; // Controla se é Aluno, Professor ou Ambos
        Funcao tipo_usuario = ALUNO;
        int estado_selecionado = 2; // 1=Ativo, 0=Inativo, 2=Ambos (padrão)
        bool continuar = true;

        while (continuar) {
            switch(tipo_usuario) {
                case ALUNO:
                    texto_do_tipo_de_usuario = "Aluno";
                    break;
                case PROFESSOR:
                    texto_do_tipo_de_usuario = "Professor";
                    break;
                default:
                    texto_do_tipo_de_usuario = "Aluno/Professor";
                    break;
            }

            string opcoes[Quantidades_opcoes] = {
                "Cadastrar Usuario",
                "Filtros de pesquisa",
                "Listar Usuarios",
                "Buscar Usuario por ID",
                "Voltar"
            };
            ConfigMenu config;
            config.titulo = "Gerenciar Usuarios < " + texto_do_tipo_de_usuario + " >" + " < " + (estado_selecionado == 0 ? "Inativos" : (estado_selecionado == 1 ? "Ativos" : "Todos")) + " >";   
            config.descricao = "Selecione uma opcao para gerenciar os usuarios do sistema.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao)
            {
                case 0: { // Cadastrar Usuario
                    constexpr int Quantidades_opcoes_cad = 3;
                    string opcoes_cad[Quantidades_opcoes_cad] = {
                        "Cadastrar Aluno",
                        "Cadastrar Professor",
                        "Voltar"
                    };
                    
                    ConfigMenu config_cad;
                    config_cad.titulo = "Cadastro de Usuarios";   
                    config_cad.descricao = "Selecione uma opcao para cadastrar novo usuario.";
                    saida_menu resultado_cad = interface_para_menu(Quantidades_opcoes_cad, opcoes_cad, config_cad);
                    
                    switch (resultado_cad.indice_da_opcao) {
                        case 0: { // Cadastrar Aluno
                            int novoId = Login_mat::realizarCadastroAluno();
                            if (novoId > 0) {
                                mostrar_caixa_informacao("SUCESSO", "Aluno cadastrado com sucesso!\nID: " + to_string(novoId));
                            }
                            break;
                        }
                        case 1: { // Cadastrar Professor
                            int novoId = Login_mat::realizarCadastroProfessor();
                            if (novoId > 0) {
                                mostrar_caixa_informacao("SUCESSO", "Professor cadastrado com sucesso!\nID: " + to_string(novoId));
                            }
                            break;
                        }
                        case 2: { // Voltar
                            break;
                        }
                    }
                    break;
                }
                case 1: {
                    constexpr int qtdVariaveis = 2;
                    string nomes_variaveis[qtdVariaveis] = {"Tipo de Usuario", "Estado do Usuario"};
                    
                    constexpr int qtdTipos = 3;
                    string tipos[qtdTipos] = {"Aluno", "Professor", "Ambos"};
                    
                    constexpr int qtdEstados = 3;
                    string estados[qtdEstados] = {"Inativo", "Ativo", "Ambos"};
                    
                    const string* opcoes_variaveis[] = {(const string*)tipos, (const string*)estados};
                    int num_opcoes[] = {qtdTipos, qtdEstados};
                    
                    ConfigConfigurador configFiltro;
                    configFiltro.titulo = "Filtros de Pesquisa";
                    configFiltro.descricao = "Defina os filtros para gerenciar usuarios.";
                    
                    saida_configurador resultadofiltro = interface_para_configurador(qtdVariaveis, nomes_variaveis, opcoes_variaveis, num_opcoes, configFiltro);
                    
                    if (resultadofiltro.confirmado && resultadofiltro.numero_variaveis >= 2) {
                        string tipo_str = resultadofiltro.valores_selecionados[0];
                        tipo_usuario_str = tipo_str; // Armazenar o tipo selecionado
                        
                        if (tipo_str == "Aluno") tipo_usuario = ALUNO;
                        else if (tipo_str == "Professor") tipo_usuario = PROFESSOR;
                        else if (tipo_str == "Ambos") tipo_usuario = ALUNO; // Padrão "Ambos" inicializa com ALUNO
                        
                        // Converter estado do usuario
                        string estado_str = resultadofiltro.valores_selecionados[1];
                        if (estado_str == "Inativo") {
                            estado_selecionado = 0;
                        }
                        else if (estado_str == "Ativo") {
                            estado_selecionado = 1;
                        }
                        else if (estado_str == "Ambos") {
                            estado_selecionado = 2;
                        }
                    }
                    break;
                }
                case 2: {
                    string dados[100][6];
                    int total = 0;
                    
                    // Se tipo_usuario_str é "Ambos", listar tanto alunos quanto professores
                    if (tipo_usuario_str == "Ambos") {
                        string dados_alunos[100][6];
                        string dados_profs[100][6];
                        int total_alunos = listar_usuarios_especificos(ALUNO, estado_selecionado, dados_alunos);
                        int total_profs = listar_usuarios_especificos(PROFESSOR, estado_selecionado, dados_profs);
                        
                        // Concatenar dados
                        for(int i = 0; i < total_alunos && total < 100; i++) {
                            for(int j = 0; j < 6; j++) {
                                dados[total][j] = dados_alunos[i][j];
                            }
                            total++;
                        }
                        for(int i = 0; i < total_profs && total < 100; i++) {
                            for(int j = 0; j < 6; j++) {
                                dados[total][j] = dados_profs[i][j];
                            }
                            total++;
                        }
                    } else {
                        total = listar_usuarios_especificos(tipo_usuario, estado_selecionado, dados);
                    }
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum usuario encontrado com os filtros selecionados.");
                        break;
                    }
                    
                    string titulos[5] = {"ID", "Nome", "Email", "Categoria", "Status"};
                    const string* dados_ptr[100]; 
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    ConfigTabela configTab;
                    configTab.titulo = "Todos os Usuarios (" + texto_do_tipo_de_usuario + ")";
                    saida_tabela usuario_selecionado = interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
                    if (usuario_selecionado.indice_linha != -1) {
                        int id = stoi(dados[usuario_selecionado.indice_linha][0]);
                        gerenciar_usuario_menu(id);
                    }
                    break;
                }
                case 3: {
                    // Capturar ID usando interface gráfica
                    ConfigEntradaTexto configBusca;
                    configBusca.titulo = "Buscar Usuario";
                    configBusca.label = "Digite o ID do usuario: ";
                    configBusca.tipo_entrada = TIPO_NUMERO;
                    configBusca.descricao = "Digite o número de identificação do usuario que deseja buscar.";
                    
                    saida_entrada_texto resultBusca = interface_para_entrada_texto(configBusca);
                    
                    if (!resultBusca.confirmado) break;
                    
                    int idBusca = stoi(resultBusca.valor);
                    
                    if (idBusca <= 0) {
                        mostrar_caixa_informacao("ERRO", "ID invalido! Digite um numero positivo.");
                        break;
                    }
                    
                    // Usar função auxiliar para gerenciar usuário
                    gerenciar_usuario_menu(idBusca);
                    break;
                }
                case 4:
                    continuar = false;
                    break;
            }
        }
    }

// =====================================================================
// SEÇÃO: FUNÇÕES DE MENU (FORA DO NAMESPACE MOD_ADM)
// Menus principais de gerenciamento
// =====================================================================

void menuCadastroCursos(){
    constexpr int Quantidades_opcoes = 5;
    bool continuar = true;
    int estado_selecionado = 2; // 0=inativo, 1=ativo, 2=ambos

    while (continuar) {
        string opcoes[Quantidades_opcoes] = {
            "Cadastrar Disciplina",
            "Filtros de pesquisa",
            "Listar Disciplinas",
            "Buscar por ID",
            "Voltar"
        };
        
        string titulo_estado = (estado_selecionado == 0 ? "Inativas" : (estado_selecionado == 1 ? "Ativas" : "Todas"));
        
        ConfigMenu config;
        config.titulo = "Gerenciar Disciplinas (" + titulo_estado + ")";
        config.descricao = "Selecione uma opcao para gerenciar as disciplinas do sistema.";
        saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
        
        std::fstream file;
        openFile(file, "disciplinas.dat");
        Disciplina disciplina_;
        
        switch (resultado.indice_da_opcao) {
            case 0: { // Cadastrar Disciplina
                ConfigEntradaTexto configNome;
                configNome.titulo = "Cadastrar Disciplina";
                configNome.label = "Nome da Disciplina: ";
                saida_entrada_texto resultNome = interface_para_entrada_texto(configNome);
                if (!resultNome.confirmado) break;
                
                ConfigEntradaTexto configCarga;
                configCarga.titulo = "Cadastrar Disciplina";
                configCarga.label = "Carga Horaria (em horas): ";
                configCarga.tipo_entrada = TIPO_NUMERO;
                saida_entrada_texto resultCarga = interface_para_entrada_texto(configCarga);
                if (!resultCarga.confirmado) break;
                
                char nome[30];
                strncpy(nome, resultNome.valor.c_str(), 29);
                nome[29] = '\0';
                int cargaHoraria = stoi(resultCarga.valor);
                
                strncpy(disciplina_.nome, nome, 29);
                disciplina_.nome[29] = '\0';
                disciplina_.cargaHoraria = cargaHoraria;
                disciplina_.ativo = 1; // Ativa por padrão
                
                file.seekp(0, std::ios::end);
                disciplina_.id = 1 + file.tellp() / sizeof(Disciplina);
                file.write((char*)&disciplina_, sizeof(Disciplina));
                file.clear();
                
                mostrar_caixa_informacao("SUCESSO", "Disciplina cadastrada com sucesso!\nID: " + to_string(disciplina_.id));
                break;
            }
            
            case 1: { // Filtros de pesquisa
                constexpr int qtdEstados = 3;
                string estados[qtdEstados] = {"Inativa", "Ativa", "Ambas"};
                
                ConfigMenu configFiltro;
                configFiltro.titulo = "Filtros de Pesquisa";
                configFiltro.descricao = "Selecione o estado das disciplinas que deseja listar.";
                saida_menu resultadofiltro = interface_para_menu(qtdEstados, estados, configFiltro);
                
                if (resultadofiltro.indice_da_opcao == 0) estado_selecionado = 0;
                else if (resultadofiltro.indice_da_opcao == 1) estado_selecionado = 1;
                else if (resultadofiltro.indice_da_opcao == 2) estado_selecionado = 2;
                break;
            }
            
            case 2: { // Listar Disciplinas
                string dados[100][6];
                int total = listar_disciplinas_especificas(estado_selecionado, dados);
                
                if (total == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma disciplina encontrada com os filtros selecionados.");
                    break;
                }
                
                string titulos[4] = {"ID", "Nome", "Carga Hor.", "Status"};
                const string* dados_ptr[100];
                for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Lista de Disciplinas";
                saida_tabela disciplina_selecionada = interface_para_tabela(total, 4, dados_ptr, titulos, 0, configTab);
                
                if (disciplina_selecionada.indice_linha != -1) {
                    int id = stoi(dados[disciplina_selecionada.indice_linha][0]);
                    gerenciar_disciplina_menu(id);
                }
                break;
            }
            
            case 3: { // Buscar por ID
                ConfigEntradaTexto configBusca;
                configBusca.titulo = "Buscar Disciplina";
                configBusca.label = "Digite o ID da disciplina: ";
                configBusca.tipo_entrada = TIPO_NUMERO;
                configBusca.descricao = "Digite o número de identificação da disciplina que deseja buscar.";
                
                saida_entrada_texto resultBusca = interface_para_entrada_texto(configBusca);
                
                if (!resultBusca.confirmado) break;
                
                int idBusca = stoi(resultBusca.valor);
                
                if (idBusca <= 0) {
                    mostrar_caixa_informacao("ERRO", "ID invalido! Digite um numero positivo.");
                    break;
                }
                
                gerenciar_disciplina_menu(idBusca);
                break;
            }
            
            case 4: { // Voltar
                continuar = false;
                break;
            }
        }
        
        file.close();
    }
}

    void consultarRelatoriosAcademicos() {
        std::fstream fileAlunos;
        openFile(fileAlunos, "alunos.dat");
        
        Aluno aluno;
        string dados[100][5];
        int total = 0;
        
        fileAlunos.seekg(0);
        while(fileAlunos.read((char*)&aluno, sizeof(Aluno)) && total < 100) {
            if(aluno.base.ativo && aluno.base.categoria == ALUNO && aluno.base.id != 20260000) {
                dados[total][0] = to_string(aluno.base.id);
                dados[total][1] = aluno.base.nome;
                dados[total][2] = aluno.base.email;
                dados[total][3] = to_string(aluno.faltas);
                dados[total][4] = aluno.base.ativo ? "Ativo" : "Inativo";
                total++;
            }
        }
        
        fileAlunos.close();
        
        if(total == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum aluno encontrado no sistema.");
            return;
        }
        
        string titulos[5] = {"ID", "Nome", "Email", "Faltas", "Status"};
        const string* dados_ptr[100];
        for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Relatorio Academico - Total de Alunos: " + to_string(total);
        interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
    }



    // ----- FUNÇÕES AUXILIARES DE TURMAS E MATRÍCULA -----

    int listar_disciplinas_para_turma(string dados[100][4]) {
        int contador = 0;
        std::fstream file;
        openFile(file, "disciplinas.dat");
        Disciplina disciplina;
        
        file.seekg(0);
        while(file.read((char*)&disciplina, sizeof(Disciplina)) && contador < 100) {
            if(disciplina.id != 0 && disciplina.ativo) {
                dados[contador][0] = to_string(disciplina.id);
                dados[contador][1] = disciplina.nome;
                dados[contador][2] = to_string(disciplina.cargaHoraria);
                dados[contador][3] = "Ativa";
                contador++;
            }
        }
        file.close();
        return contador;
    }

    int listar_professores_para_turma(string dados[100][4]) {
        int contador = 0;
        std::fstream file;
        openFile(file, "professores.dat");
        Professor professor;
        
        file.seekg(0);
        while(file.read((char*)&professor, sizeof(Professor)) && contador < 100) {
            if(professor.base.id > 0 && professor.base.ativo) {
                int vagasDisponiveis = 0;
                for(int i = 0; i < 5; i++) {
                    if(professor.turmas[i] == 0) vagasDisponiveis++;
                }
                
                if(vagasDisponiveis > 0) {
                    dados[contador][0] = to_string(professor.base.id);
                    dados[contador][1] = professor.base.nome;
                    dados[contador][2] = professor.base.email;
                    dados[contador][3] = to_string(vagasDisponiveis) + " vaga(s)";
                    contador++;
                }
            }
        }
        file.close();
        return contador;
    }

    int listar_turmas_para_matricula(string dados[100][5]) {
        int contador = 0;
        std::fstream file;
        openFile(file, "turmas.dat");
        Disciplina disc;
        std::fstream fileDisc;
        openFile(fileDisc, "disciplinas.dat");
        
        Turma turma;
        file.seekg(0);
        while(file.read((char*)&turma, sizeof(Turma)) && contador < 100) {
            if(turma.id != 0 && turma.ativo && turma.qtdAlunos < MAX_ALUNOS) {
                fileDisc.seekg((turma.idDisciplina - 1) * sizeof(Disciplina));
                fileDisc.read((char*)&disc, sizeof(Disciplina));
                
                dados[contador][0] = to_string(turma.id);
                dados[contador][1] = disc.nome;
                dados[contador][2] = to_string(turma.idProfessor);
                dados[contador][3] = to_string(turma.qtdAlunos) + "/" + to_string(MAX_ALUNOS);
                dados[contador][4] = "Ativa";
                contador++;
            }
        }
        file.close();
        fileDisc.close();
        return contador;
    }

    int listar_alunos_para_matricula(string dados[100][5]) {
        int contador = 0;
        std::fstream file;
        openFile(file, "alunos.dat");
        Aluno aluno;
        
        file.seekg(0);
        while(file.read((char*)&aluno, sizeof(Aluno)) && contador < 100) {
            if(aluno.base.id > 0 && aluno.base.ativo && aluno.base.categoria == ALUNO) {
                dados[contador][0] = to_string(aluno.base.id);
                dados[contador][1] = aluno.base.nome;
                dados[contador][2] = aluno.base.email;
                dados[contador][3] = "Ativo";
                dados[contador][4] = "";
                contador++;
            }
        }
        file.close();
        return contador;
    }

    // ----- FUNÇÕES DE TURMAS E MATRÍCULA -----

    bool verificaTurmasProf(Professor &prof, int &Index_turma){
        for(int i = 0; i < 5; i++){
            if(prof.turmas[i] == 0){
                Index_turma = i;
                return true;
            }
        }
        return false;
    }

    // Lista todas as turmas para exibição na tabela principal
    int listar_todas_turmas(string dados[100][5]) {
        int contador = 0;
        std::fstream file;
        std::fstream fileProf;
        std::fstream fileDisc;
        openFile(file, "turmas.dat");
        openFile(fileProf, "professores.dat");
        openFile(fileDisc, "disciplinas.dat");
        
        Turma turma;
        file.seekg(0);
        while(file.read((char*)&turma, sizeof(Turma)) && contador < 100) {
            if(turma.id != 0 && turma.ativo) {
                dados[contador][0] = to_string(turma.id);
                dados[contador][1] = turma.nome;
                
                // Buscar nome da disciplina
                if(turma.idDisciplina > 0) {
                    Disciplina disc;
                    memset(&disc, 0, sizeof(Disciplina));
                    fileDisc.clear();
                    fileDisc.seekg((turma.idDisciplina - 1) * sizeof(Disciplina));
                    if(fileDisc.read((char*)&disc, sizeof(Disciplina)) && disc.id == turma.idDisciplina) {
                        dados[contador][2] = disc.nome;
                    } else {
                        dados[contador][2] = "Nao atribuida";
                    }
                } else {
                    dados[contador][2] = "Nao atribuida";
                }
                
                // Buscar nome do professor
                if(turma.idProfessor > 0) {
                    Professor prof;
                    memset(&prof, 0, sizeof(Professor));
                    fileProf.clear(); // Limpar flags de erro antes de buscar
                    fileProf.seekg((turma.idProfessor - 20260001) * sizeof(Professor));
                    if(fileProf.read((char*)&prof, sizeof(Professor)) && prof.base.id == turma.idProfessor) {
                        dados[contador][3] = prof.base.nome;
                    } else {
                        dados[contador][3] = "Nao atribuido";
                    }
                } else {
                    dados[contador][3] = "Nao atribuido";
                }
                
                dados[contador][4] = to_string(turma.qtdAlunos) + "/" + to_string(MAX_ALUNOS);
                contador++;
            }
        }
        file.close();
        fileProf.close();
        fileDisc.close();
        return contador;
    }

    // Cria turma apenas com nome (definições básicas)
    void criarTurmaBasica() {
        std::fstream file;
        openFile(file, "turmas.dat");
        
        // ===== ENTRADA DO NOME DA TURMA =====
        ConfigEntradaTexto configNome;
        configNome.titulo = "Criar Nova Turma";
        configNome.descricao = "Informe o nome da turma.";
        configNome.label = "Nome da Turma: ";
        configNome.tipo_entrada = TIPO_TEXTO;
        configNome.tamanho_maximo = 99;
        
        saida_entrada_texto resultadoNome = interface_para_entrada_texto(configNome);
        
        if (!resultadoNome.confirmado || resultadoNome.valor.empty()) {
            file.close();
            return;
        }
        
        // ===== CRIAR TURMA =====
        Turma turma;
        turma.id = gerarNovoId(file, sizeof(Turma));
        turma.ativo = 1;
        turma.idDisciplina = 0;  // Não atribuído
        turma.idProfessor = 0;   // Não atribuído
        turma.id_prof = 0;
        turma.qtdAlunos = 0;
        turma.qtdAvaliacoes = 0;
        
        // Copiar nome
        strncpy(turma.nome, resultadoNome.valor.c_str(), 99);
        turma.nome[99] = '\0';
        
        // Inicializar alunos vazios
        for(int i = 0; i < MAX_ALUNOS; i++) {
            turma.alunos[i].base.id = 0;
            turma.alunos[i].base.ativo = 0;
        }
        
        // Inicializar avaliações vazias
        for(int i = 0; i < MAX_AVALIACOES; i++) {
            turma.avaliacoes[i].data[0] = '\0';
            turma.avaliacoes[i].descricao[0] = '\0';
        }
        
        // ===== SALVAR TURMA =====
        file.seekp((turma.id - 1) * sizeof(Turma));
        file.write((char*)&turma, sizeof(Turma));
        file.close();
        
        mostrar_caixa_informacao("SUCESSO", "Turma criada com sucesso!\nID: " + to_string(turma.id) + "\nNome: " + turma.nome);
    }

    // Atribui disciplina a uma turma
    void atribuirDisciplinaTurma(int idTurma) {
        string dados_disc[100][4];
        
        std::fstream file;
        std::fstream fileDisc;
        openFile(file, "turmas.dat");
        openFile(fileDisc, "disciplinas.dat");
        
        // Buscar turma
        Turma turma;
        file.seekg((idTurma - 1) * sizeof(Turma));
        file.read((char*)&turma, sizeof(Turma));
        
        if(turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma nao encontrada.");
            file.close();
            fileDisc.close();
            return;
        }
        
        // Listar disciplinas
        int total_disc = listar_disciplinas_para_turma(dados_disc);
        
        if (total_disc == 0) {
            mostrar_caixa_informacao("INFO", "Nenhuma disciplina ativa disponivel.");
            file.close();
            fileDisc.close();
            return;
        }
        
        string titulos_disc[4] = {"ID", "Nome", "Carga Hor.", "Status"};
        const string* dados_ptr_disc[100];
        for(int i = 0; i < total_disc; i++) dados_ptr_disc[i] = dados_disc[i];
        
        ConfigTabela configTab_disc;
        configTab_disc.titulo = "Atribuir Disciplina - " + string(turma.nome);
        configTab_disc.descricao = turma.idDisciplina > 0 ? "Disciplina atual: ID " + to_string(turma.idDisciplina) : "Sem disciplina atribuida";
        saida_tabela disc_selecionada = interface_para_tabela(total_disc, 4, dados_ptr_disc, titulos_disc, 0, configTab_disc);
        
        if (disc_selecionada.indice_linha == -1) {
            file.close();
            fileDisc.close();
            return;
        }
        
        int idDisciplina = stoi(dados_disc[disc_selecionada.indice_linha][0]);
        
        // Atualizar turma
        turma.idDisciplina = idDisciplina;
        file.seekp((idTurma - 1) * sizeof(Turma));
        file.write((char*)&turma, sizeof(Turma));
        
        file.close();
        fileDisc.close();
        
        mostrar_caixa_informacao("SUCESSO", "Disciplina atribuida com sucesso!");
    }

    void atribuirProfessorTurma(int idTurma) {
        string dados_prof[100][4];
        
        std::fstream file;
        std::fstream fileProf;
        openFile(file, "turmas.dat");
        openFile(fileProf, "professores.dat");
        
        // Buscar turma
        Turma turma;
        file.seekg((idTurma - 1) * sizeof(Turma));
        file.read((char*)&turma, sizeof(Turma));
        
        if(turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma nao encontrada.");
            file.close();
            fileProf.close();
            return;
        }
        
        // Se já tem professor, remover turma do professor antigo
        if(turma.idProfessor > 0) {
            Professor profAntigo;
            fileProf.seekg((turma.idProfessor - 20260001) * sizeof(Professor));
            fileProf.read((char*)&profAntigo, sizeof(Professor));
            
            if(profAntigo.base.id == turma.idProfessor) {
                for(int i = 0; i < 5; i++) {
                    if(profAntigo.turmas[i] == idTurma) {
                        profAntigo.turmas[i] = 0;
                        break;
                    }
                }
                fileProf.seekp((turma.idProfessor - 20260001) * sizeof(Professor));
                fileProf.write((char*)&profAntigo, sizeof(Professor));
            }
        }
        
        // Listar professores
        int total_prof = listar_professores_para_turma(dados_prof);
        
        if (total_prof == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum professor ativo com vagas disponiveis.");
            file.close();
            fileProf.close();
            return;
        }
        
        string titulos_prof[4] = {"ID", "Nome", "Email", "Vagas"};
        const string* dados_ptr_prof[100];
        for(int i = 0; i < total_prof; i++) dados_ptr_prof[i] = dados_prof[i];
        
        ConfigTabela configTab_prof;
        configTab_prof.titulo = "Atribuir Professor - " + string(turma.nome);
        configTab_prof.descricao = turma.idProfessor > 0 ? "Professor atual: ID " + to_string(turma.idProfessor) : "Sem professor atribuido";
        
        saida_tabela prof_selecionado = interface_para_tabela(total_prof, 4, dados_ptr_prof, titulos_prof, 0, configTab_prof);
        
        if (prof_selecionado.indice_linha == -1) {
            file.close();
            fileProf.close();
            return;
        }
        
        int idProfessor = stoi(dados_prof[prof_selecionado.indice_linha][0]);
        
        // Vincular turma ao novo professor
        Professor prof;
        fileProf.seekg((idProfessor - 20260001) * sizeof(Professor));
        fileProf.read((char*)&prof, sizeof(Professor));
        
        int indexVAGA;
        if(verificaTurmasProf(prof, indexVAGA)){
            prof.turmas[indexVAGA] = idTurma;
            fileProf.seekp((idProfessor - 20260001) * sizeof(Professor));
            fileProf.write((char*)&prof, sizeof(Professor));
        } else {
            mostrar_caixa_informacao("ERRO", "Professor nao tem vaga para novas turmas.");
            file.close();
            fileProf.close();
            return;
        }
        
        // Atualizar turma
        turma.idProfessor = idProfessor;
        turma.id_prof = idProfessor;
        file.seekp((idTurma - 1) * sizeof(Turma));
        file.write((char*)&turma, sizeof(Turma));
        
        file.close();
        fileProf.close();
        
        mostrar_caixa_informacao("SUCESSO", "Professor atribuido com sucesso!");
    }

    // Lista alunos de uma turma
    void listarAlunosDaTurma(int idTurma) {
        std::fstream file;
        openFile(file, "turmas.dat");
        
        Turma turma;
        file.seekg((idTurma - 1) * sizeof(Turma));
        file.read((char*)&turma, sizeof(Turma));
        file.close();
        
        if(turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma nao encontrada.");
            return;
        }
        
        if(turma.qtdAlunos == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum aluno matriculado nesta turma.");
            return;
        }
        
        // Montar tabela de alunos
        string dados[MAX_ALUNOS][4];
        int contador = 0;
        
        for(int i = 0; i < MAX_ALUNOS && contador < turma.qtdAlunos; i++) {
            if(turma.alunos[i].base.id > 0) {
                dados[contador][0] = to_string(turma.alunos[i].base.id);
                dados[contador][1] = turma.alunos[i].base.nome;
                dados[contador][2] = turma.alunos[i].base.email;
                dados[contador][3] = turma.alunos[i].base.ativo ? "Ativo" : "Inativo";
                contador++;
            }
        }
        
        string titulos[4] = {"ID", "Nome", "Email", "Status"};
        const string* dados_ptr[MAX_ALUNOS];
        for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Alunos da Turma - " + string(turma.nome);
        configTab.descricao = "Total: " + to_string(turma.qtdAlunos) + " aluno(s)";
        
        interface_para_tabela(contador, 4, dados_ptr, titulos, 0, configTab);
    }

    // Matricula alunos em uma turma específica
    void matricularAlunosNaTurma(int idTurma) {
        string dados_alunos[100][5];
        
        std::fstream fileAluno;
        std::fstream fileTurma;
        openFile(fileAluno, "alunos.dat");
        openFile(fileTurma, "turmas.dat");
        
        // Buscar turma
        Turma turma;
        fileTurma.seekg((idTurma - 1) * sizeof(Turma));
        fileTurma.read((char*)&turma, sizeof(Turma));
        
        if(turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma nao encontrada.");
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        if(turma.qtdAlunos >= MAX_ALUNOS) {
            mostrar_caixa_informacao("INFO", "Turma cheia. Nao ha vagas disponiveis.");
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        // Listar alunos disponíveis
        int total_alunos = listar_alunos_para_matricula(dados_alunos);
        
        if (total_alunos == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum aluno ativo disponivel.");
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        string titulos_alunos[4] = {"ID", "Nome", "Email", "Status"};
        const string* dados_ptr_alunos[100];
        for(int i = 0; i < total_alunos; i++) dados_ptr_alunos[i] = dados_alunos[i];
        
        ConfigTabela configTab_alunos;
        configTab_alunos.titulo = "Matricular Aluno - " + string(turma.nome);
        configTab_alunos.descricao = "Vagas: " + to_string(MAX_ALUNOS - turma.qtdAlunos) + " disponiveis";
        
        saida_tabela aluno_selecionado = interface_para_tabela(total_alunos, 4, dados_ptr_alunos, titulos_alunos, 0, configTab_alunos);
        
        if (aluno_selecionado.indice_linha == -1) {
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        int idAluno = stoi(dados_alunos[aluno_selecionado.indice_linha][0]);
        
        // Buscar aluno
        Aluno aluno = buscaAluno(fileAluno, idAluno);
        
        if (aluno.base.id != idAluno) {
            mostrar_caixa_informacao("ERRO", "Aluno nao encontrado.");
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        // Verificar se já está matriculado
        for (int i = 0; i < MAX_ALUNOS; i++) {
            if (turma.alunos[i].base.id == aluno.base.id) {
                mostrar_caixa_informacao("ERRO", "Aluno ja matriculado nesta turma.");
                fileAluno.close();
                fileTurma.close();
                return;
            }
        }
        
        // Encontrar vaga
        int index = -1;
        for (int i = 0; i < MAX_ALUNOS; i++) {
            if (turma.alunos[i].base.id == 0) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            mostrar_caixa_informacao("ERRO", "Nao foi possivel encontrar vaga.");
            fileAluno.close();
            fileTurma.close();
            return;
        }
        
        // Matricular aluno
        turma.alunos[index] = aluno;
        turma.qtdAlunos++;
        
        fileTurma.seekp((turma.id - 1) * sizeof(Turma));
        fileTurma.write((char*)&turma, sizeof(Turma));
        
        fileAluno.close();
        fileTurma.close();
        mostrar_caixa_informacao("SUCESSO", "Aluno matriculado com sucesso!");
    }

    // Remove aluno de uma turma
    void removerAlunoDaTurma(int idTurma) {
        std::fstream fileTurma;
        openFile(fileTurma, "turmas.dat");
        
        // Buscar turma
        Turma turma;
        fileTurma.seekg((idTurma - 1) * sizeof(Turma));
        fileTurma.read((char*)&turma, sizeof(Turma));
        
        if(turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma nao encontrada.");
            fileTurma.close();
            return;
        }
        
        if(turma.qtdAlunos == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum aluno matriculado nesta turma.");
            fileTurma.close();
            return;
        }
        
        // Montar tabela de alunos da turma
        string dados[MAX_ALUNOS][4];
        int indices[MAX_ALUNOS]; // Guardar índice real no array
        int contador = 0;
        
        for(int i = 0; i < MAX_ALUNOS && contador < turma.qtdAlunos; i++) {
            if(turma.alunos[i].base.id > 0) {
                dados[contador][0] = to_string(turma.alunos[i].base.id);
                dados[contador][1] = turma.alunos[i].base.nome;
                dados[contador][2] = turma.alunos[i].base.email;
                dados[contador][3] = "Matriculado";
                indices[contador] = i;
                contador++;
            }
        }
        
        string titulos[4] = {"ID", "Nome", "Email", "Status"};
        const string* dados_ptr[MAX_ALUNOS];
        for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Remover Aluno - " + string(turma.nome);
        configTab.descricao = "Selecione o aluno para remover da turma";
        
        saida_tabela resultado = interface_para_tabela(contador, 4, dados_ptr, titulos, 0, configTab);
        
        if(resultado.indice_linha == -1) {
            fileTurma.close();
            return;
        }
        
        // Remover aluno
        int indexRemover = indices[resultado.indice_linha];
        turma.alunos[indexRemover].base.id = 0;
        turma.alunos[indexRemover].base.ativo = 0;
        turma.qtdAlunos--;
        
        fileTurma.seekp((turma.id - 1) * sizeof(Turma));
        fileTurma.write((char*)&turma, sizeof(Turma));
        fileTurma.close();
        
        mostrar_caixa_informacao("SUCESSO", "Aluno removido da turma com sucesso!");
    }

    // Submenu de gerenciamento de alunos da turma
    void menuGerenciarAlunosTurma(int idTurma) {
        constexpr int Quantidades_opcoes = 4;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Matricular Aluno",
                "Remover Aluno",
                "Listar Alunos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gerenciar Alunos da Turma";
            config.descricao = "Selecione uma opcao.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    matricularAlunosNaTurma(idTurma);
                    break;
                case 1:
                    removerAlunoDaTurma(idTurma);
                    break;
                case 2:
                    listarAlunosDaTurma(idTurma);
                    break;
                case 3:
                    continuar = false;
                    break;
            }
        }
    }

    // Submenu de atributos de uma turma selecionada
    void menuAtributosTurmaSelecionada(int idTurma) {
        constexpr int Quantidades_opcoes = 5;
        bool continuar = true;

        // Buscar nome da turma para exibir
        std::fstream file;
        openFile(file, "turmas.dat");
        Turma turma;
        file.seekg((idTurma - 1) * sizeof(Turma));
        file.read((char*)&turma, sizeof(Turma));
        file.close();
        
        string nomeTurma = turma.nome;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Atribuir Disciplina",
                "Atribuir Professor",
                "Gerenciar Alunos",
                "Listar Alunos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Atributos da Turma: " + nomeTurma;
            config.descricao = "ID: " + to_string(idTurma);
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    atribuirDisciplinaTurma(idTurma);
                    break;
                case 1:
                    atribuirProfessorTurma(idTurma);
                    break;
                case 2:
                    menuGerenciarAlunosTurma(idTurma);
                    break;
                case 3:
                    listarAlunosDaTurma(idTurma);
                    break;
                case 4:
                    continuar = false;
                    break;
            }
        }
    }

    // Menu para definir atributos de turmas (seleciona turma e abre submenu)
    void menuDefinirAtributosTurma() {
        string dados[100][5];
        
        int total_turmas = listar_todas_turmas(dados);
        
        if (total_turmas == 0) {
            mostrar_caixa_informacao("INFO", "Nenhuma turma cadastrada.\nCrie uma turma primeiro.");
            return;
        }
        
        string titulos[5] = {"ID", "Nome da Turma", "Disciplina", "Professor", "Alunos"};
        const string* dados_ptr[100];
        for(int i = 0; i < total_turmas; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Selecionar Turma para Gerenciar";
        configTab.descricao = "Selecione uma turma para definir seus atributos.";
        
        saida_tabela turma_selecionada = interface_para_tabela(total_turmas, 5, dados_ptr, titulos, 0, configTab);
        
        if (turma_selecionada.indice_linha == -1) {
            return;
        }
        
        int idTurma = stoi(dados[turma_selecionada.indice_linha][0]);
        menuAtributosTurmaSelecionada(idTurma);
    }

    // Menu principal de gerenciamento de turmas
    void menuGerenciarTurmas() {
        constexpr int Quantidades_opcoes = 3;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Criar Turma",
                "Ver ou Modificar Turma",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gerenciar Turmas";
            config.descricao = "Crie turmas e defina disciplinas, professores e alunos.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);

            switch (resultado.indice_da_opcao) {
                case 0:
                    criarTurmaBasica();
                    break;
                case 1:
                    menuDefinirAtributosTurma();
                    break;
                case 2:
                    continuar = false;
                    break;
            }
        }
    }

    // ----- FUNÇÕES DE RELATÓRIOS E BACKUP -----

    void gerarRelatorioFinanceiro() {
        std::fstream fileLanchonete;
        openFile(fileLanchonete, "lanchonete.dat");
        
        Produto produto;
        double totalEntradas = 0;
        int totalOperacoes = 0;
        
        fileLanchonete.seekg(0);
        
        string dados[100][5];
        int contador = 0;
        
        while(fileLanchonete.read((char*)&produto, sizeof(Produto)) && contador < 100) {
            if(produto.id != 0) {
                dados[contador][0] = to_string(produto.id);
                dados[contador][1] = produto.nome;
                dados[contador][2] = to_string(produto.estoque);
                dados[contador][3] = "R$ " + to_string(produto.preco);
                dados[contador][4] = to_string(produto.estoque * produto.preco);
                
                totalEntradas += produto.estoque * produto.preco;
                totalOperacoes++;
                contador++;
            }
        }
        
        fileLanchonete.close();
        
        if(contador == 0) {
            mostrar_caixa_informacao("INFO", "Nenhuma operacao financeira encontrada.");
            return;
        }
        
        string titulos[5] = {"ID", "Produto", "QTD", "Preco", "Total"};
        const string* dados_ptr[100];
        for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Relatorio Financeiro - Lanchonete";
        interface_para_tabela(contador, 5, dados_ptr, titulos, 0, configTab);
        
        string resumo = "Total de operacoes: " + to_string(totalOperacoes) + 
                       "\nTotal de entradas: R$ " + to_string(totalEntradas);
        mostrar_caixa_informacao("RESUMO FINANCEIRO", resumo);
    }
    void gerarRelatorioPatrimonial() {
        std::fstream fileInstrumentos;
        std::fstream fileEmprestimos;
        
        openFile(fileInstrumentos, "instrumentos.dat");
        openFile(fileEmprestimos, "emprestimos.dat");
        
        Instrumento instrumento;
        Emprestimo emprestimo;
        
        string dados[100][6];
        int contador = 0;
        int totalInstrumentos = 0;
        int disponiveis = 0;
        int emprestados = 0;
        
        fileInstrumentos.seekg(0);
        
        while(fileInstrumentos.read((char*)&instrumento, sizeof(Instrumento)) && contador < 100) {
            if(instrumento.ativo) {
                totalInstrumentos++;
                dados[contador][0] = to_string(instrumento.id);
                dados[contador][1] = instrumento.nome;
                dados[contador][2] = to_string(instrumento.estoque);
                
                string status = "Disponivel";
                string emprestadoPara = "";
                string dataEmp = "";
                string dataPrev = "";
                
                if(!instrumento.disponivel) {
                    status = "Emprestado";
                    emprestados++;
                    
                    fileEmprestimos.clear();
                    fileEmprestimos.seekg(0);
                    
                    while(fileEmprestimos.read((char*)&emprestimo, sizeof(Emprestimo))) {
                        if(emprestimo.idInstrumento == instrumento.id) {
                            emprestadoPara = emprestimo.nome_Alu;
                            dataEmp = emprestimo.dataEmprestimo;
                            dataPrev = emprestimo.dataPrevista;
                            break;
                        }
                    }
                } else {
                    disponiveis++;
                }
                
                dados[contador][3] = status;
                dados[contador][4] = emprestadoPara;
                dados[contador][5] = dataPrev;
                
                contador++;
            }
        }
        
        fileInstrumentos.close();
        fileEmprestimos.close();
        
        if(contador == 0) {
            mostrar_caixa_informacao("INFO", "Nenhum instrumento encontrado no sistema.");
            return;
        }
        
        string titulos[6] = {"ID", "Nome", "Estoque", "Status", "Emprestado Para", "Data Prevista"};
        const string* dados_ptr[100];
        for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
        
        ConfigTabela configTab;
        configTab.titulo = "Relatorio Patrimonial - Instrumentos";
        interface_para_tabela(contador, 6, dados_ptr, titulos, 0, configTab);
        
        string resumo = "Total de instrumentos: " + to_string(totalInstrumentos) + 
                       "\nDisponveis: " + to_string(disponiveis) + 
                       "\nEmprestados: " + to_string(emprestados);
        mostrar_caixa_informacao("RESUMO PATRIMONIAL", resumo);
    }

    void realizarBackup() {
        // Gerar timestamp para pasta do backup
        time_t agora = time(0);
        tm *timeinfo = localtime(&agora);
        
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "backup/%Y-%m-%d_%H-%M-%S");
        string pasta_backup = oss.str();
        
        // Criar comando para mkdir (compatível com Windows)
        string cmd_mkdir = "mkdir \"" + pasta_backup + "\" 2>nul || mkdir \"" + pasta_backup + "\"";
        system(cmd_mkdir.c_str());
        
        // Lista de arquivos .dat para fazer backup
        vector<string> arquivos = {
            "alunos.dat",
            "professores.dat",
            "administradores.dat",
            "disciplinas.dat",
            "eventos.dat",
            "instrumentos.dat",
            "emprestimos.dat",
            "usuarios.dat",
            "notas.dat"
        };
        
        int arquivos_copiados = 0;
        
        // Copiar cada arquivo (compatível com Windows)
        for (const string &arquivo : arquivos) {
            ifstream origem(arquivo, ios::binary);
            if (origem.is_open()) {
                string destino = pasta_backup + "/" + arquivo;
                ofstream copia(destino, ios::binary);
                copia << origem.rdbuf();
                origem.close();
                copia.close();
                arquivos_copiados++;
            }
        }
        
        // Também copiar arquivos .txt se existirem
        vector<string> arquivos_txt = {"cadastros.txt", "lanchonete.txt", "instrumentos.txt", "eventos.txt"};
        for (const string &arquivo : arquivos_txt) {
            ifstream origem(arquivo);
            if (origem.is_open()) {
                string destino = pasta_backup + "/" + arquivo;
                ofstream copia(destino);
                copia << origem.rdbuf();
                origem.close();
                copia.close();
                arquivos_copiados++;
            }
        }
        
        mostrar_caixa_informacao("SUCESSO", "Backup realizado com sucesso!\nPasta: " + pasta_backup + 
                              "\nArquivos salvos: " + to_string(arquivos_copiados));
    }

    void restaurarBackup() {
        // Listar pastas dentro de backup/
        vector<string> backups;
        
        // Comando para listar pastas (Windows)
        system("if not exist backup mkdir backup");
        system("dir backup /B /AD > temp_backups.txt");
        
        ifstream lista_backups("temp_backups.txt");
        string linha;
        while (getline(lista_backups, linha)) {
            // Remove espaços em branco
            if (!linha.empty() && linha[0] != '.') {
                backups.push_back(linha);
            }
        }
        lista_backups.close();
        system("del temp_backups.txt");
        
        if (backups.empty()) {
            mostrar_caixa_informacao("INFO", "Nenhum backup encontrado!");
            return;
        }
        
        // Criar tabela com backups
        string dados[100][2];
        for (int i = 0; i < (int)backups.size() && i < 100; i++) {
            dados[i][0] = to_string(i + 1);
            dados[i][1] = backups[i];
        }
        
        string titulos[2] = {"#", "Data/Hora"};
        const string* dados_ptr[100];
        for (int i = 0; i < (int)backups.size(); i++) {
            dados_ptr[i] = dados[i];
        }
        
        ConfigTabela configTab;
        configTab.titulo = "Backups disponíveis";
        configTab.descricao = "Selecione um backup para gerenciar";
        saida_tabela selecionada = interface_para_tabela(backups.size(), 2, dados_ptr, titulos, 0, configTab);
        
        if (selecionada.indice_linha != -1) {
            string backup_selecionado = backups[selecionada.indice_linha];
            string pasta_backup = "backup/" + backup_selecionado;
            
            // Mostrar botões com opções
            ConfigBotoes configBotoes;
            configBotoes.titulo = "Gerenciar Backup";
            configBotoes.descricao = "Data: " + backup_selecionado;
            configBotoes.botoes[0].label = "Restaurar";
            configBotoes.botoes[0].tecla = 'R';
            configBotoes.botoes[0].valor_retorno = 1;
            configBotoes.botoes[1].label = "Apagar";
            configBotoes.botoes[1].tecla = 'A';
            configBotoes.botoes[1].valor_retorno = 0;
            configBotoes.botoes[2].label = "Voltar";
            configBotoes.botoes[2].tecla = 'V';
            configBotoes.botoes[2].valor_retorno = 2;
            configBotoes.numero_botoes = 3;
            saida_botoes acao = interface_para_botoes(configBotoes);
            
            if (acao.confirmado) {
                if (acao.valor_retorno == 1) { // Restaurar
                    // Restaurar arquivos
                    vector<string> arquivos = {
                        "alunos.dat", "professores.dat", "administradores.dat",
                        "disciplinas.dat", "eventos.dat", "instrumentos.dat",
                        "emprestimos.dat", "usuarios.dat", "notas.dat",
                        "cadastros.txt", "lanchonete.txt", "instrumentos.txt", "eventos.txt"
                    };
                    
                    int restaurados = 0;
                    for (const string &arquivo : arquivos) {
                        string origem = pasta_backup + "/" + arquivo;
                        ifstream arquivo_origem(origem, ios::binary);
                        if (arquivo_origem.is_open()) {
                            ofstream arquivo_destino(arquivo, ios::binary);
                            arquivo_destino << arquivo_origem.rdbuf();
                            arquivo_origem.close();
                            arquivo_destino.close();
                            restaurados++;
                        }
                    }
                    
                    mostrar_caixa_informacao("SUCESSO", "Backup restaurado com sucesso!\nArquivos restaurados: " + to_string(restaurados));
                    
                } else if (acao.valor_retorno == 0) { // Apagar
                    // Comando para remover pasta (Windows)
                    string cmd_remover = "rmdir /S /Q \"" + pasta_backup + "\" 2>nul || rd /S /Q \"" + pasta_backup + "\"";
                    int resultado = system(cmd_remover.c_str());
                    
                    if (resultado == 0) {
                        mostrar_caixa_informacao("SUCESSO", "Backup " + backup_selecionado + " removido com sucesso!");
                    } else {
                        mostrar_caixa_informacao("ERRO", "Erro ao remover backup!");
                    }
                }
            }
        }
    }

}

// =====================================================================
// SEÇÃO: MENU PRINCIPAL DO ADMINISTRADOR
// Ponto de entrada para o módulo de administração
// =====================================================================

// ----- MENU DE GESTÃO ACADÊMICA -----
namespace mod_ADM {
    void menuGestaoAcademica() {
        constexpr int Quantidades_opcoes = 5;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Consultar Pendencias",
                "Gerenciar Disciplinas",
                "Gerenciar Turmas",
                "Gerenciar Usuarios",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gestao Academica";
            config.descricao = "Selecione uma opcao para gerenciar aspectos academicos.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    menuConsultarPendencias();
                    break;
                case 1:
                    mod_ADM::menuCadastroCursos();
                    break;
                case 2:
                    mod_ADM::menuGerenciarTurmas();
                    break;
                case 3:
                    mod_ADM::menuGerenciarUsuarios();
                    break;
                case 4:
                    continuar = false;
                    break;
            }
        }
    }

    void menuConsultarPendencias() {
        constexpr int Quantidades_opcoes = 2;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Pendencias de Instrumentos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Consultar Pendencias";
            config.descricao = "Selecione o tipo de pendencia que deseja consultar.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    consultarPendenciasInstrumentos();
                    break;
                case 1:
                    continuar = false;
                    break;
            }
        }
    }

    // ----- MENU DE LANCHONETE -----
    void menuGestaoLanchonete() {
        constexpr int Quantidades_opcoes = 3;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Consultar Estoque",
                "Gerenciar Produtos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gestao Lanchonete";
            config.descricao = "Selecione uma opcao para gerenciar a lanchonete.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    Lanchonete::consultarEstoque();
                    break;
                case 1:
                    Lanchonete::menuCadastroProdutos();
                    break;
                case 2:
                    continuar = false;
                    break;
            }
        }
    }

    // ----- MENU DE RELATÓRIOS -----
    void menuRelatorios() {
        constexpr int Quantidades_opcoes = 4;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Relatorio Financeiro",
                "Relatorio Patrimonial",
                "Relatorios Academicos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Relatorios";
            config.descricao = "Selecione um relatorio para visualizar.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    mod_ADM::gerarRelatorioFinanceiro();
                    break;
                case 1:
                    mod_ADM::gerarRelatorioPatrimonial();
                    break;
                case 2:
                    mod_ADM::consultarRelatoriosAcademicos();
                    break;
                case 3:
                    continuar = false;
                    break;
            }
        }
    }

    // ----- MENU DE BACKUP -----
    void menuBackup() {
        constexpr int Quantidades_opcoes = 3;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Realizar Backup",
                "Restaurar Backup",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Backup e Restauracao";
            config.descricao = "Selecione uma opcao de backup para gerenciar.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            switch (resultado.indice_da_opcao) {
                case 0:
                    mod_ADM::realizarBackup();
                    break;
                case 1:
                    mod_ADM::restaurarBackup();
                    break;
                case 2:
                    continuar = false;
                    break;
            }
        }
    }
}

void abrir_menu_admin(Usuario* usuario) {
    constexpr int qtdOpcoes = 7;
    string opcoes[qtdOpcoes] = {
        "Gestao Academica",
        "Gerenciar Eventos",
        "Gerenciar Instrumentos",
        "Gestao Lanchonete",
        "Relatorios",
        "Backup e Restauracao",
        "Logout"
    };
    
    bool continuar = true;
    
    while (continuar) {
        ConfigMenu config;
        config.titulo = "AREA DO ADMINISTRADOR";
        config.descricao = "Bem-vindo(a), " + string(usuario->nome) + "!";
        
        saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
        
        switch (resultado.indice_da_opcao) {
            case 0:
                mod_ADM::menuGestaoAcademica();
                break;
            case 1:
                menu_eventos();
                break;
            case 2:
                menuInstrumentos();
                break;
            case 3:
                Lanchonete::menuCadastroProdutos();
                break;
            case 4:
                mod_ADM::menuRelatorios();
                break;
            case 5:
                mod_ADM::menuBackup();
                break;
            case 6:
                usuario->logado = false;
                continuar = false;
                break;
        }
    }
}


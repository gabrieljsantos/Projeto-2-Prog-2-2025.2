#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include "admin.h"
#include "headers.h"
#include "interface_grafica.h"
#include "lanchonete.h"
#include "login_matricula.h"

using namespace std;

// =====================================================================
// SEÇÃO: FUNÇÕES AUXILIARES GERAIS
// =====================================================================

void limparbuffer(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
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

    if(buscaId <= 0 || buscaId > total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return prof;
    }
    file.seekg((buscaId - 20260000) * sizeof(Professor));
    file.read((char*)&prof,sizeof(Professor));
    if(prof.base.id == buscaId&&prof.base.ativo){
        string info = "Nome: " + string(prof.base.nome) + "\nE-mail: " + string(prof.base.email);
        mostrar_caixa_informacao("PROFESSOR", info);
        return prof;
    }
    else{
        mostrar_caixa_informacao("ERRO", "Nenhum professor com esse ID ou Inativo!");
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
        string info = "Nome: " + string(disc.nome);
        mostrar_caixa_informacao("DISCIPLINA", info);
        return disc;
    }
    else{
        mostrar_caixa_informacao("ERRO", "Nenhuma disciplina com esse ID!");
        return disc;
    }
}

Aluno buscaAluno(std::fstream &file, int buscaId){
    Aluno aluno{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Aluno);
    
    if(buscaId <= 0 || buscaId > total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return aluno;
    }
    file.seekg((buscaId - 20260000) * sizeof(Aluno));
    file.read((char*)&aluno, sizeof(Aluno));
    if(aluno.base.id == buscaId && aluno.base.ativo){
        string info = "Nome: " + string(aluno.base.nome) + "\nE-mail: " + string(aluno.base.email);
        mostrar_caixa_informacao("ALUNO", info);
        return aluno;
    }
    else{
        mostrar_caixa_informacao("ERRO", "Nenhum aluno com esse ID ou Inativo!");
        return aluno;
    }
}

Instrumento buscarInstrumento(std::fstream &file, int buscaId){
    Instrumento inst{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Instrumento);
    if (buscaId <= 0 || buscaId > total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        return inst;
    }
    file.seekg((buscaId - 1) * sizeof(Instrumento));
    file.read((char*)&inst, sizeof(Instrumento));
    if (inst.id == buscaId){
        string info = "Nome: " + string(inst.nome) + "\nEstoque: " + to_string(inst.estoque);
        mostrar_caixa_informacao("INSTRUMENTO", info);
        return inst;
    }
    else{
        mostrar_caixa_informacao("ERRO", "Nenhum instrumento com esse ID!");
        return inst;
    }
}

Produto buscaProduto(std::fstream &file, int buscaId){
    Produto p{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Produto);
    
    if(buscaId <= 0 || buscaId > total){
        mostrar_caixa_informacao("ERRO", "ID invalido!");
        p.id = 0;
        return p;
    }
    file.seekg((buscaId - 1) * sizeof(Produto));
    file.read((char*)&p, sizeof(Produto));
    if(p.id == buscaId){
        string info = "Nome: " + string(p.nome) + "\nQTD: " + to_string(p.estoque);
        mostrar_caixa_informacao("PRODUTO", info);
        return p;
    }
    else{
        mostrar_caixa_informacao("ERRO", "Nenhum produto com esse ID!");
        p.id = 0;
        return p;
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
                    fileAlunos.seekg((emp.idAluno - 20260000) * sizeof(Aluno));
                    
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
                    fileAlunos.seekg((emp.idAluno - 20260000) * sizeof(Aluno));
                    
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

    int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, string dados[100][6]){
            // Inicializar dados com valores vazios
            for(int i = 0; i < 100; i++) {
                for(int j = 0; j < 6; j++) {
                    dados[i][j] = "";
                }
            }
            
            int contador = 0;
            int total_lidos = 0;
            
            switch (tipo_usuario)
            {
            case ALUNO: {
                std::fstream file;
                openFile(file, "alunos.dat");
                Aluno aluno;
                file.seekg(0);
                while(file.read((char*)&aluno, sizeof(Aluno)) && contador < 100) {
                    total_lidos++;
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
                    total_lidos++;
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
            
            // DEBUG: Mostrar informações sobre filtro
            std::cout << "[DEBUG] Total lidos: " << total_lidos << std::endl;
            std::cout << "[DEBUG] Usuarios encontrados: " << contador << " | Filtro ativo: " << ativo << " (1=Ativo, 0=Inativo, 2=Ambos)" << std::endl;
            system("pause"); // Pausa para leitura dos debug logs
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

    int listar_eventos_especificos(int autorizado, string dados[100][5]){
        int contador = 0;
        std::fstream file;
        openFile(file, "eventos.dat");
        Evento evento;
        
        file.seekg(0);
        while(file.read((char*)&evento, sizeof(Evento)) && contador < 100) {
            if(evento.id != 0 && evento.ativo && (evento.autorizado == autorizado || autorizado == 2)) {
                dados[contador][0] = to_string(evento.id);
                dados[contador][1] = evento.nome;
                dados[contador][2] = evento.data;
                dados[contador][3] = evento.local;
                dados[contador][4] = to_string(evento.vagasOcupadas) + "/" + to_string(evento.totalVagas);
                contador++;
            }
        }
        file.close();
        return contador;
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

        constexpr int Quantidades_opcoes = 4;
        string texto_do_tipo_de_usuario;
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
                case ADMINISTRADOR:
                    texto_do_tipo_de_usuario = "Administrador";
                    break;
                default:
                    texto_do_tipo_de_usuario = "Desconhecido";
                    break;
            }

            string opcoes[Quantidades_opcoes] = {
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
                case 0: {
                    constexpr int qtdVariaveis = 2;
                    string nomes_variaveis[qtdVariaveis] = {"Tipo de Usuario", "Estado do Usuario"};
                    
                    constexpr int qtdTipos = 3;
                    string tipos[qtdTipos] = {"Aluno", "Professor", "Administrador"};
                    
                    constexpr int qtdEstados = 3;
                    string estados[qtdEstados] = {"Ativo", "Inativo", "Ambos"};
                    
                    const string* opcoes_variaveis[] = {(const string*)tipos, (const string*)estados};
                    int num_opcoes[] = {qtdTipos, qtdEstados};
                    
                    ConfigConfigurador configFiltro;
                    configFiltro.titulo = "Filtros de Pesquisa";
                    configFiltro.descricao = "Defina os filtros para gerenciar usuarios.";
                    
                    saida_configurador resultadofiltro = interface_para_configurador(qtdVariaveis, nomes_variaveis, opcoes_variaveis, num_opcoes, configFiltro);
                    
                    if (resultadofiltro.confirmado && resultadofiltro.numero_variaveis >= 2) {
                        string tipo_str = resultadofiltro.valores_selecionados[0];
                        if (tipo_str == "Aluno") tipo_usuario = ALUNO;
                        else if (tipo_str == "Professor") tipo_usuario = PROFESSOR;
                        else if (tipo_str == "Administrador") tipo_usuario = ADMINISTRADOR;
                        
                        // Converter estado do usuario
                        string estado_str = resultadofiltro.valores_selecionados[1];
                        if (estado_str == "Ativo") {
                            estado_selecionado = 1;
                        }
                        else if (estado_str == "Inativo") {
                            estado_selecionado = 0;
                        }
                        else if (estado_str == "Ambos") {
                            estado_selecionado = 2;
                        }
                        std::cout << "[DEBUG MENU] Estado selecionado: " << estado_selecionado << " (String: '" << estado_str << "')" << std::endl;
                        std::cout << "[DEBUG MENU] Tipo selecionado: " << tipo_usuario << " (String: '" << tipo_str << "')" << std::endl;
                    }
                    break;
                }
                case 1: {
                    string dados[100][6];
                    int total = listar_usuarios_especificos(tipo_usuario, estado_selecionado, dados);
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum usuario encontrado com os filtros selecionados.");
                        break;
                    }
                    
                    string titulos[5] = {"ID", "Nome", "Email", "Categoria", "Status"};
                    const string* dados_ptr[100]; 
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    ConfigTabela configTab;
                    configTab.titulo = "Todos os Usuarios (" + texto_do_tipo_de_usuario + "s)";
                    saida_tabela usuario_selecionado = interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
                    if (usuario_selecionado.indice_linha != -1) {
                        ConfigBotoes configBotoes;
                        configBotoes.titulo = "Gerenciar Usuario ";
                        configBotoes.descricao = "id: " + dados[usuario_selecionado.indice_linha][0] + " | Usuario: " + dados[usuario_selecionado.indice_linha][1] + " | Status: " + dados[usuario_selecionado.indice_linha][4];
                        configBotoes.botoes[0].label = "Ativar";
                        configBotoes.botoes[0].tecla = 'A';
                        configBotoes.botoes[0].valor_retorno = 1;
                        configBotoes.botoes[1].label = "Desativar";
                        configBotoes.botoes[1].tecla = 'D';
                        configBotoes.botoes[1].valor_retorno = 0;
                        configBotoes.botoes[2].label = "modificar";
                        configBotoes.botoes[2].tecla = 'M';
                        configBotoes.botoes[2].valor_retorno = 2;
                        configBotoes.numero_botoes = 3;
                        saida_botoes acao_sob_usuario = interface_para_botoes(configBotoes);

                        if(acao_sob_usuario.confirmado) {
                            if(acao_sob_usuario.valor_retorno == 1) {
                                // Ativar usuario
                                int id = stoi(dados[usuario_selecionado.indice_linha][0]);
                                atualizar_estado_de_usuario(id, tipo_usuario, "Ativo");
                                mostrar_caixa_informacao("SUCESSO", "Usuario ativado com sucesso!");
                            }
                            if(acao_sob_usuario.valor_retorno == 0) {
                                // Desativar usuario
                                int id = stoi(dados[usuario_selecionado.indice_linha][0]);
                                atualizar_estado_de_usuario(id, tipo_usuario, "Inativo");
                                mostrar_caixa_informacao("SUCESSO", "Usuario desativado com sucesso!");
                            }
                        }
                    }
                    break;
                }
                case 2: {
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
                    
                    // Buscar usuario usando funcoes do modulo login_matricula
                    bool encontrado = false;
                    
                    if (tipo_usuario == ALUNO) {
                        Aluno aluno = Login_mat::lerAluno(idBusca);
                        if (aluno.base.id == idBusca) {
                            encontrado = true;
                            string status_atual = aluno.base.ativo ? "Ativo" : "Inativo";
                            
                            mostrar_caixa_informacao("INFO", "Nome: " + string(aluno.base.nome) + "\nStatus: " + status_atual);
                            
                            aluno.base.ativo = !aluno.base.ativo;
                            Login_mat::salvarAluno(aluno);
                            
                            string novo_status = aluno.base.ativo ? "Ativado" : "Desativado";
                            mostrar_caixa_informacao("SUCESSO", "Usuario " + novo_status + " com sucesso!");
                        }
                    } 
                    else if (tipo_usuario == PROFESSOR) {
                        Professor prof = Login_mat::lerProfessor(idBusca);
                        if (prof.base.id == idBusca) {
                            encontrado = true;
                            string status_atual = prof.base.ativo ? "Ativo" : "Inativo";
                            
                            mostrar_caixa_informacao("INFO", "Nome: " + string(prof.base.nome) + "\nStatus: " + status_atual);
                            
                            prof.base.ativo = !prof.base.ativo;
                            Login_mat::salvarProfessor(prof);
                            
                            string novo_status = prof.base.ativo ? "Ativado" : "Desativado";
                            mostrar_caixa_informacao("SUCESSO", "Usuario " + novo_status + " com sucesso!");
                        }
                    }
                    
                    if (!encontrado) {
                        mostrar_caixa_informacao("ERRO", "Usuario nao encontrado!");
                    }
                    break;
                }
                case 3:
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
    constexpr int Quantidades_opcoes = 6;
    bool continuar = true;

    while (continuar) {
        string opcoes[Quantidades_opcoes] = {
            "Cadastrar Disciplina",
            "Autorizar Disciplina",
            "Inativar Disciplina",
            "Vincular Professor",
            "Listar Disciplinas",
            "Voltar"
        };
        
        ConfigMenu config;
        config.titulo = "Gerenciar Disciplinas";   
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
                disciplina_.ativo = 0; // Inativa por padrão até autorização
                
                file.seekp(0, std::ios::end);
                disciplina_.id = 1 + file.tellp() / sizeof(Disciplina);
                file.write((char*)&disciplina_, sizeof(Disciplina));
                file.clear();
                
                mostrar_caixa_informacao("SUCESSO", "Disciplina cadastrada com sucesso!\nID: " + to_string(disciplina_.id));
                break;
            }
            
            case 1: { // Autorizar Disciplina - COM TABELA INTERATIVA
                string dados[100][6];
                int total = listar_disciplinas_especificas(0, dados); // 0 = Inativas
                
                if (total == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma disciplina inativa para autorizar.");
                    break;
                }
                
                string titulos[4] = {"ID", "Nome", "Carga Hor.", "Status"};
                const string* dados_ptr[100];
                for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Disciplinas para Autorizar (Inativas)";
                saida_tabela disciplina_selecionada = interface_para_tabela(total, 4, dados_ptr, titulos, 0, configTab);
                
                if (disciplina_selecionada.indice_linha != -1) {
                    int idDisc = stoi(dados[disciplina_selecionada.indice_linha][0]);
                    disciplina_ = buscaDisciplina(file, idDisc);
                    
                    if (disciplina_.id != 0 && disciplina_.ativo == 0) {
                        ConfigBotoes configBotoes;
                        configBotoes.titulo = "Autorizar Disciplina";
                        configBotoes.descricao = "ID: " + dados[disciplina_selecionada.indice_linha][0] + 
                                                " | Nome: " + dados[disciplina_selecionada.indice_linha][1] + 
                                                " | Status: Inativa";
                        configBotoes.botoes[0].label = "Autorizar";
                        configBotoes.botoes[0].tecla = 'A';
                        configBotoes.botoes[0].valor_retorno = 1;
                        configBotoes.botoes[1].label = "Cancelar";
                        configBotoes.botoes[1].tecla = 'C';
                        configBotoes.botoes[1].valor_retorno = 0;
                        configBotoes.numero_botoes = 2;
                        saida_botoes acao = interface_para_botoes(configBotoes);
                        
                        if(acao.confirmado && acao.valor_retorno == 1) {
                            disciplina_.ativo = 1;
                            file.seekp((idDisc - 1) * sizeof(Disciplina));
                            file.write((char*)&disciplina_, sizeof(Disciplina));
                            file.clear();
                            mostrar_caixa_informacao("SUCESSO", "Disciplina " + string(disciplina_.nome) + " autorizada com sucesso!");
                        }
                    }
                }
                break;
            }
            
            case 2: { // Inativar Disciplina - COM TABELA INTERATIVA
                string dados[100][6];
                int total = listar_disciplinas_especificas(1, dados); // 1 = Ativas
                
                if (total == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma disciplina ativa para inativar.");
                    break;
                }
                
                string titulos[4] = {"ID", "Nome", "Carga Hor.", "Status"};
                const string* dados_ptr[100];
                for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Disciplinas para Inativar (Ativas)";
                saida_tabela disciplina_selecionada = interface_para_tabela(total, 4, dados_ptr, titulos, 0, configTab);
                
                if (disciplina_selecionada.indice_linha != -1) {
                    int idDisc = stoi(dados[disciplina_selecionada.indice_linha][0]);
                    disciplina_ = buscaDisciplina(file, idDisc);
                    
                    if (disciplina_.id != 0 && disciplina_.ativo == 1) {
                        ConfigBotoes configBotoes;
                        configBotoes.titulo = "Inativar Disciplina";
                        configBotoes.descricao = "ID: " + dados[disciplina_selecionada.indice_linha][0] + 
                                                " | Nome: " + dados[disciplina_selecionada.indice_linha][1] + 
                                                " | Status: Ativa";
                        configBotoes.botoes[0].label = "Inativar";
                        configBotoes.botoes[0].tecla = 'I';
                        configBotoes.botoes[0].valor_retorno = 1;
                        configBotoes.botoes[1].label = "Cancelar";
                        configBotoes.botoes[1].tecla = 'C';
                        configBotoes.botoes[1].valor_retorno = 0;
                        configBotoes.numero_botoes = 2;
                        saida_botoes acao = interface_para_botoes(configBotoes);
                        
                        if(acao.confirmado && acao.valor_retorno == 1) {
                            disciplina_.ativo = 0;
                            file.seekp((idDisc - 1) * sizeof(Disciplina));
                            file.write((char*)&disciplina_, sizeof(Disciplina));
                            file.clear();
                            mostrar_caixa_informacao("SUCESSO", "Disciplina " + string(disciplina_.nome) + " inativada com sucesso!");
                        }
                    }
                }
                break;
            }
            
            case 3: { // Vincular Professor - COM TABELA INTERATIVA
                string dados[100][6];
                int total = listar_disciplinas_especificas(1, dados); // 1 = Ativas
                
                if (total == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma disciplina ativa para vincular professor.");
                    break;
                }
                
                string titulos[4] = {"ID", "Nome", "Carga Hor.", "Status"};
                const string* dados_ptr[100];
                for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Disciplinas para Vincular Professor";
                saida_tabela disciplina_selecionada = interface_para_tabela(total, 4, dados_ptr, titulos, 0, configTab);
                
                if (disciplina_selecionada.indice_linha != -1) {
                    int idDisc = stoi(dados[disciplina_selecionada.indice_linha][0]);
                    disciplina_ = buscaDisciplina(file, idDisc);
                    
                    if (disciplina_.id != 0 && disciplina_.ativo == 1) {
                        ConfigEntradaTexto configProfId;
                        configProfId.titulo = "Vincular Professor";
                        configProfId.label = "Digite o ID do Professor: ";
                        configProfId.tipo_entrada = TIPO_NUMERO;
                        saida_entrada_texto resultProfId = interface_para_entrada_texto(configProfId);
                        if (!resultProfId.confirmado) break;
                        int idProf = stoi(resultProfId.valor);
                        
                        std::fstream fileProfessores;
                        openFile(fileProfessores, "professores.dat");
                        Professor prof = buscaProf(fileProfessores, idProf);
                        fileProfessores.close();
                        
                        if (prof.base.id == 0) {
                            mostrar_caixa_informacao("ERRO", "Professor nao encontrado!");
                            break;
                        }
                        
                        if (prof.disciplina[0] != '\0') {
                            mostrar_caixa_informacao("INFO", "Professor ja possui uma disciplina vinculada!");
                            break;
                        }
                        
                        strncpy(prof.disciplina, disciplina_.nome, 29);
                        prof.disciplina[29] = '\0';
                        
                        openFile(fileProfessores, "professores.dat");
                        fileProfessores.seekp((idProf - 20260000) * sizeof(Professor));
                        fileProfessores.write((char*)&prof, sizeof(Professor));
                        fileProfessores.close();
                        file.clear();
                        
                        mostrar_caixa_informacao("SUCESSO", "Professor " + string(prof.base.nome) + " vinculado a disciplina " + string(disciplina_.nome) + "!");
                    }
                }
                break;
            }
            
            case 4: { // Listar Disciplinas
                string dados[100][4];
                int contador = 0;
                
                file.seekg(0);
                while (file.read((char*)&disciplina_, sizeof(Disciplina)) && contador < 100) {
                    if (disciplina_.id != 0) {
                        dados[contador][0] = to_string(disciplina_.id);
                        dados[contador][1] = disciplina_.nome;
                        dados[contador][2] = to_string(disciplina_.cargaHoraria);
                        dados[contador][3] = disciplina_.ativo ? "Ativa" : "Inativa";
                        contador++;
                    }
                }
                file.clear();
                
                if (contador == 0) {
                    mostrar_caixa_informacao("INFO", "Nenhuma disciplina cadastrada!");
                    break;
                }
                
                string titulos[4] = {"ID", "Nome", "Carga Hor.", "Status"};
                const string* dados_ptr[100];
                for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                
                ConfigTabela configTab;
                configTab.titulo = "Lista de Disciplinas";
                saida_tabela selecionada = interface_para_tabela(contador, 4, dados_ptr, titulos, 0, configTab);
                
                if (selecionada.indice_linha != -1) {
                    mostrar_caixa_informacao("INFO", "Disciplina selecionada: " + dados[selecionada.indice_linha][1]);
                }
                break;
            }
            
            case 5: { // Voltar
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

    // ----- MENU DE EVENTOS -----

    void menuEventos(std::fstream &file) {
        constexpr int Quantidades_opcoes = 5;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Cadastrar Evento",
                "Autorizar Evento",
                "Inativar Evento",
                "Listar Eventos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gerenciar Eventos";
            config.descricao = "Selecione uma opcao para gerenciar eventos.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            Evento *evento_ = new Evento;
            
            switch (resultado.indice_da_opcao) {
                case 0: { // Cadastrar Evento
                    ConfigEntradaTexto configNome;
                    configNome.titulo = "Novo Evento";
                    configNome.label = "Nome: ";
                    saida_entrada_texto resultNome = interface_para_entrada_texto(configNome);
                    if (!resultNome.confirmado) break;
                    
                    ConfigEntradaTexto configLocal;
                    configLocal.titulo = "Novo Evento";
                    configLocal.label = "Local do evento: ";
                    saida_entrada_texto resultLocal = interface_para_entrada_texto(configLocal);
                    if (!resultLocal.confirmado) break;
                    
                    ConfigEntradaTexto configDesc;
                    configDesc.titulo = "Novo Evento";
                    configDesc.label = "Descricao: ";
                    saida_entrada_texto resultDesc = interface_para_entrada_texto(configDesc);
                    if (!resultDesc.confirmado) break;
                    
                    ConfigEntradaTexto configData;
                    configData.titulo = "Novo Evento";
                    configData.label = "Data do evento [dd/mm/aaaa]: ";
                    saida_entrada_texto resultData = interface_para_entrada_texto(configData);
                    if (!resultData.confirmado) break;
                    
                    ConfigEntradaTexto configVagas;
                    configVagas.titulo = "Novo Evento";
                    configVagas.label = "Total de vagas: ";
                    configVagas.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultVagas = interface_para_entrada_texto(configVagas);
                    if (!resultVagas.confirmado) break;
                    
                    char nome[50];
                    strncpy(nome, resultNome.valor.c_str(), 49);
                    nome[49] = '\0';
                    char local[50];
                    strncpy(local, resultLocal.valor.c_str(), 49);
                    local[49] = '\0';
                    char descricao[100];
                    strncpy(descricao, resultDesc.valor.c_str(), 99);
                    descricao[99] = '\0';
                    char data[11];
                    strncpy(data, resultData.valor.c_str(), 10);
                    data[10] = '\0';
                    int vagas = stoi(resultVagas.valor);
                    
                    strncpy(evento_->nome, nome, 49);
                    evento_->nome[49] = '\0';
                    strncpy(evento_->local, local, 49);
                    evento_->local[49] = '\0';
                    strncpy(evento_->descricao, descricao, 99);
                    evento_->descricao[99] = '\0';
                    strncpy(evento_->data, data, 11);
                    evento_->data[10] = '\0';
                    
                    evento_->totalVagas = vagas;
                    evento_->vagasOcupadas = 0;
                    evento_->autorizado = 0;
                    evento_->ativo = 1;
                    
                    file.clear();
                    file.seekg(0, std::ios::end);
                    evento_->id = 1 + (file.tellg() / sizeof(Evento));
                    
                    file.seekp(0, std::ios::end);
                    file.write((char*)(evento_), sizeof(Evento));
                    file.clear();
                    
                    mostrar_caixa_informacao("SUCESSO", "Evento cadastrado com sucesso!\nID: " + to_string(evento_->id));
                    break;
                }
                
                case 1: { // Autorizar Evento - COM TABELA INTERATIVA
                    string dados[100][5];
                    int total = listar_eventos_especificos(0, dados); // 0 = Não autorizados
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum evento pendente para autorizar.");
                        break;
                    }
                    
                    string titulos[5] = {"ID", "Nome", "Data", "Local", "Vagas"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Eventos para Autorizar (Pendentes)";
                    saida_tabela evento_selecionado = interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
                    
                    if (evento_selecionado.indice_linha != -1) {
                        int idEvento = stoi(dados[evento_selecionado.indice_linha][0]);
                        file.clear();
                        file.seekg((idEvento - 1) * sizeof(Evento));
                        file.read((char*)evento_, sizeof(Evento));
                        
                        if (evento_->id != 0 && evento_->autorizado == 0) {
                            ConfigBotoes configBotoes;
                            configBotoes.titulo = "Autorizar Evento";
                            configBotoes.descricao = "ID: " + dados[evento_selecionado.indice_linha][0] + 
                                                    " | Nome: " + dados[evento_selecionado.indice_linha][1] + 
                                                    " | Data: " + dados[evento_selecionado.indice_linha][2];
                            configBotoes.botoes[0].label = "Autorizar";
                            configBotoes.botoes[0].tecla = 'A';
                            configBotoes.botoes[0].valor_retorno = 1;
                            configBotoes.botoes[1].label = "Cancelar";
                            configBotoes.botoes[1].tecla = 'C';
                            configBotoes.botoes[1].valor_retorno = 0;
                            configBotoes.numero_botoes = 2;
                            saida_botoes acao = interface_para_botoes(configBotoes);
                            
                            if(acao.confirmado && acao.valor_retorno == 1) {
                                evento_->autorizado = 1;
                                file.clear();
                                file.seekp((idEvento - 1) * sizeof(Evento));
                                file.write((char*)evento_, sizeof(Evento));
                                file.clear();
                                mostrar_caixa_informacao("SUCESSO", "Evento " + string(evento_->nome) + " autorizado com sucesso!");
                            }
                        }
                    }
                    break;
                }
                
                case 2: { // Inativar Evento - COM TABELA INTERATIVA
                    string dados[100][5];
                    int total = listar_eventos_especificos(2, dados); // 2 = Ambos (filtrar apenas ativos)
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum evento ativo para inativar.");
                        break;
                    }
                    
                    string titulos[5] = {"ID", "Nome", "Data", "Local", "Vagas"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Eventos para Inativar";
                    saida_tabela evento_selecionado = interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
                    
                    if (evento_selecionado.indice_linha != -1) {
                        int idEvento = stoi(dados[evento_selecionado.indice_linha][0]);
                        file.clear();
                        file.seekg((idEvento - 1) * sizeof(Evento));
                        file.read((char*)evento_, sizeof(Evento));
                        
                        if (evento_->id != 0 && evento_->ativo == 1) {
                            ConfigBotoes configBotoes;
                            configBotoes.titulo = "Inativar Evento";
                            configBotoes.descricao = "ID: " + dados[evento_selecionado.indice_linha][0] + 
                                                    " | Nome: " + dados[evento_selecionado.indice_linha][1] + 
                                                    " | Data: " + dados[evento_selecionado.indice_linha][2];
                            configBotoes.botoes[0].label = "Inativar";
                            configBotoes.botoes[0].tecla = 'I';
                            configBotoes.botoes[0].valor_retorno = 1;
                            configBotoes.botoes[1].label = "Cancelar";
                            configBotoes.botoes[1].tecla = 'C';
                            configBotoes.botoes[1].valor_retorno = 0;
                            configBotoes.numero_botoes = 2;
                            saida_botoes acao = interface_para_botoes(configBotoes);
                            
                            if(acao.confirmado && acao.valor_retorno == 1) {
                                evento_->ativo = 0;
                                file.clear();
                                file.seekp((idEvento - 1) * sizeof(Evento));
                                file.write((char*)evento_, sizeof(Evento));
                                file.clear();
                                mostrar_caixa_informacao("SUCESSO", "Evento " + string(evento_->nome) + " inativado com sucesso!");
                            }
                        }
                    }
                    break;
                }
                
                case 3: { // Listar Eventos
                    constexpr int Opcoes_listar = 3;
                    string opcoes_listar[Opcoes_listar] = {
                        "Listar Autorizados",
                        "Listar Pendentes",
                        "Voltar"
                    };
                    
                    ConfigMenu configLista;
                    configLista.titulo = "Filtrar Eventos";
                    configLista.descricao = "Selecione qual lista deseja visualizar.";
                    saida_menu resultado_lista = interface_para_menu(Opcoes_listar, opcoes_listar, configLista);
                    
                    if (resultado_lista.indice_da_opcao == 0 || resultado_lista.indice_da_opcao == 1) {
                        string dados[100][5];
                        int contador = 0;
                        bool autorizado_filtro = (resultado_lista.indice_da_opcao == 0);
                        
                        file.clear();
                        file.seekg(0);
                        
                        while (file.read((char*)evento_, sizeof(Evento)) && contador < 100) {
                            if (evento_->ativo) {
                                if ((autorizado_filtro && evento_->autorizado) || (!autorizado_filtro && !evento_->autorizado)) {
                                    dados[contador][0] = to_string(evento_->id);
                                    dados[contador][1] = evento_->nome;
                                    dados[contador][2] = evento_->data;
                                    dados[contador][3] = evento_->local;
                                    dados[contador][4] = to_string(evento_->vagasOcupadas) + "/" + to_string(evento_->totalVagas);
                                    contador++;
                                }
                            }
                        }
                        file.clear();
                        
                        if (contador == 0) {
                            string tipo = autorizado_filtro ? "Nenhum evento autorizado" : "Nenhum evento pendente";
                            mostrar_caixa_informacao("INFO", tipo + " encontrado!");
                            break;
                        }
                        
                        string titulos[5] = {"ID", "Nome", "Data", "Local", "Vagas"};
                        const string* dados_ptr[100];
                        for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                        
                        ConfigTabela configTab;
                        configTab.titulo = autorizado_filtro ? "Eventos Autorizados" : "Eventos Pendentes";
                        saida_tabela selecionada = interface_para_tabela(contador, 5, dados_ptr, titulos, 0, configTab);
                        
                        if (selecionada.indice_linha != -1) {
                            string detalhes = "ID: " + dados[selecionada.indice_linha][0] + 
                                            "\nNome: " + dados[selecionada.indice_linha][1] + 
                                            "\nData: " + dados[selecionada.indice_linha][2] + 
                                            "\nLocal: " + dados[selecionada.indice_linha][3] + 
                                            "\nVagas: " + dados[selecionada.indice_linha][4];
                            mostrar_caixa_informacao("DETALHES", detalhes);
                        }
                    }
                    break;
                }
                
                case 4: { // Voltar
                    continuar = false;
                    break;
                }
            }
            
            delete evento_;
            evento_ = nullptr;
        }
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

    void cadastrarTurma(){
        Turma turma;
        Disciplina* disc = new Disciplina;
        Professor* prof = new Professor;
        int buscaId;
        std::fstream file;
        std::fstream fileDisc;
        std::fstream fileProf;
        openFile(file, "turmas.dat");
        openFile(fileDisc, "disciplinas.dat");
        openFile(fileProf, "professores.dat");

        turma.id = gerarNovoId(file, sizeof(Turma));
        turma.ativo = 1;

        ConfigEntradaTexto configDisc;
        configDisc.titulo = "Cadastrar Turma";
        configDisc.label = "ID da Disciplina: ";
        configDisc.tipo_entrada = TIPO_NUMERO;
        saida_entrada_texto resultDisc = interface_para_entrada_texto(configDisc);
        if (!resultDisc.confirmado) {
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            return;
        }
        
        buscaId = stoi(resultDisc.valor);
        *disc = buscaDisciplina(fileDisc, buscaId);
        if (disc->id == 0){
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            return;
        }
        turma.idDisciplina = disc->id;

        ConfigEntradaTexto configProf;
        configProf.titulo = "Cadastrar Turma";
        configProf.label = "ID do Professor: ";
        configProf.tipo_entrada = TIPO_NUMERO;
        saida_entrada_texto resultProf = interface_para_entrada_texto(configProf);
        if (!resultProf.confirmado) {
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            return;
        }
        
        buscaId = stoi(resultProf.valor);
        *prof = buscaProf(fileProf, buscaId);
        if(prof->base.id == 0){
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            return;
        }
        turma.idProfessor = prof->base.id;
        
        int indexVAGA;
        if(verificaTurmasProf(*prof, indexVAGA)){
            prof->turmas[indexVAGA] = turma.id;
            fileProf.seekp((prof->base.id - 20260000) * sizeof(Professor));
            fileProf.write((const char*)prof, sizeof(Professor));
        }
        else{
            mostrar_caixa_informacao("ERRO", "Professor nao tem vaga para novas turmas.");
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            return;
        }

        turma.qtdAlunos = 0;
        for(int i = 0; i < MAX_ALUNOS; i++)
            turma.alunos[i] = 0;

        file.seekp((turma.id - 1) * sizeof(Turma));
        file.write((char*)&turma, sizeof(Turma));

        file.close();
        fileProf.close();
        fileDisc.close();

        delete prof;
        delete disc;
        
        mostrar_caixa_informacao("SUCESSO", "Turma cadastrada com sucesso!\nID: " + to_string(turma.id));
    }

    void matricularAlunoTurma() {
        std::fstream fileTurma;
        std::fstream fileAluno;
        openFile(fileAluno, "alunos.dat");
        openFile(fileTurma, "turmas.dat");
        
        Turma turma;
        Aluno aluno;
        int idAluno, idTurma;

        ConfigEntradaTexto configAluno;
        configAluno.titulo = "Matricular Aluno em Turma";
        configAluno.label = "ID do Aluno: ";
        configAluno.tipo_entrada = TIPO_NUMERO;
        saida_entrada_texto resultAluno = interface_para_entrada_texto(configAluno);
        if (!resultAluno.confirmado) {
            fileTurma.close();
            fileAluno.close();
            return;
        }
        
        idAluno = stoi(resultAluno.valor);
        aluno = buscaAluno(fileAluno, idAluno);
        
        if (aluno.base.id != idAluno) {
            mostrar_caixa_informacao("ERRO", "Aluno nao encontrado ou inativo.");
            fileTurma.close();
            fileAluno.close();
            return;
        }

        ConfigEntradaTexto configTurma;
        configTurma.titulo = "Matricular Aluno em Turma";
        configTurma.label = "ID da Turma: ";
        configTurma.tipo_entrada = TIPO_NUMERO;
        saida_entrada_texto resultTurma = interface_para_entrada_texto(configTurma);
        if (!resultTurma.confirmado) {
            fileTurma.close();
            fileAluno.close();
            return;
        }
        
        idTurma = stoi(resultTurma.valor);
        fileTurma.seekg((idTurma - 1) * sizeof(Turma));
        fileTurma.read((char*)&turma, sizeof(Turma));

        if (turma.id != idTurma || !turma.ativo) {
            mostrar_caixa_informacao("ERRO", "Turma invalida ou inativa.");
            fileTurma.close();
            fileAluno.close();
            return;
        }

        if (turma.qtdAlunos >= MAX_ALUNOS) {
            mostrar_caixa_informacao("ERRO", "Turma cheia.");
            fileTurma.close();
            fileAluno.close();
            return;
        }

        for (int i = 0; i < MAX_ALUNOS; i++) {
            if (turma.alunos[i] == aluno.base.id) {
                mostrar_caixa_informacao("ERRO", "Aluno ja matriculado nesta turma.");
                fileTurma.close();
                fileAluno.close();
                return;
            }
        }
        
        int index = -1;
        for (int i = 0; i < MAX_ALUNOS; i++) {
            if (turma.alunos[i] == 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            mostrar_caixa_informacao("ERRO", "Nao foi possivel encontrar vaga.");
            fileTurma.close();
            fileAluno.close();
            return;
        }

        turma.alunos[index] = aluno.base.id;
        turma.qtdAlunos++;

        fileTurma.seekp((turma.id - 1) * sizeof(Turma));
        fileTurma.write((char*)&turma, sizeof(Turma));

        fileTurma.close();
        fileAluno.close();
        
        mostrar_caixa_informacao("SUCESSO", "Aluno matriculado com sucesso na turma!");
    }

    // ----- MENU DE PRODUTOS (LANCHONETE) -----

    void cadastrarProdutos(){
        constexpr int Quantidades_opcoes = 6;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Cadastrar Produto",
                "Ativar Produto",
                "Inativar Produto",
                "Editar Produto",
                "Listar Produtos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gerenciar Produtos da Lanchonete";
            config.descricao = "Selecione uma opcao para gerenciar os produtos.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            std::fstream file;
            openFile(file, "lanchonete.dat");
            Produto produto_;
            
            switch (resultado.indice_da_opcao) {
                case 0: { // Cadastrar Produto
                    ConfigEntradaTexto configNome;
                    configNome.titulo = "Cadastrar Produto";
                    configNome.label = "Nome do Produto: ";
                    saida_entrada_texto resultNome = interface_para_entrada_texto(configNome);
                    if (!resultNome.confirmado) break;
                    
                    ConfigEntradaTexto configPreco;
                    configPreco.titulo = "Cadastrar Produto";
                    configPreco.label = "Preco (R$): ";
                    configPreco.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultPreco = interface_para_entrada_texto(configPreco);
                    if (!resultPreco.confirmado) break;
                    
                    ConfigEntradaTexto configEstoque;
                    configEstoque.titulo = "Cadastrar Produto";
                    configEstoque.label = "Quantidade em Estoque: ";
                    configEstoque.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultEstoque = interface_para_entrada_texto(configEstoque);
                    if (!resultEstoque.confirmado) break;
                    
                    char nome[50];
                    strncpy(nome, resultNome.valor.c_str(), 49);
                    nome[49] = '\0';
                    double preco = stod(resultPreco.valor);
                    int estoque = stoi(resultEstoque.valor);
                    
                    strncpy(produto_.nome, nome, 49);
                    produto_.nome[49] = '\0';
                    produto_.preco = preco;
                    produto_.estoque = estoque;
                    produto_.ativo = 1;
                    
                    file.seekp(0, std::ios::end);
                    produto_.id = gerarNovoId(file, sizeof(Produto));
                    file.write((char*)&produto_, sizeof(Produto));
                    file.clear();
                    
                    mostrar_caixa_informacao("SUCESSO", "Produto cadastrado com sucesso!\nID: " + to_string(produto_.id));
                    break;
                }
                
                case 1: { // Ativar Produto
                    ConfigEntradaTexto configId;
                    configId.titulo = "Ativar Produto";
                    configId.label = "ID do Produto: ";
                    configId.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultId = interface_para_entrada_texto(configId);
                    if (!resultId.confirmado) break;
                    
                    int idProduto = stoi(resultId.valor);
                    produto_ = buscaProduto(file, idProduto);
                    
                    if (produto_.id == 0) {
                        mostrar_caixa_informacao("ERRO", "Produto nao encontrado!");
                        break;
                    }
                    
                    if (produto_.ativo == 1) {
                        mostrar_caixa_informacao("INFO", "Produto ja esta ativo!");
                    } else {
                        ConfigBotoes configBotoes;
                        configBotoes.titulo = "Ativar Produto";
                        configBotoes.descricao = "ID: " + to_string(produto_.id) + " | Nome: " + string(produto_.nome);
                        configBotoes.botoes[0].label = "Ativar";
                        configBotoes.botoes[0].tecla = 'A';
                        configBotoes.botoes[0].valor_retorno = 1;
                        configBotoes.botoes[1].label = "Cancelar";
                        configBotoes.botoes[1].tecla = 'C';
                        configBotoes.botoes[1].valor_retorno = 0;
                        configBotoes.numero_botoes = 2;
                        saida_botoes acao = interface_para_botoes(configBotoes);
                        
                        if(acao.confirmado && acao.valor_retorno == 1) {
                            produto_.ativo = 1;
                            file.seekp((idProduto - 1) * sizeof(Produto));
                            file.write((char*)&produto_, sizeof(Produto));
                            file.clear();
                            mostrar_caixa_informacao("SUCESSO", "Produto " + string(produto_.nome) + " ativado com sucesso!");
                        }
                    }
                    break;
                }
                
                case 2: { // Inativar Produto
                    ConfigEntradaTexto configId;
                    configId.titulo = "Inativar Produto";
                    configId.label = "ID do Produto: ";
                    configId.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultId = interface_para_entrada_texto(configId);
                    if (!resultId.confirmado) break;
                    
                    int idProduto = stoi(resultId.valor);
                    produto_ = buscaProduto(file, idProduto);
                    
                    if (produto_.id == 0) {
                        mostrar_caixa_informacao("ERRO", "Produto nao encontrado!");
                        break;
                    }
                    
                    if (produto_.ativo == 0) {
                        mostrar_caixa_informacao("INFO", "Produto ja esta inativo!");
                    } else {
                        ConfigBotoes configBotoes;
                        configBotoes.titulo = "Inativar Produto";
                        configBotoes.descricao = "ID: " + to_string(produto_.id) + " | Nome: " + string(produto_.nome);
                        configBotoes.botoes[0].label = "Inativar";
                        configBotoes.botoes[0].tecla = 'I';
                        configBotoes.botoes[0].valor_retorno = 1;
                        configBotoes.botoes[1].label = "Cancelar";
                        configBotoes.botoes[1].tecla = 'C';
                        configBotoes.botoes[1].valor_retorno = 0;
                        configBotoes.numero_botoes = 2;
                        saida_botoes acao = interface_para_botoes(configBotoes);
                        
                        if(acao.confirmado && acao.valor_retorno == 1) {
                            produto_.ativo = 0;
                            file.seekp((idProduto - 1) * sizeof(Produto));
                            file.write((char*)&produto_, sizeof(Produto));
                            file.clear();
                            mostrar_caixa_informacao("SUCESSO", "Produto " + string(produto_.nome) + " inativado com sucesso!");
                        }
                    }
                    break;
                }
                
                case 3: { // Editar Produto
                    ConfigEntradaTexto configId;
                    configId.titulo = "Editar Produto";
                    configId.label = "ID do Produto: ";
                    configId.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultId = interface_para_entrada_texto(configId);
                    if (!resultId.confirmado) break;
                    
                    int idProduto = stoi(resultId.valor);
                    produto_ = buscaProduto(file, idProduto);
                    
                    if (produto_.id == 0 || produto_.ativo == 0) {
                        mostrar_caixa_informacao("ERRO", "Produto nao encontrado ou inativo!");
                        break;
                    }
                    
                    ConfigBotoes configBotoes;
                    configBotoes.titulo = "Editar Produto";
                    configBotoes.descricao = "ID: " + to_string(produto_.id) + " | Nome: " + string(produto_.nome) + 
                                           " | Preco: R$ " + to_string(produto_.preco);
                    configBotoes.botoes[0].label = "Editar";
                    configBotoes.botoes[0].tecla = 'E';
                    configBotoes.botoes[0].valor_retorno = 1;
                    configBotoes.botoes[1].label = "Cancelar";
                    configBotoes.botoes[1].tecla = 'C';
                    configBotoes.botoes[1].valor_retorno = 0;
                    configBotoes.numero_botoes = 2;
                    saida_botoes acao = interface_para_botoes(configBotoes);
                    
                    if(acao.confirmado && acao.valor_retorno == 1) {
                        ConfigEntradaTexto configNovoNome;
                        configNovoNome.titulo = "Editar Produto";
                        configNovoNome.label = "Novo nome: ";
                        saida_entrada_texto resultNovoNome = interface_para_entrada_texto(configNovoNome);
                        if (!resultNovoNome.confirmado) break;
                        
                        ConfigEntradaTexto configNovoPreco;
                        configNovoPreco.titulo = "Editar Produto";
                        configNovoPreco.label = "Novo preco (R$): ";
                        configNovoPreco.tipo_entrada = TIPO_NUMERO;
                        saida_entrada_texto resultNovoPreco = interface_para_entrada_texto(configNovoPreco);
                        if (!resultNovoPreco.confirmado) break;
                        
                        ConfigEntradaTexto configNovoEstoque;
                        configNovoEstoque.titulo = "Editar Produto";
                        configNovoEstoque.label = "Novo estoque: ";
                        configNovoEstoque.tipo_entrada = TIPO_NUMERO;
                        saida_entrada_texto resultNovoEstoque = interface_para_entrada_texto(configNovoEstoque);
                        if (!resultNovoEstoque.confirmado) break;
                        
                        char nome[50];
                        strncpy(nome, resultNovoNome.valor.c_str(), 49);
                        nome[49] = '\0';
                        
                        strncpy(produto_.nome, nome, 49);
                        produto_.nome[49] = '\0';
                        produto_.preco = stod(resultNovoPreco.valor);
                        produto_.estoque = stoi(resultNovoEstoque.valor);
                        
                        file.seekp((idProduto - 1) * sizeof(Produto));
                        file.write((char*)&produto_, sizeof(Produto));
                        file.clear();
                        
                        mostrar_caixa_informacao("SUCESSO", "Produto " + string(produto_.nome) + " editado com sucesso!");
                    }
                    break;
                }
                
                case 4: { // Listar Produtos
                    string dados[100][5];
                    int contador = 0;
                    
                    file.seekg(0);
                    while (file.read((char*)&produto_, sizeof(Produto)) && contador < 100) {
                        if (produto_.id != 0 && produto_.ativo) {
                            dados[contador][0] = to_string(produto_.id);
                            dados[contador][1] = produto_.nome;
                            dados[contador][2] = to_string(produto_.estoque);
                            dados[contador][3] = "R$ " + to_string(produto_.preco);
                            dados[contador][4] = produto_.ativo ? "Ativo" : "Inativo";
                            contador++;
                        }
                    }
                    file.clear();
                    
                    if (contador == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum produto cadastrado!");
                        break;
                    }
                    
                    string titulos[5] = {"ID", "Nome", "Estoque", "Preco", "Status"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Lista de Produtos";
                    saida_tabela selecionada = interface_para_tabela(contador, 5, dados_ptr, titulos, 0, configTab);
                    
                    if (selecionada.indice_linha != -1) {
                        mostrar_caixa_informacao("INFO", "Produto selecionado: " + dados[selecionada.indice_linha][1] + 
                                              " | Estoque: " + dados[selecionada.indice_linha][2]);
                    }
                    break;
                }
                
                case 5: { // Voltar
                    continuar = false;
                    break;
                }
            }
            
            file.close();
        }
    }

    // ----- MENU DE INSTRUMENTOS -----

    void menuCadastroInstrumentos() {
        constexpr int Quantidades_opcoes = 6;
        bool continuar = true;

        while (continuar) {
            string opcoes[Quantidades_opcoes] = {
                "Cadastrar Instrumento",
                "Autorizar Instrumento",
                "Inativar Instrumento",
                "Listar Instrumentos",
                "Voltar"
            };
            
            ConfigMenu config;
            config.titulo = "Gerenciar Instrumentos";   
            config.descricao = "Selecione uma opcao para gerenciar os instrumentos.";
            saida_menu resultado = interface_para_menu(Quantidades_opcoes, opcoes, config);
            
            std::fstream file;
            openFile(file, "instrumentos.dat");
            Instrumento instrumento_;
            
            switch (resultado.indice_da_opcao) {
                case 0: { // Cadastrar Instrumento
                    ConfigEntradaTexto configNome;
                    configNome.titulo = "Cadastrar Instrumento";
                    configNome.label = "Nome do Instrumento: ";
                    saida_entrada_texto resultNome = interface_para_entrada_texto(configNome);
                    if (!resultNome.confirmado) break;
                    
                    ConfigEntradaTexto configEstoque;
                    configEstoque.titulo = "Cadastrar Instrumento";
                    configEstoque.label = "Quantidade em Estoque: ";
                    configEstoque.tipo_entrada = TIPO_NUMERO;
                    saida_entrada_texto resultEstoque = interface_para_entrada_texto(configEstoque);
                    if (!resultEstoque.confirmado) break;
                    
                    char nome[30];
                    strncpy(nome, resultNome.valor.c_str(), 29);
                    nome[29] = '\0';
                    int estoque = stoi(resultEstoque.valor);
                    
                    strncpy(instrumento_.nome, nome, 29);
                    instrumento_.nome[29] = '\0';
                    instrumento_.estoque = estoque;
                    instrumento_.ativo = 1;
                    instrumento_.autorizado = 0; // Inativo por padrão até autorização
                    instrumento_.disponivel = 1;
                    
                    file.seekp(0, std::ios::end);
                    instrumento_.id = 1 + file.tellp() / sizeof(Instrumento);
                    file.write((char*)&instrumento_, sizeof(Instrumento));
                    file.clear();
                    
                    mostrar_caixa_informacao("SUCESSO", "Instrumento cadastrado com sucesso!\nID: " + to_string(instrumento_.id));
                    break;
                }
                
                case 1: { // Autorizar Instrumento - COM TABELA INTERATIVA
                    string dados[100][6];
                    int total = listar_instrumentos_especificos(0, dados); // 0 = Não autorizados
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum instrumento pendente para autorizar.");
                        break;
                    }
                    
                    string titulos[6] = {"ID", "Nome", "Estoque", "Autorizado", "Status", "Disp"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Instrumentos para Autorizar (Pendentes)";
                    saida_tabela instrumento_selecionado = interface_para_tabela(total, 6, dados_ptr, titulos, 0, configTab);
                    
                    if (instrumento_selecionado.indice_linha != -1) {
                        int idInst = stoi(dados[instrumento_selecionado.indice_linha][0]);
                        file.clear();
                        file.seekg((idInst - 1) * sizeof(Instrumento));
                        file.read((char*)&instrumento_, sizeof(Instrumento));
                        
                        if (instrumento_.id != 0 && instrumento_.autorizado == 0) {
                            ConfigBotoes configBotoes;
                            configBotoes.titulo = "Autorizar Instrumento";
                            configBotoes.descricao = "ID: " + dados[instrumento_selecionado.indice_linha][0] + 
                                                    " | Nome: " + dados[instrumento_selecionado.indice_linha][1] + 
                                                    " | Estoque: " + dados[instrumento_selecionado.indice_linha][2];
                            configBotoes.botoes[0].label = "Autorizar";
                            configBotoes.botoes[0].tecla = 'A';
                            configBotoes.botoes[0].valor_retorno = 1;
                            configBotoes.botoes[1].label = "Cancelar";
                            configBotoes.botoes[1].tecla = 'C';
                            configBotoes.botoes[1].valor_retorno = 0;
                            configBotoes.numero_botoes = 2;
                            saida_botoes acao = interface_para_botoes(configBotoes);
                            
                            if(acao.confirmado && acao.valor_retorno == 1) {
                                instrumento_.autorizado = 1;
                                file.seekp((idInst - 1) * sizeof(Instrumento));
                                file.write((char*)&instrumento_, sizeof(Instrumento));
                                file.clear();
                                mostrar_caixa_informacao("SUCESSO", "Instrumento " + string(instrumento_.nome) + " autorizado com sucesso!");
                            }
                        }
                    }
                    break;
                }
                
                case 2: { // Inativar Instrumento - COM TABELA INTERATIVA
                    string dados[100][6];
                    int total = listar_instrumentos_especificos(1, dados); // 1 = Autorizados
                    
                    if (total == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum instrumento autorizado para inativar.");
                        break;
                    }
                    
                    string titulos[6] = {"ID", "Nome", "Estoque", "Autorizado", "Status", "Disp"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Instrumentos para Inativar (Autorizados)";
                    saida_tabela instrumento_selecionado = interface_para_tabela(total, 6, dados_ptr, titulos, 0, configTab);
                    
                    if (instrumento_selecionado.indice_linha != -1) {
                        int idInst = stoi(dados[instrumento_selecionado.indice_linha][0]);
                        file.clear();
                        file.seekg((idInst - 1) * sizeof(Instrumento));
                        file.read((char*)&instrumento_, sizeof(Instrumento));
                        
                        if (instrumento_.id != 0 && instrumento_.ativo == 1) {
                            ConfigBotoes configBotoes;
                            configBotoes.titulo = "Inativar Instrumento";
                            configBotoes.descricao = "ID: " + dados[instrumento_selecionado.indice_linha][0] + 
                                                    " | Nome: " + dados[instrumento_selecionado.indice_linha][1] + 
                                                    " | Status: Ativo";
                            configBotoes.botoes[0].label = "Inativar";
                            configBotoes.botoes[0].tecla = 'I';
                            configBotoes.botoes[0].valor_retorno = 1;
                            configBotoes.botoes[1].label = "Cancelar";
                            configBotoes.botoes[1].tecla = 'C';
                            configBotoes.botoes[1].valor_retorno = 0;
                            configBotoes.numero_botoes = 2;
                            saida_botoes acao = interface_para_botoes(configBotoes);
                            
                            if(acao.confirmado && acao.valor_retorno == 1) {
                                instrumento_.ativo = 0;
                                file.seekp((idInst - 1) * sizeof(Instrumento));
                                file.write((char*)&instrumento_, sizeof(Instrumento));
                                file.clear();
                                mostrar_caixa_informacao("SUCESSO", "Instrumento " + string(instrumento_.nome) + " inativado com sucesso!");
                            }
                        }
                    }
                    break;
                }
                
                case 3: { // Listar Instrumentos
                    string dados[100][6];
                    int contador = 0;
                    
                    file.seekg(0);
                    while (file.read((char*)&instrumento_, sizeof(Instrumento)) && contador < 100) {
                        if (instrumento_.id != 0) {
                            dados[contador][0] = to_string(instrumento_.id);
                            dados[contador][1] = instrumento_.nome;
                            dados[contador][2] = to_string(instrumento_.estoque);
                            dados[contador][3] = instrumento_.autorizado ? "Autorizado" : "Pendente";
                            dados[contador][4] = instrumento_.ativo ? "Ativo" : "Inativo";
                            dados[contador][5] = instrumento_.disponivel ? "Disp" : "Indisp";
                            contador++;
                        }
                    }
                    file.clear();
                    
                    if (contador == 0) {
                        mostrar_caixa_informacao("INFO", "Nenhum instrumento cadastrado!");
                        break;
                    }
                    
                    string titulos[6] = {"ID", "Nome", "Estoque", "Autorizado", "Status", "Disp"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < contador; i++) dados_ptr[i] = dados[i];
                    
                    ConfigTabela configTab;
                    configTab.titulo = "Lista de Instrumentos";
                    saida_tabela selecionada = interface_para_tabela(contador, 6, dados_ptr, titulos, 0, configTab);
                    
                    if (selecionada.indice_linha != -1) {
                        mostrar_caixa_informacao("INFO", "Instrumento selecionado: " + dados[selecionada.indice_linha][1] + 
                                              " | Estoque: " + dados[selecionada.indice_linha][2]);
                    }
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

void abrir_menu_admin(Usuario* usuario) {
    constexpr int qtdOpcoes = 14;
    string opcoes[qtdOpcoes] = {
        "Ativar Creditos Pendentes",
        "Cadastrar Cursos",
        "Consultar Estoque",
        "Consultar Pendencias",
        "Gerenciar Eventos",
        "Gerenciar Instrumentos",
        "Gerenciar Produtos",
        "Gerenciar Usuarios",
        "Realizar Backup",
        "** Relatorio Financeiro",
        "** Relatorio Patrimonial",
        "** Relatorios Academicos",
        "Restaurar Backup",
        "Logout"
    };
    

    
    ConfigMenu config;
    config.titulo = "AREA DO ADMINISTRADOR";
    config.descricao = "Bem-vindo(a), " + string(usuario->nome) + "!";
    
    saida_menu resultado = interface_para_menu(qtdOpcoes, opcoes, config);
    
    switch (resultado.indice_da_opcao) {
        case 0: 
            Lanchonete::ativarCreditosPendentes(); 
            break;
        case 1: 
            mod_ADM::menuCadastroCursos(); 
            break;
        case 2: 
            Lanchonete::consultarEstoque(); 
            break;
        case 3: 
            consultarPendenciasInstrumentos(); 
            break;
        case 4: {
            fstream file;
            openFile(file, "eventos.dat");
            mod_ADM::menuEventos(file);
            file.close();
            break;
        }
        case 5: 
            mod_ADM::menuCadastroInstrumentos(); 
            break;
        case 6: 
            mod_ADM::cadastrarProdutos(); 
            break;
        case 7: 
            mod_ADM::menuGerenciarUsuarios(); 
            break;
        case 8: 
            mod_ADM::realizarBackup(); 
            break;
        case 9: 
            mod_ADM::gerarRelatorioFinanceiro(); 
            break;
        case 10: 
            mod_ADM::gerarRelatorioPatrimonial(); 
            break;
        case 11: 
            mod_ADM::consultarRelatoriosAcademicos(); 
            break;
        case 12: 
            mod_ADM::restaurarBackup(); 
            break;
        case 13: 
            usuario->logado = false; 
            break;
    }
}


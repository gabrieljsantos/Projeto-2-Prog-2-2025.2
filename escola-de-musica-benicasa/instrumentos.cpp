#include "instrumentos.h"
#include "interface_grafica.h"
#include "admin.h"
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// --- Variáveis Globais do Módulo ---
const int MAX_INSTRUMENTOS = 100;
Instrumento listaInstrumentos[MAX_INSTRUMENTOS];
int quantidadeInstrumentos = 0;

const char* ARQUIVO_DADOS = "instrumentos.dat";

// ═══════════════════════════════════════════════════════════════
// TEMA ROSA — Cores globais do módulo de instrumentos
// ═══════════════════════════════════════════════════════════════
static CoresMenu coresRosa()
{
    CoresMenu c;
    c.cor_opcao_selecionada = 10;
    c.cor_opcao             = 11;
    c.cor_descricao         = 12;
    c.cor_instrucao         = 13;
    c.cor_titulo            = 14;
    c.cor_caminho           = 15;
    c.cor_borda             = 16;
    c.cor_paginacao         = 17;
    c.cor_controles         = 18;
    return c;
}

// --- Funções Auxiliares de Arquivo (Internas) ---

// Função para carregar dados do arquivo binário para a memória
void carregarDadosInstrumentos() {
    ifstream file(ARQUIVO_DADOS, ios::binary);
    if (!file.is_open()) return; // Se não existir, não faz nada

    // Lê a quantidade de registros
    file.read((char*)&quantidadeInstrumentos, sizeof(int));

    // Lê o array inteiro
    if (quantidadeInstrumentos > 0) {
        file.read((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// Função para salvar dados da memória para o arquivo binário
void salvarDadosInstrumentos() {
    ofstream file(ARQUIVO_DADOS, ios::binary | ios::trunc);
    if (!file.is_open()) {
        return;
    }

    // Salva a quantidade primeiro
    file.write((char*)&quantidadeInstrumentos, sizeof(int));
    
    // Salva o array
    if (quantidadeInstrumentos > 0) {
        file.write((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// --- Inicialização e Finalização ---

void inicializarInstrumentos() {
    carregarDadosInstrumentos();
}
void finalizarInstrumentos() {
    salvarDadosInstrumentos();
}

// --- Funções de Busca Auxiliares ---

int buscarIndicePorId(int id) {
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        if (listaInstrumentos[i].id == id && listaInstrumentos[i].ativo == 1) {
            return i;
        }
    }
    return -1; // Não encontrado
}

// --- CRUD ---

void cadastrarInstrumento() {
    if (quantidadeInstrumentos >= MAX_INSTRUMENTOS) {
        mostrar_caixa_informacao("Erro", "Limite de instrumentos atingido!");
        return;
    }

    Instrumento novo;
    novo.id = (quantidadeInstrumentos == 0) ? 1 : listaInstrumentos[quantidadeInstrumentos - 1].id + 1;
    novo.ativo = 1;
    novo.autorizado = 1;

    // --- Nome ---
    ConfigEntradaTexto cfgNome;
    cfgNome.titulo       = "Cadastrar Instrumento";
    cfgNome.descricao    = "Preencha os dados do novo instrumento.";
    cfgNome.label        = "Nome do instrumento: ";
    cfgNome.caminho      = "Instrumentos > Cadastrar";
    cfgNome.tipo_entrada = TIPO_TEXTO;
    cfgNome.tamanho_maximo = TAM_NOME - 1;
    cfgNome.cores        = coresRosa();

    saida_entrada_texto resNome = interface_para_entrada_texto(cfgNome);
    if (!resNome.confirmado || resNome.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Cadastro cancelado.");
        return;
    }
    strncpy(novo.nome, resNome.valor.c_str(), TAM_NOME - 1);
    novo.nome[TAM_NOME - 1] = '\0';

    // --- Estoque ---
    ConfigEntradaTexto cfgEstoque;
    cfgEstoque.titulo       = "Cadastrar Instrumento";
    cfgEstoque.descricao    = "Instrumento: " + resNome.valor;
    cfgEstoque.label        = "Quantidade em estoque: ";
    cfgEstoque.caminho      = "Instrumentos > Cadastrar";
    cfgEstoque.tipo_entrada = TIPO_NUMERO;
    cfgEstoque.tamanho_maximo = 5;
    cfgEstoque.cores        = coresRosa();

    saida_entrada_texto resEstoque = interface_para_entrada_texto(cfgEstoque);
    if (!resEstoque.confirmado || resEstoque.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Cadastro cancelado.");
        return;
    }
    novo.estoque = atoi(resEstoque.valor.c_str());
    novo.disponivel = (novo.estoque > 0);
    novo.idAluno = 0;

    listaInstrumentos[quantidadeInstrumentos] = novo;
    quantidadeInstrumentos++;
    salvarDadosInstrumentos();

    mostrar_caixa_informacao("Sucesso", "Instrumento cadastrado com sucesso! ID: " + to_string(novo.id));
}

void listarInstrumentos() {
    if (quantidadeInstrumentos == 0) {
        mostrar_caixa_informacao("Listar Instrumentos", "Nenhum instrumento cadastrado.");
        return;
    }

    // Contar ativos
    int totalAtivos = 0;
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        if (listaInstrumentos[i].ativo == 1) totalAtivos++;
    }
    if (totalAtivos == 0) {
        mostrar_caixa_informacao("Listar Instrumentos", "Nenhum instrumento ativo.");
        return;
    }

    // Montar dados para tabela
    string dados[100][4];
    int contador = 0;
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        Instrumento& it = listaInstrumentos[i];
        if (it.ativo == 1) {
            dados[contador][0] = to_string(it.id);
            dados[contador][1] = it.nome;
            dados[contador][2] = to_string(it.estoque);
            dados[contador][3] = it.disponivel ? "Sim" : "Nao";
            contador++;
        }
    }

    const string* dados_ptr[100];
    for (int i = 0; i < contador; i++)
        dados_ptr[i] = dados[i];

    string titulos[4] = {"ID", "Nome", "Estoque", "Disponivel"};

    int larguras[4] = {6, 25, 10, 12};

    ConfigTabela cfgTab;
    cfgTab.titulo         = "Lista de Instrumentos";
    cfgTab.descricao      = "Todos os instrumentos cadastrados no sistema.";
    cfgTab.caminho        = "Instrumentos > Listar";
    cfgTab.largura_janela = 70;
    cfgTab.linhas_por_janela = 8;
    cfgTab.larguras_colunas  = larguras;
    cfgTab.num_colunas       = 4;
    cfgTab.cores             = coresRosa();

    // Tabela sem retorno útil — apenas visualização
    interface_para_tabela(contador, 4, (const string**)dados_ptr, titulos, 0, cfgTab);
}

void excluirInstrumento() {
    // --- Pedir ID ---
    ConfigEntradaTexto cfgId;
    cfgId.titulo       = "Excluir Instrumento";
    cfgId.descricao    = "Digite o ID do instrumento que deseja excluir.";
    cfgId.label        = "ID do instrumento: ";
    cfgId.caminho      = "Instrumentos > Excluir";
    cfgId.tipo_entrada = TIPO_NUMERO;
    cfgId.tamanho_maximo = 10;
    cfgId.cores        = coresRosa();

    saida_entrada_texto resId = interface_para_entrada_texto(cfgId);
    if (!resId.confirmado || resId.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Exclusao cancelada.");
        return;
    }

    int id = atoi(resId.valor.c_str());
    int idx = buscarIndicePorId(id);
    if (idx != -1) {
        listaInstrumentos[idx].ativo = 0;
        salvarDadosInstrumentos();
        mostrar_caixa_informacao("Sucesso", "Instrumento excluido com sucesso.");
    } else {
        mostrar_caixa_informacao("Erro", "Instrumento nao encontrado.");
    }
}

// --- Editar Instrumento ---

void editarInstrumento() {
    // --- Pedir ID ---
    ConfigEntradaTexto cfgId;
    cfgId.titulo       = "Editar Instrumento";
    cfgId.descricao    = "Digite o ID do instrumento que deseja editar.";
    cfgId.label        = "ID do instrumento: ";
    cfgId.caminho      = "Instrumentos > Editar";
    cfgId.tipo_entrada = TIPO_NUMERO;
    cfgId.tamanho_maximo = 10;
    cfgId.cores        = coresRosa();

    saida_entrada_texto resId = interface_para_entrada_texto(cfgId);
    if (!resId.confirmado || resId.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Edicao cancelada.");
        return;
    }

    int id = atoi(resId.valor.c_str());
    int idx = buscarIndicePorId(id);
    if (idx == -1) {
        mostrar_caixa_informacao("Erro", "Instrumento nao encontrado.");
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];

    // --- Novo Nome ---
    ConfigEntradaTexto cfgNome;
    cfgNome.titulo       = "Editar Instrumento";
    cfgNome.descricao    = "Atual: " + string(inst.nome) + " | Estoque: " + to_string(inst.estoque);
    cfgNome.label        = "Novo nome (ou ESC para manter): ";
    cfgNome.caminho      = "Instrumentos > Editar > Nome";
    cfgNome.tipo_entrada = TIPO_TEXTO;
    cfgNome.tamanho_maximo = TAM_NOME - 1;
    cfgNome.valor_inicial  = inst.nome;
    cfgNome.cores        = coresRosa();

    saida_entrada_texto resNome = interface_para_entrada_texto(cfgNome);
    if (resNome.confirmado && !resNome.valor.empty()) {
        strncpy(inst.nome, resNome.valor.c_str(), TAM_NOME - 1);
        inst.nome[TAM_NOME - 1] = '\0';
    }

    // --- Novo Estoque ---
    ConfigEntradaTexto cfgEstoque;
    cfgEstoque.titulo       = "Editar Instrumento";
    cfgEstoque.descricao    = "Instrumento: " + string(inst.nome) + " | Estoque atual: " + to_string(inst.estoque);
    cfgEstoque.label        = "Novo estoque (ou ESC para manter): ";
    cfgEstoque.caminho      = "Instrumentos > Editar > Estoque";
    cfgEstoque.tipo_entrada = TIPO_NUMERO;
    cfgEstoque.tamanho_maximo = 5;
    cfgEstoque.valor_inicial  = to_string(inst.estoque);
    cfgEstoque.cores        = coresRosa();

    saida_entrada_texto resEstoque = interface_para_entrada_texto(cfgEstoque);
    if (resEstoque.confirmado && !resEstoque.valor.empty()) {
        int novoEstoque = atoi(resEstoque.valor.c_str());
        if (novoEstoque >= 0) {
            inst.estoque = novoEstoque;
            inst.disponivel = (inst.estoque > 0);
        }
    }

    salvarDadosInstrumentos();
    mostrar_caixa_informacao("Sucesso", "Instrumento atualizado com sucesso!");
}

// --- Salvar Empréstimo ---

void salvarEmprestimo(Emprestimo emp) {
    std::ofstream file("emprestimos.dat", std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        return;
    }
    file.write((char*)&emp, sizeof(Emprestimo));
    file.close();
}

// --- Lógica de Empréstimo ---

void realizarEmprestimo() {
    // --- ID do Instrumento ---
    ConfigEntradaTexto cfgInst;
    cfgInst.titulo       = "Emprestimo de Instrumento";
    cfgInst.descricao    = "Informe o instrumento a ser emprestado.";
    cfgInst.label        = "ID do instrumento: ";
    cfgInst.caminho      = "Instrumentos > Emprestimo";
    cfgInst.tipo_entrada = TIPO_NUMERO;
    cfgInst.tamanho_maximo = 10;
    cfgInst.cores        = coresRosa();

    saida_entrada_texto resInst = interface_para_entrada_texto(cfgInst);
    if (!resInst.confirmado || resInst.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Emprestimo cancelado.");
        return;
    }

    int idInstrumento = atoi(resInst.valor.c_str());

    // 1. Validar Instrumento
    int idx = buscarIndicePorId(idInstrumento);
    if (idx == -1) {
        mostrar_caixa_informacao("Erro", "Instrumento nao encontrado.");
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];
    if (inst.estoque <= 0) {
        mostrar_caixa_informacao("Erro", "Estoque esgotado para: " + string(inst.nome));
        return;
    }

    // --- ID do Aluno ---
    ConfigEntradaTexto cfgAluno;
    cfgAluno.titulo       = "Emprestimo de Instrumento";
    cfgAluno.descricao    = "Instrumento: " + string(inst.nome) + " (Estoque: " + to_string(inst.estoque) + ")";
    cfgAluno.label        = "ID do aluno: ";
    cfgAluno.caminho      = "Instrumentos > Emprestimo > Aluno";
    cfgAluno.tipo_entrada = TIPO_NUMERO;
    cfgAluno.tamanho_maximo = 10;
    cfgAluno.cores        = coresRosa();

    saida_entrada_texto resAluno = interface_para_entrada_texto(cfgAluno);
    if (!resAluno.confirmado || resAluno.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Emprestimo cancelado.");
        return;
    }

    int idAluno = atoi(resAluno.valor.c_str());

    // 2. Validar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        mostrar_caixa_informacao("Erro", "Aluno nao encontrado no sistema.");
        return;
    }

    // 3. Regra: Aluno já tem instrumento?
    if (aluno.idInstrumento != 0) {
        mostrar_caixa_informacao("Erro", "Este aluno ja possui um instrumento emprestado.");
        return;
    }

    // 4. Executar Empréstimo
    inst.estoque--;
    if (inst.estoque == 0) inst.disponivel = false;
    inst.idAluno = idAluno;

    aluno.idInstrumento = inst.id;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();

    Emprestimo emp;
    std::fstream fileEmprestimo("emprestimos.dat", std::ios::in | std::ios::out | std::ios::binary);
    emp.idEmprestimo = gerarNovoId(fileEmprestimo, sizeof(Emprestimo));
    fileEmprestimo.close();
    emp.idAluno = aluno.base.id;
    strcpy(emp.nome_Alu, aluno.base.nome);
    emp.idInstrumento = inst.id;
    strcpy(emp.nome_In, inst.nome);

    time_t t = time(NULL);
    tm *tmPtr = localtime(&t);
    sprintf(emp.dataEmprestimo, "%02d/%02d/%04d",
            tmPtr->tm_mday, tmPtr->tm_mon + 1, tmPtr->tm_year + 1900);
    sprintf(emp.dataPrevista, "%02d/%02d/%04d",
            tmPtr->tm_mday + 7, tmPtr->tm_mon + 1, tmPtr->tm_year + 1900);

    salvarEmprestimo(emp);

    mostrar_caixa_informacao("Sucesso", "Emprestimo realizado! " + string(inst.nome) + " para " + string(aluno.base.nome));
}

void realizarDevolucao() {
    // --- ID do Aluno ---
    ConfigEntradaTexto cfgAluno;
    cfgAluno.titulo       = "Devolucao de Instrumento";
    cfgAluno.descricao    = "Informe o aluno que esta devolvendo o instrumento.";
    cfgAluno.label        = "ID do aluno: ";
    cfgAluno.caminho      = "Instrumentos > Devolucao";
    cfgAluno.tipo_entrada = TIPO_NUMERO;
    cfgAluno.tamanho_maximo = 10;
    cfgAluno.cores        = coresRosa();

    saida_entrada_texto resAluno = interface_para_entrada_texto(cfgAluno);
    if (!resAluno.confirmado || resAluno.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Devolucao cancelada.");
        return;
    }

    int idAluno = atoi(resAluno.valor.c_str());

    // 1. Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        mostrar_caixa_informacao("Erro", "Aluno nao encontrado.");
        return;
    }

    if (aluno.idInstrumento == 0) {
        mostrar_caixa_informacao("Erro", "Este aluno nao possui instrumento para devolver.");
        return;
    }

    // 2. Encontrar Instrumento
    int idx = buscarIndicePorId(aluno.idInstrumento);
    if (idx == -1) {
        mostrar_caixa_informacao("Erro", "Erro critico: Instrumento nao encontrado.");
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];

    // 3. Atualizar Instrumento
    inst.estoque++;
    inst.disponivel = true;
    inst.idAluno = 0;

    // 4. Atualizar Aluno
    aluno.idInstrumento = 0;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();

    // Zerar registro no arquivo de empréstimos
    std::fstream file("emprestimos.dat",
                      std::ios::binary | std::ios::in | std::ios::out);

    if (file.is_open()) {
        Emprestimo emp;
        while (file.read((char*)&emp, sizeof(Emprestimo))) {
            if (emp.idAluno == idAluno) {
                file.seekp(-sizeof(Emprestimo), std::ios::cur);
                Emprestimo vazio = {0};
                file.write((char*)&vazio, sizeof(Emprestimo));
                break;
            }
        }
        file.close();
    }

    mostrar_caixa_informacao("Sucesso", "Devolucao realizada! " + string(inst.nome) + " devolvido por " + string(aluno.base.nome));
}

// --- Funções com ID do Aluno (para menu do Aluno) ---

void listarInstrumentosDisponiveis() {
    if (quantidadeInstrumentos == 0) {
        mostrar_caixa_informacao("Instrumentos Disponiveis", "Nenhum instrumento disponivel.");
        return;
    }

    // Contar ativos e disponíveis
    int totalDisponiveis = 0;
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        if (listaInstrumentos[i].ativo == 1 && listaInstrumentos[i].disponivel && listaInstrumentos[i].estoque > 0) {
            totalDisponiveis++;
        }
    }
    if (totalDisponiveis == 0) {
        mostrar_caixa_informacao("Instrumentos Disponiveis", "Nenhum instrumento disponivel no momento.");
        return;
    }

    // Montar dados para tabela
    string dados[100][3];
    int contador = 0;
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        Instrumento& it = listaInstrumentos[i];
        if (it.ativo == 1 && it.disponivel && it.estoque > 0) {
            dados[contador][0] = to_string(it.id);
            dados[contador][1] = it.nome;
            dados[contador][2] = to_string(it.estoque);
            contador++;
        }
    }

    const string* dados_ptr[100];
    for (int i = 0; i < contador; i++)
        dados_ptr[i] = dados[i];

    string titulos[3] = {"ID", "Nome", "Disponivel"};
    int larguras[3] = {6, 25, 12};

    ConfigTabela cfgTab;
    cfgTab.titulo         = "Instrumentos Disponiveis";
    cfgTab.descricao      = "Instrumentos que voce pode solicitar emprestado.";
    cfgTab.caminho        = "Minha Area > Instrumentos";
    cfgTab.largura_janela = 70;
    cfgTab.linhas_por_janela = 8;
    cfgTab.larguras_colunas  = larguras;
    cfgTab.num_colunas       = 3;
    cfgTab.cores             = coresRosa();

    interface_para_tabela(contador, 3, (const string**)dados_ptr, titulos, 0, cfgTab);
}

void realizarEmprestimo(int idAluno) {
    // --- Validar Aluno ---
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        mostrar_caixa_informacao("Erro", "Aluno nao encontrado.");
        return;
    }

    // --- ID do Instrumento ---
    ConfigEntradaTexto cfgInst;
    cfgInst.titulo       = "Emprestimo de Instrumento";
    cfgInst.descricao    = "Informe o ID do instrumento desejado.";
    cfgInst.label        = "ID do instrumento: ";
    cfgInst.caminho      = "Minha Area > Instrumentos > Emprestimo";
    cfgInst.tipo_entrada = TIPO_NUMERO;
    cfgInst.tamanho_maximo = 10;
    cfgInst.cores        = coresRosa();

    saida_entrada_texto resInst = interface_para_entrada_texto(cfgInst);
    if (!resInst.confirmado || resInst.valor.empty()) {
        mostrar_caixa_informacao("Cancelado", "Emprestimo cancelado.");
        return;
    }

    int idInstrumento = atoi(resInst.valor.c_str());

    // 1. Validar Instrumento
    int idx = buscarIndicePorId(idInstrumento);
    if (idx == -1) {
        mostrar_caixa_informacao("Erro", "Instrumento nao encontrado.");
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];
    if (inst.estoque <= 0) {
        mostrar_caixa_informacao("Erro", "Estoque esgotado para: " + string(inst.nome));
        return;
    }

    // 2. Registrar Empréstimo
    inst.estoque--;
    inst.disponivel = (inst.estoque > 0);
    aluno.idInstrumento = idInstrumento;

    // Salvar alterações
    Login_mat::atualizar(idAluno, aluno);
    salvarDadosInstrumentos();

    // Registrar em arquivo de empréstimos
    std::fstream file("emprestimos.dat", std::ios::binary | std::ios::app);
    if (file.is_open()) {
        Emprestimo emp;
        emp.idAluno = idAluno;
        emp.idInstrumento = idInstrumento;
        strcpy(emp.nome_Alu, aluno.base.nome);
        strcpy(emp.nome_In, inst.nome);
        strcpy(emp.dataEmprestimo, "00/00/0000");
        strcpy(emp.dataPrevista, "00/00/0000");
        file.write((char*)&emp, sizeof(Emprestimo));
        file.close();
    }

    mostrar_caixa_informacao("Sucesso", "Emprestimo realizado com sucesso! Instrumento: " + string(inst.nome));
}

void realizarDevolucao(int idAluno) {
    // 1. Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        mostrar_caixa_informacao("Erro", "Aluno nao encontrado.");
        return;
    }

    if (aluno.idInstrumento == 0) {
        mostrar_caixa_informacao("Erro", "Voce nao possui instrumento para devolver.");
        return;
    }

    // 2. Encontrar Instrumento
    int idInstGuardado = aluno.idInstrumento; // Guardar ID antes de modificar
    int idx = buscarIndicePorId(aluno.idInstrumento);
    if (idx == -1) {
        mostrar_caixa_informacao("Erro", "Erro critico: Instrumento nao encontrado.");
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];

    // 3. Atualizar Instrumento
    inst.estoque++;
    inst.disponivel = true;

    // 4. Atualizar Aluno
    aluno.idInstrumento = 0;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();

    // 5. Zerar registro no arquivo de empréstimos
    std::fstream file("emprestimos.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (file.is_open()) {
        Emprestimo emp;
        while (file.read((char*)&emp, sizeof(Emprestimo))) {
            if (emp.idAluno == idAluno && emp.idInstrumento == idInstGuardado) {
                file.seekp(-sizeof(Emprestimo), std::ios::cur);
                Emprestimo vazio = {0};
                file.write((char*)&vazio, sizeof(Emprestimo));
                break;
            }
        }
        file.close();
    }

    mostrar_caixa_informacao("Sucesso", "Devolucao realizada! Instrumento: " + string(inst.nome));
}

void listarMeusEmprestimos(int idAluno) {
    // 1. Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        mostrar_caixa_informacao("Erro", "Aluno nao encontrado.");
        return;
    }

    // 2. Listar empréstimos
    std::ifstream file("emprestimos.dat", std::ios::binary);
    if (!file.is_open()) {
        mostrar_caixa_informacao("Meus Emprestimos", "Nenhum emprestimo registrado.");
        return;
    }

    string dados[100][3];
    int contador = 0;
    Emprestimo emp;

    while (file.read((char*)&emp, sizeof(Emprestimo))) {
        if (emp.idAluno == idAluno && emp.idAluno != -1) {
            dados[contador][0] = to_string(emp.idInstrumento);
            dados[contador][1] = emp.nome_In;
            dados[contador][2] = emp.dataEmprestimo;
            contador++;
        }
    }
    file.close();

    if (contador == 0) {
        mostrar_caixa_informacao("Meus Emprestimos", "Voce nao possui instrumentos emprestados.");
        return;
    }

    const string* dados_ptr[100];
    for (int i = 0; i < contador; i++)
        dados_ptr[i] = dados[i];

    string titulos[3] = {"ID", "Instrumento", "Data Emprestimo"};
    int larguras[3] = {6, 25, 20};

    ConfigTabela cfgTab;
    cfgTab.titulo         = "Meus Emprestimos";
    cfgTab.descricao      = "Instrumentos que voce possui emprestados.";
    cfgTab.caminho        = "Minha Area > Instrumentos";
    cfgTab.largura_janela = 70;
    cfgTab.linhas_por_janela = 8;
    cfgTab.larguras_colunas  = larguras;
    cfgTab.num_colunas       = 3;
    cfgTab.cores             = coresRosa();

    interface_para_tabela(contador, 3, (const string**)dados_ptr, titulos, 0, cfgTab);
}

// --- Menu Principal de Instrumentos ---

void menuInstrumentos() {
    bool continuar = true;

    while (continuar) {
        constexpr int TOTAL_OPCOES = 7;
        string opcoes[TOTAL_OPCOES] = {
            "Cadastrar Instrumento",
            "Editar Instrumento",
            "Realizar Emprestimo",
            "Listar Instrumentos",
            "Excluir Instrumento",
            "Realizar Devolucao",
            "Voltar"
        };

        ConfigMenu config;
        config.titulo   = "Instrumentos";
        config.descricao = "Gerencie os instrumentos da escola.";
        config.caminho   = "Instrumentos";
        config.cores     = coresRosa();

        saida_menu resultado = interface_para_menu(TOTAL_OPCOES, opcoes, config);

        switch (resultado.indice_da_opcao) {
            case 0: cadastrarInstrumento();  break;
            case 1: editarInstrumento();     break;
            case 2: realizarEmprestimo();    break;
            case 3: listarInstrumentos();    break;
            case 4: excluirInstrumento();    break;
            case 5: realizarDevolucao();     break;
            case 6:
            default:
                continuar = false;
                break;
        }
    }
}
#include <iostream>
#include <fstream>
#include "lanchonete.h"
#include "interface_grafica.h"
#include "headers.h"
#include <limits>
#include <cstring>
#include <string>

using namespace std;

void limparBuffer(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool converterInt(string valor, int &saida){
    try{
        size_t idx = 0;
        saida = stoi(valor, &idx);
        return idx == valor.size();
    }catch(...){
        return false;
    }
}

bool converterDouble(string valor, double &saida){
    for (int i = 0; i < (int)valor.size(); i++) {
        if (valor[i] == ',') valor[i] = '.';
    }

    try{
        size_t idx = 0;
        saida = stod(valor, &idx);
        return idx == valor.size();
    }catch(...){
        return false;
    }
}

namespace Lanchonete {
    int proximoIdOperacao(Credito lista[MAX_CREDITOS]){
        int maior = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_opera > maior) maior = lista[i].id_opera;
        }
        return maior + 1;
    }

    double saldoUsuarioRec(Credito lista[MAX_CREDITOS], int idUsuario, int i){
        if (i == MAX_CREDITOS) return 0;

        double soma = 0;
        if (lista[i].id_user == idUsuario && lista[i].realizado) {
            soma = lista[i].saldo;
        }

        return soma + saldoUsuarioRec(lista, idUsuario, i + 1);
    }

    double saldoUsuario(Credito lista[MAX_CREDITOS], int idUsuario){
        return saldoUsuarioRec(lista, idUsuario, 0);
    }

    int acharCreditoPorOperacao(Credito lista[MAX_CREDITOS], int idOpera) {
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_opera == idOpera && lista[i].id_opera != 0) return i;
        }
        return MAX_CREDITOS;
    }

    saida_menu exibirMenuLanchoneteAdmin(){
        const int qtdOpcoes = 6;
        string opcoes[qtdOpcoes] = {
            "Adicionar produto",
            "Remover produto",
            "Visualizar produtos",
            "Consultar estoque",
            "Alterar preco",
            "Ativar creditos pendentes"
        };

        ConfigMenu config;
        config.titulo = "LANCHONETE";
        config.caminho = "ADMIN -> LANCHONETE";
        config.descricao = "Gerenciamento da lanchonete";
        return interface_para_menu(qtdOpcoes, opcoes, config);
    }

    saida_entrada_texto entradaNumero(string titulo, string descricao, string caminho, string label, int tam){
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.descricao = descricao;
        config.caminho = caminho;
        config.label = label;
        config.tipo_entrada = TIPO_NUMERO;
        config.tamanho_maximo = tam;
        return interface_para_entrada_texto(config);
    }

    saida_entrada_texto entradaTexto(string titulo, string descricao, string caminho, string label, int tam){
        ConfigEntradaTexto config;
        config.titulo = titulo;
        config.descricao = descricao;
        config.caminho = caminho;
        config.label = label;
        config.tipo_entrada = TIPO_TEXTO;
        config.tamanho_maximo = tam;
        return interface_para_entrada_texto(config);
    }

    int selecionarProduto(Produto lista[MAX_PRODUTOS], string titulo, string caminho, bool mostrarPreco, bool mostrarEstoque){
        int qtd = 0;
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) qtd++;
        }

        if (qtd == 0) return 0;

        string *opcoes = new string[qtd];
        int *ids = new int[qtd];

        int k = 0;
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) {
                string linha = to_string(lista[i].id) + " - " + string(lista[i].nome);
                if (mostrarPreco) linha += " (R$ " + to_string(lista[i].preco) + ")";
                if (mostrarEstoque) linha += " [Estoque: " + to_string(lista[i].estoque) + "]";
                opcoes[k] = linha;
                ids[k] = lista[i].id;
                k++;
            }
        }

        ConfigMenu config;
        config.titulo = titulo;
        config.descricao = "Selecione o produto";
        config.caminho = caminho;

        saida_menu saidaMenu = interface_para_menu(qtd, opcoes, config);

        int idRetorno = 0;
        if (saidaMenu.indice_da_opcao >= 0) {
            idRetorno = ids[saidaMenu.indice_da_opcao];
        }

        delete[] opcoes;
        delete[] ids;

        return idRetorno;
    }

    int selecionarOperacaoPendente(Credito creditos[MAX_CREDITOS]){
        int qtd = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (creditos[i].id_user != 0 && creditos[i].realizado == false) qtd++;
        }

        if (qtd == 0) return 0;

        string *opcoes = new string[qtd];
        int *ops = new int[qtd];

        int k = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (creditos[i].id_user != 0 && creditos[i].realizado == false) {
                opcoes[k] = "Operacao: " + to_string(creditos[i].id_opera) +
                " | ID: " + to_string(creditos[i].id_user) +
                " | Valor: " + to_string(creditos[i].saldo);
                ops[k] = creditos[i].id_opera;
                k++;
            }
        }

        ConfigMenu config;
        config.titulo = "CREDITOS PENDENTES";
        config.descricao = "Selecione a operacao para ativar";
        config.caminho = "LANCHONETE -> CREDITOS";

        saida_menu saidaMenu = interface_para_menu(qtd, opcoes, config);

        int idOpera = 0;
        if (saidaMenu.indice_da_opcao >= 0) {
            idOpera = ops[saidaMenu.indice_da_opcao];
        }

        delete[] opcoes;
        delete[] ops;

        return idOpera;
    }

    void menuCadastroProdutos(){
        while (true) {
            saida_menu saidaMenu = exibirMenuLanchoneteAdmin();
            if (saidaMenu.indice_da_opcao < 0) return;

            switch (saidaMenu.indice_da_opcao) {
                case 0:
                    adicionarProduto();
                    break;
                case 1:
                    removerProduto();
                    break;
                case 2:
                    visualizarProdutos();
                    break;
                case 3:
                    consultarEstoque();
                    break;
                case 4:
                    alterarPrecoProduto();
                    break;
                case 5:
                    ativarCreditosPendentes();
                    break;
            }
        }
    }

    bool bancoExisteProdutos() {
        ifstream arquivo(BANCO_PRODUTOS, ios::binary);
        bool existe = arquivo.is_open();
        arquivo.close();
        return existe;
    }

    bool bancoExisteCreditos() {
        ifstream arquivo(BANCO_CREDITOS, ios::binary);
        bool existe = arquivo.is_open();
        arquivo.close();
        return existe;
    }

    void lerTodosProdutos(Produto lista[MAX_PRODUTOS]) {
        ifstream arquivo(BANCO_PRODUTOS, ios::binary);
        Produto vazio = {};

        if (!arquivo.is_open()) {
            for (int i = 0; i < MAX_PRODUTOS; i++) lista[i] = vazio;
            return;
        }

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            Produto produto;
            if (arquivo.read((char*)&produto, sizeof(Produto))) {
                lista[i] = produto;
            } else {
                lista[i] = vazio;
            }
        }

        arquivo.close();
    }

    bool escreverTodosProdutos(Produto lista[MAX_PRODUTOS]) {
        ofstream arquivo(BANCO_PRODUTOS, ios::binary | ios::trunc);
        if (!arquivo.is_open()) return false;

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            arquivo.write((char*)&lista[i], sizeof(Produto));
        }

        arquivo.close();
        return true;
    }

    void lerTodosCreditos(Credito lista[MAX_CREDITOS]) {
        ifstream arquivo(BANCO_CREDITOS, ios::binary);
        Credito vazio = {};

        if (!arquivo.is_open()) {
            for (int i = 0; i < MAX_CREDITOS; i++) {
                lista[i] = vazio;
            }
            return;
        }

        for (int i = 0; i < MAX_CREDITOS; i++) {
            Credito credito;
            if (arquivo.read((char*)&credito, sizeof(Credito))) {
                lista[i] = credito;
            } else {
                lista[i] = vazio;
            }
        }

        arquivo.close();
    }

    bool escreverTodosCreditos(Credito lista[MAX_CREDITOS]) {
        ofstream arquivo(BANCO_CREDITOS, ios::binary | ios::trunc);
        if (!arquivo.is_open()) return false;

        for (int i = 0; i < MAX_CREDITOS; i++) {
            arquivo.write((char*)&lista[i], sizeof(Credito));
        }

        arquivo.close();
        return true;
    }

    int acharVagaProduto(Produto lista[MAX_PRODUTOS]) {
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 0 || lista[i].id == 0) return i;
        }
        return MAX_PRODUTOS;
    }

    int acharProdutoPorId(Produto lista[MAX_PRODUTOS], int idProduto) {
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id == idProduto) return i;
        }
        return MAX_PRODUTOS;
    }

    int acharVagaCredito(Credito lista[MAX_CREDITOS]) {
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_user == 0) return i;
        }
        return MAX_CREDITOS;
    }

    bool bancoDeDados() {
        if (!bancoExisteProdutos()) {
            ofstream arquivo(BANCO_PRODUTOS, ios::binary | ios::trunc);
            if (!arquivo.is_open()) return false;

            Produto produto_vazio = {};
            for (int i = 0; i < MAX_PRODUTOS; i++) {
                arquivo.write((char*)&produto_vazio, sizeof(Produto));
            }
            arquivo.close();
        }

        if (!bancoExisteCreditos()) {
            ofstream arquivo(BANCO_CREDITOS, ios::binary | ios::trunc);
            if (!arquivo.is_open()) return false;

            Credito credito_vazio = {};
            for (int i = 0; i < MAX_CREDITOS; i++) {
                arquivo.write((char*)&credito_vazio, sizeof(Credito));
            }
            arquivo.close();
        }

        return true;
    }

    bool verificarAtividade(Usuario pessoa) {
        return pessoa.ativo == 1;
    }

    void solicitarCreditosUsuario() {
        saida_entrada_texto saidaId = entradaNumero("SOLICITAR CREDITOS", "Digite o ID", "LANCHONETE -> CREDITOS", "ID: ", 10);
        if (!saidaId.confirmado) return;

        int idUsuario;
        if (!converterInt(saidaId.valor, idUsuario) || idUsuario <= 0) {
            mostrar_caixa_informacao("ERRO", "ID invalido");
            return;
        }

        saida_entrada_texto saidaValor = entradaNumero("SOLICITAR CREDITOS", "Digite o valor", "LANCHONETE -> CREDITOS", "Valor: ", 12);
        if (!saidaValor.confirmado) return;

        double valor;
        if (!converterDouble(saidaValor.valor, valor) || valor <= 0) {
            mostrar_caixa_informacao("ERRO", "Valor invalido");
            return;
        }

        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            mostrar_caixa_informacao("ERRO", "Nao ha espaco para novas solicitacoes");
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].saldo = valor;
        creditos[vaga].realizado = false;

        if (!escreverTodosCreditos(creditos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar solicitacao");
            delete[] creditos;
            return;
        }

        mostrar_caixa_informacao("OK", "Solicitacao enviada");
        delete[] creditos;
    }

    void solicitarCreditosUsuario(int idUsuario) {
        saida_entrada_texto saidaValor = entradaNumero("SOLICITAR CREDITOS", "Digite o valor", "LANCHONETE -> CREDITOS", "Valor: ", 12);
        if (!saidaValor.confirmado) return;

        double valor;
        if (!converterDouble(saidaValor.valor, valor) || valor <= 0) {
            mostrar_caixa_informacao("ERRO", "Valor invalido");
            return;
        }

        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            mostrar_caixa_informacao("ERRO", "Nao ha espaco para novas solicitacoes");
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].saldo = valor;
        creditos[vaga].realizado = false;

        if (!escreverTodosCreditos(creditos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar solicitacao");
            delete[] creditos;
            return;
        }

        mostrar_caixa_informacao("OK", "Solicitacao enviada");
        delete[] creditos;
    }

    void ativarCreditosPendentes() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int idOpera = selecionarOperacaoPendente(creditos);
        if (idOpera == 0) {
            mostrar_caixa_informacao("CREDITOS", "Nenhum credito pendente");
            delete[] creditos;
            return;
        }

        int posicao = acharCreditoPorOperacao(creditos, idOpera);
        if (posicao == MAX_CREDITOS || creditos[posicao].realizado) {
            mostrar_caixa_informacao("ERRO", "Operacao nao encontrada");
            delete[] creditos;
            return;
        }

        ConfigBotoes cfg;
        cfg.titulo = "CONFIRMAR";
        cfg.descricao = "Ativar este credito?";
        cfg.pergunta = "Confirmar: ";
        cfg.numero_botoes = 2;
        cfg.botoes[0] = {'S', "Sim", 1};
        cfg.botoes[1] = {'N', "Nao", 0};

        saida_botoes confirm = interface_para_botoes(cfg);
        if (!confirm.confirmado || confirm.valor_retorno != 1) {
            delete[] creditos;
            return;
        }

        creditos[posicao].realizado = true;

        if (!escreverTodosCreditos(creditos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar");
            delete[] creditos;
            return;
        }

        mostrar_caixa_informacao("OK", "Credito ativado");
        delete[] creditos;
    }

    void adicionarProduto() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        saida_entrada_texto saidaId = entradaNumero("ADICIONAR PRODUTO", "Digite o ID do produto", "LANCHONETE -> PRODUTOS", "ID: ", 10);
        if (!saidaId.confirmado) return;

        int id;
        if (!converterInt(saidaId.valor, id) || id <= 0) {
            mostrar_caixa_informacao("ERRO", "ID invalido");
            return;
        }

        saida_entrada_texto saidaNome = entradaTexto("ADICIONAR PRODUTO", "Digite o nome do produto", "LANCHONETE -> PRODUTOS", "Nome: ", 50);
        if (!saidaNome.confirmado) return;

        saida_entrada_texto saidaPreco = entradaNumero("ADICIONAR PRODUTO", "Digite o preco", "LANCHONETE -> PRODUTOS", "Preco: ", 12);
        if (!saidaPreco.confirmado) return;

        saida_entrada_texto saidaEstoque = entradaNumero("ADICIONAR PRODUTO", "Digite o estoque", "LANCHONETE -> PRODUTOS", "Estoque: ", 10);
        if (!saidaEstoque.confirmado) return;

        double preco;
        int estoque;

        if (!converterDouble(saidaPreco.valor, preco) || preco <= 0) {
            mostrar_caixa_informacao("ERRO", "Preco invalido");
            return;
        }

        if (!converterInt(saidaEstoque.valor, estoque) || estoque < 0) {
            mostrar_caixa_informacao("ERRO", "Estoque invalido");
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        if (acharProdutoPorId(lista, id) != MAX_PRODUTOS) {
            mostrar_caixa_informacao("ERRO", "Ja existe produto com esse ID");
            delete[] lista;
            return;
        }

        Produto novo;
        novo.id = id;
        novo.ativo = 1;
        novo.preco = preco;
        novo.estoque = estoque;
        strncpy(novo.nome, saidaNome.valor.c_str(), 49);
        novo.nome[49] = '\0';

        int posicaoVagaProduto = acharVagaProduto(lista);
        if (posicaoVagaProduto == MAX_PRODUTOS) {
            mostrar_caixa_informacao("ERRO", "Nao ha espaco para mais produtos");
            delete[] lista;
            return;
        }

        lista[posicaoVagaProduto] = novo;

        if (!escreverTodosProdutos(lista)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar produto");
            delete[] lista;
            return;
        }

        mostrar_caixa_informacao("OK", "Produto salvo com sucesso");
        delete[] lista;
    }

    void alterarPrecoProduto() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int idProduto = selecionarProduto(lista, "ALTERAR PRECO", "LANCHONETE -> PRODUTOS -> ALTERAR PRECO", true, false);
        if (idProduto == 0) {
            mostrar_caixa_informacao("PRODUTOS", "Nenhum produto cadastrado");
            delete[] lista;
            return;
        }

        int posicaoProduto = acharProdutoPorId(lista, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            mostrar_caixa_informacao("ERRO", "Produto nao encontrado");
            delete[] lista;
            return;
        }

        saida_entrada_texto saidaPreco = entradaNumero("ALTERAR PRECO", "Digite o novo preco", "LANCHONETE -> PRODUTOS -> ALTERAR PRECO", "Novo preco: ", 12);
        if (!saidaPreco.confirmado) {
            delete[] lista;
            return;
        }

        double novoPreco;
        if (!converterDouble(saidaPreco.valor, novoPreco) || novoPreco <= 0) {
            mostrar_caixa_informacao("ERRO", "Preco invalido");
            delete[] lista;
            return;
        }

        lista[posicaoProduto].preco = novoPreco;

        if (!escreverTodosProdutos(lista)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar");
            delete[] lista;
            return;
        }

        mostrar_caixa_informacao("OK", "Preco alterado");
        delete[] lista;
    }

    void removerProduto() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int idProduto = selecionarProduto(lista, "REMOVER PRODUTO", "LANCHONETE -> PRODUTOS -> REMOVER", false, false);
        if (idProduto == 0) {
            mostrar_caixa_informacao("PRODUTOS", "Nenhum produto cadastrado");
            delete[] lista;
            return;
        }

        int posicaoProduto = acharProdutoPorId(lista, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            mostrar_caixa_informacao("ERRO", "Produto nao encontrado");
            delete[] lista;
            return;
        }

        ConfigBotoes cfg;
        cfg.titulo = "CONFIRMAR";
        cfg.descricao = "Remover este produto?";
        cfg.pergunta = "Confirmar: ";
        cfg.numero_botoes = 2;
        cfg.botoes[0] = {'S', "Sim", 1};
        cfg.botoes[1] = {'N', "Nao", 0};

        saida_botoes confirm = interface_para_botoes(cfg);
        if (!confirm.confirmado || confirm.valor_retorno != 1) {
            delete[] lista;
            return;
        }

        lista[posicaoProduto].ativo = 0;

        if (!escreverTodosProdutos(lista)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar");
            delete[] lista;
            return;
        }

        mostrar_caixa_informacao("OK", "Produto removido");
        delete[] lista;
    }

    void visualizarProdutos() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        string texto = "";
        int temProduto = 0;

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) {
                temProduto = 1;
                texto += "ID: " + to_string(lista[i].id) + "\n";
                texto += "Nome: " + string(lista[i].nome) + "\n";
                texto += "Preco: " + to_string(lista[i].preco) + "\n";
                texto += "Estoque: " + to_string(lista[i].estoque) + "\n\n";
            }
        }

        if (!temProduto) {
            mostrar_caixa_informacao("PRODUTOS", "Nenhum produto cadastrado");
            delete[] lista;
            return;
        }

        ConfigTexto config;
        config.titulo = "PRODUTOS";
        mostrar_texto(texto, config);

        delete[] lista;
    }

    void consultarEstoque() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        string texto = "";
        int temProduto = 0;

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) {
                temProduto = 1;
                texto += "ID: " + to_string(lista[i].id) +
                " | Nome: " + string(lista[i].nome) +
                " | Estoque: " + to_string(lista[i].estoque) + "\n";
            }
        }

        if (!temProduto) {
            mostrar_caixa_informacao("ESTOQUE", "Nenhum produto cadastrado");
            delete[] lista;
            return;
        }

        ConfigTexto config;
        config.titulo = "ESTOQUE";
        mostrar_texto(texto, config);

        delete[] lista;
    }

    void consultarSaldo(int idUsuario) {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        string msg = "Saldo: " + to_string(saldoUsuario(creditos, idUsuario));
        mostrar_caixa_informacao("SALDO", msg);

        delete[] creditos;
    }

    void removerCredito() {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        saida_entrada_texto saidaId = entradaNumero("REMOVER CREDITO", "Digite o ID do usuario", "LANCHONETE -> CREDITOS", "ID: ", 10);
        if (!saidaId.confirmado) return;

        int idUsuario;
        if (!converterInt(saidaId.valor, idUsuario) || idUsuario <= 0) {
            mostrar_caixa_informacao("ERRO", "ID invalido");
            return;
        }

        saida_entrada_texto saidaValor = entradaNumero("REMOVER CREDITO", "Digite o valor", "LANCHONETE -> CREDITOS", "Valor: ", 12);
        if (!saidaValor.confirmado) return;

        double valor;
        if (!converterDouble(saidaValor.valor, valor) || valor <= 0) {
            mostrar_caixa_informacao("ERRO", "Valor invalido");
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        double saldoAtual = saldoUsuario(creditos, idUsuario);
        if (saldoAtual < valor) {
            mostrar_caixa_informacao("ERRO", "Saldo insuficiente");
            delete[] creditos;
            return;
        }

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            mostrar_caixa_informacao("ERRO", "Nao ha espaco para registrar operacao");
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].saldo = -valor;
        creditos[vaga].realizado = true;

        if (!escreverTodosCreditos(creditos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar");
            delete[] creditos;
            return;
        }

        mostrar_caixa_informacao("OK", "Credito removido");
        delete[] creditos;
    }

    void realizarCompra(int idUsuario) {
        if (!bancoDeDados()) {
            mostrar_caixa_informacao("ERRO", "Erro ao abrir o banco");
            return;
        }

        Produto *produtos = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(produtos);

        int idProduto = selecionarProduto(produtos, "COMPRAR", "LANCHONETE -> COMPRA", true, true);
        if (idProduto == 0) {
            mostrar_caixa_informacao("PRODUTOS", "Nenhum produto cadastrado");
            delete[] produtos;
            return;
        }

        saida_entrada_texto saidaQtd = entradaNumero("COMPRAR", "Digite a quantidade", "LANCHONETE -> COMPRA", "Quantidade: ", 10);
        if (!saidaQtd.confirmado) {
            delete[] produtos;
            return;
        }

        int qtd;
        if (!converterInt(saidaQtd.valor, qtd) || qtd <= 0) {
            mostrar_caixa_informacao("ERRO", "Quantidade invalida");
            delete[] produtos;
            return;
        }

        int posicaoProduto = acharProdutoPorId(produtos, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            mostrar_caixa_informacao("ERRO", "Produto nao encontrado");
            delete[] produtos;
            return;
        }

        if (produtos[posicaoProduto].estoque < qtd) {
            mostrar_caixa_informacao("ERRO", "Estoque insuficiente");
            delete[] produtos;
            return;
        }

        double total = produtos[posicaoProduto].preco * qtd;

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        double saldoAtual = saldoUsuario(creditos, idUsuario);
        if (saldoAtual < total) {
            mostrar_caixa_informacao("ERRO", "Saldo insuficiente");
            delete[] produtos;
            delete[] creditos;
            return;
        }

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            mostrar_caixa_informacao("ERRO", "Nao ha espaco para registrar operacao");
            delete[] produtos;
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].saldo = -total;
        creditos[vaga].realizado = true;

        produtos[posicaoProduto].estoque = produtos[posicaoProduto].estoque - qtd;

        if (!escreverTodosCreditos(creditos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar saldo");
            delete[] produtos;
            delete[] creditos;
            return;
        }

        if (!escreverTodosProdutos(produtos)) {
            mostrar_caixa_informacao("ERRO", "Erro ao salvar produtos");
            delete[] produtos;
            delete[] creditos;
            return;
        }

        mostrar_caixa_informacao("OK", "Compra realizada");
        delete[] produtos;
        delete[] creditos;
    }
}

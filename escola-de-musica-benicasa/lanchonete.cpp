#include <iostream>
#include <fstream>
#include "lanchonete.h"
#include "interface_grafica.h"
#include "headers.h"
#include <limits>

using namespace std;

// Observação: sempre que comparamos a posicao com o máximo,
// como posicaoProduto == MAX_PRODUTOS, quer dizer que não
// foi possivel encontrá-lo, pois o ultimo valor é MAX_PRODUTOS - 1

namespace Lanchonete {

    void limparBuffer(){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int proximoIdOperacao(Credito lista[MAX_CREDITOS]){
        int maior = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_opera > maior) maior = lista[i].id_opera;
        }
        return maior + 1;
    }

    double saldoUsuario(Credito lista[MAX_CREDITOS], int idUsuario){
        double saldo = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_user == idUsuario && lista[i].realizado) {
                saldo = saldo + lista[i].saldo;
            }
        }
        return saldo;
    }

    Funcao tipoUsuario(Credito lista[MAX_CREDITOS], int idUsuario){
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_user == idUsuario && lista[i].tipo_user != NENHUMA) return lista[i].tipo_user;
        }
        return NENHUMA;
    }

    int acharCreditoPorOperacao(Credito lista[MAX_CREDITOS], int idOpera) {
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_opera == idOpera && lista[i].id_opera != 0) return i;
        }
        return MAX_CREDITOS;
    }

    void menuCadastroProdutos(){
        char opcao;

        do {
            system("cls || clear");
            cout << "========== LANCHONETE ==========\n";
            cout << "[1] Adicionar produto\n";
            cout << "[2] Remover produto\n";
            cout << "[3] Visualizar produtos\n";
            cout << "[4] Consultar estoque\n";
            cout << "[5] Alterar preço\n";
            cout << "[6] Ativar créditos pendentes\n";
            cout << "===============================\n";
            cout << "[0] Voltar\n";
            cout << "Opção: ";
            cin >> opcao;
            limparBuffer();

            switch (opcao) {
                case '1':
                    adicionarProduto();
                    break;
                case '2':
                    removerProduto();
                    break;
                case '3':
                    visualizarProdutos();
                    break;
                case '4':
                    consultarEstoque();
                    break;
                case '5':
                    alterarPrecoProduto();
                    break;
                case '6':
                    ativarCreditosPendentes();
                    break;
                case '0':
                    break;
                default:
                    cout << "Opção inválida!\n";
                    break;
            }

            if(opcao!='0'){
                cout << "\nAperte ENTER para continuar..."; // pausa simples pra ver o resultado
                limparBuffer();
                cin.get();
            }

        } while (opcao != '0');
        
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
            arquivo.write((char*)&lista[i], sizeof(Produto)); //grava todos os produtos de volta
        }

        arquivo.close();
        return true;
    }

    void lerTodosCreditos(Credito lista[MAX_CREDITOS]) {
        ifstream arquivo(BANCO_CREDITOS, ios::binary);
        Credito vazio = {};
        vazio.tipo_user = NENHUMA;

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

    int acharCreditoPorId(Credito lista[MAX_CREDITOS], int idUsuario) {
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_user == idUsuario) return i;
        }
        return MAX_CREDITOS;
    }

    int acharVagaCredito(Credito lista[MAX_CREDITOS]) {
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (lista[i].id_user == 0) return i;
        }
        return MAX_CREDITOS;
    }

    void mostrarProdutosRec(Produto lista[MAX_PRODUTOS], int i) {
        if (i == MAX_PRODUTOS) return;

        if (lista[i].ativo == 1 && lista[i].id != 0) {
            cout << "ID: " << lista[i].id << "\n";
            cout << "Nome: " << lista[i].nome << "\n";
            cout << "Preço: " << lista[i].preco << "\n";
            cout << "Estoque: " << lista[i].estoque << "\n\n";
        }

        mostrarProdutosRec(lista, i + 1);
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
            credito_vazio.tipo_user = NENHUMA;
            for (int i = 0; i < MAX_CREDITOS; i++) {
                arquivo.write((char*)&credito_vazio, sizeof(Credito)); // deixa o arquivo préalocado
            }
            arquivo.close();
        }

        return true;
    }

    bool verificarAtividade(Usuario pessoa) {
        return pessoa.ativo == 1;
    }

    void solicitarCreditosUsuario() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        int idUsuario;
        double valor;

        cout << "ID do usuário: ";
        cin >> idUsuario;

        if (idUsuario <= 0) {
            cout << "ID inválido!\n";
            return;
        }

        cout << "Valor: ";
        cin >> valor;

        if (valor <= 0) {
            cout << "Valor inválido!\n";
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            cout << "Não há espaço para adicionar mais créditos!\n";
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].tipo_user = tipoUsuario(creditos, idUsuario);
        creditos[vaga].saldo = valor;
        creditos[vaga].realizado = false;

        if (!escreverTodosCreditos(creditos)) {
            cout << "Erro ao salvar saldo!\n";
            delete[] creditos;
            return;
        }

        cout << "Solicitação enviada!\n";
        delete[] creditos;
    }

    void solicitarCreditosUsuario(int idUsuario, Funcao tipo_user) {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        double valor;

        if (idUsuario <= 0) {
            cout << "ID inválido!\n";
            return;
        }

        cout << "Valor: ";
        cin >> valor;

        if (valor <= 0) {
            cout << "Valor inválido!\n";
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            cout << "Não há espaço para adicionar mais créditos!\n";
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].tipo_user = tipo_user;
        creditos[vaga].saldo = valor;
        creditos[vaga].realizado = false;

        // DEBUG
        cout << "\n===== DEBUG: SOLICITACAO DE CREDITOS =====\n";
        cout << "ID Usuario: " << idUsuario << "\n";
        cout << "Tipo Usuario: " << tipo_user << "\n";
        cout << "Valor Solicitado: " << valor << "\n";
        cout << "ID Operacao: " << creditos[vaga].id_opera << "\n";
        cout << "Status: PENDENTE (realizado = false)\n";
        cout << "==========================================\n";
        cout << "\n[DEBUG] Pressione ENTER para continuar...";
        limparBuffer();
        cin.get();

        if (!escreverTodosCreditos(creditos)) {
            cout << "Erro ao salvar saldo!\n";
            delete[] creditos;
            return;
        }

        cout << "Solicitação enviada!\n";
        cout << "\n[DEBUG] Pressione ENTER para retornar...";
        limparBuffer();
        cin.get();
        delete[] creditos;
    }

    void ativarCreditosPendentes() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        int temPendente = 0;
        for (int i = 0; i < MAX_CREDITOS; i++) {
            if (creditos[i].id_user != 0 && creditos[i].realizado == false) {
                cout << "Operação: " << creditos[i].id_opera
                     << " | ID: " << creditos[i].id_user
                     << " | Valor: " << creditos[i].saldo << "\n";
                temPendente = 1;
            }
        }

        if (!temPendente) {
            cout << "Nenhum crédito pendente.\n";
            delete[] creditos;
            return;
        }

        int idOpera;
        char resposta;

        cout << "\nID da operação para ativar (0 para sair): ";
        cin >> idOpera;

        if (idOpera == 0) {
            delete[] creditos;
            return;
        }

        int posicao = acharCreditoPorOperacao(creditos, idOpera);
        if (posicao == MAX_CREDITOS || creditos[posicao].realizado) {
            cout << "Operação não encontrada!\n";
            delete[] creditos;
            return;
        }

        cout << "Operação: " << creditos[posicao].id_opera << "\n";
        cout << "ID: " << creditos[posicao].id_user << "\n";
        cout << "Valor: " << creditos[posicao].saldo << "\n";
        cout << "[S] - Confirmar\n";
        cout << "[x] - Cancelar\n";
        cout << ">>> ";
        cin >> resposta;

        if (resposta == 'S' || resposta == 's') {
            creditos[posicao].realizado = true;

            // DEBUG
            cout << "\n===== DEBUG: ATIVAR CREDITOS PENDENTES =====\n";
            cout << "ID Operacao: " << creditos[posicao].id_opera << "\n";
            cout << "ID Usuario: " << creditos[posicao].id_user << "\n";
            cout << "Valor: " << creditos[posicao].saldo << "\n";
            cout << "Status Anterior: PENDENTE\n";
            cout << "Status Novo: ATIVADO (realizado = true)\n";
            cout << "=============================================\n";
            cout << "\n[DEBUG] Pressione ENTER para confirmar salvamento...";
            limparBuffer();
            cin.get();

            if (!escreverTodosCreditos(creditos)) {
                cout << "Erro ao salvar!\n";
                delete[] creditos;
                return;
            }

            cout << "Crédito ativado!\n";
            cout << "\n[DEBUG] Pressione ENTER para retornar...";
            limparBuffer();
            cin.get();
            delete[] creditos;
            return;
        }

        cout << "Operação cancelada!\n";
        cout << "\n[DEBUG] Pressione ENTER para retornar...";
        limparBuffer();
        cin.get();
        delete[] creditos;
    }

    void adicionarProduto() {
        if (!bancoDeDados()) {
            cout << "Erro ao criar/abrir o banco!\n";
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        Produto novo;

        cout << "ID do produto: ";
        cin >> novo.id;

        if (novo.id <= 0) {
            cout << "ID inválido!\n";
            delete[] lista;
            return;
        }

        if (acharProdutoPorId(lista, novo.id) != MAX_PRODUTOS) {
            cout << "Já existe produto com esse ID!\n";
            delete[] lista;
            return;
        }

        cout << "Nome: ";
        cin.ignore();
        cin.getline(novo.nome, 50);

        cout << "Preço: ";
        cin >> novo.preco;

        cout << "Estoque: ";
        cin >> novo.estoque;

        novo.ativo = 1;

        int posicaoVagaProduto = acharVagaProduto(lista);
        if (posicaoVagaProduto == MAX_PRODUTOS) {
            cout << "Não há espaço para adicionar mais produtos!\n";
            delete[] lista;
            return;
        }

        lista[posicaoVagaProduto] = novo;

        if (!escreverTodosProdutos(lista)) {
            cout << "Erro ao salvar no arquivo!\n";
            delete[] lista;
            return;
        }

        cout << "Produto salvo com sucesso!\n";
        delete[] lista;
    }

    void alterarPrecoProduto() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int idProduto;
        double novoPreco;

        cout << "ID do produto: ";
        cin >> idProduto;

        if (idProduto <= 0) {
            cout << "ID inválido!\n";
            delete[] lista;
            return;
        }

        int posicaoProduto = acharProdutoPorId(lista, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            cout << "Produto não encontrado!\n";
            delete[] lista;
            return;
        }

        cout << "Preço atual: " << lista[posicaoProduto].preco << "\n";
        cout << "Novo preço: ";
        cin >> novoPreco;

        if (novoPreco <= 0) {
            cout << "Preço inválido!\n";
            delete[] lista;
            return;
        }

        lista[posicaoProduto].preco = novoPreco;

        if (!escreverTodosProdutos(lista)) {
            cout << "Erro ao salvar no arquivo!\n";
            delete[] lista;
            return;
        }

        cout << "Preço alterado!\n";
        delete[] lista;
    }

    void removerProduto() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int idProduto;
        char resposta;

        cout << "ID do produto: ";
        cin >> idProduto;

        if (idProduto <= 0) {
            cout << "ID inválido!\n";
            delete[] lista;
            return;
        }

        int posicaoProduto = acharProdutoPorId(lista, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            cout << "Produto não encontrado!\n";
            delete[] lista;
            return;
        }

        cout << "ID: " << lista[posicaoProduto].id << "\n";
        cout << "Nome: " << lista[posicaoProduto].nome << "\n";
        cout << "[S] - Confirmar\n";
        cout << "[x] - Cancelar\n";
        cout << ">>> ";
        cin >> resposta;

        if (resposta == 'S' || resposta == 's') {
            lista[posicaoProduto].ativo = 0; // marca como removido, não apaga do arquivo

            if (!escreverTodosProdutos(lista)) {
                cout << "Erro ao salvar no arquivo!\n";
                delete[] lista;
                return;
            }

            cout << "Produto removido!\n";
            delete[] lista;
            return;
        }

        cout << "Operação cancelada!\n";
        delete[] lista;
    }

    void visualizarProdutos() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int temProduto = 0;
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) {
                temProduto = 1;
                break;
            }
        }

        if (temProduto == 0) {
            cout << "Nenhum produto cadastrado.\n";
            delete[] lista;
            return;
        }

        mostrarProdutosRec(lista, 0);
        delete[] lista;
    }

    void consultarEstoque() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto *lista = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        int temProduto = 0;

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].ativo == 1 && lista[i].id != 0) {
                cout << "ID: " << lista[i].id
                     << " | Nome: " << lista[i].nome
                     << " | Estoque: " << lista[i].estoque << "\n";
                temProduto = 1;
            }
        }

        if (temProduto == 0)
            cout << "Nenhum produto cadastrado.\n";

        delete[] lista;
    }

    void consultarSaldo(int idUsuario) {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        cout << "Saldo: " << saldoUsuario(creditos, idUsuario) << "\n";
        delete[] creditos;
    }

    void removerCredito() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        int idUsuario;
        double valor;

        cout << "ID do usuário: ";
        cin >> idUsuario;

        if (idUsuario <= 0) {
            cout << "ID inválido!\n";
            return;
        }

        cout << "Valor: ";
        cin >> valor;

        if (valor <= 0) {
            cout << "Valor inválido!\n";
            return;
        }

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        double saldoAtual = saldoUsuario(creditos, idUsuario);
        if (saldoAtual < valor) {
            cout << "Saldo insuficiente!\n";
            delete[] creditos;
            return;
        }

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            cout << "Não há espaço para registrar operação!\n";
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].tipo_user = tipoUsuario(creditos, idUsuario);
        creditos[vaga].saldo = -valor;
        creditos[vaga].realizado = true;

        if (!escreverTodosCreditos(creditos)) {
            cout << "Erro ao salvar saldo!\n";
            delete[] creditos;
            return;
        }

        cout << "Crédito removido!\n";
        delete[] creditos;
    }

    void realizarCompra(int idUsuario) {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        int idProduto;
        int qtd;

        cout << "ID do produto: ";
        cin >> idProduto;

        cout << "Quantidade: ";
        cin >> qtd;

        if (qtd <= 0) {
            cout << "Quantidade inválida!\n";
            return;
        }

        Produto *produtos = new Produto[MAX_PRODUTOS];
        lerTodosProdutos(produtos);

        int posicaoProduto = acharProdutoPorId(produtos, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            cout << "Produto não encontrado!\n";
            delete[] produtos;
            return;
        }

        if (produtos[posicaoProduto].estoque < qtd) {
            cout << "Estoque insuficiente!\n";
            delete[] produtos;
            return;
        }

        double total = produtos[posicaoProduto].preco * qtd;

        Credito *creditos = new Credito[MAX_CREDITOS];
        lerTodosCreditos(creditos);

        double saldoAtual = saldoUsuario(creditos, idUsuario);
        if (saldoAtual < total) {
            cout << "Saldo insuficiente!\n";
            delete[] produtos;
            delete[] creditos;
            return;
        }

        int vaga = acharVagaCredito(creditos);
        if (vaga == MAX_CREDITOS) {
            cout << "Não há espaço para registrar operação!\n";
            delete[] produtos;
            delete[] creditos;
            return;
        }

        creditos[vaga].id_opera = proximoIdOperacao(creditos);
        creditos[vaga].id_user = idUsuario;
        creditos[vaga].tipo_user = tipoUsuario(creditos, idUsuario);
        creditos[vaga].saldo = -total;
        creditos[vaga].realizado = true;

        produtos[posicaoProduto].estoque = produtos[posicaoProduto].estoque - qtd;

        if (!escreverTodosCreditos(creditos)) {
            cout << "Erro ao salvar saldo!\n";
            delete[] produtos;
            delete[] creditos;
            return;
        }

        if (!escreverTodosProdutos(produtos)) {
            cout << "Erro ao salvar produtos!\n";
            delete[] produtos;
            delete[] creditos;
            return;
        }

        cout << "Compra realizada!\n";
        delete[] produtos;
        delete[] creditos;
    }
}
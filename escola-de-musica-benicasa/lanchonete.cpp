#include <iostream>
#include <fstream>
#include "lanchonete.h"
#include "headers.h"

using namespace std;

namespace Lanchonete {

    struct Saldo {
        int id_usuario;
        float saldo;
    };


    void menuCadastroProdutos(){
        system("cls || clear");
        cout << "Dentro de menuCadastroProdutos" << endl;
        // Lógica para cadastro de produtos
    }

    void adicionarCreditosUsuario() {
        system("cls || clear");
        cout << "Dentro de adicionarCreditosUsuario" << endl;
        // Lógica para adicionar créditos ao usuário
    }

    bool bancoExisteProdutos() {
        ifstream arquivo(BANCO_PRODUTOS, ios::binary);
        bool existe = arquivo.is_open();
        arquivo.close();
        return existe;
    }

    bool bancoExisteSaldo() {
        ifstream arquivo(BANCO_SALDO, ios::binary);
        bool existe = arquivo.is_open();
        arquivo.close();
        return existe;
    }

    void lerTodosProdutos(Produto lista[MAX_PRODUTOS]) {
        ifstream arquivo(BANCO_PRODUTOS, ios::binary);
        Produto vazio;

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

    void lerTodosSaldos(Saldo lista[MAX_PRODUTOS]) {
        ifstream arquivo(BANCO_SALDO, ios::binary);
        Saldo vazio;

        if (!arquivo.is_open()) {
            for (int i = 0; i < MAX_PRODUTOS; i++) {
                lista[i] = vazio;
            }
            return;
        }

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            Saldo saldo;
            if (arquivo.read((char*)&saldo, sizeof(Saldo))) {
                lista[i] = saldo;
            } else {
                lista[i] = vazio;
            }
        }

        arquivo.close();
    }

    bool escreverTodosSaldos(Saldo lista[MAX_PRODUTOS]) {
        ofstream arquivo(BANCO_SALDO, ios::binary | ios::trunc);
        if (!arquivo.is_open()) return false;

        for (int i = 0; i < MAX_PRODUTOS; i++) {
            arquivo.write((char*)&lista[i], sizeof(Saldo));
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

    int acharSaldoPorId(Saldo lista[MAX_PRODUTOS], int idUsuario) {
        for (int i = 0; i < MAX_PRODUTOS; i++) {
            if (lista[i].id_usuario == idUsuario) return i;
        }
        return MAX_PRODUTOS;
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

            Produto produto_vazio;
            for (int i = 0; i < MAX_PRODUTOS; i++) {
                arquivo.write((char*)&produto_vazio, sizeof(Produto));
            }
            arquivo.close();
        }

        if (!bancoExisteSaldo()) {
            ofstream arquivo(BANCO_SALDO, ios::binary | ios::trunc);
            if (!arquivo.is_open()) return false;

            Saldo saldo_vazio;
            for (int i = 0; i < MAX_PRODUTOS; i++) {
                arquivo.write((char*)&saldo_vazio, sizeof(Saldo));
            }
            arquivo.close();
        }

        return true;
    }

    bool verificarAtividade(Usuario pessoa) {
        return pessoa.ativo == 1;
    }

    void adicionarProduto() {
        if (!bancoDeDados()) {
            cout << "Erro ao criar/abrir o banco!\n";
            return;
        }

        Produto lista[MAX_PRODUTOS];
        lerTodosProdutos(lista);

        Produto novo;

        cout << "ID do produto: ";
        cin >> novo.id;

        if (novo.id <= 0) {
            cout << "ID inválido!\n";
            return;
        }

        if (acharProdutoPorId(lista, novo.id) != MAX_PRODUTOS) {
            cout << "Já existe produto com esse ID!\n";
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
            return;
        }

        lista[posicaoVagaProduto] = novo;

        if (!escreverTodosProdutos(lista)) {
            cout << "Erro ao salvar no arquivo!\n";
            return;
        }

        cout << "Produto salvo com sucesso!\n";
    }

    void visualizarProdutos() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto lista[MAX_PRODUTOS];
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
            return;
        }

        mostrarProdutosRec(lista, 0);
    }

    void consultarEstoque() {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Produto lista[MAX_PRODUTOS];
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
    }

    void consultarSaldo(int idUsuario) {
        if (!bancoDeDados()) {
            cout << "Erro ao abrir o banco!\n";
            return;
        }

        Saldo saldos[MAX_PRODUTOS];
        lerTodosSaldos(saldos);

        int posicaoSaldoUsuario = acharSaldoPorId(saldos, idUsuario);
        if (posicaoSaldoUsuario == MAX_PRODUTOS) {
            cout << "Saldo: 0\n";
            return;
        }

        cout << "Saldo: " << saldos[posicaoSaldoUsuario].saldo << "\n";
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

        Produto produtos[MAX_PRODUTOS];
        lerTodosProdutos(produtos);

        int posicaoProduto = acharProdutoPorId(produtos, idProduto);
        if (posicaoProduto == MAX_PRODUTOS) {
            cout << "Produto não encontrado!\n";
            return;
        }

        if (produtos[posicaoProduto].estoque < qtd) {
            cout << "Estoque insuficiente!\n";
            return;
        }

        float total = produtos[posicaoProduto].preco * qtd;

        Saldo saldos[MAX_PRODUTOS];
        lerTodosSaldos(saldos);

        int posicaoSaldoUsuario = acharSaldoPorId(saldos, idUsuario);
        if (posicaoSaldoUsuario == MAX_PRODUTOS) {
            cout << "Saldo insuficiente!\n";
            return;
        }

        if (saldos[posicaoSaldoUsuario].saldo < total) {
            cout << "Saldo insuficiente!\n";
            return;
        }

        saldos[posicaoSaldoUsuario].saldo = saldos[posicaoSaldoUsuario].saldo - total;
        produtos[posicaoProduto].estoque = produtos[posicaoProduto].estoque - qtd;

        if (!escreverTodosSaldos(saldos)) {
            cout << "Erro ao salvar saldo!\n";
            return;
        }

        if (!escreverTodosProdutos(produtos)) {
            cout << "Erro ao salvar produtos!\n";
            return;
        }

        cout << "Compra realizada!\n";
    }
}

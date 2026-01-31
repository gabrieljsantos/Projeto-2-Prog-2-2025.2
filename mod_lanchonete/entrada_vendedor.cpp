#include <iostream>
#include "../headers.h"
#include "../utilidades.h"

using namespace std;

const char* ARQUIVO = "../database/cantina.dat";
const int MAX_REGISTROS = 100;

void verificar_banco() {
    ifstream teste(ARQUIVO);
    if (!teste) {
        ofstream criar(ARQUIVO, ios::binary);
        Produto_Cantina vazio;
        vazio.ativo = 0;
        vazio.id = 0;
        
        for (int i = 0; i < MAX_REGISTROS; i++) {
            criar.write((char*)&vazio, sizeof(Produto_Cantina));
        }
        criar.close();
    } else {
        teste.close();
    }
}

void adicionar_produto() {
    fstream cantina(ARQUIVO, ios::binary | ios::in | ios::out);
    if (!cantina) return;

    Produto_Cantina produto;
    bool salvo = false;
    int indice = 0;

    while (cantina.read((char*)&produto, sizeof(Produto_Cantina))) {
        if (produto.ativo == 0) {
            produto.id = indice + 1;
            produto.ativo = 1;

            cout << "Nome: "; 
            cin.getline(produto.nome, 50);
            cout << "Preço: "; 
            cin >> produto.preco;
            cout << "Qtd: "; 
            cin >> produto.quantidade_estoque;
            cin.ignore(); 

            cantina.seekp(indice * sizeof(Produto_Cantina));
            cantina.write((char*)&produto, sizeof(Produto_Cantina));
            
            cout << "Salvo no slot " << indice << " com ID " << produto.id << endl;
            salvo = true;
            break;
        }
        indice++;
    }
    
    if (!salvo) cout << "Estoque cheio (Limite 100).\n";
    cantina.close();
}

void remover_produto() {
    int id;
    char confirmacao;
    cout << "ID para remover: "; cin >> id;
    cin.ignore();

    fstream cantina(ARQUIVO, ios::binary | ios::in | ios::out);
    if (!cantina) return;

    Produto_Cantina produto;
    bool achou = false;

    while (cantina.read((char*)&produto, sizeof(Produto_Cantina))) {
        if (produto.id == id && produto.ativo == 1) {
            cout << "Produto encontrado: " << produto.nome << endl;
            cout << "Deseja remover este produto? (S/N): ";
            cin >> confirmacao;
            cin.ignore();

            if (confirmacao == 'S' || confirmacao == 's') {
                produto.ativo = 0;

                // Volte para o inicio do registro (no cursor de escrita) e zere-o
                cantina.seekp(-(sizeof(Produto_Cantina)), ios::cur);
                cantina.write((char*)&produto, sizeof(Produto_Cantina));
                cout << "Produto removido com sucesso.\n";
            } else {
                cout << "Operação cancelada.\n";
            }
            achou = true;
            break; 
        }
    }
    cantina.close();

    if (!achou) cout << "ID não encontrado.\n";
}

void visualizar_estoque() {
    ifstream cantina(ARQUIVO, ios::binary);
    if (!cantina) return;

    Produto_Cantina produto;

    cout << "\n";
    cout << left << setw(5)  << "ID" 
         << left << setw(30) << "Nome" 
         << left << setw(15) << "Preço"
         << left << setw(5)  << "Qtd" << endl;

    cout << "--------------------------------------------------------" << endl;
    
    while (cantina.read((char*)&produto, sizeof(Produto_Cantina))) {
        if (produto.ativo == 1) {
            cout << left << setw(5)  << produto.id 
                 << left << setw(30) << produto.nome;
            
            cout << "R$ " << left << fixed << setprecision(2) << setw(12) << produto.preco 
                 << produto.quantidade_estoque << endl;
        }
    }
    cantina.close();
}

void janela_vendedor(login_info info) {
    if (info.info_basica.id != VENDEDOR) {
        cout << "Acesso negado. Usuário não é vendedor.\n";
        return;
    }

    verificar_banco();
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nGerenciador de Lanchonete\n";
        cout << "1 - Adicionar Produto\n";
        cout << "2 - Remover Produto\n";
        cout << "3 - Visualizar Estoque\n";
        cout << "4 - Registrar Venda\n";
        cout << "5 - Visualizar Vendas\n";
        cout << "0 - Voltar\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                cout << "Adicionar produto...\n";
                adicionar_produto();
                cout << "\nEnter para voltar...";
                cin.get();
                break;
            case 2:
                cout << "Remover produto...\n";
                remover_produto();
                cout << "\nEnter para voltar...";
                cin.get();
                break;
            case 3:
                cout << "Visualizando estoque...\n";
                visualizar_estoque();
                cout << "\nEnter para voltar...";
                cin.get();
                break;
            case 4:
                cout << "Registrando venda...\n";
                cout << "\nEnter para voltar...";
                cin.get();
                break;
            case 5:
                cout << "Visualizando vendas...\n";
                cout << "\nEnter para voltar...";
                cin.get();
                break;
            case 0:
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
}
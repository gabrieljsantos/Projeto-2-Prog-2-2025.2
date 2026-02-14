#include <iostream>
#include "lanchonete.h"
#include "headers.h"
#include <fstream>

using namespace std;

namespace Lanchonete {
    bool verificarAtividade(Usuario pessoa){
        if(pessoa.ativo==1)
            return true;
        else
            return false;
    }

    void adicionarProduto(){
        Produto p;
        ofstream arquivo("produtos.dat", ios::binary | ios::app);
        if(arquivo.is_open()){
            cout << "ID: ";
            cin >> p.id;
            cout << "Nome: ";
            cin.ignore();
            cin.getline(p.nome, 50);
            cout << "Preço: ";
            cin >> p.preco;
            cout << "Estoque: ";
            cin >> p.estoque;
            p.ativo = 1;

            arquivo.write((char*)&p, sizeof(Produto));
            arquivo.close();
            cout << "Produto salvo com sucesso!\n";
        }
        else{
            cout<<"Erro ao abrir o arquivo!\n";
        }
        arquivo.close();
    }
}


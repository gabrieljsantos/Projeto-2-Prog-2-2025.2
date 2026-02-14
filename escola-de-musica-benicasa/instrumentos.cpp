#include "instrumentos.h"
#include <iostream>
#include <fstream>
using namespace std;
Instrumentos novo;
Instrumentos vazio = { 0 , "" , false, ""}; //Aqui vai ser usado pra criar os espaços vazios e deletar algum instrumento.
fstream Rinstrumentos;
void menuCadastroInstrumentos(){
    cout << "============================================"<<endl;
    cout << "       BEM VINDO AO CADASTRO DE INSTRUMENTOS       "<<endl;
    cout << "Aqui você vai realizar o cadastro dos intrumentos que serão disponibilizados para emprestimo"<<endl;
    openFile();
    //criando uma struct para armazenar as informações que o usuario vai colocar
    int id;
    bool continuar = 1;
    while(continuar == true){
        bool gravado = 0;
        cout << "Insira o numero de identificação do instrumento que será disponibilizado para emprestimo: ";
        cin >> id;
        while(gravado != true)
        {
        while(id < 1 || id > 100)
        {
            cout << "Erro! tente Novamente!"<<endl;
            cout << "Adicione a numeração do instrumento: (1-100)";
            cin >> id;
        }
        Rinstrumentos.seekg((id - 1) * sizeof(Instrumentos));
        Rinstrumentos.read((char*)&novo, sizeof(Instrumentos));
        if(novo.id == 0)
        {
        novo.id = id;
        cout << "Nome do instrumento: ";
        cin.ignore();
        cin.getline(novo.nome, 20);
        novo.disponivel = 1;

        Rinstrumentos.seekp((novo.id - 1)* sizeof(Instrumentos));
        Rinstrumentos.write((char*)&novo, sizeof(Instrumentos));
        gravado = true;
        }
        else {
            cout << "Essa numeraçao ja pertence a outro intrumento, insira outra: "<<endl;
            cin >> id;
        }
        }
    cout<< "Instrumento gravado com sucesso!"<<endl;
    cout<< "Escolha uma das opções abaixo: "<<endl;
    cout<< "1 - Cadastrar outro instrumento"<<endl;
    cout<< "0 - Sair para o menu principal\n";
    cout<< "Opçao : ";
    cin >> continuar;
}
Rinstrumentos.close();
}

void deleteInstrumentos(){
    openFile();

    cout << "";
}

void mostrarInstrumentosCadastrados(){
    openFile();
    for (int i = 0; i<100; i++){
        Rinstrumentos.seekg(i * sizeof(Instrumentos));
        Rinstrumentos.read((char*)&novo, sizeof(Instrumentos));
        if(novo.id != 0){
            cout << "Nome: "<< novo.nome<< " "<< "ID: "<< novo.id<< " Situacao:";
            if(novo.disponivel == 1)
                cout << " Disponivel \n";
            else
                cout << " Indisponivel \n";
        }
    }
    Rinstrumentos.close();
}

void openFile(){
    Rinstrumentos.open("instrumentos.dat", ios::in | ios::out | ios::binary);
    if(!Rinstrumentos){
        Rinstrumentos.open("instrumentos.dat", ios::out | ios::binary);
        for(int i=0; i<100; i++){
            Rinstrumentos.write((const char*)(&vazio), sizeof(Instrumentos));
        }
        Rinstrumentos.close();
        Rinstrumentos.open("instrumentos.dat", ios::in | ios::out | ios::binary);
    }
}


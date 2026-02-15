#include <iostream>
#include <fstream>
#include "admin.h"
#include "headers.h"

using namespace std;

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
namespace Modulo_admin {

    void menuGerenciarUsuarios(std::fstream &file){
    char escolha;
    int Id;
    Usuario *usuario_ = new Usuario;
    char resposta;
    do
    {
        std::cout
        <<"========== Usuarios==========\n\n"
        <<"[1] - Ativar cadastro\n"
        <<"[2] - Desativar cadastro\n"
        <<"[3] - Usuarios Ativos\n"
        <<"[4] - Usuarios Inativos\n"
        <<"=============================\n"
        <<"[x] - Qualquer tecla (SAIR)\n";
        escolha = std::cin.get();
        limparbuffer();

        switch (escolha)
        {
        case '1':
            std::cout<<"Para ativar o cadastro do usuario\ninsira o Id da conta a ser ativada. Ex:[123]\n>>> ";
            do{
                std::cin>>Id;
                std::cin.ignore();
            }while(!checkId_ok(Id));

            file.seekg((Id-1) * sizeof(Usuario));
            file.read((char*)(usuario_), sizeof(Usuario));

           if(!(*usuario_).ativo && (*usuario_).id !=0){    
                std::cout<<"Nome: "<< (*usuario_).nome<<"\n"
                <<"[S] - Confirma\n"
                <<"==============================\n"
                <<"[x] - Qualquer tecla (NAO)\n>>> ";
                std::cin>>resposta;
                limparbuffer();

                if(resposta == 'S'|| resposta == 's'){
                    (*usuario_).ativo = 1;
                    file.seekp((Id -1)*sizeof(Usuario));
                    file.write((char*)(usuario_),sizeof(Usuario));
                    std::cout<<"Usuario: "<<(*usuario_).nome<<" Ativo.\n";
                }
            }
            else{
                if(usuario_->id==0)
                    std::cout<<"Usuario nao existete";
                else
                    std::cout<<"Usuario se encontra ativo!";
            }
            
            break;
         case '2':
            std::cout<<"Para inativar o cadastro do usuario\ninsira o Id da conta a ser inativada\n>>> ";
              do{
                std::cin>>Id;
                std::cin.ignore();
            }while(!checkId_ok(Id));

            file.seekg((Id-1) * sizeof(Usuario));
            file.read((char*)(usuario_), sizeof(Usuario));

           if((*usuario_).ativo && (*usuario_).id !=0){  
                std::cout<<"Nome: "<< (*usuario_).nome<<"\n"
                <<"[OBS: Usuario nao podera acessar o sistema]\n"
                <<"[S] - Confirma\n"
                <<"==============================\n"
                <<"[x] - Qualquer tecla (NAO)\n>>> ";
                resposta = std::cin.get();
                std::cin.ignore();

                if(resposta == 'S'|| resposta == 's'){
                    (*usuario_).ativo = 0;
                    file.seekp((Id -1)*sizeof(Usuario));
                    file.write((char*)(usuario_),sizeof(Usuario));
                    std::cout<<"Usuario: "<<(*usuario_).nome<<" Inativo.\n";
                }
           }
           else{ 
                if(usuario_->id==0)
                    std::cout<<"Usuario nao existete";
                else
                    std::cout<<"Usuario se encontra inativo!\n";
           }
           
             break;
         case '3':
         
         std::cout<<"Lista de usuarios ativos\n"
         <<"======================================\n";

        file.seekg(0);
        while(file.read((char *)(usuario_),sizeof(Usuario)))
        {
            if((*usuario_).ativo){
            std::cout
            <<"Nome: "<<(*usuario_).nome<<"\n"
            <<"Categoria: "<<categoria((*usuario_))<<"\n"
            <<"ID: "<< (*usuario_).id<<"\n"
            <<"E-mail: "<<(*usuario_).email<<"\n"
            <<"======================================\n";
        }
        }
        file.clear();
             break;
         case '4':
                    
         std::cout<<"Lista de usuarios Inativos\n"
         <<"======================================\n";

        file.seekg(0);
        while(file.read((char *)(usuario_), sizeof(Usuario)))
        {
            if(!(*usuario_).ativo && (*usuario_).id!=0){
            std::cout
            <<"Nome: "<<(*usuario_).nome<<"\n"
            <<"Categoria: "<< categoria((*usuario_))<<"\n"
            <<"ID: "<< (*usuario_).id<<"\n"
            <<"E-mail: "<<(*usuario_).email<<"\n"
            <<"======================================\n";
            }

        }
        file.clear();
            break;
        
        }
        pausar();
        system("cls");

    } while (escolha == '1' || escolha== '2' || escolha== '3'|| escolha== '4');
    delete usuario_;
    usuario_= nullptr;
    
}

    void menuCadastroCursos() {
        system("cls || clear");
        cout << "Dentro de menuCadastroCursos" << endl;
        // Lógica para cadastro de cursos
    }
    void consultarRelatoriosAcademicos() {
        system("cls || clear");
        cout << "Dentro de consultarRelatoriosAcademicos" << endl;
        // Lógica para consultar relatórios acadêmicos
    }

    void menuEventos(std::fstream &file) {
    char opcao;
    Evento *evento_ = new Evento;

    do {
        std::cout
        << "\n=========  Eventos  =========\n"
        << "[1]- Cadastrar evento\n"
        << "[2]- Autorizar evento\n"
        << "[3]- Listar eventos\n"
        << "=================================\n"
        << "[x]- Voltar\n"
        << ">>> ";
        std::cin >> opcao;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcao) {
            case '1':
                system("cls");

                std::cout << "\n--- Novo Evento ---\n"
                << "Nome: ";
                std::cin.getline(evento_->nome, 49);

                std::cout << "Local do evento: ";
                std::cin.getline(evento_->local, 49);

                std::cout << "Descricao: ";
                std::cin.getline(evento_->descricao, 99);

                std::cout << "Data do evento [dd/mm/aaaa]: ";
                std::cin.getline(evento_->data,11);
                limparbuffer();

                std::cout << "Total de vagas: ";
                std::cin >> evento_->totalVagas;
                limparbuffer();

                evento_->vagasOcupadas = 0;
                evento_->autorizado = 0;
                evento_->ativo = 1;

                file.seekg(0, std::ios::end);
                evento_->id = 1 + (file.tellg() / sizeof(Evento));
                file.clear();
                std::cout << "ID do evento: " << evento_->id << "\n";

                file.seekp(0, std::ios::end);
                file.write((char*)(evento_), sizeof(Evento));
                file.clear();

                std::cout << "Evento cadastrado com sucesso!\n";
                break;

            case '2': {
                system("cls");
                bool achado = false;
                int idBusca;
                std::cout << "Informe o ID do evento: ";
                std::cin >> idBusca;
                limparbuffer();


                file.clear();
                file.seekg((idBusca-1)*sizeof(Evento));
                file.read((char*)evento_,sizeof(Evento));

                    if (evento_->autorizado){
                        achado = true;
                    std::cout << "Evento se encontra autorizado.\n";
                }


                    else if (evento_->id == idBusca && evento_->ativo == 1) {
                        achado = true;
                        std::cout 
                        << "\nID: " << evento_->id << "\n"
                        << "Nome: " << evento_->nome << "\n"
                        << "Local: " << evento_->local << "\n"
                        << "Data: " << evento_->data << "\n";

                        evento_->autorizado = 1;

                        file.seekp((idBusca-1)*sizeof(Evento));
                        file.write((char*)evento_, sizeof(Evento));
                        std::cout<< "Evento autorizado com sucesso.\n";
                    }
                
                file.clear();

                if (!achado)
                    std::cout << "Evento nao encontrado.\n";
                break;
            }

            case '3': {
                system("cls");
                bool vazio;
                char opc;
                std::cout
                <<"========== Listar ==========\n"
                <<"[1]- Autorizados\n"
                <<"[2]- Pendentes\n"
                <<"=============================\n"
                <<"[x]- voltar\n: ";
                std::cin>>opc;
                limparbuffer();
                switch (opc)
                {
                case '1':
                    std::cout<<"======== Autorizados ========\n";
                    file.seekg(0);
                    vazio = true;
                    while (file.read((char*)evento_, sizeof(Evento))) {
                        if (evento_->ativo  && evento_->autorizado) {
                            vazio = false;
                            std::cout
                            << "\nID: " << evento_->id << "\n"
                            << "Nome: " << evento_->nome << "\n"
                            << "Local: " << evento_->local << "\n"
                            << "Data: " << evento_->data << "\n"
                            << "Descricao: " << evento_->descricao << "\n"
                            << "Vagas: " << evento_->vagasOcupadas << "/" << evento_->totalVagas << "\n"
                            << "Autorizado: " << (evento_->autorizado ? "Sim" : "Nao") << "\n"
                            << "================================\n";
                        }
                    }

                    file.clear();

                    if (vazio)
                        std::cout << "Nenhum evento cadastrado.\n";

                    break;

                case '2':
                    std::cout<<"======== Pendentes ========\n";
                    file.seekg(0);
                     vazio = true;
                    while (file.read((char*)evento_, sizeof(Evento))) {
                        if (evento_->ativo && !evento_->autorizado) {
                            vazio = false;
                            std::cout
                            << "\nID: " << evento_->id << "\n"
                            << "Nome: " << evento_->nome << "\n"
                            << "Local: " << evento_->local << "\n"
                            << "Data: " << evento_->data << "\n"
                            << "Descricao: " << evento_->descricao << "\n"
                            << "Vagas: " << evento_->vagasOcupadas << "/" << evento_->totalVagas << "\n"
                            << "Autorizado: " << (evento_->autorizado ? "Sim" : "Nao") << "\n"
                            << "================================\n";
                        }
                    }                                           
                    file.clear();   
                    if (vazio)
                        std::cout << "Nenhum evento cadastrado.\n";
                    break;
                    
                    case 'x':
                        std::cout << "Voltando...\n";
                        break;
                default:
                    std::cout << "Invalido! tente novamente\n";
                    break;
                }
            }
        }
        pausar();
        system("cls");

    } while (opcao != 'X' && opcao != 'x' );

    delete evento_;
    evento_ = nullptr;
}

//========

    void listarTodosEventos() {
        system("cls || clear");
        cout << "Dentro de listarTodosEventos" << endl;
        // Lógica para listar todos os eventos
    }
    void menuCadastroInstrumentos() {
        system("cls || clear");
        cout << "Dentro de menuCadastroInstrumentos" << endl;
        // Lógica para cadastro de instrumentos
    }
    void liberarInstrumentos() {
        system("cls || clear");
        cout << "Dentro de liberarInstrumentos" << endl;
        // Lógica para liberar instrumentos
    }
    void consultarPendenciasInstrumentos() {
        system("cls || clear");
        cout << "Dentro de consultarPendenciasInstrumentos" << endl;
        // Lógica para consultar pendências de instrumentos
    }
    void gerarRelatorioFinanceiro() {
        system("cls || clear");
        cout << "Dentro de gerarRelatorioFinanceiro" << endl;
        // Lógica para gerar relatório financeiro
    }
    void gerarRelatorioPatrimonial() {
        system("cls || clear");
        cout << "Dentro de gerarRelatorioPatrimonial" << endl;
        // Lógica para gerar relatório patrimonial
    }
    void realizarBackup() {
        system("cls || clear");
        cout << "Dentro de realizarBackup" << endl;
        // Lógica para realizar backup
    }
    void restaurarBackup() {
        system("cls || clear");
        cout << "Dentro de restaurarBackup" << endl;
        // Lógica para restaurar backup
    }

}

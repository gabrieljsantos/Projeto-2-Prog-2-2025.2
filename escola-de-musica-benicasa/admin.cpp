
#include <iostream>
#include <fstream>

void menuGerenciarUsuarios(std::fstream &file){
    char escolha;
    int Id;
    Usuario *usuario_ = new Usuario;
    char resposta;
    do
    {
        std::cout<<"==== Menu ====\n\n"
        <<"[1] - Ativar cadastro\n"
        <<"[2] - Desativar cadastro\n"
        <<"[3] - Usuarios Ativos\n"
        <<"[4] - Usuarios Inativos\n"
        <<"=============================\n"
        <<"[x] - Qualquer tecla (SAIR)\n";
        escolha = std::cin.get();
        std::cin.ignore();

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
                std::cin.ignore();

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

int main(){
    std::fstream arquivo;
    openFile(arquivo,"usuarios.dat");
    menuCadastroUsuarios(arquivo);
    menuGerenciarUsuarios(arquivo);

    arquivo.close();
    return 0;
}
    

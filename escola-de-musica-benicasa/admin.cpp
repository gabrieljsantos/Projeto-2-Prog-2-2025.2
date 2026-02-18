#include <iostream>
#include <fstream>
#include "admin.h"
#include "headers.h"
#include "interface_grafica.h"
#include "lanchonete.h"

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
Professor buscaProf(std::fstream &file,int buscaId){
    Professor prof{};
    file.clear();
    file.seekg(0, std::ios::end);
    int tamanho = file.tellg();
    int total = tamanho / sizeof(Professor);

    if(buscaId <= 0 || buscaId > total){
        std::cout<<"ID invalido!\n";
        return prof;
    }
    file.seekg((buscaId-1)*sizeof(Professor));
    file.read((char*)&prof,sizeof(Professor));
    if(prof.base.id == buscaId&&prof.base.ativo){
        std::cout
        <<"================================\n"
        <<"Nome: "<<prof.base.nome<<"\n"
        <<"E-mail: "<<prof.base.email<<"\n";
        return prof;
    }
    else{
        std::cout<<"Nenhum professor com esse ID ou Inativo!\n";
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
        std::cout << "ID invalido!\n";
        return disc;
    }
    file.seekg((buscaId - 1) * sizeof(Disciplina));
    file.read((char*)&disc, sizeof(Disciplina));
    if (disc.id == buscaId){
        std::cout
        <<"================================\n"
        <<"Nome: "<<disc.nome<<"\n";

        return disc;
    }
    else{
        std::cout << "Nenhuma disciplina com esse ID!\n";
        return disc;
    }
}

void consultarPendenciasInstrumentos() {
    system("cls");
    std::fstream fileEmprestimo;
    std::fstream fileAlunos;
    openFile(fileEmprestimo,"emprestimos.dat");
    openFile(fileAlunos,"alunos.dat");

    Emprestimo emp;
    Aluno user;

    bool encontrou = false;

    fileEmprestimo.clear();
    fileEmprestimo.seekg(0);

    while(fileEmprestimo.read((char*)&emp, sizeof(Emprestimo))) {

        fileAlunos.clear();
        fileAlunos.seekg((emp.idAluno - 1) * sizeof(Aluno));

        if(!fileAlunos.read((char*)&user, sizeof(Aluno)))
            continue;

        if(user.base.categoria ==ALUNO && user.base.ativo) {

            encontrou = true;

            std::cout
            << "===== Pendencia de Aluno =====\n"
            << "Cod emprestimo: " << emp.idEmprestimo << "\n"
            << "Aluno: " << user.base.nome << "\n"
            << "Email: " << user.base.email << "\n"
            << "Instrumento: " << emp.nome_In << "\n"
            << "id Instrumento: " << emp.idInstrumento << "\n"
            << "Data emprestimo: " << emp.dataEmprestimo << "\n"
            << "Data prevista devolucao: " << emp.dataPrevista << "\n";

            std::cout << "====================================\n\n";
        }
    }

    if(!encontrou)
        std::cout << "Nenhuma pendencia encontrada.\n";

    fileEmprestimo.close();
    fileAlunos.close();
}


namespace Modulo_admin {


    int listar_usuarios_especificos(Funcao tipo_usuario, int ativo, string dados[100][6]){
            int contador = 0;
            switch (tipo_usuario)
            {
            case ALUNO: {
                std::fstream file;
                openFile(file, "alunos.dat");
                Aluno aluno;
                file.seekg(0);
                while(file.read((char*)&aluno, sizeof(Aluno)) && contador < 100) {
                    if((aluno.base.ativo == ativo || ativo == 2) && aluno.base.id != 0 ) {
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
                    if((prof.base.ativo == ativo || ativo == 2) && prof.base.id != 0) {
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
            case ADMINISTRADOR: {
                std::fstream file;
                openFile(file, "admin.dat");
                Admin admin;
                file.seekg(0);
                while(file.read((char*)&admin, sizeof(Admin)) && contador < 100) {
                    if((admin.base.ativo == ativo || ativo == 2) && admin.base.id != 0) {
                        dados[contador][0] = to_string(admin.base.id); 
                        dados[contador][1] = admin.base.nome;
                        dados[contador][2] = admin.base.email;
                        dados[contador][3] = to_string(admin.base.categoria);
                        dados[contador][4] = admin.base.ativo ? "Ativo" : "Inativo";
                        contador++;
                    }
                }
                file.close();
                break;
            }
            default:
                break;
            }


            return contador;
    }

    void menuCadastroUsuarios() {
        system("cls || clear");
        cout << "Dentro de menuCadastroUsuarios" << endl;
    }

    void menuGerenciarUsuarios(){

        constexpr int Quantidades_opcoes = 4;
        string texto_do_tipo_de_usuario;
        Funcao tipo_usuario = ALUNO;
        int estado_selecionado = 2; // 0=Ativo, 1=Inativo, 2=Ambos (padrão)
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
                case NENHUMA:
                    break;
            }

            string opcoes[Quantidades_opcoes] = {
                "Filtros de pesquisa",
                "Listar Usuarios",
                "Buscar Usuario por ID",
                "Voltar"
            };
            ConfigMenu config;
            config.titulo = "Gerenciar Usuarios < " + texto_do_tipo_de_usuario + "s >";   
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
                    
                    cout << "Tipo Usuario selecionado: " << resultadofiltro.valores_selecionados[0] << endl;
                    cout << "Estado Usuario selecionado: " << resultadofiltro.valores_selecionados[1] << endl;
                    system("pause");
                    if (resultadofiltro.confirmado && resultadofiltro.numero_variaveis >= 2) {
                        // Converter tipo de usuario
                        string tipo_str = resultadofiltro.valores_selecionados[0];
                        if (tipo_str == "Aluno") tipo_usuario = ALUNO;
                        else if (tipo_str == "Professor") tipo_usuario = PROFESSOR;
                        else if (tipo_str == "Administrador") tipo_usuario = ADMINISTRADOR;
                        
                        // Converter estado do usuario
                        string estado_str = resultadofiltro.valores_selecionados[1];
                        if (estado_str == "Ativo") estado_selecionado = 1;
                        else if (estado_str == "Inativo") estado_selecionado = 0;
                        else if (estado_str == "Ambos") estado_selecionado = 2;
                    }
                    break;
                }
                case 1: {
                    string dados[100][6];
                    int total = listar_usuarios_especificos(tipo_usuario, estado_selecionado, dados);
                    cout << "Total de usuarios encontrados: " << total << endl;
                    for (int i = 0; i < total; i++) {
                        cout << "ID: " << dados[i][0] << " | Nome: " << dados[i][1] << " | Email: " << dados[i][2] << " | Categoria: " << dados[i][3] << " | Status: " << dados[i][4] << endl;
                    }
                    system("pause");
                    string titulos[5] = {"ID", "Nome", "Email", "Categoria", "Status"};
                    const string* dados_ptr[100];
                    for(int i = 0; i < total; i++) dados_ptr[i] = dados[i];
                    ConfigTabela configTab;
                    configTab.titulo = "Todos os Usuarios (" + texto_do_tipo_de_usuario + "s)";
                    saida_tabela usuario_selecionado = interface_para_tabela(total, 5, dados_ptr, titulos, 0, configTab);
                    system("pause");
                    cout << "Usuario selecionado: " << usuario_selecionado.valor_retorno << endl;
                    cout << "Linha: " << usuario_selecionado.indice_linha << " | Coluna: " << usuario_selecionado.indice_coluna << endl;
                    
                    system("pause");
                    break;
                }
                case 2: {
                    int idBusca;
                    mostrar_caixa_informacao("BUSCAR", "Digite o ID do usuario (ou -1 para cancelar):");
                    cin >> idBusca;
                    limparbuffer();
                    
                    if (idBusca > 0) {
                        std::fstream file;
                        bool encontrado = false;
                        
                        if (tipo_usuario == ALUNO) {
                            openFile(file, "alunos.dat");
                            Aluno aluno;
                            file.seekg((idBusca - 1) * sizeof(Aluno));
                            if (file.read((char*)&aluno, sizeof(Aluno)) && aluno.base.id == idBusca) {
                                encontrado = true;
                                string status_atual = aluno.base.ativo ? "Ativo" : "Inativo";
                                mostrar_caixa_informacao("INFO", "Nome: " + string(aluno.base.nome) + "\nStatus: " + status_atual);
                                aluno.base.ativo = !aluno.base.ativo;
                                file.seekp((idBusca - 1) * sizeof(Aluno));
                                file.write((char*)&aluno, sizeof(Aluno));
                                string novo_status = aluno.base.ativo ? "Ativado" : "Desativado";
                                mostrar_caixa_informacao("SUCESSO", "Usuario " + novo_status + " com sucesso!");
                            }
                        } else if (tipo_usuario == PROFESSOR) {
                            openFile(file, "professores.dat");
                            Professor prof;
                            file.seekg((idBusca - 1) * sizeof(Professor));
                            if (file.read((char*)&prof, sizeof(Professor)) && prof.base.id == idBusca) {
                                encontrado = true;
                                string status_atual = prof.base.ativo ? "Ativo" : "Inativo";
                                mostrar_caixa_informacao("INFO", "Nome: " + string(prof.base.nome) + "\nStatus: " + status_atual);
                                prof.base.ativo = !prof.base.ativo;
                                file.seekp((idBusca - 1) * sizeof(Professor));
                                file.write((char*)&prof, sizeof(Professor));
                                string novo_status = prof.base.ativo ? "Ativado" : "Desativado";
                                mostrar_caixa_informacao("SUCESSO", "Usuario " + novo_status + " com sucesso!");
                            }
                        } else if (tipo_usuario == ADMINISTRADOR) {
                            openFile(file, "admin.dat");
                            Admin admin;
                            file.seekg((idBusca - 1) * sizeof(Admin));
                            if (file.read((char*)&admin, sizeof(Admin)) && admin.base.id == idBusca) {
                                encontrado = true;
                                string status_atual = admin.base.ativo ? "Ativo" : "Inativo";
                                mostrar_caixa_informacao("INFO", "Nome: " + string(admin.base.nome) + "\nStatus: " + status_atual);
                                admin.base.ativo = !admin.base.ativo;
                                file.seekp((idBusca - 1) * sizeof(Admin));
                                file.write((char*)&admin, sizeof(Admin));
                                string novo_status = admin.base.ativo ? "Ativado" : "Desativado";
                                mostrar_caixa_informacao("SUCESSO", "Usuario " + novo_status + " com sucesso!");
                            }
                        }
                        
                        if (!encontrado) {
                            mostrar_caixa_informacao("ERRO", "Usuario nao encontrado!");
                        }
                        file.close();
                    }
                    break;
                }
                case 3:
                    continuar = false;
                    break;
            }
        }
    }

/*
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

*/


void menuCadastroCursos(){
    /*
    Disciplina disciplina_;
    char escolha;
    std::fstream file;
    openFile(file,"disciplinas.dat");

    do {
        system("cls");
        std::cout
        <<"======== Geraciar Disciplina ========\n"
        <<"[1]- Cadastrar Disciplina\n"
        <<"[2]- Autorizar Disciplina\n"
        <<"[3]- Inativar Disciplina\n"
        <<"[4]- Vincular Professor ao Disciplina\n"
        <<"[5]- Mostrar Disciplinas\n"
        <<"======================================\n"
        <<"[x]- Sair\n";
        std::cin>>escolha;
        limparbuffer();
        switch (escolha)
        {
        case '1':{
            system("cls");
            std::cout<<"==== Cadastrar Disciplina ====\n\n"
            <<"Nome: ";
            std::cin.getline(disciplina_.nome,30);
            std::cout<<"vagas: ";
            std::cin>>disciplina_.vagas;
            limparbuffer();
            std::cout<<"Carga horaria: ";
            std::cin>>disciplina_.cargaHoraria;
            limparbuffer();

            file.seekp(0,std::ios::end);
            disciplina_.id = 1 + file.tellp() / sizeof(Disciplina);
            disciplina_.ativo = 1;
            disciplina_.idProfessor = 0;
            file.write((char*)&disciplina_,sizeof(Disciplina));

            std::cout<<"Id da disciplina: "<<disciplina_.id<<"\n";
            std::cout<<"Disciplina cadastrada com sucesso\n";
            file.clear();
            break;
        }
        case '2':{
            system("cls");
            std::cout<<"===== Autorizar Disciplina =====\n";

                int buscaId;
                std::cout<<"Digite o id da disciplina\n: ";
                std::cin>>buscaId;
                limparbuffer();


                disciplina_ = buscaDisciplina(file,buscaId);

                if(disciplina_.ativo == 0){

                disciplina_.ativo = 1;


                file.seekp((buscaId-1)*sizeof(Disciplina));
                file.write((char*)&disciplina_,sizeof(Disciplina));
                file.clear();
                std::cout<<"Disciplina autorizada com sucesso!\n";
                }
                else if(disciplina_.ativo==1&&disciplina_.id!=0)
                    std::cout<<"Disciplina se encontra ativa!\n";

                break;
            }

        case '3':{
            system("cls");
            std::cout<<"===== Inativar Disciplina =====\n";

            int buscaId;
            std::cout<<"Digite o id da disciplina\n: ";
            std::cin>>buscaId;
            limparbuffer();

            disciplina_ = buscaDisciplina(file,buscaId);

            if(disciplina_.ativo == 1){

                disciplina_.ativo = 0;

                file.seekp((buscaId-1)*sizeof(Disciplina));
                file.write((char*)&disciplina_,sizeof(Disciplina));
                file.clear();
                std::cout<<"Disciplina inativada com sucesso!\n";
            }
            else if(disciplina_.ativo==0&&disciplina_.id!=0){
                std::cout<<"Disciplina se encontra inativa!\n";
            }

                break;
            }


        case '4':{
            system("cls");
            std::cout
                <<"===== Vincular Professor =====\n";

            std::fstream File_Prof;
            openFile(File_Prof,"professores.dat");
            int buscaId;
            int prof_Id;
            std::cout<<"Digite o id da disciplina\n: ";
            std::cin>>buscaId;
            limparbuffer();

            disciplina_ = buscaDisciplina(file,buscaId);

            if(disciplina_.ativo == 0 && disciplina_.id != 0)
                std::cout<<"Disciplina nao pode ser inativa!\n";

            else if(disciplina_.ativo  && disciplina_.idProfessor == 0){
                std::cout
                <<"Digite o Id do professor\n: ";
                std::cin>>prof_Id;
                limparbuffer();
                Professor prof = buscaProf(File_Prof,prof_Id);
                if(prof.base.id!=0 && prof.disciplina[0]!='\0'){
                    disciplina_.idProfessor = prof.base.id;
                  strcpy(prof.disciplina,disciplina_.nome);

                  file.seekp((buscaId-1)*sizeof(Disciplina));
                  file.write((char*)&disciplina_,sizeof(Disciplina));

                  File_Prof.seekp((prof_Id-1)*sizeof(Professor));
                  File_Prof.write((char*)&prof,sizeof(Professor));
                  std::cout<<"Prof: "<<prof.base.nome<<std::endl;
                  std::cout<<"Vinculado a disciplina: "<<disciplina_.nome<<std::endl;
                  file.clear();
                  File_Prof.clear();
                }

                else if(prof.base.id!=0) std::cout<<"professor ja possui disciplina\n";
            }
            else std::cout<<"Tente novamente!\n";


            File_Prof.close();
            break;}

        case '5': {
            system("cls");
            std::cout << "\n===== LISTA DE DISCIPLINAS =====\n\n";

            file.seekg(0);
            while (file.read((char*)&disciplina_, sizeof(Disciplina))) {

                if (disciplina_.id!=0) {
                    std::cout << "ID: " << disciplina_.id << "\n"
                     << "Nome: " << disciplina_.nome << "\n"
                    << "Ativa: " << (disciplina_.ativo == 0? "nao": "sim") << "\n"
                     << "Carga Horaria: " << disciplina_.cargaHoraria << "h\n"
                     << "ID Professor: "
                     << (disciplina_.idProfessor == 0 ? "Sem professor" : std::to_string(disciplina_.idProfessor))<<"\n"
                     << "Vagas: " << disciplina_.vagas << "\n"
                     << "-----------------------------\n";
                }
            }
            file.clear();

            break;
            }
            case 'x':
                break;

            default:std::cout<<"Invalido!";
            break;

        }
        pausar();
        system("cls");

    }while(escolha != 'X' && escolha != 'x');
    file.close();
*/
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


    void menuCadastroInstrumentos() {
        system("cls || clear");
        cout << "Dentro de menuCadastroInstrumentos" << endl;
        // Lógica para cadastro de instrumentos
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
        "Relatorio Financeiro",
        "Relatorio Patrimonial",
        "Relatorios Academicos",
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
            Modulo_admin::menuCadastroCursos(); 
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
            Modulo_admin::menuEventos(file);
            file.close();
            break;
        }
        case 5: 
            Modulo_admin::menuCadastroInstrumentos(); 
            break;
        case 6: 
            Lanchonete::menuCadastroProdutos(); 
            break;
        case 7: 
            Modulo_admin::menuGerenciarUsuarios(); 
            break;
        case 8: 
            Modulo_admin::realizarBackup(); 
            break;
        case 9: 
            Modulo_admin::gerarRelatorioFinanceiro(); 
            break;
        case 10: 
            Modulo_admin::gerarRelatorioPatrimonial(); 
            break;
        case 11: 
            Modulo_admin::consultarRelatoriosAcademicos(); 
            break;
        case 12: 
            Modulo_admin::restaurarBackup(); 
            break;
        case 13: 
            usuario->logado = false; 
            break;
    }
    

}

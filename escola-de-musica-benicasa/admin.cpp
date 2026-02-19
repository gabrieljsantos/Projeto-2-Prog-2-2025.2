#include"headers.h"
#include"admin.h"
 

namespace mod_ADM{

    void limparbuffer(){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void pausar() {
        std::cout << "\nPressione ENTER para continuar...";
        std::cin.get();
        limparbuffer();
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

    Disciplina buscaDisciplina(std::fstream &file, int buscaId ){
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
    
    Instrumento buscarInstrumento(std::fstream &file, int buscaId ){
        Instrumento inst{};
        file.clear();
        file.seekg(0, std::ios::end);
        int tamanho = file.tellg();
        int total = tamanho / sizeof(Instrumento);
        if (buscaId <= 0 || buscaId > total){
            std::cout << "ID invalido!\n";
            return inst;
        }
        std::cout<< (char)buscaId;
        file.seekg((buscaId - 1) * sizeof(Instrumento));
        file.read((char*)&inst, sizeof(Instrumento));
        if (inst.id == buscaId){
            std::cout
            <<"================================\n"
            <<"Nome: "<<inst.nome<<"\n";
        
            return inst;
        }
        else{
            std::cout << "Nenhum instrumento com esse ID!\n";
            return inst;
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

    Aluno buscaAluno(std::fstream &file,int buscaId){
        Aluno aluno{};
        file.clear();
        file.seekg(0, std::ios::end);
        int tamanho = file.tellg();
        int total = tamanho / sizeof(Aluno);
        
        if(buscaId <= 0 || buscaId > total){
            std::cout<<"ID invalido!\n";
            return aluno;
        }
        file.seekg((buscaId-1)*sizeof(Aluno));
        file.read((char*)&aluno,sizeof(Aluno));
        if(aluno.base.id == buscaId&&aluno.base.ativo){
            std::cout
            <<"================================\n"
            <<"Nome: "<<aluno.base.nome<<"\n"
            <<"E-mail: "<<aluno.base.email<<"\n";
            return aluno;
        }
        else{
            std::cout<<"Nenhum Aluno com esse ID ou Inativo!\n";
            return aluno;
        }
    }

    Produto buscaProduto(std::fstream &file,int buscaId){
        Produto p{};
        file.clear();
        file.seekg(0, std::ios::end);
        int tamanho = file.tellg();
        int total = tamanho / sizeof(Produto);
        
        if(buscaId <= 0 || buscaId > total){
            std::cout<<"ID invalido!\n";
            p.id = 0;
            return p;
        }
        file.seekg((buscaId-1)*sizeof(Produto));
        file.read((char*)&p,sizeof(Produto));
        if(p.id == buscaId){
            std::cout
            <<"================================\n"
            <<"Nome: "<<p.nome<<"\n"
            <<"QTD: "<<p.estoque<<"\n";
            return p;
        }
        else{
            std::cout<<"Nenhum prosuto com esse ID!\n";
            p.id = 0;
            return p;
        }
    }

    int gerarNovoId(std::fstream &file, size_t tamanhoStruct){
        
        file.clear();
        file.seekg(0, std::ios::end);

        std::streampos bytes = file.tellg();

        int totalRegistros = bytes / tamanhoStruct;

        return totalRegistros + 1;
    }

  

     void cadastrarTurma(){
        Turma turma;
        Disciplina* disc= new Disciplina;
        Professor* prof = new Professor;
        int buscaId;
        std::fstream file;
        std::fstream fileDisc;
        std::fstream fileProf;
        mod_ADM::openFile(file, "turmas.dat");
        mod_ADM::openFile(fileDisc, "disciplina.dat");
        mod_ADM::openFile(fileProf, "professores.dat");

        turma.id = gerarNovoId(file, sizeof(Turma));
        turma.ativo = 1;

        std::cout << "ID da Disciplina: ";
        std::cin>>buscaId;
        limparbuffer();
        *disc = buscaDisciplina(fileDisc,buscaId);
        if (disc->id ==0){
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            prof = nullptr;
            disc = nullptr;                        
            return;
        }
        turma.idDisciplina = disc->id;

        std::cout << "ID do Professor: ";
        std::cin >>buscaId;
        *prof = buscaProf(fileProf,buscaId);
        if(prof->base.id ==0){
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            prof = nullptr;
            disc = nullptr;            
            return;
        }
        turma.idProfessor = prof->base.id;
        int indexVAGA;
        if(verificaTurmasProf(*prof,indexVAGA)){
            prof->turmas[indexVAGA] = turma.id;

            fileProf.seekp((prof->base.id-1)*sizeof(Professor));
            fileProf.write((const char*)prof,sizeof(Professor));
        }
        else{
            std::cout<<"Professor nao tem vaga para novas turmas.\n";
            file.close();
            fileDisc.close();
            fileProf.close();
            delete prof;
            delete disc;
            prof = nullptr;
            disc = nullptr;
            return;
        }                

        turma.qtdAlunos = 0;

        for(int i = 0; i < MAX_ALUNOS; i++)
            turma.alunos[i] = 0;

        file.seekp((turma.id - 1) * sizeof(Turma));
        file.write((char*)&turma, sizeof(Turma));

        file.close();
        fileProf.close();
        fileDisc.close();

        delete prof;
        delete disc;
        prof = nullptr;
        disc = nullptr;
    }

    void matricularAlunoTurma() {
    std::fstream fileTurma;
    std::fstream fileAluno;
    openFile(fileAluno,"alunos.dat");
    openFile(fileTurma,"turmas.dat");
    Turma *turma_ = new Turma;
    Aluno *aluno_ = new Aluno;
    int idAluno;
    int idTurma;

    std::cout<<"Digite o Id do Aluno: ";
    std::cin>> idAluno;
    limparbuffer();
        *aluno_ = buscaAluno(fileAluno,idAluno);
    if (aluno_->base.id != idAluno || !aluno_->base.ativo) {
        std::cout << "Aluno inativo.\n";
        fileTurma.close();
        fileAluno.close();
        delete aluno_;
        delete turma_;
        aluno_ = nullptr;
        turma_ = nullptr;
        return;
    }

    std::cout<<"Digite o Id da turma: ";
    std::cin>> idTurma;
    limparbuffer();
    
    fileTurma.seekg((idTurma - 1) * sizeof(Turma));
    fileTurma.read((char*)&turma_, sizeof(Turma));

    if (turma_->id != idTurma || !turma_->ativo) {
        std::cout << "Turma invalida ou inativa.\n";
        fileTurma.close();
        fileAluno.close();
        delete aluno_;
        delete turma_;
        aluno_ = nullptr;
        turma_ = nullptr;        
        return;
    }

    if (turma_->qtdAlunos >= MAX_ALUNOS) {
        std::cout << "Turma cheia.\n";
            fileTurma.close();
            fileAluno.close();
            delete aluno_;
            delete turma_;
            aluno_ = nullptr;
            turma_ = nullptr;
        return;
    }

    for (int i = 0; i < MAX_ALUNOS; i++) {
        if (turma_->alunos[i] == aluno_->base.id) {
            std::cout << "Aluno ja matriculado.\n";
            fileTurma.close();
            fileAluno.close();
            delete aluno_;
            delete turma_;
            aluno_ = nullptr;
            turma_ = nullptr;
            return;
        }
    }
    int index;
     for (int i = 0; i < MAX_ALUNOS; i++) {
        if (turma_->alunos[i] == 0) {
            index = i;
            break;;
        }
    }

    turma_->alunos[index] = aluno_->base.id;
    turma_->qtdAlunos++;

   
    fileTurma.seekp((turma_->id - 1) * sizeof(Turma));
    fileTurma.write((char*)&turma_, sizeof(Turma));

    fileTurma.close();
    fileAluno.close();
    delete aluno_;
    delete turma_;
    aluno_ = nullptr;
    turma_ = nullptr;
    std::cout << "Aluno matriculado com sucesso!\n";

        
    }   

    bool verificaTurmasProf(Professor &prof,int &Index_turma){
        for(int i= 0; i<5; i++ ){
            if(prof.turmas[i] == 0){
                Index_turma = i;
                return true;
            }
        }
        return false;
    }

    void menuCadastroCursos(){
        Disciplina disciplina_;
        char escolha;
        std::fstream file;
        openFile(file,"disciplina.dat");
        
        
        do {
            system("cls");
            std::cout
            <<"======== Geraciar Disciplina ========\n"
            <<"[1]- Cadastrar Disciplina\n"
            <<"[2]- Autorizar Disciplina\n"
            <<"[3]- Inativar Disciplina\n"
            <<"[4]- Mostrar Disciplinas\n"
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
                std::cout<<"Carga horaria: ";
                std::cin>>disciplina_.cargaHoraria;
                limparbuffer();
                
                disciplina_.id = gerarNovoId(file,sizeof(Disciplina)); 
                disciplina_.ativo = 1;

                file.seekp((disciplina_.id-1)*sizeof(Disciplina));
                file.write((char*)&disciplina_,sizeof(Disciplina));
                
                std::cout<<"Id da disciplina: "<<disciplina_.id<<"\n";
                std::cout<<"------------------------------------\n";
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
            
            case '4': {
                system("cls");
                std::cout << "\n===== LISTA DE DISCIPLINAS =====\n\n";  
            
                file.seekg(0);
                while (file.read((char*)&disciplina_, sizeof(Disciplina))) {   

                    if (disciplina_.id!=0) {
                        std::cout << "ID: " << disciplina_.id << "\n"
                        << "Nome: " << disciplina_.nome << "\n"
                        << "Ativa: " << (disciplina_.ativo == 0? "nao": "sim") << "\n"
                        << "Carga Horaria: " << disciplina_.cargaHoraria << "h\n"
                        << "-----------------------------\n";
                    }
                }
                file.clear();

                break;
                }
            
            }
            pausar();
            system("cls");

        }while(escolha != 'X' && escolha != 'x');
        file.close();
    }

    void menuEventos() {
        char opcao;
        Evento evento_; 
        std::fstream file;
        mod_ADM::openFile(file,"eventos.txt");

        do {
            system("cls");
            std::cout
            << "\n=========  Eventos  =========\n"
            << "[1]- Cadastrar evento\n"
            << "[2]- Autorizar evento\n"
            << "[3]- Listar eventos\n"
            << "=================================\n"
            << "[x]- Voltar\n"
            << ">>> ";
            std::cin >> opcao;
            limparbuffer();

            switch (opcao) {
                case '1':{
                    system("cls");

                    std::cout << "\n--- Novo Evento ---\n"
                    << "Nome: ";
                    std::cin.getline(evento_.nome, 49);

                    std::cout << "Local do evento: ";
                    std::cin.getline(evento_.local, 49);

                    std::cout << "Descricao: ";
                    std::cin.getline(evento_.descricao, 99);

                    std::cout << "Data do evento [dd/mm/aaaa]: ";
                    std::cin.getline(evento_.data,11);
                    limparbuffer();

                    std::cout << "Total de vagas: ";
                    std::cin >> evento_.totalVagas;
                    limparbuffer();

                    evento_.vagasOcupadas = 0;
                    evento_.autorizado = 0;
                    evento_.ativo = 1;

                    
                    evento_.id = gerarNovoId(file,sizeof(Evento));
                    file.clear();
                    std::cout << "ID do evento: " << evento_.id << "\n";

                    file.seekp((evento_.id-1)*sizeof(Evento));
                    file.write((char*)&evento_, sizeof(Evento));
                    file.clear();

                    std::cout << "------------------------------\n";
                    std::cout << "Evento cadastrado com sucesso!\n";
                    break;
                }
                
                case '2': {
                    system("cls");
                    bool achado = false;
                    int idBusca;
                    std::cout << "Informe o ID do evento: ";
                    std::cin >> idBusca;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


                    file.clear();
                    file.seekg((idBusca-1)*sizeof(Evento));
                    file.read((char*)&evento_,sizeof(Evento));

                        if (evento_.autorizado && evento_.id==idBusca){
                            achado = true;
                        std::cout << "Evento se encontra autorizado.\n";
                    }


                        else if (evento_.id == idBusca && evento_.ativo == 1) {
                            achado = true;
                            std::cout 
                            << "\nID: " << evento_.id << "\n"
                            << "Nome: " << evento_.nome << "\n"
                            << "Local: " << evento_.local << "\n"
                            << "Data: " << evento_.data << "\n";

                            evento_.autorizado = 1;

                            file.seekp((idBusca-1)*sizeof(Evento));
                            file.write((char*)&evento_, sizeof(Evento));
                            std::cout<< "------------------------------\n";
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
                        while (file.read((char*)&evento_, sizeof(Evento))) {
                            if (evento_.ativo  && evento_.autorizado) {
                                vazio = false;
                                std::cout
                                << "\nID: " << evento_.id << "\n"
                                << "Nome: " << evento_.nome << "\n"
                                << "Local: " << evento_.local << "\n"
                                << "Data: " << evento_.data << "\n"
                                << "Descricao: " << evento_.descricao << "\n"
                                << "Vagas: " << evento_.vagasOcupadas << "/" << evento_.totalVagas << "\n"
                                << "Autorizado: " << (evento_.autorizado ? "Sim" : "Nao") << "\n"
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
                        while (file.read((char*)&evento_, sizeof(Evento))) {
                            if (evento_.ativo && !evento_.autorizado) {
                                vazio = false;
                                std::cout
                                << "\nID: " << evento_.id << "\n"
                                << "Nome: " << evento_.nome << "\n"
                                << "Local: " << evento_.local << "\n"
                                << "Data: " << evento_.data << "\n"
                                << "Descricao: " << evento_.descricao << "\n"
                                << "Vagas: " << evento_.vagasOcupadas << "/" << evento_.totalVagas << "\n"
                                << "Autorizado: " << (evento_.autorizado ? "Sim" : "Nao") << "\n"
                                << "================================\n";
                            }
                        }                                           

                        file.clear();   

                        if (vazio)
                            std::cout << "Nenhum evento cadastrado.\n";

                        break;
                    }
                }

            }

            pausar();
            system("cls");

        } while (opcao != 'X' && opcao != 'x' );
        file.close();

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

    void cadastrarProdutos(){
    Produto  item;
    item.id=0; 
    std::fstream fileProduto;
    mod_ADM::openFile(fileProduto,"produtos.dat");
    char escolha;
    do{   
        system("cls");
        std::cout
        <<"================================\n"
        <<"[1]- Cadastrar produto\n"
        <<"[2]- Ativar produto\n"
        <<"[3]- Inativar produto\n"
        <<"[4]- Editar produto\n"
        <<"[5]- Listar produtos\n"
        <<"================================\n"
        <<"[X]- Sair\n";
        std::cin>>escolha;
        limparbuffer();
        
        switch (escolha)
        {
        case '1': {
            system("cls");
            std::cout << "===== Cadastrar Produto =====\n\n";

            std::cout << "Nome: ";
            std::cin.getline(item.nome, 50);

            std::cout << "Preco: ";
            std::cin >> item.preco;
            limparbuffer();

            std::cout << "Estoque: ";
            std::cin >> item.estoque;
            limparbuffer();

            fileProduto.clear();

            item.id = gerarNovoId(fileProduto, sizeof(Produto));
            item.ativo = 1;

            fileProduto.seekp((item.id-1)*sizeof(Produto));
            fileProduto.write((char*)&item, sizeof(Produto));
    
            std::cout << "\nProduto cadastrado com sucesso!\n";
            std::cout << "ID gerado: " << item.id << "\n";
            
            break;
        }

        case '2':{
            system("cls");
            int buscaId;
            std::cout<<"======= Ativar Produtos =======\n\n";
            std::cout<<"Digite o ID do produto\n";
            std::cin>>buscaId;
            limparbuffer();

            item = buscaProduto(fileProduto,buscaId);
            
            if(item.ativo ==1)
                std::cout<<"Produto se encontra ativo\n";
            
            else if(item.ativo == 0){
                char rspt;
                std::cout<<"Deseja ativar? \n[S] para ativar: ";
                std::cin>>rspt;
                limparbuffer();
                if (rspt=='S'||rspt =='s'){
                    item.ativo = 1;
                    fileProduto.clear();
                    fileProduto.seekp((buscaId-1)*sizeof(Produto));
                    fileProduto.write((char*)&item,sizeof(Produto));
                    std::cout<<item.nome<<" ativado!\n";
                }
                else std::cout<<"Saindo...\n";
            }
            break;
        }
        
        case '3':{
            system("cls");
            int buscaId;
            std::cout<<"======= Inativar Produtos =======\n\n";
            std::cout<<"Digite o ID do produto\n";
            std::cin>>buscaId;
            limparbuffer();

            item = buscaProduto(fileProduto,buscaId);
            
            if(item.ativo ==0)
                std::cout<<"Produto se encontra inativo\n";
            
            else if(item.ativo == 1){
                char rspt;
                std::cout<<"Deseja inativar? \n[S] para inativar: ";
                std::cin>>rspt;
                limparbuffer();
                if (rspt=='S'||rspt =='s'){
                    item.ativo = 0;
                    fileProduto.clear();
                    fileProduto.seekp((buscaId-1)*sizeof(Produto));
                    fileProduto.write((char*)&item,sizeof(Produto));
                    std::cout<<item.nome<<" inativo!\n";
                }
                else std::cout<<"Saindo...\n";
            }
            break;}
        
        case '4':{
            system("cls");
            int buscaId;
            std::cout<<"======= Editar Produtos =======\n\n";
            std::cout<<"Digite o ID do produto\n";
            std::cin>>buscaId;
            limparbuffer();
            char rspt = 'n';
            item = buscaProduto(fileProduto,buscaId);
            if(item.ativo ==1 &&item.id!= 0){
                std::cout<<"Preco: "<<item.preco<<"\n";
                std::cout<<"Deseja Editar? \n[S] para editar: ";
                std::cin>>rspt;
                limparbuffer();
            
                if (rspt=='S'|| rspt =='s'){
                std::cout<<"\nNome: ";
                std::cin.getline(item.nome,50);
                std::cout<<"Preco: ";
                std::cin>>item.preco;
                limparbuffer();
                std::cout<<"QTD: ";
                std::cin>>item.estoque;
                limparbuffer();
                fileProduto.clear();
                fileProduto.seekp((item.id-1)*sizeof(Produto));
                fileProduto.write((char*)&item,sizeof(Produto));
                std::cout<<item.nome<<" Editado!\n";
                }
                else std::cout<<"Saindo...\n";
            }
            else std::cout<<"Saindo...\n";
            
            pausar();
            break;}
        
        case '5':{
            system("cls");
            fileProduto.clear();
            fileProduto.seekg(0);

            std::cout
            << "======= Lista de Produtos =======\n\n";

            while(fileProduto.read((char*)&item, sizeof(Produto))){
                if(item.id != 0){
                    std::cout
                    << "ID: " << item.id << "\n"
                    << "Nome: " << item.nome << "\n"
                    << "Preco: R$ " << item.preco << "\n"
                    << "Estoque: " << item.estoque << "\n"
                    << "Estado: " << (item.ativo ? "ativo":"inativo") << "\n"
                    << "---------------------------------\n";
                }
            }

            break;
        }            
    
    
        }
        pausar();
    }while (escolha !='x'&&escolha !='X');
    
}

    void menuInstrumentos() {
    char opcao;
    std::fstream file;
    openFile(file,"instrumentos.dat");

    do {
        system("cls");

        std::cout << "======= GERENCIAR INSTRUMENTOS =======\n\n";
        std::cout << "[1] - Cadastrar Instrumento\n";
        std::cout << "[2] - Autorizar Instrumento\n";
        std::cout << "[3] - Desautorizar Instrumento\n";
        std::cout << "[4] - Atualizar Estoque\n";
        std::cout << "[5] - Inativar Instrumento\n";
        std::cout << "[6] - Listar Instrumentos\n";
        std::cout << "[0] - Voltar\n\n";
        std::cout << "Escolha: ";
        std::cin >> opcao;
        std::cin.ignore();

        switch (opcao) {

            case '1':{
            Instrumento inst;

            file.clear();
            file.seekp(0, std::ios::end);

            inst.id = gerarNovoId(file,sizeof(Instrumento));
            inst.ativo = 1;
            inst.autorizado = 0;      // começa não autorizado
            inst.disponivel = false;  // começa indisponível
            inst.estoque = 0;

            std::cout << "Nome do instrumento: ";
            std::cin.getline(inst.nome, 30);

            file.write((char*)&inst, sizeof(Instrumento));

            std::cout << "\nInstrumento cadastrado com sucesso!\n";
            
                break;
            }

            case '2': {
                int id;
                std::cout << "Digite o ID para autorizar: ";
                std::cin >> id;
                Instrumento inst = buscarInstrumento(file, id);

                if (inst.id == 0) {
                    break;
                }

                inst.autorizado = 1;

                file.seekp((id - 1) * sizeof(Instrumento));
                file.write((char*)&inst, sizeof(Instrumento));

                std::cout << "Instrumento autorizado!\n";
                break;
            }

            case '3': {
                int id;
                std::cout << "Digite o ID para desautorizar: ";
                std::cin >> id;
                            
                Instrumento inst = buscarInstrumento(file, id);

                if (inst.id == 0) {
                    break;
                }

                inst.autorizado = 0;

                file.seekp((id - 1) * sizeof(Instrumento));
                file.write((char*)&inst, sizeof(Instrumento));

                std::cout << "Instrumento desautorizado!\n";

                break;
            }

            case '4': {
                int id, estoque;
                std::cout << "Digite o ID: ";
                std::cin >> id;
                std::cout << "Novo estoque: ";
                std::cin >> estoque;
                Instrumento inst = buscarInstrumento(file, id);

                if (inst.id == 0) {
                    break;
                }

                inst.estoque = estoque;

                // Atualiza disponibilidade automaticamente
                inst.disponivel = (inst.estoque> 0);

                file.seekp((id - 1) * sizeof(Instrumento));
                file.write((char*)&inst, sizeof(Instrumento));

                std::cout << "Estoque atualizado!\n";
            
                break;
            }

            case '5': {
                int id;
                std::cout << "Digite o ID para inativar: ";
                std::cin >> id;
                 
                Instrumento inst = buscarInstrumento(file, id);

                if (inst.id == 0) {
                    break;;
                }

                inst.ativo = 0;

                file.seekp((id - 1) * sizeof(Instrumento));
                file.write((char*)&inst, sizeof(Instrumento));

                std::cout << "Instrumento removido!\n";
            
                break;
            }

            case '6':{
                Instrumento inst;

                file.clear();
                file.seekg(0);

                std::cout << "\n====== LISTA DE INSTRUMENTOS ======\n\n";

                while (file.read((char*)&inst, sizeof(Instrumento))) {

                    if (inst.ativo == 1) {

                        std::cout << "ID: " << inst.id << "\n";
                        std::cout << "Nome: " << inst.nome << "\n";
                        std::cout << "Autorizado: " << (inst.autorizado ? "Sim" : "Nao") << "\n";
                        std::cout << "Disponivel: " << (inst.disponivel ? "Sim" : "Nao") << "\n";
                        std::cout << "Estoque: " << inst.estoque << "\n";
                        std::cout << "-----------------------------\n";
                    }
                }
            
                break;
            }
            default:
                std::cout << "Opcao invalida!\n";
        }

        if (opcao != '0') {
            pausar();
        }

    } while (opcao != '0');

    file.close();
}

}


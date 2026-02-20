#include "instrumentos.h"
#include "admin.h"
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// --- Variáveis Globais do Módulo ---
const int MAX_INSTRUMENTOS = 100; // Limite do array
Instrumento listaInstrumentos[MAX_INSTRUMENTOS];
int quantidadeInstrumentos = 0;

const char* ARQUIVO_DADOS = "instrumentos.dat";

// --- Funções Auxiliares de Arquivo (Internas) ---

// Função para carregar dados do arquivo binário para a memória
void carregarDadosInstrumentos() {
    ifstream file(ARQUIVO_DADOS, ios::binary);
    if (!file.is_open()) return; // Se não existir, não faz nada

    // Lê a quantidade de registros
    file.read((char*)&quantidadeInstrumentos, sizeof(int));

    // Lê o array inteiro
    if (quantidadeInstrumentos > 0) {
        file.read((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// Função para salvar dados da memória para o arquivo binário
void salvarDadosInstrumentos() {
    ofstream file(ARQUIVO_DADOS, ios::binary | ios::trunc);
    if (!file.is_open()) {
        cout << "Erro ao salvar arquivo de instrumentos!" << endl;
        return;
    }

    // Salva a quantidade primeiro
    file.write((char*)&quantidadeInstrumentos, sizeof(int));
    
    // Salva o array
    if (quantidadeInstrumentos > 0) {
        file.write((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// --- Inicialização e Finalização ---

void inicializarInstrumentos() {
    carregarDadosInstrumentos();
}
void finalizarInstrumentos() {
    salvarDadosInstrumentos();
}

// --- Funções de Busca Auxiliares ---

int buscarIndicePorId(int id) {
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        if (listaInstrumentos[i].id == id && listaInstrumentos[i].ativo == 1) {
            return i;
        }
    }
    return -1; // Não encontrado
}

// --- CRUD ---

void cadastrarInstrumento() {
    if (quantidadeInstrumentos >= MAX_INSTRUMENTOS) {
        cout << "Limite de instrumentos atingido!" << endl;
        return;
    }

    Instrumento novo;
    
    // Geração de ID sequencial simples
    novo.id = (quantidadeInstrumentos == 0) ? 1 : listaInstrumentos[quantidadeInstrumentos - 1].id + 1;
    novo.ativo = 1;
    novo.autorizado = 1; 
    
    cout << "\n--- Cadastro de Instrumento ---" << endl;
    cout << "Nome: ";
    cin.ignore(); // Limpa buffer residual
    cin.getline(novo.nome, TAM_NOME);
    
    cout << "Quantidade em Estoque: ";
    cin >> novo.estoque;
    
    novo.disponivel = (novo.estoque > 0);
    novo.idAluno = 0; 

    listaInstrumentos[quantidadeInstrumentos] = novo;
    quantidadeInstrumentos++;
    
    salvarDadosInstrumentos(); // Salva imediatamente após cadastro
    cout << "Instrumento cadastrado com sucesso! ID: " << novo.id << endl;
}

void listarInstrumentos() {
    cout << "\n--- Lista de Instrumentos ---" << endl;
    if (quantidadeInstrumentos == 0) {
        cout << "Nenhum instrumento cadastrado." << endl;
        return;
    }

    for (int i = 0; i < quantidadeInstrumentos; i++) {
        Instrumento& it = listaInstrumentos[i];
        if (it.ativo == 1) {
            cout << "ID: " << it.id 
                 << " | Nome: " << it.nome 
                 << " | Estoque: " << it.estoque 
                 << " | Disponivel: " << (it.disponivel ? "Sim" : "Nao") << endl;
        }
    }
}

void excluirInstrumento() {
    int id;
    cout << "Digite o ID do instrumento a excluir: ";
    cin >> id;

    int idx = buscarIndicePorId(id);
    if (idx != -1) {
        listaInstrumentos[idx].ativo = 0; // Exclusão lógica
        salvarDadosInstrumentos();
        cout << "Instrumento excluido com sucesso." << endl;
    } else {
        cout << "Instrumento nao encontrado." << endl;
    }
}

// --- Editar Instrumento ---

void editarInstrumento() {
    int id;
    cout << "\n--- Editar Instrumento ---" << endl;
    cout << "Digite o ID do instrumento: ";
    cin >> id;

    int idx = buscarIndicePorId(id);
    if (idx == -1) {
        cout << "Instrumento nao encontrado." << endl;
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];

    cout << "Dados atuais:" << endl;
    cout << "  Nome: " << inst.nome << endl;
    cout << "  Estoque: " << inst.estoque << endl;

    cout << "\nNovo Nome (ou ENTER para manter): ";
    cin.ignore();
    char novoNome[TAM_NOME];
    cin.getline(novoNome, TAM_NOME);
    if (strlen(novoNome) > 0) {
        strcpy(inst.nome, novoNome);
    }

    cout << "Novo Estoque (-1 para manter): ";
    int novoEstoque;
    cin >> novoEstoque;
    if (novoEstoque >= 0) {
        inst.estoque = novoEstoque;
        inst.disponivel = (inst.estoque > 0);
    }

    salvarDadosInstrumentos();
    cout << "Instrumento atualizado com sucesso!" << endl;
}

// --- Salvar Empréstimo ---

void salvarEmprestimo(Emprestimo emp) {
    std::ofstream file("emprestimos.dat", std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        cout << "Erro ao salvar emprestimo!" << endl;
        return;
    }
    file.write((char*)&emp, sizeof(Emprestimo));
    file.close();
}

// --- Lógica de Empréstimo ---

void realizarEmprestimo() {
    int idInstrumento, idAluno;
    cout << "\n--- Emprestimo ---" << endl;
    cout << "ID do Instrumento: "; cin >> idInstrumento;
    cout << "ID do Aluno: "; cin >> idAluno;

    // 1. Validar Instrumento
    int idx = buscarIndicePorId(idInstrumento);
    if (idx == -1) {
        cout << "Erro: Instrumento nao encontrado." << endl;
        return;
    }
    
    Instrumento& inst = listaInstrumentos[idx];

    if (inst.estoque <= 0) {
        cout << "Erro: Estoque esgotado." << endl;
        return;
    }

    // 2. Validar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);

    if (aluno.base.id == 0) {
        cout << "Erro: Aluno nao encontrado no sistema." << endl;
        return;
    }

    // 3. Regra: Aluno já tem instrumento?
    if (aluno.idInstrumento != 0) {
        cout << "Erro: Este aluno ja possui um instrumento emprestado." << endl;
        return;
    }

    // 4. Executar Empréstimo
    inst.estoque--;
    if (inst.estoque == 0) inst.disponivel = false;
    inst.idAluno = idAluno;

    aluno.idInstrumento = inst.id;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();


    Emprestimo emp;

    // Gerar ID simples baseado em timestamp
    emp.idEmprestimo = time(NULL);

    emp.idAluno = aluno.base.id;
    strcpy(emp.nome_Alu, aluno.base.nome);

    emp.idInstrumento = inst.id;
    strcpy(emp.nome_In, inst.nome);

    // Data atual
    time_t t = time(NULL);
    tm *tmPtr = localtime(&t);

    sprintf(emp.dataEmprestimo, "%02d/%02d/%04d",
            tmPtr->tm_mday,
            tmPtr->tm_mon + 1,
            tmPtr->tm_year + 1900);

    // Data prevista (7 dias depois - simples)
    sprintf(emp.dataPrevista, "%02d/%02d/%04d",
            tmPtr->tm_mday + 7,
            tmPtr->tm_mon + 1,
            tmPtr->tm_year + 1900);

    salvarEmprestimo(emp);

    cout << "Emprestimo realizado e registrado com sucesso!" << endl;
}

void realizarDevolucao() {
    int idAluno;
    cout << "\n--- Devolucao ---" << endl;
    cout << "ID do Aluno: "; cin >> idAluno;

    // 1️⃣ Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        cout << "Erro: Aluno nao encontrado." << endl;
        return;
    }

    if (aluno.idInstrumento == 0) {
        cout << "Erro: Este aluno nao possui instrumento para devolver." << endl;
        return;
    }

    // 2️⃣ Encontrar Instrumento
    int idx = buscarIndicePorId(aluno.idInstrumento);
    if (idx == -1) {
        cout << "Erro critico: Instrumento nao encontrado." << endl;
        return;
    }

    Instrumento& inst = listaInstrumentos[idx];

    // 3️⃣ Atualizar Instrumento
    inst.estoque++;
    inst.disponivel = true;
    inst.idAluno = 0;

    // 4️⃣ Atualizar Aluno
    aluno.idInstrumento = 0;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();

    // =====================================
    // 📌 ZERAR REGISTRO NO ARQUIVO
    // =====================================

    std::fstream file("emprestimos.dat",
                      std::ios::binary | std::ios::in | std::ios::out);

    if (file.is_open()) {

        Emprestimo emp;

        while (file.read((char*)&emp, sizeof(Emprestimo))) {

            if (emp.idAluno == idAluno) {

                // Volta o ponteiro para sobrescrever
                file.seekp(-sizeof(Emprestimo), std::ios::cur);

                Emprestimo vazio = {0};  // struct totalmente zerada

                file.write((char*)&vazio, sizeof(Emprestimo));

                break;
            }
        }

        file.close();
    }

    cout << "Devolucao realizada e registro zerado com sucesso!" << endl;
}

// --- Menu Principal de Instrumentos ---

void menuInstrumentos() {
    int opcao;

    do {
        cout << "\n===== INSTRUMENTOS =====" << endl;
        cout << "[1] Cadastrar Instrumento" << endl;
        cout << "[2] Editar Instrumento" << endl;
        cout << "[3] Realizar Emprestimo" << endl;
        cout << "[4] Listar Instrumentos" << endl;
        cout << "[5] Excluir Instrumento" << endl;
        cout << "[6] Realizar Devolucao" << endl;
        cout << "[0] Voltar" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: cadastrarInstrumento();  break;
            case 2: editarInstrumento();     break;
            case 3: realizarEmprestimo();    break;
            case 4: listarInstrumentos();    break;
            case 5: excluirInstrumento();    break;
            case 6: realizarDevolucao();     break;
            case 0: cout << "Voltando..." << endl; break;
            default: cout << "Opcao invalida!" << endl; break;
        }
    } while (opcao != 0);
}
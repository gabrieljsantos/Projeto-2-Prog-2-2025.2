#include <iostream>
#include <fstream>
#include <cstring>
#include <locale.h>
#include "servico_cadastros.h"
#include "../utilidades.h"
#include "../headers.h"

using namespace std;

void inicializar_banco_dados() {
    const char* nome_arquivo = "database/usuarios.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    // Se o arquivo já existe, não fazer nada
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    // Arquivo não existe, criar e adicionar usuário ROOT
    Identidade usuario_root;
    usuario_root.id = 0;                              // ID = 0000
    usuario_root.ativo = 1;
    usuario_root.categoria = ADMIN;
    strcpy(usuario_root.nome, "USUARIO_ROOT");
    strcpy(usuario_root.cpf, "000.000.000-00");
    strcpy(usuario_root.email, "root@sistema.edu");
    strcpy(usuario_root.senha, "0000");               // Senha = "0000"
    strcpy(usuario_root.especialidade, "");
    
    ofstream arquivo(nome_arquivo, ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(&usuario_root), sizeof(Identidade));
        arquivo.close();
    }
}

void obter_caminho_arquivo(funcao_pessoa categoria, char* caminho) {
    // Banco de dados único para todos os usuários
    strcpy(caminho, "database/usuarios.dat");
}

int obter_proximo_id() {
    const char* nome_arquivo = "database/usuarios.dat";
    ifstream arquivo(nome_arquivo, ios::binary);
    
    int ultimo_id = -1; // ID inicial se arquivo não existir (primeiro será 0000)
    Identidade usuario;
    
    if (arquivo.is_open()) {
        while (arquivo.read(reinterpret_cast<char*>(&usuario), sizeof(Identidade))) {
            if (usuario.id > ultimo_id) {
                ultimo_id = usuario.id;
            }
        }
        arquivo.close();
    }
    
    return ultimo_id + 1;
}

bool salvar_identidade(const char* nome_arquivo, const Identidade& usuario) {
    ofstream arquivo(nome_arquivo, ios::binary | ios::app);
    
    if (!arquivo.is_open()) {
        cout << "Erro: Não foi possível abrir o arquivo de banco de dados.\n";
        return false;
    }
    
    arquivo.write(reinterpret_cast<const char*>(&usuario), sizeof(Identidade));
    arquivo.close();
    
    cout << "Usuário cadastrado com sucesso!\n";
    return true;
}

void listar_usuarios(const char* nome_arquivo) {
    ifstream arquivo(nome_arquivo, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhum usuário cadastrado ainda.\n";
        return;
    }
    
    Identidade usuario;
    int contador = 0;
    
    cout << "\n========== USUÁRIOS CADASTRADOS ==========\n";
    while (arquivo.read(reinterpret_cast<char*>(&usuario), sizeof(Identidade))) {
        if (usuario.ativo) {
            contador++;
            printf("ID: %04d | Nome: %s | CPF: %s | Email: %s\n", usuario.id, usuario.nome, usuario.cpf, usuario.email);
        }
    }
    
    if (contador == 0) {
        cout << "Nenhum usuário cadastrado.\n";
    } else {
        cout << "Total de usuários ativos: " << contador << "\n";
    }
    
    cout << "==========================================\n\n";
    arquivo.close();
}

bool gerenciar_cadastro_usuario(funcao_pessoa categoria) {
    int opcao = -1;
    char caminho[100];
    
    obter_caminho_arquivo(categoria, caminho);
    
    while (opcao != 0) {
        limpar_tela();
        cout << "----------------------------------------\n";
        cout << "Cadastro de Usuários - Categoria: " << categoria << "\n";
        cout << "----------------------------------------\n";
        cout << "\n";
        cout << "#  1 - Novo Cadastro\n";
        cout << "#  2 - Listar Cadastros\n";
        cout << "#  3 - Modificar Cadastro\n";
        cout << "#  0 - Voltar\n";
        cout << "\n";
        cout << "Escolha uma opção: ";
        
        cin >> opcao;
        cin.ignore();
        
        switch (opcao) {
            case 1: {
                // Novo cadastro
                limpar_tela();
                Identidade novo_usuario;
                
                // ID = último ID + 1
                novo_usuario.id = obter_proximo_id();
                novo_usuario.ativo = 1;
                novo_usuario.categoria = categoria;
                
                cout << "========== NOVO CADASTRO ==========\n";
                cout << "Nome completo: ";
                cin.getline(novo_usuario.nome, sizeof(novo_usuario.nome));
                
                cout << "CPF: ";
                cin.getline(novo_usuario.cpf, sizeof(novo_usuario.cpf));
                
                cout << "Email: ";
                cin.getline(novo_usuario.email, sizeof(novo_usuario.email));
                
                // Senha padrão
                strcpy(novo_usuario.senha, "senha");
                
                // Se for professor, pedir especialidade
                if (categoria == PROFESSOR) {
                    cout << "Especialidade (Ex: Piano, Canto): ";
                    cin.getline(novo_usuario.especialidade, sizeof(novo_usuario.especialidade));
                } else {
                    strcpy(novo_usuario.especialidade, "");
                }
                
                if (salvar_identidade(caminho, novo_usuario)) {
                    cout << "Pressione Enter para continuar...";
                    cin.get();
                }
                break;
            }
            
            case 2: {
                // Listar cadastros
                listar_usuarios(caminho);
                cout << "Pressione Enter para continuar...";
                cin.get();
                break;
            }
            
            case 3: {
                // Modificar cadastro (implementação simplificada)
                cout << "Funcionalidade de modificação em desenvolvimento.\n";
                cout << "Pressione Enter para continuar...";
                cin.get();
                break;
            }
            
            case 0:
                break;
                
            default:
                cout << "Opção inválida!\n";
                cout << "Pressione Enter para continuar...";
                cin.get();
        }
    }
    
    return true;
}

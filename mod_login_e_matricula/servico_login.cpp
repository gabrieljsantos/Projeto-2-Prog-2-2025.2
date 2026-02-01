#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "servico_login.h"
#include "../utilidades.h"
#include "../headers.h"

using namespace std;


login_info janela_login() {
    login_info info;
    info.status = ID_OU_SENHA_INVALIDO; // Inicializar como inválido
    
    while (info.status != VALIDO) {
        limpar_tela();
        cout << "----------------------------------------\n";
        cout << "> Login/\n";
        cout << "----------------------------------------\n";
        cout << "\n";
        cout << "#########  JANELA DE LOGIN  ###########\n";
        cout << "########################################\n";
        cout << "#                                      #\n";
        cout << "#  Escolha o tipo de login:            #\n";
        cout << "#  1 - Fazer login com ID              #\n";
        cout << "#  2 - Fazer login com Email           #\n";
        cout << "#  0 - Voltar                          #\n";
        cout << "#                                      #\n";
        cout << "########################################\n";
        cout << "\n";
        cout << "Escolha uma opção: ";
        
        int tipo_login;
        cin >> tipo_login;
        cin.ignore();
        
        if (tipo_login == 0) {
            // Voltar sem fazer login (retornar com status inválido)
            return info;
        }
        
        if (tipo_login < 1 || tipo_login > 2) {
            cout << "Opção inválida!\n";
            system("pause");
            continue;
        }
        
        char credencial[100];
        char senha[30];
        
        // Pedir credencial baseado no tipo de login
        if (tipo_login == 1) {
            cout << "Digite seu ID de usuário: ";
            cin.getline(credencial, sizeof(credencial));
        } else if (tipo_login == 2) {
            cout << "Digite seu email: ";
            cin.getline(credencial, sizeof(credencial));
        }
        
        cout << "Digite sua senha: ";
        cin.getline(senha, sizeof(senha));

        cout << "\nFazendo verificação de login...\n";
        info = validar_login_dinamico(tipo_login, credencial, senha);

        if (info.status == VALIDO) {
            cout << "Login bem-sucedido! Bem-vindo! " << ".\n";
            cout << "\n";
            cout << "identidade do usuario: \n";
            cout << "Nome: " << info.info_basica.nome << "\n"; 
            cout << "CPF: " << info.info_basica.cpf << "\n";

            cout << "Categoria: " << enumerado_para_texto(info.info_basica.categoria) << "\n"; 
            
            if (strcmp(info.info_basica.especialidade, "null") != 0) { 
                cout << "Especialidade: " << info.info_basica.especialidade << "\n" << "\n" << "\n";
            }
            
            system("pause");
        } else {
            cout << "Falha no login. Credenciais inválidas.\n";
            cout << "Tente novamente.\n";
            system("pause");
        }
    }

    return info;
}

login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;
    
    // Inicializar com falha padrão
    info.status = ID_OU_SENHA_INVALIDO;
    info.chave_acesso = ALUNO; // Permissão padrão (mínima)
    
    // Abrir banco de dados de usuários
    const char* nome_arquivo = "database/usuarios.dat";
    ifstream arquivo(nome_arquivo, ios::binary);
    
    if (!arquivo.is_open()) {
        // Banco de dados não existe
        return info;
    }
    
    // Procurar usuário no banco de dados
    Identidade usuario_encontrado;
    bool usuario_existe = false;
    
    while (arquivo.read(reinterpret_cast<char*>(&usuario_encontrado), sizeof(Identidade))) {
        if (usuario_encontrado.id == *id_usuario && usuario_encontrado.ativo) {
            usuario_existe = true;
            
            // Verificar se a senha está correta
            if (strcmp(usuario_encontrado.senha, senha) == 0) {
                // Login bem-sucedido
                info.status = VALIDO;
                info.chave_acesso = usuario_encontrado.categoria;
                
                // Preencher info_basica com dados do banco
                info.info_basica.id = usuario_encontrado.id;
                info.info_basica.ativo = usuario_encontrado.ativo;
                strcpy(info.info_basica.nome, usuario_encontrado.nome);
                strcpy(info.info_basica.cpf, usuario_encontrado.cpf);
                info.info_basica.categoria = usuario_encontrado.categoria;
                strcpy(info.info_basica.especialidade, usuario_encontrado.especialidade);
            }
            break;
        }
    }
    
    arquivo.close();
    return info;
}

login_info validar_login_dinamico(int tipo_login, const char* credencial, const char* senha) {
    login_info info;
    
    // Inicializar com falha padrão
    info.status = ID_OU_SENHA_INVALIDO;
    info.chave_acesso = ALUNO; // Permissão padrão (mínima)
    
    // Abrir banco de dados de usuários
    const char* nome_arquivo = "database/usuarios.dat";
    ifstream arquivo(nome_arquivo, ios::binary);
    
    if (!arquivo.is_open()) {
        // Banco de dados não existe
        return info;
    }
    
    // Procurar usuário no banco de dados
    Identidade usuario_encontrado;
    bool encontrado = false;
    
    while (arquivo.read(reinterpret_cast<char*>(&usuario_encontrado), sizeof(Identidade))) {
        if (!usuario_encontrado.ativo) continue;
        
        bool credencial_bate = false;
        
        // Verificar credencial baseado no tipo de login
        if (tipo_login == 1) {
            // Login por ID
            credencial_bate = (usuario_encontrado.id == atoi(credencial));
        } else if (tipo_login == 2) {
            // Login por Email
            credencial_bate = (strcmp(usuario_encontrado.email, credencial) == 0);
        }
        
        if (credencial_bate) {
            // Verificar se a senha está correta
            if (strcmp(usuario_encontrado.senha, senha) == 0) {
                // Login bem-sucedido
                info.status = VALIDO;
                info.chave_acesso = usuario_encontrado.categoria;
                
                // Preencher info_basica com dados do banco
                info.info_basica.id = usuario_encontrado.id;
                info.info_basica.ativo = usuario_encontrado.ativo;
                strcpy(info.info_basica.nome, usuario_encontrado.nome);
                strcpy(info.info_basica.cpf, usuario_encontrado.cpf);
                info.info_basica.categoria = usuario_encontrado.categoria;
                strcpy(info.info_basica.especialidade, usuario_encontrado.especialidade);
                encontrado = true;
            }
            break;
        }
    }
    
    arquivo.close();
    return info;
}

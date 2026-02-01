#include <iostream>
#include <fstream>
#include <cstring>
#include "inicializadores_de_tabela.h"
#include "../utilidades.h"
#include "../headers.h"

using namespace std;

void inicializar_banco_dados_usuarios() {
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
    strcpy(usuario_root.nome_usuario, "root");       // Nome de usuário = "root"
    strcpy(usuario_root.cpf, "000.000.000-00");
    strcpy(usuario_root.email, "root@sistema.edu");
    strcpy(usuario_root.senha, "0000");               // Senha = "0000"
    strcpy(usuario_root.especialidade, "null");
    
    ofstream arquivo(nome_arquivo, ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(&usuario_root), sizeof(Identidade));
        arquivo.close();
    }
}

void inicializar_banco_dados_recursos() {
    const char* nome_arquivo = "database/recursos_individuais.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    // Se o arquivo já existe, não fazer nada
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    // Arquivo não existe, criar com um registro vazio (ID = 0 para compatibilidade)
    Recursos_Individuais recurso_padrao;
    recurso_padrao.id_proprietario = 0;          // ID do ROOT
    recurso_padrao.saldo = 0.0;                  // Sem saldo inicial
    recurso_padrao.qtd_livros = 0;               // Sem livros emprestados
    recurso_padrao.ativo = 1;                    // Conta ativa
    
    // Inicializar itens como vazios (id_livro = 0)
    for (int i = 0; i < 5; i++) {
        recurso_padrao.itens[i].id_livro = 0;
        strcpy(recurso_padrao.itens[i].data_emprestimo, "00/00/0000");
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(&recurso_padrao), sizeof(Recursos_Individuais));
        arquivo.close();
    }
}

void inicializar_todos_bancos_dados() {
    inicializar_banco_dados_usuarios();
    inicializar_banco_dados_recursos();
}

#include <iostream>
#include <fstream>
#include <cstring>
#include "inicializadores.h"
#include "../utilidades.h"

using namespace std;

// ========== INICIALIZADORES DE STRUCTS ==========

Identidade criar_identidade_padrao() {
    Identidade id = {};
    id.id = 0;
    id.ativo = 1;
    strcpy(id.nome, "");
    strcpy(id.cpf, "");
    strcpy(id.senha, "");
    strcpy(id.email, "");
    id.categoria = ALUNO;
    strcpy(id.especialidade, "");
    return id;
}

Identidade criar_identidade_usuario_root() {
    Identidade id = criar_identidade_padrao();
    id.id = 0;
    id.ativo = 1;
    id.categoria = ADMIN;
    strcpy(id.nome, "USUARIO_ROOT");
    strcpy(id.cpf, "000.000.000-00");
    strcpy(id.email, "root@sistema.edu");
    strcpy(id.senha, "0000");
    strcpy(id.especialidade, "root");
    return id;
}

login_identidadade criar_login_identidade() {
    login_identidadade login = {};
    login.id = 0;
    login.ativo = 0;
    strcpy(login.nome, "");
    strcpy(login.cpf, "");
    login.categoria = ALUNO;
    strcpy(login.especialidade, "");
    return login;
}

Turma criar_turma_padrao() {
    Turma turma = {};
    turma.id = 0;
    turma.ano_letivo = 0;
    strcpy(turma.nome, "");
    turma.id_professor = 0;
    turma.qtd_alunos = 0;
    turma.qtd_horarios = 0;
    turma.qtd_aulas = 0;
    turma.ativo = 1;
    turma.indice_ultima_aula = 0;
    turma.qtd_eventos = 0;
    
    for (int i = 0; i < 50; i++) {
        turma.id_alunos[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        strcpy(turma.horario[i], "");
    }
    for (int i = 0; i < 10; i++) {
        turma.eventos[i].qtd = 0;
        strcpy(turma.eventos[i].nome, "");
        for (int j = 0; j < 20; j++) {
            strcpy(turma.eventos[i].data[j], "");
        }
    }
    return turma;
}

Historico_Individual criar_historico_padrao() {
    Historico_Individual historico = {};
    historico.id_proprietario = 0;
    historico.id_turma = 0;
    historico.qtd_notas = 0;
    historico.status = ABERTO;
    
    for (int i = 0; i < 100; i++) {
        historico.frequencia[i] = false;
    }
    for (int i = 0; i < 10; i++) {
        historico.notas[i] = 0.0;
    }
    return historico;
}

Recursos_Individuais criar_recursos_padrao() {
    Recursos_Individuais recursos = {};
    recursos.id_proprietario = 0;
    recursos.saldo = 0.0;
    recursos.qtd_livros = 0;
    recursos.ativo = 1;
    
    for (int i = 0; i < 5; i++) {
        recursos.itens[i].id_livro = 0;
        strcpy(recursos.itens[i].data_emprestimo, "00/00/0000");
    }
    return recursos;
}

Produto_Cantina criar_produto_cantina_padrao() {
    Produto_Cantina produto = {};
    produto.id = 0;
    strcpy(produto.nome, "");
    produto.preco = 0.0;
    produto.quantidade_estoque = 0;
    produto.ativo = 1;
    return produto;
}

Inventario_Instrumentos criar_inventario_instrumentos_padrao() {
    Inventario_Instrumentos inventario = {};
    inventario.id = 0;
    strcpy(inventario.nome, "");
    inventario.quantidade_total = 0;
    inventario.quantidade_emprestada = 0;
    inventario.ativo = 1;
    return inventario;
}

RegistroNotas criar_registro_notas_padrao() {
    RegistroNotas registro = {};
    registro.id_aluno = 0;
    registro.id_turma = 0;
    registro.qtd_notas = 0;
    registro.ativo = 1;
    
    for (int i = 0; i < 10; i++) {
        registro.notas[i] = 0.0;
    }
    return registro;
}

EventoAgenda criar_evento_padrao() {
    EventoAgenda evento = {};
    evento.id = 0;
    strcpy(evento.nome, "");
    strcpy(evento.data, "");
    evento.vagas_totais = 0;
    evento.qtd_vagas_ocupadas = 0;
    evento.ativo = 1;
    
    for (int i = 0; i < 50; i++) {
        evento.ids_dos_inscritos[i] = 0;
    }
    return evento;
}

Venda_Cantina criar_venda_cantina_padrao() {
    Venda_Cantina venda = {};
    venda.id = 0;
    venda.id_vendedor = 0;
    venda.id_aluno = 0;
    venda.id_produto = 0;
    venda.quantidade = 0;
    venda.valor_unitario = 0.0;
    venda.valor_total = 0.0;
    strcpy(venda.data_hora, "");
    venda.ativo = 1;
    return venda;
}

Resumo_Vendedor criar_resumo_vendedor_padrao() {
    Resumo_Vendedor resumo = {};
    resumo.id_vendedor = 0;
    resumo.total_vendido = 0.0;
    resumo.quantidade_vendas = 0;
    resumo.qtd_transacoes = 0;
    strcpy(resumo.data, "");
    resumo.ativo = 1;
    return resumo;
}

Saldo_Cantina criar_saldo_cantina_padrao() {
    Saldo_Cantina saldo = {};
    saldo.id_aluno = 0;
    saldo.saldo_cantina = 0.0;
    saldo.qtd_compras = 0;
    strcpy(saldo.ultima_compra, "00/00/0000");
    saldo.ativo = 1;
    return saldo;
}

Atraso_Instrumento criar_atraso_instrumento_padrao() {
    Atraso_Instrumento atraso = {};
    atraso.id_aluno = 0;
    atraso.id_instrumento = 0;
    atraso.dias_atraso = 0;
    atraso.multa_total = 0.0;
    strcpy(atraso.data_vencimento, "00/00/0000");
    atraso.ativo = 0;
    return atraso;
}

// ========== INICIALIZADORES DE BANCOS DE DADOS ==========

void inicializar_banco_dados_usuarios() {
    const char* nome_arquivo = "database/usuarios.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    Identidade usuario_root = criar_identidade_usuario_root();
    ofstream arquivo(nome_arquivo, ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(&usuario_root), sizeof(Identidade));
        arquivo.close();
    }
}

void inicializar_banco_dados_recursos() {
    const char* nome_arquivo = "database/recursos_individuais.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    Recursos_Individuais recurso_padrao = criar_recursos_padrao();
    recurso_padrao.id_proprietario = 0;
    recurso_padrao.ativo = 1;
    
    ofstream arquivo(nome_arquivo, ios::binary);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char*>(&recurso_padrao), sizeof(Recursos_Individuais));
        arquivo.close();
    }
}

void inicializar_banco_dados_turmas() {
    const char* nome_arquivo = "database/turmas.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_historicos() {
    const char* nome_arquivo = "database/historicos.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_produtos_cantina() {
    const char* nome_arquivo = "database/produtos_cantina.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_inventario_instrumentos() {
    const char* nome_arquivo = "database/inventario_instrumentos.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_registros_notas() {
    const char* nome_arquivo = "database/registros_notas.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_eventos() {
    const char* nome_arquivo = "database/eventos.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_vendas_cantina() {
    const char* nome_arquivo = "database/vendas_cantina.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_resumos_vendedor() {
    const char* nome_arquivo = "database/resumos_vendedor.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_saldos_cantina() {
    const char* nome_arquivo = "database/saldos_cantina.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_banco_dados_atrasos_instrumentos() {
    const char* nome_arquivo = "database/atrasos_instrumentos.dat";
    ifstream arquivo_verificar(nome_arquivo, ios::binary);
    
    if (arquivo_verificar.is_open()) {
        arquivo_verificar.close();
        return;
    }
    
    ofstream arquivo(nome_arquivo, ios::binary);
    arquivo.close();
}

void inicializar_todos_bancos_dados() {
    inicializar_banco_dados_usuarios();
    inicializar_banco_dados_recursos();
    inicializar_banco_dados_turmas();
    inicializar_banco_dados_historicos();
    inicializar_banco_dados_produtos_cantina();
    inicializar_banco_dados_inventario_instrumentos();
    inicializar_banco_dados_registros_notas();
    inicializar_banco_dados_eventos();
    inicializar_banco_dados_vendas_cantina();
    inicializar_banco_dados_resumos_vendedor();
    inicializar_banco_dados_saldos_cantina();
    inicializar_banco_dados_atrasos_instrumentos();
}

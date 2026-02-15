#ifndef LANCHONETE_H
#define LANCHONETE_H

#include "headers.h"
#include <fstream>

#define MAX_PRODUTOS 50
#define MAX_CREDITOS 300

#define BANCO_PRODUTOS "lanchonete_produtos.dat"
#define BANCO_CREDITOS "lanchonete_creditos.dat"

namespace Lanchonete {
    bool bancoDeDados();
    bool verificarAtividade(Usuario pessoa);
    void menuCadastroProdutos();
    void adicionarCreditosUsuario();
    void consultarEstoque();
    void adicionarProduto();
    void removerProduto();
    void realizarCompra(int id);
    void removerCredito();
    void visualizarProdutos();
    void consultarSaldo(int id);
}

#endif

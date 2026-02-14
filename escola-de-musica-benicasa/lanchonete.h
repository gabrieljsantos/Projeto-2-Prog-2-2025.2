#include "headers.h"
#include <fstream>

#define MAX_PRODUTOS 50
#define BANCO_PRODUTOS "lanchonete_produtos.dat"
#define BANCO_SALDO "lanchonete_saldo.dat"

namespace Lanchonete {
    bool bancoDeDados();//Feito
    bool verificarAtividade(Usuario pessoa);//Feito
    void menuCadastroProdutos();//Pra fazer
    void adicionarCreditosUsuario();//Pra fazer
    void consultarEstoque();//Feito
    void adicionarProduto();//Feito
    void removerProduto();//Pra fazer
    void realizarCompra(int id);//Feito
    void removerCredito();//Pra fazer
    void visualizarProdutos();//Feito
    void consultarSaldo(int id);//Feito
}

#include "../headers.h"
#include "entrada_vendedor.h"

int main() {
    login_info info_teste;
    info_teste.info_basica.id = VENDEDOR; 
    
    janela_vendedor(info_teste);

    return 0;
}

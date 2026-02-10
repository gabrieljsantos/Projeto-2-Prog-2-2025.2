#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <string>
#include "doctest.h"
#include "login_matricula.h"
#include "headers.h"


TEST_CASE("cadastro") {

    SUBCASE("teste: cadastrar novo aluno com cadastrar_novo_usuario()") {
        CHECK(somar(3, 3) == 6);
    }
}

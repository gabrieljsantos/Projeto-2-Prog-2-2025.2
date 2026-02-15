#include <iostream>
#include <fstream>
#include "aluno.h"
#include "headers.h"

using namespace std;

namespace Aluno {
    void consultarNotas(int valor){
        system("cls || clear");
        cout << "Dentro de consultarNotas - Entrada: " << valor << endl;
        // Lógica para consultar notas do aluno com ID = valor
    }
    void consultarMedias(int idAluno) {
        system("cls || clear");
        cout << "Dentro de consultarMedias - Entrada: " << idAluno << endl;
        // Lógica para consultar médias do aluno com ID = idAluno
    }
    void consultarSituacaoAcademica(int idAluno) {
        system("cls || clear");
        cout << "Dentro de consultarSituacaoAcademica - Entrada: " << idAluno << endl;
        // Lógica para consultar situação acadêmica do aluno com ID = idAluno
    }
    void visualizarEventosDisponiveis() {
        system("cls || clear");
        cout << "Dentro de visualizarEventosDisponiveis" << endl;
        // Lógica para visualizar eventos disponíveis para inscrição
    }
    void inscreverEmEvento(int idAluno) {
        system("cls || clear");
        cout << "Dentro de inscreverEmEvento - Entrada: " << idAluno << endl;
        // Lógica para inscrever o aluno com ID = idAluno em um evento
    }
    void consultarMinhasInscricoes(int idAluno) {
        system("cls || clear");
        cout << "Dentro de consultarMinhasInscricoes - Entrada: " << idAluno << endl;
        // Lógica para consultar as inscrições do aluno com ID = idAluno
    }
    void visualizarInstrumentosDisponiveis() {
        system("cls || clear");
        cout << "Dentro de visualizarInstrumentosDisponiveis" << endl;
        // Lógica para visualizar instrumentos disponíveis para empréstimo
    }
    void solicitarEmprestimo(int idAluno) {
        system("cls || clear");
        cout << "Dentro de solicitarEmprestimo - Entrada: " << idAluno << endl;
        // Lógica para solicitar empréstimo de um instrumento para o aluno com ID = idAluno
    }
    void realizarDevolucao(int idAluno) {
        system("cls || clear");
        cout << "Dentro de realizarDevolucao - Entrada: " << idAluno << endl;
        // Lógica para realizar devolução de um instrumento emprestado pelo aluno com ID = idAluno
    }
    void consultarMeusEmprestimos(int idAluno) {
        system("cls || clear");
        cout << "Dentro de consultarMeusEmprestimos - Entrada: " << idAluno << endl;
        // Lógica para consultar os empréstimos do aluno com ID = idAluno
    }

}

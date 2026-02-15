#include "professor.h"
#include "headers.h"
#include <fstream>
#include <string>
#include <iostream>

namespace Calculos{
    float somaRecursivaNotas(float notas[], int n){
        if(n == 0){
            return 0;
        }
        else{
            return notas[n - 1] + somaRecursivaNotas(notas, n -1);
        }
    }

    float calcularMedia(float notas[], int n){
        float somaTotal = Calculos::somaRecursivaNotas(notas, n);

        float media = somaTotal / n;

        if(media < 0.0)
            return 0;

        return media;
    }

    float calcularMediasTurma(Usuario u){

    std::ifstream lendomedias;
    lendomedias.open("notas.txt", std::ios::in);
    int quant_medias = 0, id_aluno;
    float notas[2], media_individual = 0.0, mediaGeral = 0.0;

    if(lendomedias.is_open()){
        system("cls || clear");
        std::cout << "Dentro de registrarNotas - Entrada: " << u.id << std::endl;

        while(lendomedias >> id_aluno >> notas[0] >> notas[1] >> media_individual){
            mediaGeral += media_individual;
            quant_medias++;
        }

        lendomedias.close();

        if(quant_medias == 0)
            return 0;

        return mediaGeral / quant_medias;
    }
    else {
        std::cout << "Erro: Nao foi possivel abrir o arquivo para calcular a media geral." << std::endl;
        return 0;
    }

    }


}

bool verificacaoDoUsuario(Usuario u){

    if(u.categoria == PROFESSOR && u.ativo == 1){
        return true;
    }

    return false;
}

void registrarNotas(Usuario u, float notas[], int n){
    bool verificar = verificacaoDoUsuario(u);
    std :: ofstream gravandoNotas;
    int id_aluno = 0;
    if(verificar == true){
       
        gravandoNotas.open("notas.txt", std::ios::out | std::ios::app);

        if(gravandoNotas.is_open()){
            system("cls || clear");
            std::cout << "Dentro de registrarNotas - Entrada: " << u.id << std::endl;
            std::cout << "Digite o ID do aluno para registrar as suas notas: ";
            std::cin >> id_aluno;

            for(int i = 0; i < n; i++){
                std::cout << "Digite a " << i + 1 << " nota do aluno: ";
                std::cin >> notas[i];
            }

            float media = Calculos::calcularMedia(notas, n);

            gravandoNotas << id_aluno;
            for(int i = 0; i < n; i++){
                gravandoNotas <<  "     "<< notas[i];
            }
                
            gravandoNotas << "     " << media << std::endl;

            gravandoNotas.close();

        }  
    }
}

void consultarDesempenhoAcademico(int idProfessor) {
        system("cls || clear");
        std::cout << "Dentro de consultarDesempenhoAcademico - Entrada: " << idProfessor << std::endl;
        // Lógica para consultar desempenho acadêmico dos alunos
}

void consultarEventosDisponiveis() {
    system("cls || clear");
    std::cout << "Dentro de consultarEventosDisponiveis" << std::endl;
        // Lógica para consultar eventos disponíveis
}

void consultarInstrumentosDisponiveis() {
    system("cls || clear");
    std::cout << "Dentro de consultarInstrumentosDisponiveis" << std::endl;
        // Lógica para consultar instrumentos disponíveis
}

void registrarAvaliacoes(int idProfessor) {
    system("cls || clear");
    std::cout << "Dentro de registrarAvaliacoes - Entrada: " << idProfessor << std::endl;
        // Lógica para registrar avaliações
}

void consultarAlunosMatriculados(int idProfessor) {
    system("cls || clear");
    std::cout << "Dentro de consultarAlunosMatriculados - Entrada: " << idProfessor << std::endl;
        // Lógica para consultar alunos matriculados de uma turma
}

int main(){
    
    return 0;
}

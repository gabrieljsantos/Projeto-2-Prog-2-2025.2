#include "professor.h"
#include "headers.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace SomaNotas{
    float somaRecursivaNotas(float notas[], int n){
        if(n == 0){
            return 0;
        }
        else{
            return notas[n - 1] + somaRecursivaNotas(notas, n -1);
        }
    }

    float calcularMedia(float notas[], int n){
        float somaTotal = SomaNotas::somaRecursivaNotas(notas, n);

        float media = somaTotal / n;

        if(media < 0.0)
            return 0;

        return media;
    }
}

bool verificacaoDoUsuario(Usuario u){

    if(u.categoria == PROFESSOR && u.ativo == 1){
        return true;
    }

    return false;
}

namespace Modulo_professor {
    void registrarNotas(int idProfessor, float notas[], int n) {
        system("cls || clear");
        cout << "Dentro de registrarNotas - Entrada: " << idProfessor << endl;

        Usuario u;
        u.id = idProfessor;
        bool verificar = verificacaoDoUsuario(u);
        ofstream gravandoNotas;
        int id_auxiliar = 0;
        if(verificar == true){
           
            gravandoNotas.open("notas.txt", ios::out | ios::app);

            if(gravandoNotas.is_open()){
                cout << "Digite o ID do aluno para registrar as suas notas: ";
                cin >> id_auxiliar;

                if(id_auxiliar == u.id){
                    for(int i = 0; i < n; i++){
                        cout << "Digite a " << i + 1 << " nota do aluno: ";
                        cin >> notas[i];
                    }
                    float media = SomaNotas::calcularMedia(notas, n);

                    gravandoNotas << u.id;
                    for(int i = 0; i < n; i++){
                        gravandoNotas <<  "    "<< notas[i];
                    }
                    
                    gravandoNotas << "     " << media << endl;

                    gravandoNotas.close();
                }
            }
        }
    }
    void registrarAvaliacoes(int idProfessor) {
        system("cls || clear");
        cout << "Dentro de registrarAvaliacoes - Entrada: " << idProfessor << endl;
        // Lógica para registrar avaliações
    }
    void consultarAlunosMatriculados(int idProfessor) {
        system("cls || clear");
        cout << "Dentro de consultarAlunosMatriculados - Entrada: " << idProfessor << endl;
        // Lógica para consultar alunos matriculados de uma turma
    }
    void calcularMediasTurma(int idProfessor) {
        system("cls || clear");
        cout << "Dentro de calcularMediasTurma - Entrada: " << idProfessor << endl;
        
        Usuario u;
        u.id = idProfessor;
        ifstream lendomedias;
        lendomedias.open("notas.txt", ios::in);
        int quant_medias = 0;
        float notas[2], media_individual = 0.0, mediaGeral = 0.0;

        if(lendomedias.is_open()){
            while(lendomedias >> u.id >> notas[0] >> notas[1] >> media_individual){
                mediaGeral += media_individual;
                quant_medias++;
            }

            lendomedias.close();

            if(quant_medias == 0)
                cout << "Nenhuma media calculada" << endl;
            else
                cout << "Media geral: " << mediaGeral / quant_medias << endl;
        }
        else {
            cout << "Erro: Nao foi possivel abrir o arquivo para calcular a media geral." << endl;
        }
    }
    void consultarDesempenhoAcademico(int idProfessor) {
        system("cls || clear");
        cout << "Dentro de consultarDesempenhoAcademico - Entrada: " << idProfessor << endl;
        // Lógica para consultar desempenho acadêmico dos alunos
    }
    void consultarEventosDisponiveis() {
        system("cls || clear");
        cout << "Dentro de consultarEventosDisponiveis" << endl;
        // Lógica para consultar eventos disponíveis
    }
    void consultarInstrumentosDisponiveis() {
        system("cls || clear");
        cout << "Dentro de consultarInstrumentosDisponiveis" << endl;
        // Lógica para consultar instrumentos disponíveis
    }
}

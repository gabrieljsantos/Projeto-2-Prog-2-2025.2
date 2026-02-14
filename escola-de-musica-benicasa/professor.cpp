#include "professor.h"
#include "headers.h"
#include <fstream>
#include <string>
#include <iostream>

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

void registrarNotas(Usuario u, float notas[], int n){
    bool verificar = verificacaoDoUsuario(u);
    std :: ofstream gravandoNotas;
    int id_auxiliar = 0;
    if(verificar == true){
       
        gravandoNotas.open("notas.txt", std::ios::out | std::ios::app);

        if(gravandoNotas.is_open()){
            std::cout << "Digite o ID do aluno para registrar as suas notas: ";
            std::cin >> id_auxiliar;

            if(id_auxiliar  == u.id){

                for(int i = 0; i < n; i++){
                    std::cout << "Digite a " << i + 1 << " nota do aluno: ";
                    std::cin >> notas[i];
                }
                float media = SomaNotas::calcularMedia(notas, n);

                gravandoNotas << u.id;
                for(int i = 0; i < n; i++){
                    gravandoNotas <<  "    "<< notas[i];
                }
                
                gravandoNotas << "     " << media << std::endl;

                gravandoNotas.close();

            }
            
        }

        
    }
}

float calcularMediasTurma(Usuario u){

    std::ifstream lendomedias;
    lendomedias.open("notas.txt", std::ios::in);
    int quant_medias = 0;
    float notas[2], media_individual = 0.0, mediaGeral = 0.0;

    if(lendomedias.is_open()){
        while(lendomedias >> u.id >> notas[0] >> notas[1] >> media_individual){
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


int main(){
    
    return 0;
}

#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H
void menuCadastroInstrumentos();
void deleteInstrumentos();
void mostrarInstrumentosCadastrados();
void openFile();
struct Instrumentos{
    int id;
    char nome[20];
    bool disponivel;
    char responsavel[50];
};
#endif


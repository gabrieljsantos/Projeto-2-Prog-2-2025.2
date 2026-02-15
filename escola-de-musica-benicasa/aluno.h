#ifndef ALUNO_H
#define ALUNO_H


namespace Aluno {
    void consultarNotas(int idAluno);
    void consultarMedias(int idAluno);
    void consultarSituacaoAcademica(int idAluno);
    void visualizarEventosDisponiveis();
    void inscreverEmEvento(int idAluno);
    void consultarMinhasInscricoes(int idAluno);
    void visualizarInstrumentosDisponiveis();
    void solicitarEmprestimo(int idAluno);
    void realizarDevolucao(int idAluno);
    void consultarMeusEmprestimos(int idAluno);
}
#endif

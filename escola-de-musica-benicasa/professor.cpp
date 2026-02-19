#include "professor.h"
#include "login_matricula.h"
#include "headers.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

#define MAX_TURMAS 30
#define MAX_ALUNOS 20
#define MAX_TURMAS_PROF 5
#define MEDIA_MINIMA 7.0
#define FALTAS_MAXIMAS 10

namespace Calculos
{
    float somaRecursivaNotas(float notas[], int n)
    {
        if (n == 0)
            return 0;
        return notas[n - 1] + somaRecursivaNotas(notas, n - 1);
    }

    float calcularMedia(float notas[], int n)
    {
        if (n == 0)
            return 0;
        float media = somaRecursivaNotas(notas, n) / n;
        return media < 0 ? 0 : media;
    }
}

bool turmaPermitida(int id_prof, int turma_index)
{
    if (turma_index < 1 || turma_index > MAX_TURMAS)
        return false;

    ifstream file("turmas.dat", ios::binary);
    if (!file)
        return false;

    file.seekg((turma_index - 1) * sizeof(Turma));
    Turma turma;
    file.read((char *)&turma, sizeof(Turma));
    file.close();

    if (turma.id_prof != id_prof)
        return false;

    Professor prof = Login_mat::lerProfessor(id_prof);

    for (int i = 0; i < MAX_TURMAS_PROF; i++)
        if (prof.turmas[i] == turma.id)
            return true;

    return false;
}

namespace ModuloProfessor
{
    static bool lerTurma(int index, Turma &turma)
    {
        ifstream file("turmas.dat", ios::binary);
        if (!file)
            return false;

        file.seekg((index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        file.close();
        return true;
    }

    void consultarSaldoLanchonete(int id_prof)
    {
        Professor prof = Login_mat::lerProfessor(id_prof);
        cout << "Saldo: R$ " << fixed << setprecision(2) << prof.saldo << "\n";
    }

    void consultarSaldoLanchonete(Professor &prof)
    {
        cout << "Saldo: R$ " << fixed << setprecision(2) << prof.saldo << "\n";
    }

    void registrarNotas(int id_prof)
    {
        int turma_index, id_aluno;
        cout << "Numero da turma: ";
        cin >> turma_index;

        if (!turmaPermitida(id_prof, turma_index))
        {
            cout << "Turma nao pertence ao professor.\n";
            return;
        }

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        cout << "ID do aluno: ";
        cin >> id_aluno;

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            if (turma.alunos[i].base.id == id_aluno)
            {
                float notas[2];

                for (int j = 0; j < 2; j++)
                {
                    cout << "Nota " << j + 1 << ": ";
                    cin >> notas[j];
                    turma.alunos[i].notas[j] = notas[j];
                }

                turma.alunos[i].media = Calculos::calcularMedia(notas, 2);

                file.seekp((turma_index - 1) * sizeof(Turma));
                file.write((char *)&turma, sizeof(Turma));
                file.close();

                cout << "Notas registradas.\n";
                return;
            }
        }

        cout << "Aluno nao encontrado.\n";
    }

    void registrarNotasTodo(int id_prof)
    {
        int turma_index;
        cout << "Numero da turma: ";
        cin >> turma_index;

        if (!turmaPermitida(id_prof, turma_index))
        {
            cout << "Turma nao pertence ao professor.\n";
            return;
        }

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            cout << "Aluno: " << (*aluno).base.nome << "\n";

            float notas[2];
            for (int j = 0; j < 2; j++)
            {
                cout << "Nota " << j + 1 << ": ";
                cin >> notas[j];
            }

            (*aluno).notas[0] = notas[0];
            (*aluno).notas[1] = notas[1];
            (*aluno).media = Calculos::calcularMedia(notas, 2);
        }

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();

        cout << "Notas registradas para a turma.\n";
    }

    void consultarAlunosMatriculados(int id_prof)
    {
        Professor *prof = new Professor;
        *prof = Login_mat::lerProfessor(id_prof);

        for (int t = 0; t < MAX_TURMAS_PROF; t++)
        {
            if ((*prof).turmas[t] == 0)
                continue;

            Turma turma;
            if (!lerTurma((*prof).turmas[t], turma))
                continue;

            cout << "\nTurma: " << turma.nome << "\n";

            for (int i = 0; i < MAX_ALUNOS; i++)
            {
                Aluno *aluno = &turma.alunos[i];

                if ((*aluno).base.id == 0)
                    continue;

                cout << (*aluno).base.id << " - "
                     << (*aluno).base.nome << " | "
                     << "N1: " << (*aluno).notas[0]
                     << " | N2: " << (*aluno).notas[1]
                     << " | Media: " << (*aluno).media
                     << " | Faltas: " << (*aluno).faltas
                     << "\n";
            }
        }

        delete prof;
    }

    void consultarDesempenhoAcademico(int id_prof)
    {
        int turma_index;
        cout << "Numero da turma: ";
        cin >> turma_index;

        if (!turmaPermitida(id_prof, turma_index))
        {
            cout << "Turma nao pertence ao professor.\n";
            return;
        }

        Turma turma;
        if (!lerTurma(turma_index, turma))
            return;

        Aluno **criticos = new Aluno *[MAX_ALUNOS];
        int qtd = 0;

        int aprovados = 0, reprovadosNota = 0, reprovadosFalta = 0;
        float soma = 0;
        int total = 0;

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            total++;
            soma += (*aluno).media;

            if ((*aluno).faltas > FALTAS_MAXIMAS)
            {
                reprovadosFalta++;
                criticos[qtd++] = aluno;
            }
            else if ((*aluno).media >= MEDIA_MINIMA)
                aprovados++;
            else
            {
                reprovadosNota++;
                criticos[qtd++] = aluno;
            }
        }

        if (total == 0)
        {
            cout << "Nenhum aluno.\n";
            delete[] criticos;
            return;
        }

        cout << "Media da turma: " << soma / total << "\n";
        cout << "Aprovados: " << aprovados << "\n";
        cout << "Reprovados por nota: " << reprovadosNota << "\n";
        cout << "Reprovados por falta: " << reprovadosFalta << "\n";

        if (qtd > 0)
        {
            cout << "Alunos em situacao critica:\n";
            for (int i = 0; i < qtd; i++)
                cout << "- " << (*criticos[i]).base.nome
                     << " | Media: " << (*criticos[i]).media
                     << " | Faltas: " << (*criticos[i]).faltas
                     << "\n";
        }

        delete[] criticos;
    }

    void registrarFaltas(int id_prof)
    {
        int turma_index;
        cout << "Numero da turma: ";
        cin >> turma_index;

        if (!turmaPermitida(id_prof, turma_index))
            return;

        fstream file("turmas.dat", ios::in | ios::out | ios::binary);
        if (!file)
            return;

        Turma turma;
        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            Aluno *aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            char opcao;
            cout << (*aluno).base.nome << " faltou? (s/n): ";
            cin >> opcao;

            if (opcao == 's' || opcao == 'S')
                (*aluno).faltas++;
        }

        file.seekp((turma_index - 1) * sizeof(Turma));
        file.write((char *)&turma, sizeof(Turma));
        file.close();

        cout << "Faltas registradas.\n";
    }

    void consultarTurma(int id_prof)
    {
        int turma_index;
        cout << "Numero da turma: ";
        cin >> turma_index;

        if (!turmaPermitida(id_prof, turma_index))
        {
            cout << "Turma nao pertence ao professor.\n";
            return;
        }

        Turma turma;

        ifstream file("turmas.dat", ios::binary);
        if (!file)
        {
            cout << "Erro ao abrir arquivo.\n";
            return;
        }

        file.seekg((turma_index - 1) * sizeof(Turma));
        file.read((char *)&turma, sizeof(Turma));
        file.close();

        cout << "\nTurma: " << turma.nome << "\n";
        cout << "Professor ID: " << turma.id_prof << "\n";

        Aluno *aluno;

        for (int i = 0; i < MAX_ALUNOS; i++)
        {
            aluno = &turma.alunos[i];

            if ((*aluno).base.id == 0)
                continue;

            cout << (*aluno).base.id << " - "
                 << (*aluno).base.nome
                 << " | Media: " << (*aluno).media
                 << " | Faltas: " << (*aluno).faltas
                 << "\n";
        }
    }
}

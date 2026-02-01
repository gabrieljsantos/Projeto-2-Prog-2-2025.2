#ifndef HEADERS_H
#define HEADERS_H

typedef enum {
    ALUNO,
    PROFESSOR,
    ADMIN,
    VENDEDOR,
    LOCADOR
    //ESTAGIARIO // função: Fazer tudo que ninguém quer fazer... brincadeira, isso não deve existir,
} funcao_pessoa; // atributo verificado no login para liberar acessos,
// funciona como sigaa, faz login e libera acesso conforme o cargo, portal do discente
// portal do docente, portal do admin, etc.

typedef enum {
    ABERTO,
    APROVADO,
    REPROVADO,
} status_historico;

typedef struct {
    int id;                       // ID único no sistema
    int ativo;                    // 1 = Ativo/Matriculado, 0 = Inativo/Deletado
    char nome[100];               // Nome completo
    char cpf[20];                 // Apenas números ou formatado
    char senha[30];               // Senha de login
    char email[100];              // Email de contato
    funcao_pessoa categoria;        // Categoria da pessoa (Aluno, Professor, etc.)
    char especialidade[50];       // Ex: "Piano", "Canto", etc. (apenas para professores)
    // especialidade vazia se for aluno, por padrão é "reclamar", brincadeira, na verdade é ""  
    // tambem pode ser secretario, diretor.
    // essa atribuição não afeta nada, serve apenas com label informativo
} Identidade;


typedef struct {
    int id;                       // ID único no sistema
    int ativo;                    // 1 = Ativo/Matriculado, 0 = Inativo/Deletado
    char nome[100];               // Nome completo
    char cpf[20];                 // Apenas números ou formatado
    funcao_pessoa categoria;        // Categoria da pessoa (Aluno, Professor, etc.)
    char especialidade[50];       // Ex: "Piano", "Canto", etc. (apenas para professores)
} login_identidadade;

typedef enum {
    ID_OU_SENHA_INVALIDO,
    VALIDO,
} status_login;


typedef struct {
    status_login status;      // Status do login
    login_identidadade info_basica; // Informações básicas do login
    funcao_pessoa chave_acesso;        // Permissões de acesso
} login_info;

typedef struct
{
    char data[10][20];                // [i]"DD/MM/AAAA"
    int qtd;          // Quantidade
    char nome[20];                 // Nome do evento
} Evento_Turma;

typedef struct {
    int id;                       // Código da turma
    int ano_letivo;               // Ex: 2026
    char nome[50];                // Ex: "Violão I"
    int id_professor;             // ID do professor responsável
    int id_alunos[50];            // Lista de chamada (IDs dos alunos)
    int qtd_alunos;               // Quantos alunos na turma (0 a 50)
    char horario[4][10]; 
    // até 4 códigos de horário
    // cada string até 9 chars + '\0'
    // exemplo:
    // [0] = "2N1234"
    // [1] = "4T56"
    int qtd_horarios;              // Quantidade de horários por semana, maximo 4
    int qtd_aulas;               // Quantidade de aulas que serão ministradas, maximo 100
    int ativo;                    // 1 = Aberta, 0 = Arquivada
    int indice_ultima_aula;        // Índice da última aula ministrada (0 a 99)
    // o proximo atributo pode oparecer muito absurdo, qualquer coisa será simplificado depois
    Evento_Turma eventos[10];       // Eventos relacionados à turma
    /*
    exemplo 1:
    eventos[0].qtd = 3 
    eventos[0].data[0] = "15/03/2024"
    eventos[0].data[1] = "01/05/2024"
    eventos[0].data[2] = "25/07/2024"
    eventos[0].nome = "Prova Teórica"  
    exemplo 2:
    eventos[1].data[0] = "20/12/2024"
    eventos[1].qtd = 1
    eventos[1].nome = "atividade de casa"
    */
    int qtd_eventos;                // Quantidade de eventos na turma
} Turma;

typedef struct {
    int id_proprietario;                 // identificação do dono do histórico, no caso dos alunos
    int id_turma;                 // Código da turma, dai obtemos o ano letivo e toda informação da turma
    bool frequencia[100];      // Vetor de presença/ausência em cada aula (true = presente, false = ausente)
    int qtd_notas;                // Quantidade de notas lançadas
    float notas[10];              // Notas obtidas pelo aluno na turma
    status_historico status;       // Status final na turma (Aprovado, Reprovado, Aberto)
} Historico_Individual;

typedef struct {
    int id_proprietario;                  // ID do Aluno, professor ou funcionário
    float saldo;                  // Valor em R$ (ex: 50.00)
    struct {
        int id_livro;             // ID do livro/instrumento (0 = vazio)
        char data_emprestimo[12]; // Formato "DD/MM/AAAA"
    } itens[5];                   // Lista de itens emprestados
    int qtd_livros;               // Quantidade atual de itens (0 a 5)
    int ativo;                    // 1 = Conta válida, 0 = Removida
} Recursos_Individuais;

typedef struct {
    int id;                       // Código do produto
    char nome[50];                // Ex: "Coxinha"
    float preco;                  // Preço unitário
    int quantidade_estoque;       // Quantidade física
    int ativo;                    // 1 = Venda ativa, 0 = Removido
} Produto_Cantina;

typedef struct {
    int id;                       // Código único do instrumento
    char nome[100];               // Nome do instrumento (Ex: "Violão", "Piano")
    int quantidade_total;         // Quantidade total disponível
    int quantidade_emprestada;    // Quantidade em posse de alunos
    int ativo;                    // 1 = Instrumento disponível, 0 = Removido
} Inventario_Instrumentos;

typedef struct {
    int id_aluno;                 // ID do aluno (dono da nota)
    int id_turma;                 // ID da turma referente
    float notas[10];              // Notas lançadas (0.0 a 10.0)
    int qtd_notas;                // Quantas notas existem (controle do array)
    int ativo;                    // 1 = Válido, 0 = Deletado
} RegistroNotas;


typedef struct {
    int id;                       // Código do evento
    char nome[100];               // Nome do evento
    char data[20];                // "DD/MM/AAAA"
    int vagas_totais;             // Capacidade máxima
    int ids_dos_inscritos[50];    // IDs dos inscritos
    int qtd_vagas_ocupadas;       // quantidade de vagas ocupadas
    int ativo;                    // 1 = Agendado, 0 = Cancelado
} EventoAgenda;

typedef struct {
    int id;                       // Código único da venda
    int id_vendedor;              // ID do vendedor responsável
    int id_aluno;                 // ID do aluno/cliente que comprou
    int id_produto;               // ID do produto vendido
    int quantidade;               // Quantidade vendida
    float valor_unitario;         // Preço unitário no momento da venda
    float valor_total;            // Total da venda (quantidade * valor_unitario)
    char data_hora[20];           // "DD/MM/AAAA HH:MM"
    int ativo;                    // 1 = Venda realizada, 0 = Cancelada
} Venda_Cantina;

typedef struct {
    int id_vendedor;              // ID do vendedor
    float total_vendido;          // Total em R$ das vendas do dia/período
    int quantidade_vendas;        // Quantidade de itens vendidos
    int qtd_transacoes;           // Quantidade de transações realizadas
    char data[20];                // "DD/MM/AAAA"
    int ativo;                    // 1 = Registro válido, 0 = Cancelado
} Resumo_Vendedor;

typedef struct {
    int id_aluno;                 // ID do aluno/cliente
    float saldo_cantina;          // Saldo em R$ (negativo = deve, positivo = crédito)
    int qtd_compras;              // Total de compras realizadas
    char ultima_compra[12];       // "DD/MM/AAAA"
    int ativo;                    // 1 = Conta ativa, 0 = Removida
} Saldo_Cantina;

typedef struct {
    int id_aluno;                 // ID do aluno com atraso
    int id_instrumento;           // ID do instrumento
    int dias_atraso;              // Quantidade de dias em atraso
    float multa_total;            // Multa acumulada
    char data_vencimento[12];     // "DD/MM/AAAA"
    int ativo;                    // 1 = Atraso ativo, 0 = Regularizado
} Atraso_Instrumento;

#endif

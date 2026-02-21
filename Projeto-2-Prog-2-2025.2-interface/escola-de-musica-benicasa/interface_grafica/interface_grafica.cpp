#include <iostream>
#include <curses.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <locale>
#include <vector>

#include "interface_grafica.h"

using namespace std;

// ─── Helpers internos ───────────────────────────────────────────

static void desenhar_linha_horizontal(WINDOW *win, int y, int largura, int cor_pair)
{
    wattron(win, COLOR_PAIR(cor_pair));
    mvwaddch(win, y, 0, ACS_LTEE);
    for (int i = 1; i < largura - 1; i++)
        mvwaddch(win, y, i, ACS_HLINE);
    mvwaddch(win, y, largura - 1, ACS_RTEE);
    wattroff(win, COLOR_PAIR(cor_pair));
}

static void preencher_linha(WINDOW *win, int y, int x_inicio, int largura, int cor_pair)
{
    wattron(win, COLOR_PAIR(cor_pair));
    for (int i = x_inicio; i < largura; i++)
        mvwaddch(win, y, i, ' ');
    wattroff(win, COLOR_PAIR(cor_pair));
}

static int centro_x(int largura_janela, int tamanho_texto)
{
    int pos = (largura_janela - tamanho_texto) / 2;
    return pos > 0 ? pos : 1;
}

// ─── Word wrap para quebra de linha ─────────────────────────────
static int contar_linhas_com_wrap(const string& texto, int largura_disponivel)
{
    if (texto.empty()) return 0;

    int largura_efetiva = largura_disponivel - 4;
    if (largura_efetiva < 10) largura_efetiva = 10;

    int linhas = 1;
    int pos = 0;

    while (pos < (int)texto.length())
    {
        int prox_newline = texto.find('\n', pos);
        if (prox_newline == (int)string::npos) prox_newline = texto.length();

        int tamanho_linha = prox_newline - pos;
        int quebras_necessarias = (tamanho_linha + largura_efetiva - 1) / largura_efetiva;

        linhas += (quebras_necessarias - 1);
        pos = prox_newline + 1;
    }

    return linhas;
}

// ─── Desenhar texto com word wrap ───────────────────────────────
static int desenhar_texto_wrap(WINDOW *win, int y, int x, const string& texto,
                               int largura_disponivel, int cor_pair)
{
    if (texto.empty()) return 0;

    int largura_efetiva = largura_disponivel - 4;
    if (largura_efetiva < 10) largura_efetiva = 10;

    wattron(win, COLOR_PAIR(cor_pair));

    int linhas_desenhadas = 0;
    int pos = 0;

    while (pos < (int)texto.length())
    {
        int fim_linha = pos + largura_efetiva;
        if (fim_linha > (int)texto.length()) fim_linha = texto.length();

        // Procura última quebra de linha dentro do intervalo
        int ultima_newline = texto.rfind('\n', fim_linha);
        if (ultima_newline != (int)string::npos && ultima_newline >= pos)
        {
            fim_linha = ultima_newline;
        }
        else if (fim_linha < (int)texto.length())
        {
            // Procura último espaço
            int ultimo_espaco = texto.rfind(' ', fim_linha);
            if (ultimo_espaco > pos)
            {
                fim_linha = ultimo_espaco;
            }
        }

        string linha = texto.substr(pos, fim_linha - pos);
        mvwprintw(win, y + linhas_desenhadas, x, "%s", linha.c_str());
        linhas_desenhadas++;

        pos = fim_linha;
        if (pos < (int)texto.length() && texto[pos] == ' ') pos++;
        if (pos < (int)texto.length() && texto[pos] == '\n') pos++;
    }

    wattroff(win, COLOR_PAIR(cor_pair));

    return linhas_desenhadas;
}

// ─── Função principal ───────────────────────────────────────────

void mostrar_caixa_informacao(const string& titulo, const string& mensagem) {
    // Calcular largura baseada no tamanho do título e mensagem
    int largura_titulo = titulo.size() + 4;
    int largura_base = 50;
    int largura = max({largura_titulo, largura_base, (int)mensagem.size() + 4});
    
    // Limitar ao tamanho da tela
    if (largura > COLS - 4) largura = COLS - 4;
    
    // Calcular altura baseada no número de linhas da mensagem
    int linhas_mensagem = contar_linhas_com_wrap(mensagem, largura);
    // Altura: +1 título, +1 separador, +1 espaço antes da instrução, +1 instrução, +2 bordas
    int altura = linhas_mensagem + 6;
    
    // Limitar ao tamanho da tela
    if (altura > LINES - 2) altura = LINES - 2;

    int starty = (LINES - altura) / 2;
    int startx = (COLS - largura) / 2;

    WINDOW* win = newwin(altura, largura, starty, startx);
    wbkgd(win, COLOR_PAIR(0));
    wclear(win);
    box(win, 0, 0);

    // Título centralizado
    int pos_titulo = centro_x(largura, titulo.size());
    mvwprintw(win, 1, pos_titulo, "%s", titulo.c_str());

    // Desenhar linha separadora
    for (int i = 1; i < largura - 1; i++)
        mvwaddch(win, 2, i, ACS_HLINE);

    // Mensagem com word wrap (começa na linha 3)
    desenhar_texto_wrap(win, 3, 2, mensagem, largura, 0);

    // Instrução sempre na penúltima linha
    std::string instrucao = "Pressione ENTER";
    int pos_instrucao = centro_x(largura, instrucao.size());
    mvwprintw(win, altura - 2, pos_instrucao, "%s", instrucao.c_str());

    wrefresh(win);

    // Espera ENTER
    int ch;
    while ((ch = wgetch(win)) != '\n' && ch != KEY_ENTER) {}

    delwin(win);
    refresh();
}

saida_menu interface_para_menu(
    int              total_opcoes,
    string           opcoes[],
    const ConfigMenu &config)
{
    saida_menu resultado;

    int opcao_selecionada = config.opcaoAtual;
    int tecla;
    int pagina_atual  = 0;
    int total_paginas = (int)ceil((double)total_opcoes / config.opcoes_por_janela);

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    // ════════════════════════════════════════════════════════
    //  PALETA: MARFIM sobre PRETO — baixíssimo contraste
    //
    //  Filosofia:
    //    • Fundo geral: preto puro (0,0,0)
    //    • Texto geral: marfim apagado, quase cinza-bege
    //    • Título: sombreado sutil — fundo eleva levemente
    //    • Seleção: sombreado sutil — fundo eleva levemente
    //    • Tudo o mais: sem fundo, sem destaque forte
    // ════════════════════════════════════════════════════════
    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            // ── Cores customizadas ──────────────────────────

            // Cor 16: Preto muito escuro (fundo geral) #0F0F0F
            init_color(16, 59, 59, 59);

            // Cor 17: Marfim principal — tom bege claro, baixa intensidade
            //         #B8B8A0 → rgb(184,184,160) → ncurses(721,721,627)
            //         Reduzido para ficar mais sutil:
            init_color(17, 600, 600, 520);

            // Cor 18: Marfim escurecido — para bordas, paginação, controles
            //         Um tom ainda mais apagado que o texto normal
            init_color(18, 380, 380, 320);

            // Cor 19: Sombreado do título — fundo quase imperceptível
            //         Um cinza-quente muito escuro, leve elevação
            //         ~#1A1916 → ncurses(102, 98, 86)
            init_color(19, 102, 98, 86);

            // Cor 20: Sombreado da seleção — fundo sutilmente mais claro
            //         Um cinza-quente escuro, distinguível do preto
            //         ~#22211C → ncurses(133, 129, 110)
            init_color(20, 133, 129, 110);

            // Cor 21: Marfim para título (levemente mais claro que o geral)
            init_color(21, 680, 680, 600);

            // Cor 22: Marfim para seleção (o mais claro, mas ainda discreto)
            init_color(22, 720, 720, 640);

            // Cor 23: Marfim muito apagado — para instrução/controles
            init_color(23, 300, 300, 260);
        }

        // ── Pares de cor ────────────────────────────────────
        //
        //  pair 1: Opção selecionada  → marfim claro sobre sombreado seleção
        //  pair 2: Opções normais     → marfim apagado sobre preto
        //  pair 3: Descrição          → marfim principal sobre preto
        //  pair 4: Instrução          → marfim muito apagado sobre preto
        //  pair 5: Título             → marfim claro sobre sombreado título
        //  pair 6: Caminho            → marfim escurecido sobre preto
        //  pair 7: Bordas             → marfim escurecido sobre preto
        //  pair 8: Paginação          → marfim escurecido sobre preto
        //  pair 9: Controles          → marfim muito apagado sobre preto

        init_pair(1,  22, 20);   // seleção: texto claro, fundo sombreado
        init_pair(2,  17, 16);   // opções normais
        init_pair(3,  17, 16);   // descrição
        init_pair(4,  23, 16);   // instrução (bem apagada)
        init_pair(5,  21, 19);   // título: texto claro, fundo sombreado
        init_pair(6,  18, 16);   // caminho
        init_pair(7,  18, 16);   // bordas
        init_pair(8,  18, 16);   // paginação
        init_pair(9,  23, 16);   // controles

        // ── Definir fundo global preto ──────────────────────
        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;

    // Calcular altura considerando word wrap
    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.instrucao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    int espaco_apos_cabecalho = (linhas_cabecalho > 0) ? 2 : 0; // Divisória + espaço

    int altura = 2 + linhas_cabecalho + espaco_apos_cabecalho + config.opcoes_por_janela + 3;

    if (altura  > max_y - 4) altura  = max_y - 4;
    if (largura > max_x - 4) largura = max_x - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);

    // Fundo da janela = preto
    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        pagina_atual = opcao_selecionada / config.opcoes_por_janela;

        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        //    Sombreado: fundo levemente elevado do preto
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            // ── Construir barra com sombreado ───────────────
            //    O "sombreado" vem do par 5: fundo cor 19
            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            // Preencher fundo do título (3 linhas de sombreado)
            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            // Linha acima (sombra superior sutil)
            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            // Linha do título
            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(
                start_y - 2,
                titulo_start_x,
                "%s", linha_titulo.c_str()
            );
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            // Linha abaixo (sombra inferior sutil)
            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────

        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        int linha_opcoes_inicio = linha;

        // ── Opções ──────────────────────────────────────────
        int idx_inicio = pagina_atual * config.opcoes_por_janela;
        int idx_fim    = min(idx_inicio + config.opcoes_por_janela, total_opcoes);

        for (int i = idx_inicio; i < idx_fim; i++)
        {
            int y = linha_opcoes_inicio + (i - idx_inicio);

            if (i == opcao_selecionada)
            {
                // ── Barra de seleção com sombreado ──────────
                //    Fundo sutilmente elevado (cor 20)
                preencher_linha(win, y, 1, largura - 1, config.cores.cor_opcao_selecionada);

                wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_BOLD);
                mvwprintw(win, y, config.x, "  > %s", opcoes[i].c_str());
                wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_BOLD);
            }
            else
            {
                wattron(win, COLOR_PAIR(config.cores.cor_opcao));
                mvwprintw(win, y, config.x, "    %s", opcoes[i].c_str());
                wattroff(win, COLOR_PAIR(config.cores.cor_opcao));
            }
        }

        // ── Paginação ───────────────────────────────────────
        if (total_paginas > 1)
        {
            int linha_pag = altura - 2;

            desenhar_linha_horizontal(win, linha_pag - 1, largura, config.cores.cor_borda);

            char buf_pag[64];
            snprintf(buf_pag, sizeof(buf_pag), " Pagina %d de %d ", pagina_atual + 1, total_paginas);

            string indicador = "";
            if (pagina_atual > 0)
                indicador += "< ";
            indicador += buf_pag;
            if (pagina_atual < total_paginas - 1)
                indicador += " >";

            wattron(win, COLOR_PAIR(config.cores.cor_paginacao));
            mvwprintw(
                win, linha_pag,
                centro_x(largura, (int)indicador.length()),
                "%s", indicador.c_str()
            );
            wattroff(win, COLOR_PAIR(config.cores.cor_paginacao));
        }

        // ── Controles ───────────────────────────────────────
        // Exibir instrucao customizada ou padrão (SEMPRE embaixo)
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = config.instrucao.empty() ? config.instrucao_controles : config.instrucao;
            int cor = config.instrucao.empty() ? config.cores.cor_controles : config.cores.cor_instrucao;
            attron(COLOR_PAIR(cor) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(cor) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        switch (tecla)
        {
            case KEY_UP:
                opcao_selecionada--;
                if (opcao_selecionada < 0)
                    opcao_selecionada = total_opcoes - 1;
                break;

            case KEY_DOWN:
                opcao_selecionada++;
                if (opcao_selecionada >= total_opcoes)
                    opcao_selecionada = 0;
                break;

            case KEY_PPAGE:
                opcao_selecionada -= config.opcoes_por_janela;
                if (opcao_selecionada < 0)
                    opcao_selecionada = 0;
                break;

            case KEY_NPAGE:
                opcao_selecionada += config.opcoes_por_janela;
                if (opcao_selecionada >= total_opcoes)
                    opcao_selecionada = total_opcoes - 1;
                break;

            case KEY_LEFT:
                opcao_selecionada -= config.opcoes_por_janela;
                if (opcao_selecionada < 0)
                    opcao_selecionada = 0;
                break;

            case KEY_RIGHT:
                opcao_selecionada += config.opcoes_por_janela;
                if (opcao_selecionada >= total_opcoes)
                    opcao_selecionada = total_opcoes - 1;
                break;

            case 10: // ENTER
                resultado.indice_da_opcao = opcao_selecionada;
                resultado.opcao           = opcoes[opcao_selecionada];
                delwin(win);
                endwin();
                return resultado;

            case 27: // ESC
                resultado.indice_da_opcao = -1;
                resultado.opcao           = "";
                delwin(win);
                endwin();
                return resultado;
        }
    }

    return resultado;
}

// ─── Função para seleção em tabela ──────────────────────────────

saida_tabela interface_para_tabela(
    int                      linhas,
    int                      colunas,
    const string**           matriz,
    const string             titulos[],
    int                      coluna_retorno,
    const ConfigTabela&      config)
{
    saida_tabela resultado;
    resultado.indice_linha   = config.linhaSelecionada;
    resultado.indice_coluna  = config.colunaSelecionada;
    resultado.valor_retorno  = "";

    // Validações básicas
    if (linhas <= 0 || colunas <= 0 || !matriz || coluna_retorno < 0 || coluna_retorno >= colunas)
    {
        resultado.indice_linha = -1;
        return resultado;
    }

    int linha_selecionada = config.linhaSelecionada;
    int coluna_selecionada = config.colunaSelecionada;
    int tecla;
    int pagina_atual = 0;
    int total_paginas = (int)ceil((double)linhas / config.linhas_por_janela);
    int pagina_coluna_atual = 0;
    int total_paginas_colunas = 1;

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            init_color(16, 59, 59, 59);
            init_color(17, 600, 600, 520);
            init_color(18, 380, 380, 320);
            init_color(19, 102, 98, 86);
            init_color(20, 133, 129, 110);
            init_color(21, 680, 680, 600);
            init_color(22, 720, 720, 640);
            init_color(23, 300, 300, 260);
        }

        init_pair(1,  22, 20);
        init_pair(2,  17, 16);
        init_pair(3,  17, 16);
        init_pair(4,  23, 16);
        init_pair(5,  21, 19);
        init_pair(6,  18, 16);
        init_pair(7,  18, 16);
        init_pair(8,  18, 16);
        init_pair(9,  23, 16);

        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;
    if (largura > max_x - 4) largura = max_x - 4;

    // Calcular quantas colunas cabem na largura disponível
    int largura_util = largura - 4; // Desconta bordas
    int colunas_por_pagina = 0;
    {
        int pos = 0;
        for (int c = 0; c < colunas; c++)
        {
            int larg = (config.larguras_colunas && c < config.num_colunas) ?
                       config.larguras_colunas[c] : (largura_util) / colunas;
            if (pos + larg <= largura_util)
            {
                colunas_por_pagina++;
                pos += larg;
            }
            else if (colunas_por_pagina == 0)
            {
                colunas_por_pagina = 1; // Pelo menos uma coluna
                break;
            }
            else
            {
                break;
            }
        }
    }
    if (colunas_por_pagina <= 0) colunas_por_pagina = 1;
    total_paginas_colunas = (int)ceil((double)colunas / colunas_por_pagina);

    // Calcular altura
    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.instrucao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    int espaco_apos_cabecalho = (linhas_cabecalho > 0) ? 2 : 0;
    int altura = 2 + linhas_cabecalho + espaco_apos_cabecalho + 2 + config.linhas_por_janela + 4;

    if (altura > max_y - 4) altura = max_y - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);
    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        pagina_atual = linha_selecionada / config.linhas_por_janela;
        pagina_coluna_atual = coluna_selecionada / colunas_por_pagina;

        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(start_y - 2, titulo_start_x, "%s", linha_titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────
        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        // ── Cabeçalho da tabela (títulos das colunas) ───────
        int pos_col = 1;

        // Desenhar separador superior
        desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
        linha++;

        // Desenhar títulos com as larguras configuradas
        int col_inicio = pagina_coluna_atual * colunas_por_pagina;
        int col_fim = min(col_inicio + colunas_por_pagina, colunas);

        wattron(win, COLOR_PAIR(config.cores.cor_instrucao) | A_BOLD);
        for (int c = col_inicio; c < col_fim; c++)
        {
            string titulo_col = titulos[c];
            int larg = (config.larguras_colunas && c < config.num_colunas) ?
                       config.larguras_colunas[c] : (largura - 2) / colunas;

            // Truncar se necessário
            if ((int)titulo_col.length() > larg - 2)
                titulo_col = titulo_col.substr(0, larg - 2);

            // Centralizar título
            int espaco = (larg - (int)titulo_col.length()) / 2;
            string titulo_formatado(espaco, ' ');
            titulo_formatado += titulo_col;
            titulo_formatado += string(larg - espaco - (int)titulo_col.length(), ' ');

            mvwprintw(win, linha, pos_col, "%s", titulo_formatado.c_str());
            pos_col += larg;
        }
        wattroff(win, COLOR_PAIR(config.cores.cor_instrucao) | A_BOLD);
        linha++;

        // Desenhar separador após títulos
        desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
        linha++;

        int linha_dados_inicio = linha;

        // ── Dados da tabela ──────────────────────────────────
        int idx_inicio = pagina_atual * config.linhas_por_janela;
        int idx_fim    = min(idx_inicio + config.linhas_por_janela, linhas);

        for (int i = idx_inicio; i < idx_fim; i++)
        {
            int y = linha_dados_inicio + (i - idx_inicio);
            pos_col = 1;

            // Verificar se é a linha selecionada
            if (i == linha_selecionada)
            {
                preencher_linha(win, y, 1, largura - 1, config.cores.cor_opcao_selecionada);
                wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_BOLD);
            }
            else
            {
                wattron(win, COLOR_PAIR(config.cores.cor_opcao));
            }

            // Desenhar células
            for (int c = col_inicio; c < col_fim; c++)
            {
                string conteudo = matriz[i][c];
                int larg = (config.larguras_colunas && c < config.num_colunas) ?
                           config.larguras_colunas[c] : (largura - 2) / colunas;

                // Truncar se necessário
                if ((int)conteudo.length() > larg - 2)
                    conteudo = conteudo.substr(0, larg - 2);

                // Alinhar conteúdo
                string conteudo_formatado(2, ' ');
                conteudo_formatado += conteudo;
                conteudo_formatado += string(larg - 2 - (int)conteudo.length(), ' ');

                mvwprintw(win, y, pos_col, "%s", conteudo_formatado.c_str());
                pos_col += larg;
            }

            if (i == linha_selecionada)
            {
                wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_BOLD);
            }
            else
            {
                wattroff(win, COLOR_PAIR(config.cores.cor_opcao));
            }
        }

        // ── Paginação ───────────────────────────────────────
        if (total_paginas > 1 || total_paginas_colunas > 1)
        {
            int linha_pag = altura - 2;

            desenhar_linha_horizontal(win, linha_pag - 1, largura, config.cores.cor_borda);

            char buf_pag[128];
            string indicador_pag = "";

            // Paginação vertical
            if (total_paginas > 1)
            {
                snprintf(buf_pag, sizeof(buf_pag), "Página %d de %d ", pagina_atual + 1, total_paginas);
                indicador_pag += buf_pag;
            }

            // Paginação horizontal
            if (total_paginas_colunas > 1)
            {
                if (!indicador_pag.empty()) indicador_pag += "| ";
                snprintf(buf_pag, sizeof(buf_pag), "Colunas %d de %d ", pagina_coluna_atual + 1, total_paginas_colunas);
                indicador_pag += buf_pag;
            }

            string indicador = "";
            if (pagina_atual > 0 || pagina_coluna_atual > 0)
                indicador += "< ";
            indicador += indicador_pag;
            if (pagina_atual < total_paginas - 1 || pagina_coluna_atual < total_paginas_colunas - 1)
                indicador += " >";

            wattron(win, COLOR_PAIR(config.cores.cor_paginacao));
            mvwprintw(
                win, linha_pag,
                centro_x(largura, (int)indicador.length()),
                "%s", indicador.c_str()
            );
            wattroff(win, COLOR_PAIR(config.cores.cor_paginacao));
        }

        // ── Controles ───────────────────────────────────────
        // Exibir instrucao customizada ou padrão (somente uma)
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = config.instrucao.empty() ? config.instrucao_controles : config.instrucao;
            int cor = config.instrucao.empty() ? config.cores.cor_controles : config.cores.cor_instrucao;
            attron(COLOR_PAIR(cor) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(cor) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        switch (tecla)
        {
            // ═══════════════════════════════════════════════════════
            // NAVEGAÇÃO: Apenas VERTICAL (linhas)
            // UP/DOWN: navegação linha-a-linha
            // LEFT/RIGHT: pulo de página (± limite_linhas)
            // ═══════════════════════════════════════════════════════

            case KEY_UP:
                // Navegar linha por linha para cima
                if (linha_selecionada > 0)
                    linha_selecionada--;
                break;

            case KEY_DOWN:
                // Navegar linha por linha para baixo
                if (linha_selecionada < linhas - 1)
                    linha_selecionada++;
                break;

            case KEY_LEFT:
                // Pulo de página para cima (- limite_linhas)
                linha_selecionada -= config.linhas_por_janela;
                if (linha_selecionada < 0)
                    linha_selecionada = 0;
                break;

            case KEY_RIGHT:
                // Pulo de página para baixo (+ limite_linhas)
                linha_selecionada += config.linhas_por_janela;
                if (linha_selecionada >= linhas)
                    linha_selecionada = linhas - 1;
                break;

            case KEY_PPAGE:
                // Pulo de página para cima (alternativa)
                linha_selecionada -= config.linhas_por_janela;
                if (linha_selecionada < 0)
                    linha_selecionada = 0;
                break;

            case KEY_NPAGE:
                // Pulo de página para baixo (alternativa)
                    linha_selecionada = linhas - 1;
                break;

            case 10: // ENTER
                resultado.indice_linha   = linha_selecionada;
                resultado.indice_coluna  = coluna_selecionada;
                resultado.valor_retorno  = matriz[linha_selecionada][coluna_retorno];
                delwin(win);
                endwin();
                return resultado;

            case 27: // ESC
                resultado.indice_linha   = -1;
                resultado.indice_coluna  = -1;
                resultado.valor_retorno  = "";
                delwin(win);
                endwin();
                return resultado;
        }
    }

    return resultado;
}

// ─── Função para configurador de variáveis ─────────────────────

saida_configurador interface_para_configurador(
    int                           numero_variaveis,
    const string                  nomes_variaveis[],
    const string**                opcoes_variaveis,
    const int*                    num_opcoes_por_variavel,
    const ConfigConfigurador&     config)
{
    saida_configurador resultado;
    resultado.confirmado = false;
    resultado.numero_variaveis = numero_variaveis;
    resultado.valores_selecionados = new string[numero_variaveis];

    // Validações
    if (numero_variaveis <= 0 || !nomes_variaveis || !opcoes_variaveis || !num_opcoes_por_variavel)
    {
        resultado.confirmado = false;
        return resultado;
    }

    // Inicializar índices de seleção (qual valor está selecionado para cada variável)
    int* indices_selecionados = new int[numero_variaveis];
    for (int i = 0; i < numero_variaveis; i++)
    {
        indices_selecionados[i] = 0;
        resultado.valores_selecionados[i] = opcoes_variaveis[i][0];
    }

    int variavel_atual = 0;
    int tecla;

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    // ── Cores ───────────────────────────────────────────────
    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            init_color(16, 59, 59, 59);
            init_color(17, 600, 600, 520);
            init_color(18, 380, 380, 320);
            init_color(19, 102, 98, 86);
            init_color(20, 133, 129, 110);
            init_color(21, 680, 680, 600);
            init_color(22, 720, 720, 640);
            init_color(23, 300, 300, 260);
        }

        init_pair(1,  22, 20);
        init_pair(2,  17, 16);
        init_pair(3,  17, 16);
        init_pair(4,  23, 16);
        init_pair(5,  21, 19);
        init_pair(6,  18, 16);
        init_pair(7,  18, 16);
        init_pair(8,  18, 16);
        init_pair(9,  23, 16);

        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;

    // Calcular altura
    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.instrucao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    int espaco_apos_cabecalho = (linhas_cabecalho > 0) ? 2 : 0;

    int linhas_a_exibir = min(numero_variaveis, 10);
    int altura = 2 + linhas_cabecalho + espaco_apos_cabecalho + linhas_a_exibir + 3;

    if (altura  > max_y - 4) altura  = max_y - 4;
    if (largura > max_x - 4) largura = max_x - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);

    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(start_y - 2, titulo_start_x, "%s", linha_titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────
        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        // ── Variáveis ───────────────────────────────────────
        for (int i = 0; i < numero_variaveis && i < 10; i++)
        {
            // Determinar cores
            int cor_pair = config.cores.cor_opcao;
            int cor_atrib = 0;

            if (i == variavel_atual)
            {
                cor_pair = config.cores.cor_opcao_selecionada;
                cor_atrib = A_BOLD;
                preencher_linha(win, linha, 1, largura - 1, config.cores.cor_opcao_selecionada);
            }

            wattron(win, COLOR_PAIR(cor_pair) | cor_atrib);

            // Construir linha: Nome da variável e valor
            string nome_var = nomes_variaveis[i];
            string valor_atual = opcoes_variaveis[i][indices_selecionados[i]];

            // Formatar: "Nome [valor1] valor2 [valor3] <valor4>"
            // Mostrar 3 opções ao redor da atual
            int idx_ini = max(0, indices_selecionados[i] - 1);
            int idx_fim = min(num_opcoes_por_variavel[i] - 1, indices_selecionados[i] + 1);

            string linha_conteudo = "  " + nome_var + ": ";

            for (int j = idx_ini; j <= idx_fim; j++)
            {
                if (j == indices_selecionados[i])
                {
                    linha_conteudo += "[" + opcoes_variaveis[i][j] + "]";
                }
                else if (j == indices_selecionados[i] - 1)
                {
                    linha_conteudo += "< " + opcoes_variaveis[i][j] + " ";
                }
                else if (j == indices_selecionados[i] + 1)
                {
                    linha_conteudo += " " + opcoes_variaveis[i][j] + " >";
                }
                else
                {
                    linha_conteudo += opcoes_variaveis[i][j] + " ";
                }
            }

            if (linha_conteudo.length() > (size_t)(largura - 4))
            {
                linha_conteudo = linha_conteudo.substr(0, largura - 7);
                linha_conteudo += "...";
            }

            mvwprintw(win, linha, config.x, "%s", linha_conteudo.c_str());

            wattroff(win, COLOR_PAIR(cor_pair) | cor_atrib);

            linha++;
        }

        // ── Instrução/Controles ─────────────────────────────
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = config.instrucao.empty() ? config.instrucao_controles : config.instrucao;
            int cor = config.instrucao.empty() ? config.cores.cor_controles : config.cores.cor_instrucao;
            attron(COLOR_PAIR(cor) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(cor) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        switch (tecla)
        {
            case KEY_UP:
                variavel_atual--;
                if (variavel_atual < 0)
                    variavel_atual = numero_variaveis - 1;
                break;

            case KEY_DOWN:
                variavel_atual++;
                if (variavel_atual >= numero_variaveis)
                    variavel_atual = 0;
                break;

            case KEY_LEFT:
                indices_selecionados[variavel_atual]--;
                if (indices_selecionados[variavel_atual] < 0)
                    indices_selecionados[variavel_atual] = num_opcoes_por_variavel[variavel_atual] - 1;
                resultado.valores_selecionados[variavel_atual] =
                    opcoes_variaveis[variavel_atual][indices_selecionados[variavel_atual]];
                break;

            case KEY_RIGHT:
                indices_selecionados[variavel_atual]++;
                if (indices_selecionados[variavel_atual] >= num_opcoes_por_variavel[variavel_atual])
                    indices_selecionados[variavel_atual] = 0;
                resultado.valores_selecionados[variavel_atual] =
                    opcoes_variaveis[variavel_atual][indices_selecionados[variavel_atual]];
                break;

            case 10: // ENTER
                resultado.confirmado = true;
                delwin(win);
                endwin();
                delete[] indices_selecionados;
                return resultado;

            case 27: // ESC
                resultado.confirmado = false;
                delwin(win);
                endwin();
                delete[] indices_selecionados;
                delete[] resultado.valores_selecionados;
                resultado.valores_selecionados = nullptr;
                return resultado;
        }
    }

    delete[] indices_selecionados;
    return resultado;
}

// ─── Função para entrada de texto ───────────────────────────────

saida_entrada_texto interface_para_entrada_texto(
    const ConfigEntradaTexto& config)
{
    saida_entrada_texto resultado;
    resultado.confirmado = false;
    resultado.valor = "";

    string entrada_atual = config.valor_inicial;
    int tecla;

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(1);

    // ── Cores ───────────────────────────────────────────────
    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            init_color(16, 59, 59, 59);
            init_color(17, 600, 600, 520);
            init_color(18, 380, 380, 320);
            init_color(19, 102, 98, 86);
            init_color(20, 133, 129, 110);
            init_color(21, 680, 680, 600);
            init_color(22, 720, 720, 640);
            init_color(23, 300, 300, 260);
        }

        init_pair(1,  22, 20);
        init_pair(2,  17, 16);
        init_pair(3,  17, 16);
        init_pair(4,  23, 16);
        init_pair(5,  21, 19);
        init_pair(6,  18, 16);
        init_pair(7,  18, 16);
        init_pair(8,  18, 16);
        init_pair(9,  23, 16);

        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;

    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    int altura = 2 + linhas_cabecalho + 5; // +5 para campo de entrada

    if (altura  > max_y - 4) altura  = max_y - 4;
    if (largura > max_x - 4) largura = max_x - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);

    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // Obter tipo de entrada como string
    string tipo_label = "";
    switch (config.tipo_entrada)
    {
        case TIPO_TEXTO:      tipo_label = "Texto"; break;
        case TIPO_NUMERO:     tipo_label = "Numero"; break;
        case TIPO_EMAIL:      tipo_label = "Email"; break;
        case TIPO_TELEFONE:   tipo_label = "Telefone"; break;
        case TIPO_SENHA:      tipo_label = "Senha"; break;
    }

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(start_y - 2, titulo_start_x, "%s", linha_titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────
        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        // ── Label de tipo de entrada ────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_instrucao));
        mvwprintw(win, linha, config.x, "Tipo: %s", tipo_label.c_str());
        wattroff(win, COLOR_PAIR(config.cores.cor_instrucao));
        linha++;

        // ── Campo de entrada ────────────────────────────────
        linha++;
        wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));
        preencher_linha(win, linha, 1, largura - 1, config.cores.cor_opcao_selecionada);

        // Exibir entrada (mascarar se for senha)
        string entrada_exibida = entrada_atual;
        if (config.tipo_entrada == TIPO_SENHA)
        {
            entrada_exibida = string(entrada_atual.length(), '*');
        }

        mvwprintw(win, linha, config.x, "%s%s", config.label.c_str(), entrada_exibida.c_str());
        wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));

        // ── Instrução/Controles ─────────────────────────────
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = config.instrucao_controles;
            attron(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        if (tecla >= 32 && tecla <= 126)  // Caracteres imprimíveis
        {
            if ((int)entrada_atual.length() < config.tamanho_maximo)
            {
                // Validar conforme tipo
                bool valido = true;

                if (config.tipo_entrada == TIPO_NUMERO)
                    valido = (tecla >= '0' && tecla <= '9') || tecla == '-' || tecla == '.';
                else if (config.tipo_entrada == TIPO_TELEFONE)
                    valido = (tecla >= '0' && tecla <= '9') || tecla == '-' || tecla == '(' || tecla == ')' || tecla == ' ';

                if (valido)
                    entrada_atual += (char)tecla;
            }
        }
        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8 || tecla == 263)  // Backspace (múltiplas variações)
        {
            if (!entrada_atual.empty())
                entrada_atual.pop_back();
        }
        else if (tecla == 10)  // ENTER
        {
            resultado.confirmado = true;
            resultado.valor = entrada_atual;
            delwin(win);
            endwin();
            return resultado;
        }
        else if (tecla == 27)  // ESC
        {
            resultado.confirmado = false;
            resultado.valor = "";
            delwin(win);
            endwin();
            return resultado;
        }
    }

    return resultado;
}

// ─── Função para entrada de texto grande (até 200 caracteres) ───

saida_entrada_texto interface_para_entrada_texto_grande(
    const ConfigEntradaTexto& config)
{
    saida_entrada_texto resultado;
    resultado.confirmado = false;
    resultado.valor = "";

    // Versão para texto grande com caixa 50x5
    ConfigEntradaTexto config_ajustada = config;
    if (config_ajustada.tamanho_maximo > 250)
        config_ajustada.tamanho_maximo = 250;
    else if (config_ajustada.tamanho_maximo == 100)
        config_ajustada.tamanho_maximo = 250;

    // Parâmetros da caixa de texto multi-linha
    const int LARGURA_CAIXA = 50;  // 50 caracteres de largura
    const int ALTURA_CAIXA = 5;     // 5 linhas de altura
    const int MAX_CARACTERES = 250; // Total de caracteres (50*5)

    string entrada_atual = config.valor_inicial;
    int posicao_cursor = entrada_atual.length();  // Posição absoluta no texto
    int tecla;

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(1);

    // ── Cores ───────────────────────────────────────────────
    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            init_color(16, 59, 59, 59);
            init_color(17, 600, 600, 520);
            init_color(18, 380, 380, 320);
            init_color(19, 102, 98, 86);
            init_color(20, 133, 129, 110);
            init_color(21, 680, 680, 600);
            init_color(22, 720, 720, 640);
            init_color(23, 300, 300, 260);
        }

        init_pair(1,  22, 20);
        init_pair(2,  17, 16);
        init_pair(3,  17, 16);
        init_pair(4,  23, 16);
        init_pair(5,  21, 19);
        init_pair(6,  18, 16);
        init_pair(7,  18, 16);
        init_pair(8,  18, 16);
        init_pair(9,  23, 16);

        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;

    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    // Altura = cabeçalho + info + caixa de texto (50x5)
    int altura = 2 + linhas_cabecalho + 2 + (ALTURA_CAIXA + 2);

    if (altura  > max_y - 4) altura  = max_y - 4;
    if (largura > max_x - 4) largura = max_x - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);

    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // Obter tipo de entrada como string
    string tipo_label = "";
    switch (config.tipo_entrada)
    {
        case TIPO_TEXTO:      tipo_label = "Texto"; break;
        case TIPO_NUMERO:     tipo_label = "Numero"; break;
        case TIPO_EMAIL:      tipo_label = "Email"; break;
        case TIPO_TELEFONE:   tipo_label = "Telefone"; break;
        case TIPO_SENHA:      tipo_label = "Senha"; break;
    }

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(start_y - 2, titulo_start_x, "%s", linha_titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────
        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        // ── Informações de entrada ──────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_instrucao));
        // Calcular linha e coluna da posição do cursor
        int linha_cursor = posicao_cursor / LARGURA_CAIXA;
        int coluna_cursor = posicao_cursor % LARGURA_CAIXA;
        mvwprintw(win, linha, config.x, "Tipo: %s | Caracteres: %d/%d | Linha: %d Col: %d", 
                  tipo_label.c_str(), (int)entrada_atual.length(), MAX_CARACTERES, linha_cursor + 1, coluna_cursor + 1);
        wattroff(win, COLOR_PAIR(config.cores.cor_instrucao));
        linha++;

        // ── Campo de entrada multi-linha (50x5) ─────────────
        linha++;

        // Preparar texto a ser exibido
        string entrada_exibida = entrada_atual;
        
        // Mascarar se for senha
        if (config.tipo_entrada == TIPO_SENHA)
        {
            entrada_exibida = string(entrada_atual.length(), '*');
        }

        // Desenhar a caixa de texto (50 largura x 5 linhas)
        int campo_y_inicio = linha;

        for (int l = 0; l < ALTURA_CAIXA; l++)
        {
            wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));
            
            // Preencher a linha com espaços
            for (int c = 0; c < LARGURA_CAIXA; c++)
            {
                mvwaddch(win, linha + l, config.x + c, ' ');
            }

            // Extrair 50 caracteres dessa linha do texto
            int posicao_inicio_linha = l * LARGURA_CAIXA;
            string texto_linha = "";
            
            if (posicao_inicio_linha < (int)entrada_exibida.length())
            {
                int chars_restantes = (int)entrada_exibida.length() - posicao_inicio_linha;
                int chars_para_exibir = (chars_restantes > LARGURA_CAIXA) ? LARGURA_CAIXA : chars_restantes;
                texto_linha = entrada_exibida.substr(posicao_inicio_linha, chars_para_exibir);
            }

            // Desenhar o texto dessa linha
            mvwprintw(win, linha + l, config.x, "%s", texto_linha.c_str());

            // Desenhar o cursor se estiver nessa linha
            if (l == linha_cursor)
            {
                // Destacar a posição do cursor
                wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_REVERSE);
                mvwaddch(win, linha + l, config.x + coluna_cursor, 
                        (coluna_cursor < (int)texto_linha.length()) ? texto_linha[coluna_cursor] : ' ');
                wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada) | A_REVERSE);
            }

            wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));
        }

        // ── Instrução/Controles ─────────────────────────────
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = "[Digitar] Texto  [Setas] Navegar  [ENTER] Confirmar  [ESC] Cancelar";
            attron(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        if (tecla >= 32 && tecla <= 126)  // Caracteres imprimíveis
        {
            if ((int)entrada_atual.length() < MAX_CARACTERES)
            {
                // Validar conforme tipo
                bool valido = true;

                if (config.tipo_entrada == TIPO_NUMERO)
                    valido = (tecla >= '0' && tecla <= '9') || tecla == '-' || tecla == '.';
                else if (config.tipo_entrada == TIPO_TELEFONE)
                    valido = (tecla >= '0' && tecla <= '9') || tecla == '-' || tecla == '(' || tecla == ')' || tecla == ' ';

                if (valido)
                {
                    entrada_atual.insert(posicao_cursor, 1, (char)tecla);
                    posicao_cursor++;

                    // Se atingiu 50 caracteres nessa linha, pular para próxima linha automaticamente
                    int coluna_atual = posicao_cursor % LARGURA_CAIXA;
                    if (coluna_atual == 0 && posicao_cursor < MAX_CARACTERES)
                    {
                        // Já está no início da próxima linha
                    }
                }
            }
        }
        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8 || tecla == 263)  // Backspace
        {
            if (posicao_cursor > 0)
            {
                entrada_atual.erase(posicao_cursor - 1, 1);
                posicao_cursor--;
            }
        }
        else if (tecla == KEY_DC || tecla == 330)  // Delete
        {
            if (posicao_cursor < (int)entrada_atual.length())
            {
                entrada_atual.erase(posicao_cursor, 1);
            }
        }
        else if (tecla == KEY_LEFT || tecla == 260)  // Seta esquerda
        {
            if (posicao_cursor > 0)
                posicao_cursor--;
        }
        else if (tecla == KEY_RIGHT || tecla == 261)  // Seta direita
        {
            if (posicao_cursor < (int)entrada_atual.length())
                posicao_cursor++;
        }
        else if (tecla == KEY_UP || tecla == 259)  // Seta para cima
        {
            int linha_cursor_atual = posicao_cursor / LARGURA_CAIXA;
            int coluna_cursor_atual = posicao_cursor % LARGURA_CAIXA;
            
            if (linha_cursor_atual > 0)
            {
                posicao_cursor = (linha_cursor_atual - 1) * LARGURA_CAIXA + coluna_cursor_atual;
                if (posicao_cursor > (int)entrada_atual.length())
                    posicao_cursor = entrada_atual.length();
            }
        }
        else if (tecla == KEY_DOWN || tecla == 258)  // Seta para baixo
        {
            int linha_cursor_atual = posicao_cursor / LARGURA_CAIXA;
            int coluna_cursor_atual = posicao_cursor % LARGURA_CAIXA;
            
            if (linha_cursor_atual < ALTURA_CAIXA - 1)
            {
                int nova_posicao = (linha_cursor_atual + 1) * LARGURA_CAIXA + coluna_cursor_atual;
                if (nova_posicao < (int)entrada_atual.length())
                    posicao_cursor = nova_posicao;
                else if ((int)entrada_atual.length() > 0)
                    posicao_cursor = entrada_atual.length();
            }
        }
        else if (tecla == KEY_HOME || tecla == 262)  // Home - início da linha
        {
            int linha_cursor_atual = posicao_cursor / LARGURA_CAIXA;
            posicao_cursor = linha_cursor_atual * LARGURA_CAIXA;
        }
        else if (tecla == KEY_END || tecla == 360)  // End - fim da linha
        {
            int linha_cursor_atual = posicao_cursor / LARGURA_CAIXA;
            int proximo_inicio_linha = (linha_cursor_atual + 1) * LARGURA_CAIXA;
            
            if (proximo_inicio_linha <= (int)entrada_atual.length())
            {
                posicao_cursor = proximo_inicio_linha - 1;
            }
            else
            {
                posicao_cursor = entrada_atual.length();
            }
        }
        else if (tecla == 10)  // ENTER
        {
            resultado.confirmado = true;
            resultado.valor = entrada_atual;
            delwin(win);
            endwin();
            return resultado;
        }
        else if (tecla == 27)  // ESC
        {
            resultado.confirmado = false;
            resultado.valor = "";
            delwin(win);
            endwin();
            return resultado;
        }
    }

    return resultado;
}

// ─── Função para botões com retorno ─────────────────────────────


saida_botoes interface_para_botoes(
    const ConfigBotoes& config)
{
    saida_botoes resultado;
    resultado.confirmado = false;
    resultado.valor_retorno = -999;
    resultado.tecla_pressionada = '\0';

    // Validar
    if (config.numero_botoes <= 0 || config.numero_botoes > 4)
    {
        resultado.confirmado = false;
        return resultado;
    }

    int tecla;

    // ── Inicializar ncurses ─────────────────────────────────
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    // ── Cores ───────────────────────────────────────────────
    if (has_colors())
    {
        start_color();

        if (can_change_color())
        {
            init_color(16, 59, 59, 59);
            init_color(17, 600, 600, 520);
            init_color(18, 380, 380, 320);
            init_color(19, 102, 98, 86);
            init_color(20, 133, 129, 110);
            init_color(21, 680, 680, 600);
            init_color(22, 720, 720, 640);
            init_color(23, 300, 300, 260);
        }

        init_pair(1,  22, 20);
        init_pair(2,  17, 16);
        init_pair(3,  17, 16);
        init_pair(4,  23, 16);
        init_pair(5,  21, 19);
        init_pair(6,  18, 16);
        init_pair(7,  18, 16);
        init_pair(8,  18, 16);
        init_pair(9,  23, 16);

        bkgd(COLOR_PAIR(2));
    }

    // ── Dimensões ───────────────────────────────────────────
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int largura = config.largura_janela;

    int linhas_cabecalho = 0;
    linhas_cabecalho += contar_linhas_com_wrap(config.descricao, largura);
    linhas_cabecalho += contar_linhas_com_wrap(config.caminho, largura);

    int altura = 2 + linhas_cabecalho + 6; // +6 para pergunta e botões

    if (altura  > max_y - 4) altura  = max_y - 4;
    if (largura > max_x - 4) largura = max_x - 4;

    int espaco_titulo = config.titulo.empty() ? 0 : 3;
    int start_y = (max_y - altura - espaco_titulo) / 2 + espaco_titulo;
    int start_x = (max_x - largura) / 2;

    WINDOW *win = newwin(altura, largura, start_y, start_x);

    wbkgd(win, COLOR_PAIR(2));

    clear();
    refresh();

    // ════════════════════════════════════════════════════════
    //  LOOP PRINCIPAL
    // ════════════════════════════════════════════════════════
    while (true)
    {
        werase(win);

        // ── Bordas ──────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_borda));
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(config.cores.cor_borda));

        // ── Título (acima da janela) ────────────────────────
        if (!config.titulo.empty())
        {
            for (int i = start_y - 3; i < start_y; i++)
            {
                move(i, 0);
                clrtoeol();
            }

            string titulo_decorado = config.titulo;
            int titulo_len = (int)titulo_decorado.length();
            int espaco_lateral = (largura - titulo_len - 4) / 2;
            if (espaco_lateral < 1) espaco_lateral = 1;

            string linha_titulo = "";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";
            linha_titulo += "  " + titulo_decorado + "  ";
            for (int i = 0; i < espaco_lateral; i++) linha_titulo += " ";

            int titulo_start_x = centro_x(max_x, (int)linha_titulo.length());
            int titulo_width   = (int)linha_titulo.length();

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 3, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));

            attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
            mvprintw(start_y - 2, titulo_start_x, "%s", linha_titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

            attron(COLOR_PAIR(config.cores.cor_titulo));
            move(start_y - 1, titulo_start_x);
            for (int i = 0; i < titulo_width; i++) addch(' ');
            attroff(COLOR_PAIR(config.cores.cor_titulo));
        }

        int linha = 1;

        // ── Cabeçalho ───────────────────────────────────────
        if (!config.descricao.empty())
        {
            linha += desenhar_texto_wrap(win, linha, config.x, config.descricao, largura, config.cores.cor_descricao);
        }

        if (!config.caminho.empty())
        {
            string caminho_formatado = "> " + config.caminho;
            linha += desenhar_texto_wrap(win, linha, config.x, caminho_formatado, largura, config.cores.cor_caminho);
        }

        if (linhas_cabecalho > 0)
        {
            desenhar_linha_horizontal(win, linha, largura, config.cores.cor_borda);
            linha++;
            linha++;
        }
        else
        {
            linha++;
        }

        // ── Pergunta ─────────────────────────────────────────
        wattron(win, COLOR_PAIR(config.cores.cor_descricao));
        mvwprintw(win, linha, config.x, "%s", config.pergunta.c_str());
        wattroff(win, COLOR_PAIR(config.cores.cor_descricao));
        linha += 2;

        // ── Botões ───────────────────────────────────────────
        // Construir string com os botões
        string linha_botoes = "  ";
        for (int i = 0; i < config.numero_botoes; i++)
        {
            char tecla_upper = toupper(config.botoes[i].tecla);
            linha_botoes += "[";
            linha_botoes += tecla_upper;
            linha_botoes += "] ";
            linha_botoes += config.botoes[i].label;

            if (i < config.numero_botoes - 1)
                linha_botoes += "    ";
        }

        wattron(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));
        preencher_linha(win, linha, 1, largura - 1, config.cores.cor_opcao_selecionada);
        mvwprintw(win, linha, config.x, "%s", linha_botoes.c_str());
        wattroff(win, COLOR_PAIR(config.cores.cor_opcao_selecionada));

        // ── Instrução/Controles ─────────────────────────────
        {
            int linha_ctrl = start_y + altura + 1;
            move(linha_ctrl, 0);
            clrtoeol();

            string controles = config.instrucao_controles;
            attron(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
            mvprintw(
                linha_ctrl,
                centro_x(max_x, (int)controles.length()),
                "%s", controles.c_str()
            );
            attroff(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
        }

        wrefresh(win);
        refresh();

        // ── Input ───────────────────────────────────────────
        tecla = getch();

        // Procurar pela tecla pressionada
        char tecla_lower = tolower(tecla);
        for (int i = 0; i < config.numero_botoes; i++)
        {
            if (tolower(config.botoes[i].tecla) == tecla_lower)
            {
                resultado.confirmado = true;
                resultado.valor_retorno = config.botoes[i].valor_retorno;
                resultado.tecla_pressionada = config.botoes[i].tecla;
                delwin(win);
                endwin();
                return resultado;
            }
        }

        // Verificar ESC
        if (tecla == 27)
        {
            resultado.confirmado = false;
            resultado.valor_retorno = -999;
            resultado.tecla_pressionada = '\0';
            delwin(win);
            endwin();
            return resultado;
        }
    }

    return resultado;
}

// ═══════════════════════════════════════════════════════════════
// MOSTRAR DETALHES - TÓPICOS COM DESCRIÇÕES E PAGINAÇÃO
// ═══════════════════════════════════════════════════════════════
saida_detalhes mostrar_detalhes(
    const TopicDetalhes topicos[],
    int numero_topicos,
    const ConfigDetalhes& config)
{
    saida_detalhes resultado;
    resultado.confirmado = false;
    resultado.pagina_atual = 0;

    if (numero_topicos <= 0 || numero_topicos > 10)
        return resultado;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();
        init_pair(1, 22, 20);
        init_pair(2, 17, 16);
        init_pair(3, 17, 16);
        init_pair(4, 23, 16);
        init_pair(5, 21, 19);
        init_pair(6, 18, 16);
        init_pair(7, 18, 16);
        init_pair(8, COLOR_YELLOW, COLOR_BLACK);
    }

    int topicos_por_pagina = config.topicos_por_pagina;
    if (topicos_por_pagina <= 0) topicos_por_pagina = 5;

    int total_paginas = (numero_topicos + topicos_por_pagina - 1) / topicos_por_pagina;
    int pagina_atual = 0;

    while (true)
    {
        clear();

        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);

        // ── Título ──────────────────────────────────────
        attron(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);
        mvprintw(0, centro_x(max_x, (int)config.titulo.length()), "%s", config.titulo.c_str());
        attroff(COLOR_PAIR(config.cores.cor_titulo) | A_BOLD);

        // ── Descrição geral ─────────────────────────────
        if (!config.descricao.empty())
        {
            attron(COLOR_PAIR(config.cores.cor_descricao));
            mvprintw(1, config.x, "%s", config.descricao.c_str());
            attroff(COLOR_PAIR(config.cores.cor_descricao));
        }

        // ── Separador superior ──────────────────────────
        attron(COLOR_PAIR(config.cores.cor_borda));
        mvhline(2, 0, ACS_HLINE, max_x);
        attroff(COLOR_PAIR(config.cores.cor_borda));

        // ── Tópicos da página ───────────────────────────
        int inicio = pagina_atual * topicos_por_pagina;
        int fim = inicio + topicos_por_pagina;
        if (fim > numero_topicos) fim = numero_topicos;

        int linha = 3;
        int largura_wrap = max_x - config.x - 4;
        if (largura_wrap < 20) largura_wrap = 20;

        int limite_inferior = max_y - 3;

        for (int i = inicio; i < fim; i++)
        {
            if (linha >= limite_inferior) break;

            // ── Título do tópico ────────────────────────
            attron(COLOR_PAIR(config.cores.cor_opcao) | A_BOLD);
            mvprintw(linha, config.x, "> %s", topicos[i].titulo.c_str());
            attroff(COLOR_PAIR(config.cores.cor_opcao) | A_BOLD);
            linha++;

            // ── Descrição com word-wrap manual ──────────
            if (!topicos[i].descricao.empty() && linha < limite_inferior)
            {
                const string& desc = topicos[i].descricao;
                int pos = 0;
                int len = (int)desc.length();

                while (pos < len && linha < limite_inferior)
                {
                    // Determinar quantos caracteres cabem nesta linha
                    int restante = len - pos;
                    int pedaco = (restante > largura_wrap) ? largura_wrap : restante;

                    // Se não é o fim do texto e cortamos no meio,
                    // recuar até o último espaço para não quebrar palavra
                    if (pos + pedaco < len && pedaco == largura_wrap)
                    {
                        int ultimo_espaco = -1;
                        for (int j = pedaco - 1; j >= 0; j--)
                        {
                            if (desc[pos + j] == ' ')
                            {
                                ultimo_espaco = j;
                                break;
                            }
                        }
                        if (ultimo_espaco > 0)
                            pedaco = ultimo_espaco + 1; // inclui o espaço no pedaço
                    }

                    // Extrair a substring desta linha
                    string linha_texto = desc.substr(pos, pedaco);

                    // Remover espaços no início/fim da linha visual
                    // (só trailing, para não desalinhar)
                    while (!linha_texto.empty() && linha_texto.back() == ' ')
                        linha_texto.pop_back();
                    // Leading spaces só na continuação
                    if (pos > 0)
                    {
                        size_t first = linha_texto.find_first_not_of(' ');
                        if (first != string::npos && first > 0)
                            linha_texto = linha_texto.substr(first);
                    }

                    attron(COLOR_PAIR(config.cores.cor_descricao));
                    mvprintw(linha, config.x + 2, "%s", linha_texto.c_str());
                    attroff(COLOR_PAIR(config.cores.cor_descricao));

                    linha++;
                    pos += pedaco;

                    // Pular espaços no início do próximo pedaço
                    while (pos < len && desc[pos] == ' ')
                        pos++;
                }
            }

            // ── Espaço entre tópicos ────────────────────
            linha++;
        }

        // ── Separador inferior ──────────────────────────
        attron(COLOR_PAIR(config.cores.cor_borda));
        mvhline(max_y - 3, 0, ACS_HLINE, max_x);
        attroff(COLOR_PAIR(config.cores.cor_borda));

        // ── Paginação ───────────────────────────────────
        if (total_paginas > 1)
        {
            string pg = "Pagina " + to_string(pagina_atual + 1) + "/" + to_string(total_paginas);
            attron(COLOR_PAIR(config.cores.cor_paginacao));
            mvprintw(max_y - 2, config.x, "%s", pg.c_str());
            attroff(COLOR_PAIR(config.cores.cor_paginacao));
        }

        // ── Controles ──────────────────────────────────
        attron(COLOR_PAIR(config.cores.cor_controles) | A_DIM);
        mvprintw(max_y - 1,
                 centro_x(max_x, (int)config.instrucao_controles.length()),
                 "%s", config.instrucao_controles.c_str());
        attroff(COLOR_PAIR(config.cores.cor_controles) | A_DIM);

        refresh();

        // ── Input ───────────────────────────────────────
        int tecla = getch();

        if (tecla == KEY_RIGHT || tecla == 'l' || tecla == 'L')
        {
            if (pagina_atual < total_paginas - 1)
                pagina_atual++;
        }
        else if (tecla == KEY_LEFT || tecla == 'h' || tecla == 'H')
        {
            if (pagina_atual > 0)
                pagina_atual--;
        }
        else if (tecla == '\n' || tecla == 10 || tecla == 13)
        {
            resultado.confirmado = true;
            resultado.pagina_atual = pagina_atual;
            endwin();
            return resultado;
        }
        else if (tecla == 27 || tecla == 'q' || tecla == 'Q')
        {
            resultado.confirmado = false;
            resultado.pagina_atual = pagina_atual;
            endwin();
            return resultado;
        }
    }

    endwin();
    return resultado;
}


// ═══════════════════════════════════════════════════════════════
// TEXTO CORRIDO COM PAGINAÇÃO
// ═══════════════════════════════════════════════════════════════



void mostrar_texto(const string& texto, const ConfigTexto& config)
{
    if (texto.empty()) return;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();
        init_pair(1, 22, 20);
        init_pair(2, 17, 16);
        init_pair(3, 17, 16);
        init_pair(4, 10, 16);
        init_pair(5, 21, 19);
        init_pair(6, 18, 16);
    }

    int pagina_atual = 0;

    while (true)
    {
        clear();
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);

        int largura_wrap = max_x - config.x - 4;
        if (largura_wrap < 20) largura_wrap = 20;

        // ── Quebrar texto em linhas visuais ─────────
        vector<string> linhas;
        size_t pos = 0;

        while (pos < texto.length())
        {
            size_t nl = texto.find('\n', pos);
            string paragrafo;
            if (nl != string::npos)
            {
                paragrafo = texto.substr(pos, nl - pos);
                pos = nl + 1;
            }
            else
            {
                paragrafo = texto.substr(pos);
                pos = texto.length();
            }

            if (paragrafo.empty())
            {
                linhas.push_back("");
                continue;
            }

            int p = 0;
            int len = (int)paragrafo.length();

            while (p < len)
            {
                if (p > 0)
                    while (p < len && paragrafo[p] == ' ') p++;
                if (p >= len) break;

                int restante = len - p;
                int pedaco = (restante > largura_wrap) ? largura_wrap : restante;

                if (p + pedaco < len && pedaco == largura_wrap)
                {
                    for (int j = pedaco - 1; j >= 0; j--)
                    {
                        if (paragrafo[p + j] == ' ')
                        {
                            pedaco = j + 1;
                            break;
                        }
                    }
                }

                string lt = paragrafo.substr(p, pedaco);
                while (!lt.empty() && lt.back() == ' ') lt.pop_back();
                linhas.push_back(lt);
                p += pedaco;
            }
        }

        // ── Paginação ──────────────────────────────
        int linhas_por_pagina = max_y - 6;
        if (linhas_por_pagina < 1) linhas_por_pagina = 1;

        int total_paginas = ((int)linhas.size() + linhas_por_pagina - 1) / linhas_por_pagina;
        if (total_paginas < 1) total_paginas = 1;
        if (pagina_atual >= total_paginas) pagina_atual = total_paginas - 1;

        // ── Título ──────────────────────────────────
        attron(COLOR_PAIR(config.cor_titulo) | A_BOLD);
        mvprintw(0, centro_x(max_x, (int)config.titulo.length()), "%s", config.titulo.c_str());
        attroff(COLOR_PAIR(config.cor_titulo) | A_BOLD);

        attron(COLOR_PAIR(config.cor_borda));
        mvhline(1, 0, ACS_HLINE, max_x);
        attroff(COLOR_PAIR(config.cor_borda));

        // ── Texto ───────────────────────────────────
        int inicio = pagina_atual * linhas_por_pagina;
        int fim = inicio + linhas_por_pagina;
        if (fim > (int)linhas.size()) fim = (int)linhas.size();

        int linha_tela = 2;
        for (int i = inicio; i < fim; i++)
        {
            attron(COLOR_PAIR(config.cor_texto));
            mvprintw(linha_tela, config.x, "%s", linhas[i].c_str());
            attroff(COLOR_PAIR(config.cor_texto));
            linha_tela++;
        }

        attron(COLOR_PAIR(config.cor_borda));
        mvhline(max_y - 3, 0, ACS_HLINE, max_x);
        attroff(COLOR_PAIR(config.cor_borda));

        if (total_paginas > 1)
        {
            string pg = "Pagina " + to_string(pagina_atual + 1) + "/" + to_string(total_paginas);
            attron(COLOR_PAIR(config.cor_paginacao));
            mvprintw(max_y - 2, config.x, "%s", pg.c_str());
            attroff(COLOR_PAIR(config.cor_paginacao));
        }

        attron(COLOR_PAIR(config.cor_controles) | A_DIM);
        mvprintw(max_y - 1, centro_x(max_x, (int)config.instrucao_controles.length()),
                 "%s", config.instrucao_controles.c_str());
        attroff(COLOR_PAIR(config.cor_controles) | A_DIM);

        refresh();

        // ── Input ───────────────────────────────────
        int tecla = getch();

        if (tecla == KEY_RIGHT || tecla == 'l')
        {
            if (pagina_atual < total_paginas - 1) pagina_atual++;
        }
        else if (tecla == KEY_LEFT || tecla == 'h')
        {
            if (pagina_atual > 0) pagina_atual--;
        }
        else if (tecla == 27 || tecla == 'q' || tecla == '\n' || tecla == 10)
        {
            endwin();
            return;
        }
    }
}
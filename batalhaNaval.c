#include <stdio.h>
#include <stdlib.h> // Necessário para abs()

#define BOARD 10       // Tamanho do tabuleiro 10x10
#define SHIP_SIZE 3    // Navios de tamanho 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5   // Área afetada por habilidade
#define COMBINADO 8    // Navio + habilidade

#define SKILL_N 5      // Tamanho das matrizes de habilidade (5x5)

// Protótipos
void inicializaTabuleiro(int tab[BOARD][BOARD]);
void posicionaNavios(int tab[BOARD][BOARD]);
void exibirTabuleiro(int tab[BOARD][BOARD]);
void criarCone(int mapa[SKILL_N][SKILL_N]);
void criarCruz(int mapa[SKILL_N][SKILL_N]);
void criarOctaedro(int mapa[SKILL_N][SKILL_N]);
void aplicarHabilidade(int tab[BOARD][BOARD], int mapa[SKILL_N][SKILL_N],
                       int origemLinha, int origemColuna, int centroLinhaMapa,
                       int centroColunaMapa, int usaTopoComoOrigem);

int main() {
    int tab[BOARD][BOARD];

    // 1️⃣ Inicializa o tabuleiro com água
    inicializaTabuleiro(tab);

    // 2️⃣ Posiciona os navios
    posicionaNavios(tab);

    // 3️⃣ Cria mapas de habilidade
    int cone[SKILL_N][SKILL_N];
    int cruz[SKILL_N][SKILL_N];
    int octaedro[SKILL_N][SKILL_N];

    criarCone(cone);
    criarCruz(cruz);
    criarOctaedro(octaedro);

    // 4️⃣ Define pontos de origem no tabuleiro
    int origemConeLin = 0, origemConeCol = 4;    // ponta do cone
    int origemCruzLin = 5, origemCruzCol = 2;    // centro da cruz
    int origemOctLin  = 3, origemOctCol  = 5;    // centro do octaedro

    // 5️⃣ Aplica habilidades ao tabuleiro
    aplicarHabilidade(tab, cone, origemConeLin, origemConeCol, 0, SKILL_N/2, 1);
    aplicarHabilidade(tab, cruz, origemCruzLin, origemCruzCol, SKILL_N/2, SKILL_N/2, 0);
    aplicarHabilidade(tab, octaedro, origemOctLin, origemOctCol, SKILL_N/2, SKILL_N/2, 0);

    // 6️⃣ Exibe tabuleiro final
    exibirTabuleiro(tab);

    // 7️⃣ Legenda
    printf("\nLegenda:\n");
    printf("0 -> Água\n");
    printf("3 -> Navio (sem habilidade)\n");
    printf("5 -> Área afetada por habilidade (sem navio)\n");
    printf("8 -> Navio + Área de habilidade (interseção)\n");

    return 0;
}

/* ============================
   Funções auxiliares
   ============================ */

void inicializaTabuleiro(int tab[BOARD][BOARD]) {
    for (int i = 0; i < BOARD; i++)
        for (int j = 0; j < BOARD; j++)
            tab[i][j] = AGUA;
}

void posicionaNavios(int tab[BOARD][BOARD]) {
    // Navio 1: horizontal (2,1)
    int lh = 2, ch = 1;
    for (int c = ch; c < ch + SHIP_SIZE; c++)
        tab[lh][c] = NAVIO;

    // Navio 2: vertical (4,6)
    int lv = 4, cv = 6;
    for (int r = lv; r < lv + SHIP_SIZE; r++)
        tab[r][cv] = NAVIO;

    // Navio 3: diagonal ↘ (1,7)
    int ld1 = 1, cd1 = 7;
    for (int k = 0; k < SHIP_SIZE; k++)
        tab[ld1 + k][cd1 + k] = NAVIO;

    // Navio 4: diagonal ↙ (7,7)
    int ld2 = 7, cd2 = 7;
    for (int k = 0; k < SHIP_SIZE; k++)
        tab[ld2 - k][cd2 + k] = NAVIO;
}

void exibirTabuleiro(int tab[BOARD][BOARD]) {
    printf("\nTABULEIRO (valores numéricos):\n\n");
    printf("   ");
    for (int c = 0; c < BOARD; c++) printf("%d ", c);
    printf("\n");
    for (int r = 0; r < BOARD; r++) {
        printf("%2d ", r);
        for (int c = 0; c < BOARD; c++)
            printf("%d ", tab[r][c]);
        printf("\n");
    }
}

/* ============================
   Criação dinâmica das matrizes de habilidade
   ============================ */

void criarCone(int mapa[SKILL_N][SKILL_N]) {
    int centro = SKILL_N / 2;
    for (int i = 0; i < SKILL_N; i++)
        for (int j = 0; j < SKILL_N; j++)
            mapa[i][j] = 0;
    for (int r = 0; r < SKILL_N; r++) {
        int minC = centro - r;
        int maxC = centro + r;
        for (int c = 0; c < SKILL_N; c++)
            if (c >= minC && c <= maxC)
                if (c >= 0 && c < SKILL_N)
                    mapa[r][c] = 1;
    }
}

void criarCruz(int mapa[SKILL_N][SKILL_N]) {
    int centro = SKILL_N / 2;
    for (int i = 0; i < SKILL_N; i++)
        for (int j = 0; j < SKILL_N; j++)
            mapa[i][j] = 0;
    for (int k = 0; k < SKILL_N; k++) {
        mapa[centro][k] = 1;
        mapa[k][centro] = 1;
    }
}

void criarOctaedro(int mapa[SKILL_N][SKILL_N]) {
    int centro = SKILL_N / 2;
    int raio = centro;
    for (int i = 0; i < SKILL_N; i++)
        for (int j = 0; j < SKILL_N; j++)
            mapa[i][j] = 0;
    for (int i = 0; i < SKILL_N; i++)
        for (int j = 0; j < SKILL_N; j++)
            if (abs(i - centro) + abs(j - centro) <= raio)
                mapa[i][j] = 1;
}

void aplicarHabilidade(int tab[BOARD][BOARD], int mapa[SKILL_N][SKILL_N],
                       int origemLinha, int origemColuna, int centroLinhaMapa,
                       int centroColunaMapa, int usaTopoComoOrigem) {
    for (int r = 0; r < SKILL_N; r++) {
        for (int c = 0; c < SKILL_N; c++) {
            if (mapa[r][c] == 1) {
                int dr = r - centroLinhaMapa;
                int dc = c - centroColunaMapa;
                int tabR = origemLinha + dr;
                int tabC = origemColuna + dc;
                if (tabR >= 0 && tabR < BOARD && tabC >= 0 && tabC < BOARD) {
                    if (tab[tabR][tabC] == AGUA)
                        tab[tabR][tabC] = HABILIDADE;
                    else if (tab[tabR][tabC] == NAVIO)
                        tab[tabR][tabC] = COMBINADO;
                }
            }
        }
    }
}

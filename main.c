/* Integrantes: Bernardo Scheuer, Bryan Allan Jahn, João Daniel de Araújo Lima, João HUmberto Tecila Mayer, Vitor Rovani Marcelino */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ldde.h"

#define NUM_PREDIOS 5
#define INF 999999
#define TAM_STR 64

/* Nomes e siglas dos predios indexados de 0 a 4 */
const char *NOMES_PREDIOS[NUM_PREDIOS] = {
    "Bloco A",
    "Bloco B",
    "Biblioteca",
    "Laboratorio",
    "Refeitorio"};

const char SIGLAS_PREDIOS[NUM_PREDIOS] = {'A', 'B', 'C', 'L', 'R'};

/* Prototipos das funcoes do sistema */
void inicializarGrafo(int grafo[NUM_PREDIOS][NUM_PREDIOS]);
void calcularFloydWarshall(int grafo[NUM_PREDIOS][NUM_PREDIOS], int alcancavel[NUM_PREDIOS][NUM_PREDIOS]);
void exibirMapa(int grafo[NUM_PREDIOS][NUM_PREDIOS]);
void dijkstra(int grafo[NUM_PREDIOS][NUM_PREDIOS], int origem, int destino);
void verificarConexao(int alcancavel[NUM_PREDIOS][NUM_PREDIOS]);
void exibirMatrizAlcancabilidade(int alcancavel[NUM_PREDIOS][NUM_PREDIOS]);
int lerPredio(const char *mensagem);
void paraMinusculo(char *str);
void limparQuebraLinha(char *str);

int main(void)
{
    int grafo[NUM_PREDIOS][NUM_PREDIOS];
    int alcancavel[NUM_PREDIOS][NUM_PREDIOS];
    char buffer[TAM_STR];
    int opcao;
    int origem, destino;

    inicializarGrafo(grafo);
    calcularFloydWarshall(grafo, alcancavel);

    do
    {
        printf("\n==========================================\n");
        printf("        SISTEMA DE ROTAS DO CAMPUS\n");
        printf("==========================================\n\n");
        printf("1 - Exibir mapa do campus\n");
        printf("2 - Encontrar menor rota entre dois predios\n");
        printf("3 - Verificar conexao entre dois predios\n");
        printf("4 - Exibir matriz de alcancabilidade\n");
        printf("0 - Sair\n\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            break;
        }
        limparQuebraLinha(buffer);
        opcao = atoi(buffer);

        if (opcao == 0 && strcmp(buffer, "0") != 0)
        {
            opcao = -1;
        }

        switch (opcao)
        {
        case 1:
            exibirMapa(grafo);
            break;
        case 2:
            printf("\n--- MENOR ROTA (DIJKSTRA) ---\n");
            origem = lerPredio("Informe o predio de origem (numero ou nome): ");
            if (origem == -1)
            {
                break;
            }
            destino = lerPredio("Informe o predio de destino (numero ou nome): ");
            if (destino != -1)
            {
                dijkstra(grafo, origem, destino);
            }
            break;
        case 3:
            verificarConexao(alcancavel);
            break;
        case 4:
            exibirMatrizAlcancabilidade(alcancavel);
            break;
        case 0:
            printf("\nEncerrando o sistema...\n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    return 0;
}

/* Preenche a matriz de adjacencia simetrica com as distancias em metros */
void inicializarGrafo(int grafo[NUM_PREDIOS][NUM_PREDIOS])
{
    int i, j;

    for (i = 0; i < NUM_PREDIOS; i++)
    {
        for (j = 0; j < NUM_PREDIOS; j++)
        {
            grafo[i][j] = 0;
        }
    }

    /* Indices: 0=Bloco A, 1=Bloco B, 2=Biblioteca, 3=Laboratorio, 4=Refeitorio */
    grafo[1][3] = 240;
    grafo[3][1] = 240; /* Bloco B <-> Laboratorio */
    grafo[3][0] = 210;
    grafo[0][3] = 210; /* Laboratorio <-> Bloco A */
    grafo[0][4] = 190;
    grafo[4][0] = 190; /* Bloco A <-> Refeitorio */
    grafo[4][2] = 280;
    grafo[2][4] = 280; /* Refeitorio <-> Biblioteca */
    grafo[2][0] = 160;
    grafo[0][2] = 160; /* Biblioteca <-> Bloco A */
}

/* RF01: Exibe as conexoes do campus sem duplicidade */
void exibirMapa(int grafo[NUM_PREDIOS][NUM_PREDIOS])
{
    int i, j;

    printf("\n=== MAPA DO CAMPUS (CONEXOES) ===\n");
    for (i = 0; i < NUM_PREDIOS; i++)
    {
        for (j = i + 1; j < NUM_PREDIOS; j++)
        {
            if (grafo[i][j] > 0)
            {
                printf("- %s <-> %s: %d m\n",
                       NOMES_PREDIOS[i], NOMES_PREDIOS[j], grafo[i][j]);
            }
        }
    }
}

/* RF02 e RF05: Calcula o menor caminho com Dijkstra e armazena a rota na LDDE */
void dijkstra(int grafo[NUM_PREDIOS][NUM_PREDIOS], int origem, int destino)
{
    int dist[NUM_PREDIOS];
    int visitado[NUM_PREDIOS];
    int anterior[NUM_PREDIOS];
    int i, count, u, v, minDist, atual, tamRota, vertice;
    LDDE *rota;

    for (i = 0; i < NUM_PREDIOS; i++)
    {
        dist[i] = INF;
        visitado[i] = 0;
        anterior[i] = -1;
    }

    dist[origem] = 0;

    for (count = 0; count < NUM_PREDIOS - 1; count++)
    {
        minDist = INF;
        u = -1;

        for (i = 0; i < NUM_PREDIOS; i++)
        {
            if (!visitado[i] && dist[i] < minDist)
            {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1)
        {
            break;
        }

        visitado[u] = 1;

        for (v = 0; v < NUM_PREDIOS; v++)
        {
            if (!visitado[v] && grafo[u][v] > 0 && dist[u] != INF &&
                dist[u] + grafo[u][v] < dist[v])
            {
                dist[v] = dist[u] + grafo[u][v];
                anterior[v] = u;
            }
        }
    }

    if (dist[destino] == INF)
    {
        printf("\nNao existe caminho entre %s e %s.\n",
               NOMES_PREDIOS[origem], NOMES_PREDIOS[destino]);
        return;
    }

    rota = criarLDDE();
    if (rota == NULL)
    {
        printf("\nErro ao alocar memoria para a rota.\n");
        return;
    }

    atual = destino;
    while (atual != -1)
    {
        if (!inserirInicio(rota, atual))
        {
            printf("\nErro ao montar a rota.\n");
            destruirLDDE(rota);
            return;
        }
        atual = anterior[atual];
    }

    tamRota = tamanhoLDDE(rota);
    printf("\nMenor rota encontrada:\n");
    printf("Sequencia: ");
    for (i = 0; i < tamRota; i++)
    {
        if (obterElemento(rota, i, &vertice))
        {
            printf("%s", NOMES_PREDIOS[vertice]);
            if (i < tamRota - 1)
            {
                printf(" -> ");
            }
        }
    }
    printf("\nDistancia total: %d m\n", dist[destino]);

    destruirLDDE(rota);
}

/* Executa Floyd-Warshall e gera a matriz binaria de alcancabilidade */
void calcularFloydWarshall(int grafo[NUM_PREDIOS][NUM_PREDIOS], int alcancavel[NUM_PREDIOS][NUM_PREDIOS])
{
    int distFW[NUM_PREDIOS][NUM_PREDIOS];
    int i, j, k;

    for (i = 0; i < NUM_PREDIOS; i++)
    {
        for (j = 0; j < NUM_PREDIOS; j++)
        {
            if (i == j)
            {
                distFW[i][j] = 0;
            }
            else if (grafo[i][j] > 0)
            {
                distFW[i][j] = grafo[i][j];
            }
            else
            {
                distFW[i][j] = INF;
            }
        }
    }

    for (k = 0; k < NUM_PREDIOS; k++)
    {
        for (i = 0; i < NUM_PREDIOS; i++)
        {
            for (j = 0; j < NUM_PREDIOS; j++)
            {
                if (distFW[i][k] != INF && distFW[k][j] != INF &&
                    distFW[i][k] + distFW[k][j] < distFW[i][j])
                {
                    distFW[i][j] = distFW[i][k] + distFW[k][j];
                }
            }
        }
    }

    for (i = 0; i < NUM_PREDIOS; i++)
    {
        for (j = 0; j < NUM_PREDIOS; j++)
        {
            alcancavel[i][j] = (distFW[i][j] < INF) ? 1 : 0;
        }
    }
}

/* RF03: Consulta se existe caminho entre dois predios via Floyd-Warshall */
void verificarConexao(int alcancavel[NUM_PREDIOS][NUM_PREDIOS])
{
    int p1, p2;

    printf("\n--- VERIFICAR CONEXAO (FLOYD-WARSHALL) ---\n");
    p1 = lerPredio("Informe o primeiro predio (numero ou nome): ");
    if (p1 == -1)
    {
        return;
    }
    p2 = lerPredio("Informe o segundo predio (numero ou nome): ");
    if (p2 == -1)
    {
        return;
    }

    if (alcancavel[p1][p2] == 1)
    {
        printf("\nSIM! Existe caminho entre %s e %s.\n",
               NOMES_PREDIOS[p1], NOMES_PREDIOS[p2]);
    }
    else
    {
        printf("\nNAO! Nao existe caminho entre %s e %s.\n",
               NOMES_PREDIOS[p1], NOMES_PREDIOS[p2]);
    }
}

/* RF04: Exibe a matriz de alcancabilidade (1 = conectado, 0 = desconectado) */
void exibirMatrizAlcancabilidade(int alcancavel[NUM_PREDIOS][NUM_PREDIOS])
{
    int i, j;

    printf("\n=== MATRIZ DE ALCANCABILIDADE ===\n");
    printf("Legenda: A=Bloco A | B=Bloco B | C=Biblioteca | L=Laboratorio | R=Refeitorio\n\n");
    printf("    ");
    for (j = 0; j < NUM_PREDIOS; j++)
    {
        printf("%c ", SIGLAS_PREDIOS[j]);
    }
    printf("\n");

    for (i = 0; i < NUM_PREDIOS; i++)
    {
        printf("%c | ", SIGLAS_PREDIOS[i]);
        for (j = 0; j < NUM_PREDIOS; j++)
        {
            printf("%d ", alcancavel[i][j]);
        }
        printf("\n");
    }
    printf("\n1: Existe caminho | 0: Nao existe caminho\n");
}

/* Permite selecionar o predio por numero (1 a 5) ou digitando o nome */
int lerPredio(const char *mensagem)
{
    char entrada[TAM_STR];
    char nomeTemp[TAM_STR];
    int i, num;

    printf("\nPredios disponiveis:\n");
    for (i = 0; i < NUM_PREDIOS; i++)
    {
        printf("  %d - %s\n", i + 1, NOMES_PREDIOS[i]);
    }

    printf("%s", mensagem);
    if (fgets(entrada, sizeof(entrada), stdin) == NULL)
    {
        return -1;
    }
    limparQuebraLinha(entrada);

    num = atoi(entrada);
    if (num >= 1 && num <= NUM_PREDIOS)
    {
        return num - 1;
    }

    paraMinusculo(entrada);
    for (i = 0; i < NUM_PREDIOS; i++)
    {
        strcpy(nomeTemp, NOMES_PREDIOS[i]);
        paraMinusculo(nomeTemp);
        if (strcmp(entrada, nomeTemp) == 0)
        {
            return i;
        }
    }

    printf("Predio '%s' nao encontrado!\n", entrada);
    return -1;
}

void paraMinusculo(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void limparQuebraLinha(char *str)
{
    size_t len;
    len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        len--;
    }
}
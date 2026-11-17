/* Integrantes: Bernardo Scheuer, Bryan Allan Jahn, João Daniel de Araújo Lima, João HUmberto Tecila Mayer, Vitor Rovani Marcelino */

#include <stdio.h>
#include <stdlib.h>
#include "ldde_priv.h"

LDDE *criarLDDE(void)
{
    LDDE *lista;

    lista = (LDDE *)malloc(sizeof(LDDE));
    if (lista != NULL)
    {
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

void destruirLDDE(LDDE *lista)
{
    struct no *atual;
    struct no *prox;

    if (lista == NULL)
    {
        return;
    }

    atual = lista->inicio;
    while (atual != NULL)
    {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(lista);
}

int listaVazia(LDDE *lista)
{
    if (lista == NULL || lista->tamanho == 0)
    {
        return 1;
    }
    return 0;
}

int tamanhoLDDE(LDDE *lista)
{
    if (lista == NULL)
    {
        return 0;
    }
    return lista->tamanho;
}

int inserirInicio(LDDE *lista, int dado)
{
    struct no *novo;

    if (lista == NULL)
    {
        return 0;
    }

    novo = (struct no *)malloc(sizeof(struct no));
    if (novo == NULL)
    {
        return 0;
    }

    novo->dado = dado;
    novo->ant = NULL;
    novo->prox = lista->inicio;

    if (lista->inicio != NULL)
    {
        lista->inicio->ant = novo;
    }
    else
    {
        lista->fim = novo;
    }

    lista->inicio = novo;
    lista->tamanho++;
    return 1;
}

int inserirFim(LDDE *lista, int dado)
{
    struct no *novo;

    if (lista == NULL)
    {
        return 0;
    }

    novo = (struct no *)malloc(sizeof(struct no));
    if (novo == NULL)
    {
        return 0;
    }

    novo->dado = dado;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim != NULL)
    {
        lista->fim->prox = novo;
    }
    else
    {
        lista->inicio = novo;
    }

    lista->fim = novo;
    lista->tamanho++;
    return 1;
}

int removerInicio(LDDE *lista, int *dado)
{
    struct no *removido;

    if (listaVazia(lista))
    {
        return 0;
    }

    removido = lista->inicio;
    if (dado != NULL)
    {
        *dado = removido->dado;
    }

    lista->inicio = removido->prox;
    if (lista->inicio != NULL)
    {
        lista->inicio->ant = NULL;
    }
    else
    {
        lista->fim = NULL;
    }

    free(removido);
    lista->tamanho--;
    return 1;
}

int obterElemento(LDDE *lista, int posicao, int *dado)
{
    struct no *atual;
    int i;

    if (listaVazia(lista) || posicao < 0 || posicao >= lista->tamanho || dado == NULL)
    {
        return 0;
    }

    atual = lista->inicio;
    for (i = 0; i < posicao; i++)
    {
        atual = atual->prox;
    }

    *dado = atual->dado;
    return 1;
}
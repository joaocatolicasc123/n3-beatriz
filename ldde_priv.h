/* Integrantes: Bernardo Scheuer, Bryan Allan Jahn, João Daniel de Araújo Lima, João HUmberto Tecila Mayer, Vitor Rovani Marcelino */

#ifndef LDDE_PRIV_H
#define LDDE_PRIV_H

#include "ldde.h"

struct no {
    int dado;
    struct no *ant;
    struct no *prox;
};

struct ldde {
    struct no *inicio;
    struct no *fim;
    int tamanho;
};

#endif

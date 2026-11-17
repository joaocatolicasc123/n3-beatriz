/* Integrantes: Bernardo Scheuer, Bryan Allan Jahn, João Daniel de Araújo Lima, João HUmberto Tecila Mayer, Vitor Rovani Marcelino */

#ifndef LDDE_H
#define LDDE_H

/* Ponteiro opaco: oculta os campos internos da lista */
typedef struct ldde LDDE;

LDDE *criarLDDE(void);
void destruirLDDE(LDDE *lista);
int listaVazia(LDDE *lista);
int tamanhoLDDE(LDDE *lista);
int inserirInicio(LDDE *lista, int dado);
int inserirFim(LDDE *lista, int dado);
int removerInicio(LDDE *lista, int *dado);
int obterElemento(LDDE *lista, int posicao, int *dado);

#endif

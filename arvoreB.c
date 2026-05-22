#include "arvoreB.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

/*estrutura de fila da biblioteca sys/queue*/
struct fila {
  struct nodo *no;
  TAILQ_ENTRY(fila) ant_prox;
};

TAILQ_HEAD(fila_head, fila); // cria fila

/*verifica se houve falha na alocação de memoria, se sim encerra o programa*/
void naoAlocado() {
  fprintf(stderr, "Falha ao alocar memoria.\n");
  exit(1);
}

/*cria nodo da arvore*/
struct nodo *criarNodo(int32_t t_arvore) {
  struct nodo *no = malloc(sizeof(struct nodo));
  if (!no)
    naoAlocado();

  /*inicializa as variaveis*/
  no->n = 0;
  no->ehFolha = true;
  no->chaves = calloc((2 * t_arvore - 1), sizeof(int32_t));
  no->filhos = calloc(2 * t_arvore, sizeof(struct nodo *));

  if (!no->chaves || !no->filhos)
    naoAlocado();

  return (no);
}

/*cria a arvore*/
struct arvoreB *criarArvoreB(int32_t t_arvore) {
  struct arvoreB *arvore = malloc(sizeof(struct arvoreB));
  if (!arvore)
    naoAlocado();

  arvore->t_arvore = t_arvore;        // grau minimo da arvore
  arvore->raiz = criarNodo(t_arvore); // cria raiz

  return (arvore);
}

/*reparte filho cheio da arvore, criando um filho adicional*/
void repartirNo(struct arvoreB *arvore, struct nodo *no, int i) {
  struct nodo *novo = criarNodo(arvore->t_arvore);
  struct nodo *cheio = no->filhos[i]; // recebe filho cheio
  novo->ehFolha = cheio->ehFolha;
  int t = arvore->t_arvore;
  novo->n = t - 1;

  for (int j = 0; j < t - 1; j++) // novo recebe metade das chaves de cheio
    novo->chaves[j] = cheio->chaves[j + t];

  /*se o no cheio não for folha, novo recebe metade dos filhos do no cheio*/
  if (!cheio->ehFolha) {
    for (int j = 0; j < t; j++)
      novo->filhos[j] = cheio->filhos[j + t];
  }

  cheio->n = t - 1;

  for (int j = no->n; j >= i + 1; j--)
    no->filhos[j + 1] = no->filhos[j]; // abre espaço

  no->filhos[i + 1] = novo; // coloca na posição correta
  for (int j = no->n - 1; j >= i; j--)
    no->chaves[j + 1] = no->chaves[j];

  no->chaves[i] = cheio->chaves[t - 1]; // sobe a chave do meio para o pai
  no->n++;                              // atualiza numero de chaves
}

/*insere chave em no não cheio, interativo*/
void inserirEmNoNaoCheio(struct nodo *no, struct arvoreB *arvore,
                         int32_t chave) {
  int t = arvore->t_arvore;

  while (1) {
    int i = no->n - 1;

    /*tenta encontrar onde o lugar onde a chave deve ficar*/
    if (no->ehFolha) {
      while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i];
        i--;
      }

      no->chaves[i + 1] = chave; // insere a chave na posição correta
      no->n++;                   // aumenta o numero de chaves do no

      return; // retorna, pois ja encontrou
    }

    /*se não é folha, encontra filho correto para descer*/
    while (i >= 0 && chave < no->chaves[i])
      i--;

    i++;

    /*se estiver cheio, divide antes*/
    if (no->filhos[i]->n == 2 * t - 1) {
      repartirNo(arvore, no, i);

      if (chave > no->chaves[i])
        i++;
    }

    no = no->filhos[i]; // desce para o filho
  }
}

/*insere chave na arvore*/
void inserirArvoreB(struct arvoreB *arvore, int32_t chave) {
  struct nodo *raiz = arvore->raiz;
  int t = arvore->t_arvore;

  /*se a raiz esta cheia*/
  if (raiz->n == 2 * t - 1) {

    struct nodo *nova_raiz = criarNodo(t); // cria novo raiz
    nova_raiz->ehFolha = false;
    arvore->raiz = nova_raiz;
    nova_raiz->filhos[0] = raiz; // raiz antiga vira filho da nova

    repartirNo(arvore, nova_raiz, 0); // divide raiz antiga
    inserirEmNoNaoCheio(nova_raiz, arvore, chave);

  } else {
    inserirEmNoNaoCheio(raiz, arvore, chave); // insere direto
  }
}

/*imprime arvore B*/
void imprimirArvoreB(struct arvoreB *arvore) {
  if (!arvore || !arvore->raiz)
    return;

  struct fila_head fila;
  TAILQ_INIT(&fila); // inicializa fila

  struct fila *novo = malloc(sizeof(struct fila));
  if (!novo)
    exit(1);

  novo->no = arvore->raiz; // coloca raiz na fila
  TAILQ_INSERT_TAIL(&fila, novo, ant_prox);

  int nivel = 0;

  while (!TAILQ_EMPTY(&fila)) {

    int tamanhoNivel = 0;

    struct fila *tmp;
    TAILQ_FOREACH(tmp, &fila, ant_prox) {
      tamanhoNivel++; // conta quantos nos tem no nivel
    }

    printf("----//----\n");
    printf("Nivel %d\n", nivel);
    printf("----//----\n");

    for (int k = 0; k < tamanhoNivel; k++) {

      struct fila *atual = TAILQ_FIRST(&fila);
      struct nodo *no = atual->no;

      TAILQ_REMOVE(&fila, atual, ant_prox);
      free(atual);

      if (no->ehFolha)
        printf("F ");
      else
        printf("I ");

      printf("(n:%d) [", no->n);

      /*imprime*/
      for (int i = 0; i < no->n; i++) {
        printf("%d", no->chaves[i]);
        if (i < no->n - 1)
          printf(" ");
      }

      printf("]");

      if (k < tamanhoNivel - 1)
        printf("  ");

      /*adiciona filhos na fila*/
      if (!no->ehFolha) {
        for (int i = 0; i <= no->n; i++) {

          struct fila *novo = malloc(sizeof(struct fila));
          if (!novo)
            exit(1);

          novo->no = no->filhos[i];
          TAILQ_INSERT_TAIL(&fila, novo, ant_prox);
        }
      }
    }

    printf("\n");
    nivel++; // vai para o proximo nivel
  }
}

/*busca uma chave na arvore*/
struct nodo *buscarArvoreB(struct arvoreB *arvore, int32_t chave,
                           int32_t *idxEncontrado) {
  if (!arvore)
    return NULL;

  int i;
  struct nodo *no = arvore->raiz;
  *idxEncontrado = -1; // indice não encontrado

  /*encontra a posição onde a chave pode estar*/
  while (no != NULL) {
    i = 0;
    while (i <= no->n - 1 && chave > no->chaves[i])
      i++;

    /*encontrou a chave*/
    if (i <= no->n - 1 && chave == no->chaves[i]) {
      *idxEncontrado = i;
      return no; // retorna nodo
    }

    no = no->filhos[i]; // desce para o proximo filho
  }

  return no; // não encontrou
}

/*imprime os nodos*/
void imprimirNodos(struct nodo *no) {
  if (!no)
    return;

  int i;
  for (i = 0; i < no->n; i++) {
    imprimirNodos(no->filhos[i]);
    printf("%d ", no->chaves[i]);
  }

  imprimirNodos(no->filhos[i]);

  return;
}

/*imprime chaves ordenadas*/
void imprimirEmOrdem(struct arvoreB *Arvore) {
  if (!Arvore || !Arvore->raiz) {
    printf("Árvore vazia.\n");
    return;
  }

  printf("Em ordem: ");
  imprimirNodos(Arvore->raiz);
  printf("\n");

  return;
}

/*libera memoria dos nodos*/
void liberarMemoria(struct nodo *no, int t) {
  if (no == NULL)
    return;

  if (!no->ehFolha) {
    for (int i = 0; i <= no->n; i++) {
      liberarMemoria(no->filhos[i], t);
    }
  }

  free(no->chaves);
  free(no->filhos);

  free(no);
}

/*libera memoria da arvore*/
void deletarArvore(struct arvoreB *arvore) {
  if (arvore == NULL)
    return;

  int t = arvore->t_arvore;

  liberarMemoria(arvore->raiz, t);
  free(arvore);
}

/*percorre o caminho mais a direita ate encontrar o antecessor*/
struct nodo *antecessor(struct nodo *no, int *idx) {
  struct nodo *aux = no; 

  while (!aux->ehFolha)
    aux = aux->filhos[aux->n];

  *idx = aux->n - 1; 
  return aux;        // retorna o nodo e o indice do antecessor
}

/*percorre o caminho mais a esquerda ate encontrar o sucessor*/
struct nodo *sucessor(struct nodo *no, int *idx) {
  struct nodo *aux = no;

  while (!aux->ehFolha)
    aux = aux->filhos[0];

  *idx = 0; 
  return aux; // retorna o nodo e o indice do sucessor
}

/*Encontra o irmão mais proximo e com mais chaves*/
struct nodo *irmaoImediatoComMaisChaves(struct arvoreB *arvore, struct nodo *no,
                                        int i) {
  // irmão a esquerda
  if (i > 0 && no->filhos[i - 1]->n >= arvore->t_arvore)
    return no->filhos[i - 1];

  // irmão a direita
  if (i < no->n && no->filhos[i + 1]->n >= arvore->t_arvore)
    return no->filhos[i + 1];

  return NULL;
}

/*Junta dois nodos, realocando as chaves*/
void merge(struct nodo *noFe, struct nodo *noFd, int32_t chave) {
  /*desce a chave do nodo pai, e aumenta o n do filho esquerdo*/
  noFe->chaves[noFe->n] = chave;
  noFe->n++;

  int nOriginal = noFe->n;

  /*passa as chaves do filho direito do nodo pai para o esquerdo*/
  for (int i = 0; i < noFd->n; i++) {
    noFe->chaves[noFe->n] = noFd->chaves[i];
    noFe->n++;
  }

  /*passa os filhos do noFd para o noFe*/
  if (!noFd->ehFolha)
    for (int i = 0; i <= noFd->n; i++)
      noFe->filhos[nOriginal + i] = noFd->filhos[i];

  free(noFd->chaves);
  free(noFd->filhos);
  free(noFd);
  return;
}

/*reajusta chaves do nodo pai*/
void reajustarPai(struct nodo *no, int idx) {
  for (int j = idx; j < no->n - 1; j++)
    no->chaves[j] = no->chaves[j + 1];
  for (int j = idx + 1; j < no->n; j++)
    no->filhos[j] = no->filhos[j + 1];

  no->n--;
  return;
}

/*pega chaves do irmão da direita*/
void pegarChavesDireita(struct nodo *no, int i, struct nodo *irmao) {
  struct nodo *aux = no->filhos[i];

  aux->chaves[aux->n] = no->chaves[i];
  if (!aux->ehFolha)
    aux->filhos[aux->n + 1] = irmao->filhos[0];
  aux->n++;

  no->chaves[i] = irmao->chaves[0];

  for (int j = 0; j < irmao->n - 1; j++)
    irmao->chaves[j] = irmao->chaves[j + 1];

  if (!irmao->ehFolha)
    for (int j = 0; j < irmao->n; j++)
      irmao->filhos[j] = irmao->filhos[j + 1];

  irmao->n--;
}

/*pega as chaves do irmão da esquerda*/
void pegarChavesEsquerda(struct nodo *no, int i, struct nodo *irmao) {
  struct nodo *aux = no->filhos[i];

  for (int j = aux->n; j > 0; j--)
    aux->chaves[j] = aux->chaves[j - 1];

  if (!aux->ehFolha)
    for (int j = aux->n + 1; j > 0; j--)
      aux->filhos[j] = aux->filhos[j - 1];

  aux->chaves[0] = no->chaves[i - 1];
  if (!aux->ehFolha)
    aux->filhos[0] = irmao->filhos[irmao->n];
  aux->n++;

  no->chaves[i - 1] = irmao->chaves[irmao->n - 1];
  irmao->n--;
}

bool excluirChaveArvoreB(struct arvoreB *arvore, struct nodo *no,
                         int32_t chave) {
  if (!arvore || !no)
    return false;

  int i = 0;
  while (i < no->n && chave > no->chaves[i])
    i++;

  /*verifica se i é um indice valido*/
  if (i < no->n && chave == no->chaves[i]) {
    /*Caso 1: nodo a ser removido esta na folha*/
    if (no->ehFolha) {
      for (int j = i; j < no->n - 1; j++)
        no->chaves[j] = no->chaves[j + 1];
      no->n--;
      return true;
    }
    /*Caso 2: a chave esta no nodo atual e ele não é uma folha*/
    else {
      int idx = -1;
      /*Caso 2a*/
      if (no->filhos[i]->n >=
          arvore->t_arvore) { // filho a esquerda tem chaves suficientes
        struct nodo *ant = antecessor(no->filhos[i], &idx);
        no->chaves[i] = ant->chaves[idx];
        return excluirChaveArvoreB(arvore, no->filhos[i], ant->chaves[idx]);
      }
      /*Caso 2b*/
      else if (no->filhos[i + 1]->n >=
               arvore->t_arvore) { // filho a direita tem chaves sufientes
        struct nodo *suc = sucessor(no->filhos[i + 1], &idx);
        no->chaves[i] = suc->chaves[idx];
        return excluirChaveArvoreB(arvore, no->filhos[i + 1], suc->chaves[idx]);
      }
      /*Caso 2c*/
      else { // nenhum dos filhos tem chaves suficientes
        merge(no->filhos[i], no->filhos[i + 1], chave); // junta os dois filhos
        reajustarPai(no, i);

        /*chave estava na raiz*/
        if (no == arvore->raiz && no->n == 0) {
          arvore->raiz = no->filhos[i]; // nova raiz
          free(no->chaves);
          free(no->filhos);
          free(no);
          return excluirChaveArvoreB(arvore, arvore->raiz, chave);
        }

        return excluirChaveArvoreB(
            arvore, no->filhos[i],
            chave); // continua procurando a chave no filho que foi juntado
      }
    }
  } else {
    /*Caso 3: a chave não esta no nodo atual*/
    if (no->ehFolha)
      return false; // não há mais onde procurar

    /*Caso 3a*/
    if (no->filhos[i]->n < arvore->t_arvore) { //filho tem chaves insuficientes
      struct nodo *irmao = irmaoImediatoComMaisChaves(arvore, no, i);

      /*verifica se irmão tem pelo menos t chaves*/
      if (irmao != NULL) {

        if (irmao == no->filhos[i + 1])
          pegarChavesDireita(no, i, irmao);
        else
          pegarChavesEsquerda(no, i, irmao);

      } else {
        /*caso 3b*/ 
        if (i < no->n) { //nenhum irmão tem chaves suficientes
          merge(no->filhos[i], no->filhos[i + 1], no->chaves[i]); //junta os dois filhos
          reajustarPai(no, i);

        } else { //filho só possui irmão a esquerda
          merge(no->filhos[i - 1], no->filhos[i], no->chaves[i - 1]);
          reajustarPai(no, i - 1);

          i--;
        }
      }
    }

    /*chave estava na raiz, substituir pelo nodo do merge anterior*/
    if (no == arvore->raiz && no->n == 0) {
      arvore->raiz = no->filhos[0];
      free(no->chaves);
      free(no->filhos);
      free(no);
      return excluirChaveArvoreB(arvore, arvore->raiz, chave);
    }

    /*o nodo possui chaves suficientes para continuar a exclusão*/
    return excluirChaveArvoreB(arvore, no->filhos[i], chave);
  }
}

/*remove uma chave da arvore B, se conseguir retorna true, caso contrario
 * false*/
bool removerChaveArvoreB(struct arvoreB *arvore, int32_t chave) {
  if (!arvore)
    return false;

  return (excluirChaveArvoreB(arvore, arvore->raiz, chave));
}
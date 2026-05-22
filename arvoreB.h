#ifndef ARVORE_B_H_
#define ARVORE_B_H_

#include <stdbool.h>
#include <stdint.h>

struct nodo {
  int32_t n;
  int32_t *chaves;
  struct nodo **filhos;
  bool ehFolha;
};

struct arvoreB {
  struct nodo *raiz;
  int32_t t_arvore; // grau da arvore
};

struct arvoreB *criarArvoreB(int32_t t_arvore);

void inserirArvoreB(struct arvoreB *arvore, int32_t chave);

void imprimirArvoreB(struct arvoreB *arvore);

void imprimirEmOrdem(struct arvoreB *arvore);

struct nodo *buscarArvoreB(struct arvoreB *arvore, int32_t chave,
                           int32_t *idxEncontrado);

bool removerChaveArvoreB(struct arvoreB *arvore, int32_t chave);

void deletarArvore(struct arvoreB *arvore);

#endif
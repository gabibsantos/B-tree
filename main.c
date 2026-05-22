#include "arvoreB.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRAU_ARVORE 3
#define LIM_SIMULACAO 150 // valor máx dos valores aleatórios
#define N_INSERCOES 30
#define N_BUSCAS 10
#define N_EXCLUSOES 5

int main() {
  srand(time(NULL));

  int32_t t = GRAU_ARVORE;
  int32_t idx, chaveK, valores;
  int32_t chavesExclusao[N_EXCLUSOES];
  struct arvoreB *arvoreTeste;
  struct nodo *encontrado;

  printf("--- INICIANDO SIMULAÇÃO DE ÁRVORE B PARA TESTE. ---\n\n");

  // cria ÁrvoreB
  printf("Criando ÁrvoreB com t = %d...\n", t);

  arvoreTeste = criarArvoreB(t);
  printf("A ÁrvoreB foi criada com sucesso.\n");
  printf("\n------------------------------------\n\n");

  // insere valores aleatórios na ÁrvoreB
  printf("Inserindo %d valores aleatórios na ÁrvoreB\n", N_INSERCOES);

  printf("Inserindo: ");
  for (int i = 0; i < N_INSERCOES; i++) {
    valores = rand() % LIM_SIMULACAO;
    printf("%d ", valores);

    if (i < N_EXCLUSOES)
      chavesExclusao[i] = valores;

    inserirArvoreB(arvoreTeste, valores);
  }
  printf("\nTodos os valores foram inseridos com sucesso.\n");
  printf("\n------------------------------------\n\n");

  // imprime os valores da ÁrvoreB em ordem
  printf("Impressão em-ordem (Crescente):\n\n");
  imprimirEmOrdem(arvoreTeste);
  printf("\n------------------------------------\n\n");

  // procura valores aleatórios dentro da ÁrvoreB
  printf("Teste de busca com %d valores aleatórios:\n\n", N_BUSCAS);

  for (int i = 0; i < N_BUSCAS; i++) {
    chaveK = rand() % LIM_SIMULACAO;
    encontrado = buscarArvoreB(arvoreTeste, chaveK, &idx);

    if (encontrado != NULL) {
      printf("Chave %d encontrada no nó %p, índice [%d]\n", chaveK, encontrado,
             idx);
    } else {
      printf("Chave %d não encontrada na ÁrvoreB.\n", chaveK);
    }
  }
  printf("\n------------------------------------\n\n");

  // imprime a estrutura da ÁrvoreB
  printf("Impressão da ÁrvoreB:\n\n");
  imprimirArvoreB(arvoreTeste);
  printf("\n------------------------------------\n\n");

  // exlcui chaves da ÁrvoreB
  printf("Excluindo chaves da ÁrvoreB... \n\n");
  for (int i = 0; i < N_EXCLUSOES; i++) {
    if (removerChaveArvoreB(arvoreTeste, chavesExclusao[i])) {
      printf("Chave %d encontrada e excluída com sucesso.\n\n",
             chavesExclusao[i]);
      imprimirArvoreB(arvoreTeste);
    } else
      printf("Chave %d já foi removida).\n", chavesExclusao[i]);

    printf("\n------------------------------------\n\n");
  }

  // libera a memória alocada pela ÁrvoreB
  printf("Deletando a ÁrvoreB e liberando a memória...\n");
  deletarArvore(arvoreTeste);
  printf("A ÁrvoreB foi deletada com sucesso.\n\n");

  printf("--- FIM DO TESTE. ---\n");

  return 0;
}
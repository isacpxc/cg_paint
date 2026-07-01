#include "Estruturas.h"
#include <stdio.h>
#include <stdlib.h>


int salvarCena(CenaGrafica *cena, const char *nomeArquivo) {
  FILE *arquivo = fopen(nomeArquivo, "wb");

  if (arquivo == NULL) {
    printf("[ERRO] Nao foi possivel criar o arquivo '%s'.\n", nomeArquivo);
    return 0;
  }

  size_t gravados = fwrite(cena, sizeof(CenaGrafica), 1, arquivo);
  fclose(arquivo);

  if (gravados == 1) {
    printf("Cena salva com sucesso em '%s'!\n", nomeArquivo);
    return 1;
  } else {
    printf("[ERRO] Falha ao gravar os dados no arquivo '%s'.\n", nomeArquivo);
    return 0;
  }
}

int carregarCena(CenaGrafica *cena, const char *nomeArquivo) {
  FILE *arquivo = fopen(nomeArquivo, "rb");

  if (arquivo == NULL) {
    printf("[ERRO] Arquivo '%s' nao encontrado.\n", nomeArquivo);
    return 0;
  }

  size_t lidos = fread(cena, sizeof(CenaGrafica), 1, arquivo);
  fclose(arquivo);

  if (lidos == 1) {
    printf("Cena carregada com sucesso de '%s'!\n", nomeArquivo);
    return 1;
  } else {
    printf("[ERRO] Arquivo '%s' corrompido ou incompleto.\n", nomeArquivo);
    return 0;
  }
}

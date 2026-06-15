#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

void salvarCena(CenaGrafica *cena, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Não foi possível criar o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    size_t gravados = fwrite(cena, sizeof(CenaGrafica), 1, arquivo);
    fclose(arquivo);

    if (gravados == 1) {
        printf("Cena salva com sucesso em '%s'!\n", nomeArquivo);
    } else {
        printf("Falha ao gravar os dados no arquivo '%s'.\n", nomeArquivo);
    }
}

void carregarCena(CenaGrafica *cena, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado.\n", nomeArquivo);
        return;
    }

    size_t lidos = fread(cena, sizeof(CenaGrafica), 1, arquivo);
    fclose(arquivo);

    if (lidos == 1) {
        printf("Cena carregada com sucesso de '%s'!\n", nomeArquivo);
    } else {
        printf("O arquivo '%s' está corrompido ou deu ruim.\n", nomeArquivo);
    }
}

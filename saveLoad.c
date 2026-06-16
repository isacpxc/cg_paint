// saveLoad.c
// Modulo de Persistencia - Responsavel: Joao Edson de Sousa
// Salva e carrega a CenaGrafica em arquivo binario.

#include <stdio.h>
#include <stdlib.h>
#include "Estruturas.h"

// =============================================================================
// salvarCena
// -----------------------------------------------------------------------------
// Salva toda a CenaGrafica em um arquivo binario.
// Nota: o arquivo tera ~1.6 MB pois salva os arrays completos (1000 slots),
// mesmo os nao utilizados. Para o escopo do trabalho, isso e aceitavel.
//
// Retorno: 1 se salvou com sucesso, 0 se falhou.
// =============================================================================
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

// =============================================================================
// carregarCena
// -----------------------------------------------------------------------------
// Carrega uma CenaGrafica de um arquivo binario, substituindo a cena atual.
//
// Retorno: 1 se carregou com sucesso, 0 se falhou.
// =============================================================================
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

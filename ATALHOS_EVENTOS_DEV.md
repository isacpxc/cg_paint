# Atalhos e eventos implementados na branch `dev`

Este arquivo documenta a parte de eventos integrada ao `main.c`.

## Inicializacao OpenGL/GLUT

O `main.c` agora centraliza a inicializacao do core da aplicacao:

- `inicializarContexto(&ctx)` prepara `CenaGrafica`, `EstadoInterface`, ferramenta inicial, cor, espessura e animacao.
- `inicializarOpenGL(argc, argv)` cria a janela GLUT, configura modo `GLUT_DOUBLE | GLUT_RGB` e define a cor de fundo.
- `registrarCallbacks()` registra `display`, `mouse`, `movimentoMouse`, `teclado`, `tecladoEspecial`, `redimensionar` e `timer`.

## Ferramentas

| Atalho | Acao |
|---|---|
| `1` | Ponto |
| `2` | Reta |
| `3` | Poligono |
| `4` | Selecao |
| `5` | Borracha |

## Mouse

| Acao | Comportamento |
|---|---|
| Clique esquerdo em Ponto | Cria ponto |
| Clique esquerdo em Reta | Primeiro clique define P1, segundo clique define P2 |
| Clique esquerdo em Poligono | Adiciona vertice temporario |
| Clique direito em Poligono | Finaliza o poligono atual |
| Clique esquerdo em Selecao | Chama `selecionarObjeto` |
| Arrastar em Selecao | Chama `transladarObjeto` com delta do mouse |
| Clique esquerdo em Borracha | Seleciona e exclui objeto clicado |

## Atalhos gerais

| Atalho | Acao |
|---|---|
| `C` | Cor anterior |
| `c` | Proxima cor |
| `+` / `-` | Aumentar/diminuir espessura |
| `L` | Limpar tela |
| `D` | Desselecionar todos |
| `Backspace` | Remove ultimo vertice temporario; fora do poligono, exclui selecionados |
| `Enter` | Finaliza poligono em construcao |
| `Esc` | Cancela operacao temporaria; se nao houver operacao, sai |
| `H` ou `F1` | Mostra ajuda no console |

## Arquivo e animacao

| Atalho | Acao |
|---|---|
| `S` ou `F5` | Salva em `cena_cg_paint.bin` |
| `O` ou `F9` | Carrega de `cena_cg_paint.bin` |
| `Espaco` ou `A` | Liga/desliga animacao |

## Transformacoes integradas

| Atalho | Funcao chamada |
|---|---|
| Setas | `transladarObjeto` |
| `[` / `]` | `rotacionarObjeto` |
| `,` / `.` | `escalarObjeto` |
| `X` / `Y` / `Z` | `refletirObjeto` |
| `J` / `K` | `cisalharObjeto` |

## Observacao sobre os fallbacks `weak`

A branch `dev` ainda nao tem todos os modulos (`Transformacoes.c`, selecao completa, etc.). Por isso, o `main.c` inclui implementacoes temporarias `weak` para algumas funcoes ainda ausentes.

Quando outro arquivo `.c` implementar a mesma funcao, o GCC/MinGW usa a implementacao real e ignora o fallback. Isso permite testar a parte de eventos agora sem travar o restante do grupo.

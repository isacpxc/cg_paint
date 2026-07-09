# 🎨 CG Paint em OpenGL

Este é um projeto de Computação Gráfica que implementa uma aplicação estilo "Paint" totalmente construída em C utilizando a biblioteca OpenGL (FreeGLUT).

## 🚀 Funcionalidades

O **CG Paint** suporta a criação, seleção, exclusão, transformação geométrica e animação de formas primitivas 2D. 

Principais recursos:
- Desenho de pontos, retas e polígonos.
- Ferramenta lápis para desenho livre.
- Cores e espessuras ajustáveis.
- Transformações geométricas implementadas manualmente utilizando matrizes homogêneas (Translação, Rotação, Escala, Reflexão e Cisalhamento).
- Sistema de seleção e remoção de objetos ("Borracha").
- Persistência: Salvamento e carregamento da cena em arquivo binário.
- Animação simples de objetos na tela (movimentos lineares e orbitais).

---

## ⚙️ Como Compilar e Executar

Existem duas formas principais de compilar o projeto no Windows:

### Opção 1: Via Code::Blocks (Recomendado)
O projeto já conta com o arquivo de configuração do Code::Blocks (`cg_paint.cbp`).
1. Abra o Code::Blocks.
2. Acesse `File > Open...` e selecione o arquivo **`cg_paint.cbp`**.
3. Pressione a tecla **`F9`** (ou acesse `Build > Build and run`). O ambiente cuidará de compilar e abrir a janela do OpenGL.

### Opção 2: Via Terminal (GCC)
Se você possui o GCC configurado com as bibliotecas do FreeGLUT e OpenGL instaladas (`-lfreeglut`, `-lopengl32`, `-lglu32`), abra o terminal na pasta raiz do projeto e execute:

```bash
gcc main.c primitivas.c saveLoad.c Selecao.c animacao.c tranformacoes.c convexo.c -o bin/Debug/cg_paint.exe -lfreeglut -lopengl32 -lglu32
```
Em seguida, basta rodar o executável gerado:
```bash
./bin/Debug/cg_paint.exe
```

---

## 🎮 Manual de Uso e Atalhos (Teclado)

Após abrir o programa, utilize o mouse e os seguintes atalhos no teclado para interagir com a aplicação:

### Ferramentas de Desenho
* **`1`** : Ponto (Clique para desenhar)
* **`2`** : Reta (Clique 2 vezes: P1 e P2)
* **`3`** : Polígono (Botão Esquerdo para adicionar vértices, Botão Direito para finalizar)
* **`4`** : Seleção (Clique próximo aos objetos para selecioná-los)
* **`5`** : Borracha (Clique no objeto para excluí-lo)
* **`6`** : Lápis (Segure e arraste para desenhar livremente)

### Controles Gerais
* **`C`** : Trocar cor (cicla pelas cores da paleta)
* **`+` / `-`** : Aumentar ou diminuir a espessura da linha / tamanho do ponto
* **`L`** : Limpar a tela inteira
* **`F1`** : Salvar cena atual (salva em `cena_salva.bin`)
* **`F2`** : Carregar cena salva
* **`ESC`** : Sair do programa

### Transformações Geométricas (Aplicadas a objetos selecionados)
* **Translação**: `T` (Direita), `G` (Esquerda), `U` (Cima), `J` (Baixo)
* **Rotação**: `R` (+15 graus), `F` (-15 graus)
* **Escala**: `E` (Aumentar +20%), `N` (Diminuir -20%)
* **Reflexão**: `X` (Eixo X), `Y` (Eixo Y), `O` (Na Origem)
* **Cisalhamento**: `H` (+X), `K` (-X)

### Animação (Aplicada a objetos selecionados)
* **`P`** : Ativar / Pausar animação
* **Movimento Linear**: `W` (Cima), `S` (Baixo), `A` (Esquerda), `D` (Direita)
* **Movimento Orbital**: `M` (Circular)

---

## 🏗️ Estrutura do Código
* `Estruturas.h`: Define as structs core do projeto (`Ponto`, `Reta`, `Poligono`, `CenaGrafica`, matrizes, etc).
* `main.c`: Configuração do OpenGL, Loop principal, tratamento de teclado e mouse.
* `primitivas.c`: Lógica matemática de adição das primitivas na cena.
* `Selecao.c`: Algoritmos de seleção (Cohen-Sutherland para retas e colisões para polígonos/pontos) e remoção.
* `tranformacoes.c`: Implementação matricial das transformações geométricas 2D.
* `animacao.c`: Controle de frame a frame para mover objetos.
* `saveLoad.c`: Leitura e escrita binária para manter persistência.

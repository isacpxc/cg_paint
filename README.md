# 🎨 Projeto Paint em OpenGL (Trabalho de CG)

Olá equipe! 👋

Bem-vindos ao repositório do nosso projeto de Computação Gráfica. O objetivo deste trabalho é construir uma aplicação estilo "Paint" utilizando C/C++ e a biblioteca OpenGL. 

Esta documentação serve como ponto de partida para que todos entendam o que já foi modelado até agora e como vamos seguir com a implementação.

---

## 🏗️ Arquitetura e Modelagem Atual

Para garantir que o nosso código não vire uma bagunça conforme o projeto crescer, nós focamos inicialmente em definir muito bem a **arquitetura de dados** do projeto. 

Toda a nossa modelagem foi feita no arquivo **`Estruturas.h`**. Ele é o coração do projeto e já cobre **100% dos requisitos** exigidos pelo professor. Abaixo, um resumo do que vocês vão encontrar nele:

### 1. Objetos Geométricos e Desenho
* **`Ponto`**: Estrutura base de coordenada (x, y).
* **`Matriz3x3`**: Estrutura para implementarmos manualmente as nossas matrizes de transformações geométricas.
* **`PontoDesenho`, `Reta`, `Poligono`**: Nossas estruturas visuais. Elas guardam as coordenadas e as propriedades visuais (cor, espessura, se estão **selecionadas** ou não).
* **`CenaGrafica`**: A struct mestre que armazena *todos* os objetos desenhados na tela de forma organizada (vetores de pontos, retas e polígonos).

### 2. Controle de Estado e Interface
* **Modos de Ferramenta**: Enums para sabermos se o usuário está usando Lápis, Linha, Polígono, Seleção ou Borracha.
* **`EstadoAnimacao`**: Estrutura preparada para lidar com os requisitos de animação.
* **`EstadoInterface` e `ContextoPaint`**: Estruturas que guardam tudo que está acontecendo no momento (qual cor está selecionada, se o usuário está no meio do clique de uma reta, qual ferramenta está ativa, etc.).

---

## ⚙️ Como rodar o projeto

Para compilar e executar o projeto, nós estamos utilizando o **Code::Blocks**.

Siga o passo a passo:
1. Abra o Code::Blocks.
2. Vá em `File > Open...` e selecione o arquivo **`cg_paint.cbp`** na raiz deste repositório.
3. Com o projeto aberto, aperte a tecla **`F9`** (ou vá em `Build > Build and run` no menu superior).
4. O Code::Blocks irá compilar todos os arquivos `.c` e abrirá a janela gráfica do OpenGL!

---

## 🚀 Próximos Passos: Onde a equipe deve focar?

Tendo o esqueleto pronto (`Estruturas.h`), nosso trabalho agora é "dar vida" aos protótipos listados no final deste arquivo. 

Podemos dividir as tarefas entre a equipe nas seguintes frentes (criando os arquivos `.c` correspondentes):

1. **`Main.c` (Loop e Eventos):**
   * Configurar a janela OpenGL (`glutInit`, `glutDisplayFunc`, etc).
   * Capturar os eventos de mouse (`glutMouseFunc`, `glutMotionFunc`) e repassar para a nossa `Interface`.
   * Capturar eventos de teclado para trocar as ferramentas.

2. **`Desenho.c` (Renderização):**
   * Implementar a função `desenharCena(...)` percorrendo a `CenaGrafica` e usando as primitivas do OpenGL (`GL_POINTS`, `GL_LINES`, `GL_POLYGON`) para pintar nossos objetos na tela.

3. **`Interacao.c` (Lógica de Criação/Seleção):**
   * Implementar a lógica de adicionar novos pontos, retas e polígonos.
   * Criar os algoritmos de seleção (clicou perto do objeto, ele fica `selecionado = 1`).
   * Lógica de exclusão (apagar os selecionados).

4. **`Transformacoes.c` (Geometria):**
   * Implementar as operações matemáticas nas funções `transladarObjeto`, `rotacionarObjeto`, `escalarObjeto`, `refletirObjeto` e `cisalharObjeto`. Aqui nós vamos multiplicar nossos vértices pela `Matriz3x3`.

5. **`Arquivos.c` e `Animacao.c`:**
   * Lógica simples para ler/escrever nossa struct `CenaGrafica` em um arquivo binário (`fread`/`fwrite`).
   * Lógica da função `glutTimerFunc` para animar os objetos.

---

## 🤝 Dicas para o Desenvolvimento

* **Leiam o `Estruturas.h`!** Antes de codificar, dêem uma olhada no arquivo para entenderem os nomes das variáveis e as estruturas disponíveis.
* **Mantenham as coisas modulares:** Evitem fazer tudo no `main.c`. Vamos colocar a lógica de desenho junto com o desenho, a de matemática junto com a matemática.
* Qualquer dúvida sobre a arquitetura ou como acessar um dado, é só mandar mensagem no grupo!

Bora codar! 🚀

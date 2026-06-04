# 🛠️ Boas Práticas de Git e Fluxo de Trabalho (Workflow)

Para que nosso projeto flua perfeitamente, sem que o código de um membro quebre ou apague o código do outro, vamos utilizar um fluxo de trabalho estruturado no Git (baseado no Git Flow). 

Nossa regra de ouro é: **NUNCA comite diretamente na `main`!**

---

## 🌳 Arquitetura de Branches (Ramificações)

Nosso repositório terá duas ramificações principais permanentes:

1. **`main`**: É a versão "Apresentação". Aqui o código **sempre** tem que compilar e funcionar. Só mandaremos código para a `main` quando o projeto (ou uma grande versão dele) estiver 100% testado.
2. **`dev` (Development)**: É o nosso "Canteiro de Obras Principal". Todos os códigos novos que estiverem funcionando se encontram aqui. 

---

## 🚀 Como desenvolver uma nova tarefa (Feature Branch)

Quando você for iniciar a sua parte do trabalho (ex: Felipe vai implementar a seleção de objetos), **nunca** programe direto na `dev` ou na `main`. Você deve criar uma branch (ramificação) isolada só para você.

### Passo a passo do trabalho:

**1. Atualize sua `dev` com as últimas novidades da equipe:**
```bash
git checkout dev
git pull origin dev
```

**2. Crie uma branch para a sua feature específica:**
Use o prefixo `feature/` seguido do que você vai fazer (sem espaços ou acentos).
```bash
git checkout -b feature/selecao-objetos
```
*(Outros exemplos: `feature/transladar`, `feature/salvar-arquivo`, `feature/interface-botoes`)*

**3. Trabalhe, codifique e faça commits frequentes:**
Conforme for terminando blocos lógicos que funcionam, faça commits na sua branch.
```bash
git add .
git commit -m "feat: implementa algoritmo de colisao para selecionar poligonos"
```

**4. Mande sua branch para o Github:**
```bash
git push origin feature/selecao-objetos
```

**5. Integre seu código (Merge na `dev`):**
Após concluir e testar sua feature, acesse o Github, abra um **Pull Request (PR)** da sua branch `feature/selecao-objetos` apontando para a branch `dev`. 
Peça para alguém do grupo dar uma lida rápida. Estando aprovado, você fará o *merge* para a `dev`!

---

## 📝 Regras para Commits (Padrão Conventional Commits)

Vamos manter o histórico do projeto limpo e legível. Comece a mensagem do seu commit com as seguintes tags:

* **`feat:`** Quando adicionar uma nova funcionalidade (ex: `feat: funcao de adicionar circulo`).
* **`fix:`** Quando resolver um bug/erro (ex: `fix: arruma crash ao fechar poligono aberto`).
* **`docs:`** Quando mexer apenas em arquivos de texto/documentação (ex: `docs: atualiza divisao de tarefas`).
* **`refactor:`** Quando mudar o código interno sem alterar como ele funciona pro usuário (ex: `refactor: divide main em funcoes menores`).

---

## ⚠️ Resumo do Fluxo Diário

1. `git pull origin dev` (para baixar o que a equipe fez)
2. `git checkout -b feature/nome-da-sua-tarefa` (criar sua área de trabalho)
3. Trabalhar no código 🧑‍💻
4. `git commit -m "..."` (salvar o progresso com mensagens claras)
5. `git push` (mandar para a nuvem)
6. Abrir Pull Request para a branch `dev` (integrar com o resto do projeto)

Seguindo esse padrão, se algo quebrar, basta voltarmos um passo para trás e ninguém perde o trabalho de ninguém! 🚀

---

## 🛡️ Entendendo as Regras de Proteção (Rulesets)

Como o nosso repositório é **Público**, nós ativamos **Rulesets** (Regras de Proteção de Branch) no GitHub. Isso significa que o próprio servidor do GitHub vai ativamente bloquear comandos perigosos e proteger o nosso código de cagadas.

As regras ativas para a nossa `main` e `dev` são:
* **Require a pull request before merging:** Ninguém consegue enviar (`git push`) código direto para as ramificações principais. Tudo tem que passar por uma revisão no site abrindo um Pull Request.
* **Block force pushes:** Proíbe forçar comandos (como `git push -f`) que reescrevem o histórico do Git (o que apagaria o trabalho local dos outros de forma irreversível). O terminal vai dar erro de permissão.
* **Restrict deletions:** Impede que qualquer membro do grupo exclua acidentalmente as ramificações "alvo" (como a `main` ou `dev`). Apenas o **Administrador** (criador do repositório) tem "bypass" (permissão de desvio) para excluir a branch caso ocorra uma emergência absoluta.

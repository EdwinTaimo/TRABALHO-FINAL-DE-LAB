# Sistema de Alocação de Bolsas e Vagas — "Um Estudante, Um Computador"

Este projeto foi desenvolvido como **Trabalho Final** para a cadeira de **Laboratório de Engenharia Informática II** na **Universidade Licungo (UniLicungo)**, sob a orientação do docente **Daniel Gimo**.

O sistema foi desenhado para gerir e automatizar o processo de seleção de estudantes para programas de assistência social (como a atribuição de bolsas de estudo e computadores portáteis), garantindo critérios justos baseados em mérito académico e vulnerabilidade financeira.

---

## 🚀 Funcionalidades do Sistema

1. **Inscrição de Estudantes:** Registo com validação de ID único.
2. **Atribuição Automática de Bolsas:** Seleciona e remove o estudante com maior prioridade da fila.
3. **Programa "Um Estudante, Um Computador":** Gestão de stock de equipamentos e entrega baseada no topo da fila de prioridade.
4. **Pesquisa Rápida (O(log n)):** Busca direta de informações de qualquer estudante através do ID.
5. **Listagem Geral:** Exibição ordenada de todos os inscritos (Ordem Crescente de ID).
6. **Atualização Dinâmica:** Permite alterar notas ou rendas, reordenando a fila de prioridades automaticamente.
7. **Painel Estatístico:** Exibe médias académicas, total de inscritos e contagem de estudantes em situação de vulnerabilidade social (Renda $\le$ 3000 MT).
8. **Persistência de Dados:** Gravação automática do estado atual em `estudantes.txt` e histórico de benefícios em `beneficiados.txt`.

---

## 🛠️ Estruturas de Dados Aplicadas

Para otimizar o desempenho do sistema, foram implementadas duas estruturas de dados principais de raiz (sem bibliotecas externas como STL):

* **Max-Heap (Fila de Prioridade Máxima):** Garante acesso em tempo constante $O(1)$ ao estudante com maior pontuação de prioridade. As operações de inserção e remoção ocorrem em $O(\log n)$.
* **Árvore Binária de Busca (BST):** Atua como um índice secundário organizado por ID. Permite pesquisas, inserções e remoções eficientes, além de possibilitar a listagem ordenada através do percurso *Em-Ordem* (*In-Order*).

### 📐 Algoritmo de Prioridade

A pontuação de prioridade de cada estudante é calculada de forma ponderada ($60\%$ nota e $40\%$ fator renda):

$$Pontuação = (Nota \times 0.6) + (FatorRenda \times 0.4)$$

> **Nota:** O *FatorRenda* é inversamente proporcional à renda familiar mensal, garantindo que estudantes com menos recursos tenham um incremento justo na sua classificação.

---

## 📂 Estrutura do Repositório

* `projeto edwin.cpp`: Código-fonte principal com a implementação do menu, persistência e estruturas de dados.
* `estudantes.txt`: Base de dados textual que guarda os registos dos estudantes no formato `ID;Nome;Nota;Renda`.
* `beneficiados.txt`: Ficheiro de log (histórico) que regista cada ação de entrega de bolsas ou computadores.

---

## 💻 Como Compilar e Executar

Certifica-te de que tens um compilador de C++ instalado (como o `g++`).

1. Abre o terminal na pasta do projeto e compila o código:
   ```bash
   g++ -o sistema_bolsas "projeto edwin.cpp"

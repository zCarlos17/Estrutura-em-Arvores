<div align="center">

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![Make](https://img.shields.io/badge/build-make-008000?style=flat-square)
![LaTeX](https://img.shields.io/badge/docs-LaTeX-008080?style=flat-square&logo=latex&logoColor=white)
![Status](https://img.shields.io/badge/status-concluído-brightgreen?style=flat-square)


# Modelagem e Implementação de Estruturas em Árvores Avançadas
<p>

Trabalho Prático desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados II** do curso de Engenharia de Computação do **Centro Federal de Educação Tecnológica de Minas Gerais (CEFET-MG)**
</p>

</div>

---

## 📌 Visão Geral

Este repositório contém a implementação em C++17, análise assintótica, validação funcional e testes empíricos de desempenho de cinco estruturas de dados hierárquicas avançadas:

1. **Trie (Árvore de Prefixos):** Indexação e busca rápida de cadeias de caracteres por decomposição de caracteres.  
2. **Patricia (Radix Tree comprimida):** Árvore de prefixos com compressão de caminhos unários, reduzindo o consumo de nós e memória.  
3. **Árvore Splay:** Árvore binária de busca autoajustável via rotações (*splay*), otimizada para cenários de forte localidade temporal de acesso.  
4. **Treap:** Estrutura híbrida (BST \+ Heap) com balanceamento probabilístico garantido por prioridades estocásticas.  
5. **KD-Tree ($k$-Dimensional Tree):** Decomposição ortogonal do espaço multidimensional para indexação geométrica e busca de pontos no $\\mathbb{R}^2$.

---

```markdown
## 📁 Estrutura de Diretórios

```text
.
├── benchmarks/                      # Scripts e códigos para experimentação
│   ├── datasets/                    # Conjuntos de dados gerados
│   ├── executar_benchmarks.cpp      # Painel interativo de testes
│   └── executar_experimentos_cientificos.cpp # Bateria de 10 execuções
├── docs/                            # Interface Web Interativa (HTML/JS/CSS)
├── graficos/                        # Diretório de saída dos gráficos exportados (SVG)
├── include/                         # Implementações em C++ das estruturas (.hpp)
├── tests/                           # Testes unitários (test_trees.cpp)
└── Makefile                         # Regras de compilação
```

---

## ⚙️ Pré-requisitos

Para compilar e executar o projeto completo, são necessárias as seguintes ferramentas:

* Compilador C++ com suporte para C++17 (`g++`)  
* Utilitário `make`  
* Graphviz (`dot`) para conversão de diagramas estruturais  
* Distribuição LaTeX (`pdflatex`) para compilação do relatório

No Ubuntu / Debian / WSL:

sudo apt update

sudo apt install build-essential graphviz texlive-latex-base texlive-latex-extra

---

## 🚀 Como Compilar e Executar

A compilação do projeto é inteiramente orquestrada via `Makefile` a partir da raiz:

### 1\. Compilar os binários principais

make

Gera os executáveis em `bin/testes_funcionais` e `bin/gerar_visualizacoes`.

### 2\. Executar os testes funcionais

Executa a validação funcional completa (67 testes de inserção, busca e remoção nas cinco estruturas):

make run-testes

### 3\. Gerar os diagramas estruturais (.dot e .png)

Gera os estados em Graphviz e converte-os automaticamente para imagens PNG em `relatorio/figuras/`:

make run-vis

### 4\. Compilar o Relatório Técnico em PDF

Gera o ficheiro `relatorio.pdf` com resolução de referências, tabelas e sumário:

make pdf

### 5\. Limpar binários e ficheiros temporários

make clean

---

## 📊 Relatório Técnico

O relatório analítico completo, contendo as deduções de complexidade assintótica, discussões sobre consumo de memória, comportamento sob localidade temporal e gráficos de benchmark, encontra-se disponível em:

* [`relatorio/relatorio.pdf`](http://relatorio/relatorio.pdf)

---

## 👤 Autor

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/zCarlos17">
        <img src="https://github.com/zCarlos17.png" width="120px;" alt="Carlos Daniel" style="border-radius: 8px;"/><br>
        <sub><b>Carlos Daniel</b></sub>
      </a>
      <br>
      <a href="https://github.com/zCarlos17?tab=followers">
        <img src="https://img.shields.io/github/followers/zCarlos17?label=Seguidores&style=social" alt="Seguidores GitHub"/>
      </a>
    </td>
  </tr>
</table>
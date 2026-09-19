# =====================================================================
# Makefile - Modelagem e Implementação de Estruturas em Árvores
# Disciplina: Algoritmos e Estruturas de Dados II - CEFET-MG
# =====================================================================

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O3 -Iinclude
LATEX := pdflatex

DIR_SRC := src
DIR_INC := include
DIR_BIN := bin
DIR_TEST := testes
DIR_VIS := visualizacoes
DIR_REL := relatorio

TARGET_TESTES := $(DIR_BIN)/testes_funcionais
TARGET_VIS := $(DIR_BIN)/gerar_visualizacoes

# Todos os .cpp de implementação em src/
SRCS := $(wildcard $(DIR_SRC)/*.cpp)

.PHONY: all setup clean run-testes run-vis pdf help

all: setup $(TARGET_TESTES) $(TARGET_VIS)

setup:
	@mkdir -p $(DIR_BIN)
	@mkdir -p $(DIR_VIS)/trie $(DIR_VIS)/patricia $(DIR_VIS)/splay $(DIR_VIS)/treap $(DIR_VIS)/kdtree

# Compilação dos testes funcionais usando testes/testes_funcionais.cpp
$(TARGET_TESTES): $(DIR_TEST)/testes_funcionais.cpp $(SRCS)
	@echo "==> Compilando testes funcionais..."
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilação do gerador de visualizações usando testes/main_visualizacoes.cpp
$(TARGET_VIS): $(DIR_TEST)/main_visualizacoes.cpp $(SRCS)
	@echo "==> Compilando gerador de visualizacoes..."
	$(CXX) $(CXXFLAGS) $^ -o $@

run-testes: $(TARGET_TESTES)
	@echo "==> Executando bateria de testes funcionais..."
	./$(TARGET_TESTES)

run-vis: $(TARGET_VIS) setup
	@echo "==> Gerando arquivos .dot das árvores..."
	./$(TARGET_VIS)
	@echo "==> Convertendo arquivos .dot para PNG dentro de relatorio/figuras..."
	@mkdir -p $(DIR_REL)/figuras
	@for dir in $(DIR_VIS)/*/; do \
		for f in "$$dir"*.dot; do \
			[ -e "$$f" ] || continue; \
			dot -Tpng -Gdpi=300 "$$f" -o "$(DIR_REL)/figuras/$$(basename "$$f" .dot).png"; \
		done \
	done
	@echo "==> Visualizações atualizadas com sucesso!"

pdf:
	@echo "==> Compilando relatorio.tex..."
	@cd $(DIR_REL) && $(LATEX) relatorio.tex && $(LATEX) relatorio.tex
	@echo "==> PDF gerado com sucesso em: $(DIR_REL)/relatorio.pdf"

clean:
	@echo "==> Limpando binários e arquivos temporários..."
	@rm -rf $(DIR_BIN)/*
	@rm -f $(DIR_REL)/*.aux $(DIR_REL)/*.log $(DIR_REL)/*.out $(DIR_REL)/*.toc $(DIR_REL)/*.synctex.gz
	@echo "==> Concluído!"
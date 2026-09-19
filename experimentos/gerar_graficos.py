#!/usr/bin/env python3
# ============================================================================
# gerar_graficos.py
#
# Le os CSVs de resultados/ e gera os graficos da Secao 5 do relatorio.
#
# Requisitos (instalar uma vez):
#   pip install matplotlib pandas --break-system-packages
#
# Rodar (a partir da pasta experimentos/):
#   python3 gerar_graficos.py
#
# Gera tudo em resultados/graficos/
# ============================================================================

import os
import pandas as pd
import matplotlib.pyplot as plt

PASTA_RESULTADOS = "resultados"
PASTA_GRAFICOS = os.path.join(PASTA_RESULTADOS, "graficos")
os.makedirs(PASTA_GRAFICOS, exist_ok=True)

def salvar(fig, nome):
    caminho = os.path.join(PASTA_GRAFICOS, nome)
    fig.savefig(caminho, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"Gerado: {caminho}")


# ----------------------------------------------------------------------
# Carrega os dados
# ----------------------------------------------------------------------
caminho_tempos = os.path.join(PASTA_RESULTADOS, "tempos.csv")
caminho_localidade = os.path.join(PASTA_RESULTADOS, "localidade_splay.csv")

if not os.path.exists(caminho_tempos):
    print(f"ERRO: {caminho_tempos} nao encontrado. Rode o benchmark primeiro.")
    exit(1)

df = pd.read_csv(caminho_tempos)


# ----------------------------------------------------------------------
# GRAFICO 1 -- Tempo de insercao: Trie vs Patricia (log-log, por caso)
# ----------------------------------------------------------------------
fig, eixos = plt.subplots(1, 3, figsize=(15, 4.5), sharey=True)
for eixo, caso in zip(eixos, ["curto", "medio", "longo"]):
    sub = df[(df["caso"] == caso) & (df["estrutura"].isin(["Trie", "Patricia"]))]
    for estrutura, grupo in sub.groupby("estrutura"):
        grupo = grupo.sort_values("tamanho")
        eixo.plot(grupo["tamanho"], grupo["tempo_insercao_ms"], marker="o", label=estrutura)
    eixo.set_xscale("log")
    eixo.set_yscale("log")
    eixo.set_xlabel("n (quantidade de chaves)")
    eixo.set_title(f"strings '{caso}'")
    eixo.grid(True, which="major", alpha=0.4)
eixos[0].set_ylabel("tempo de insercao (ms)")
eixos[0].legend()
fig.suptitle("Tempo de insercao: Trie vs Patricia")
salvar(fig, "01_insercao_trie_vs_patricia.png")


# ----------------------------------------------------------------------
# GRAFICO 2 -- Tempo de busca: Trie vs Patricia (log-log, por caso)
# ----------------------------------------------------------------------
fig, eixos = plt.subplots(1, 3, figsize=(15, 4.5), sharey=True)
for eixo, caso in zip(eixos, ["curto", "medio", "longo"]):
    sub = df[(df["caso"] == caso) & (df["estrutura"].isin(["Trie", "Patricia"]))]
    for estrutura, grupo in sub.groupby("estrutura"):
        grupo = grupo.sort_values("tamanho")
        eixo.plot(grupo["tamanho"], grupo["tempo_busca_ms"], marker="o", label=estrutura)
    eixo.set_xscale("log")
    eixo.set_yscale("log")
    eixo.set_xlabel("n (quantidade de chaves)")
    eixo.set_title(f"strings '{caso}'")
    eixo.grid(True, which="major", alpha=0.4)
eixos[0].set_ylabel("tempo de busca (ms)")
eixos[0].legend()
fig.suptitle("Tempo de busca: Trie vs Patricia")
salvar(fig, "02_busca_trie_vs_patricia.png")


# ----------------------------------------------------------------------
# GRAFICO 3 -- Memoria estimada: Trie vs Patricia (a prova da compactacao)
# ----------------------------------------------------------------------
fig, eixos = plt.subplots(1, 2, figsize=(12, 5))

sub = df[(df["tamanho"] == 1000) & (df["estrutura"].isin(["Trie", "Patricia"]))]
pivot_nos = sub.pivot(index="caso", columns="estrutura", values="quantidade_nos").reindex(["curto", "medio", "longo"])
pivot_nos.plot(kind="bar", ax=eixos[0], rot=0)
eixos[0].set_ylabel("quantidade de nos")
eixos[0].set_title("Numero de nos (n=1000)")
eixos[0].grid(True, axis="y", alpha=0.3)

pivot_mem = sub.pivot(index="caso", columns="estrutura", values="memoria_bytes_estimada").reindex(["curto", "medio", "longo"])
(pivot_mem / 1024).plot(kind="bar", ax=eixos[1], rot=0)
eixos[1].set_ylabel("memoria estimada (KB)")
eixos[1].set_title("Memoria estimada (n=1000)")
eixos[1].grid(True, axis="y", alpha=0.3)

fig.suptitle("Trie vs Patricia: a vantagem de memoria cresce com o comprimento da chave")
salvar(fig, "03_memoria_trie_vs_patricia.png")


# ----------------------------------------------------------------------
# GRAFICO 4 -- Splay/Treap numerico: aleatorio vs ordenado
# ----------------------------------------------------------------------
fig, eixos = plt.subplots(1, 2, figsize=(12, 4.5), sharey=True)
for eixo, estrutura in zip(eixos, ["Splay", "Treap"]):
    sub = df[df["estrutura"] == estrutura]
    for caso, grupo in sub.groupby("caso"):
        grupo = grupo.sort_values("tamanho")
        eixo.plot(grupo["tamanho"], grupo["tempo_insercao_ms"], marker="o", label=f"insercao ({caso})")
        eixo.plot(grupo["tamanho"], grupo["tempo_busca_ms"], marker="s", linestyle="--", label=f"busca ({caso})")
    eixo.set_xscale("log")
    eixo.set_yscale("log")
    eixo.set_xlabel("n (quantidade de chaves)")
    eixo.set_title(estrutura)
    eixo.legend(fontsize=8)
    eixo.grid(True, which="major", alpha=0.4)
eixos[0].set_ylabel("tempo (ms)")
fig.suptitle("Splay vs Treap: aleatorio vs ordenado (pior caso classico de BST)")
salvar(fig, "04_splay_treap_aleatorio_vs_ordenado.png")


# ----------------------------------------------------------------------
# GRAFICO 5 -- KD-Tree: insercao, busca e remocao vs tamanho
# ----------------------------------------------------------------------
fig, eixo = plt.subplots(figsize=(7, 5))
sub = df[df["estrutura"] == "KDTree"].sort_values("tamanho")
eixo.plot(sub["tamanho"], sub["tempo_insercao_ms"], marker="o", label="insercao")
eixo.plot(sub["tamanho"], sub["tempo_busca_ms"], marker="s", label="busca")
eixo.plot(sub["tamanho"], sub["tempo_remocao_ms"], marker="^", label="remocao")
eixo.set_xscale("log")
eixo.set_yscale("log")
eixo.set_xlabel("n (quantidade de pontos)")
eixo.set_ylabel("tempo (ms)")
eixo.set_title("KD-Tree: tempo por operacao")
eixo.legend()
eixo.grid(True, which="major", alpha=0.4)
salvar(fig, "05_kdtree_operacoes.png")


# ----------------------------------------------------------------------
# GRAFICO 6 -- Localidade temporal: Splay vs Treap (o experimento central)
# ----------------------------------------------------------------------
if os.path.exists(caminho_localidade):
    dfl = pd.read_csv(caminho_localidade)

    fig, eixo = plt.subplots(figsize=(8, 5))
    largura = 0.2
    tamanhos_acesso = sorted(dfl["qtd_acessos"].unique())
    posicoes = list(range(len(tamanhos_acesso)))

    # 4 barras por grupo (Splay-enviesado, Splay-uniforme, Treap-enviesado,
    # Treap-uniforme), cada uma num "slot" proprio para nao sobrepor
    combinacoes = [
        ("Splay", "enviesado", "tab:blue",   "Splay enviesado (80/20)"),
        ("Splay", "uniforme",  "tab:orange", "Splay uniforme"),
        ("Treap", "enviesado", "tab:green",  "Treap enviesado (80/20)"),
        ("Treap", "uniforme",  "tab:red",    "Treap uniforme"),
    ]

    for slot, (estrutura, tipo, cor, rotulo) in enumerate(combinacoes):
        offset = (slot - 1.5) * largura
        valores = [
            dfl[(dfl["estrutura"] == estrutura) & (dfl["tipo_acesso"] == tipo) & (dfl["qtd_acessos"] == q)]
               ["tempo_medio_por_acesso_ms"].values[0]
            for q in tamanhos_acesso
        ]
        eixo.bar([p + offset for p in posicoes], valores, largura, label=rotulo, color=cor)

    eixo.set_xticks(posicoes)
    eixo.set_xticklabels([str(q) for q in tamanhos_acesso])
    eixo.set_xlabel("quantidade de acessos")
    eixo.set_ylabel("tempo medio por acesso (ms)")
    eixo.set_title("Localidade temporal: Splay se beneficia do padrao enviesado, Treap nao")
    eixo.legend(fontsize=8)
    eixo.grid(True, axis="y", alpha=0.3)
    salvar(fig, "06_localidade_splay_vs_treap.png")
else:
    print(f"AVISO: {caminho_localidade} nao encontrado -- grafico 6 pulado")


print("\nTodos os graficos foram gerados em", PASTA_GRAFICOS)
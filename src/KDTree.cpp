#include "KDTree.hpp"

NoKD::NoKD(double px, double py) {
    x = px;
    y = py;
    esquerda = nullptr;
    direita = nullptr;
}

KDTree::KDTree() { raiz = nullptr; }
KDTree::~KDTree() { liberar(raiz); }

void KDTree::liberar(NoKD* no) {
    if (no == nullptr) return;
    liberar(no->esquerda);
    liberar(no->direita);
    delete no;
}

NoKD* KDTree::inserirAux(NoKD* no, double x, double y, int profundidade) {
    if (no == nullptr) return new NoKD(x, y);

    int eixo = profundidade % 2;
    double valorNo, valorNovo;
    if (eixo == 0) { valorNo = no->x; valorNovo = x; }
    else { valorNo = no->y; valorNovo = y; }

    if (valorNovo < valorNo) {
        no->esquerda = inserirAux(no->esquerda, x, y, profundidade + 1);
    } else {
        no->direita = inserirAux(no->direita, x, y, profundidade + 1);
    }
    return no;
}

void KDTree::inserir(double x, double y) { raiz = inserirAux(raiz, x, y, 0); }

bool KDTree::buscarAux(NoKD* no, double x, double y, int profundidade) {
    if (no == nullptr) return false;
    if (no->x == x && no->y == y) return true;

    int eixo = profundidade % 2;
    double valorNo, valorNovo;
    if (eixo == 0) { valorNo = no->x; valorNovo = x; }
    else { valorNo = no->y; valorNovo = y; }

    if (valorNovo < valorNo) return buscarAux(no->esquerda, x, y, profundidade + 1);
    else return buscarAux(no->direita, x, y, profundidade + 1);
}

bool KDTree::buscar(double x, double y) { return buscarAux(raiz, x, y, 0); }

void KDTree::exportarDotAux(NoKD* no, std::ofstream& arquivo) {
    if (no == nullptr) return;
    arquivo << "    node" << no << " [label=\"(" << no->x << "," << no->y << ")\"];\n";
    if (no->esquerda != nullptr) {
        arquivo << "    node" << no << " -> node" << no->esquerda << ";\n";
        exportarDotAux(no->esquerda, arquivo);
    }
    if (no->direita != nullptr) {
        arquivo << "    node" << no << " -> node" << no->direita << ";\n";
        exportarDotAux(no->direita, arquivo);
    }
}

void KDTree::exportarDot(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    arquivo << "digraph KDTree {\n";
    exportarDotAux(raiz, arquivo);
    arquivo << "}\n";
    arquivo.close();
}
#include "Treap.hpp"
#include <iostream>
#include <cstdlib>

NoTreap::NoTreap(int valor) {
    chave = valor;
    prioridade = rand();
    esquerda = nullptr;
    direita = nullptr;
}

Treap::Treap() { raiz = nullptr; }
Treap::~Treap() { liberar(raiz); }

void Treap::liberar(NoTreap* no) {
    if (no == nullptr) return;
    liberar(no->esquerda);
    liberar(no->direita);
    delete no;
}

NoTreap* Treap::rotacionarDir(NoTreap* y) {
    NoTreap* x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;
    return x;
}

NoTreap* Treap::rotacionarEsq(NoTreap* y) {
    NoTreap* x = y->direita;
    y->direita = x->esquerda;
    x->esquerda = y;
    return x;
}

NoTreap* Treap::inserirAux(NoTreap* no, int chave) {
    if (no == nullptr) return new NoTreap(chave);

    if (chave < no->chave) {
        no->esquerda = inserirAux(no->esquerda, chave);
        if (no->esquerda->prioridade > no->prioridade) no = rotacionarDir(no);
    } else if (chave > no->chave) {
        no->direita = inserirAux(no->direita, chave);
        if (no->direita->prioridade > no->prioridade) no = rotacionarEsq(no);
    }
    return no;
}

void Treap::inserir(int chave) { raiz = inserirAux(raiz, chave); }

bool Treap::buscarAux(NoTreap* no, int chave) {
    if (no == nullptr) return false;
    if (chave == no->chave) return true;
    if (chave < no->chave) return buscarAux(no->esquerda, chave);
    return buscarAux(no->direita, chave);
}

bool Treap::buscar(int chave) { return buscarAux(raiz, chave); }

NoTreap* Treap::removerAux(NoTreap* no, int chave) {
    if (no == nullptr) return nullptr;

    if (chave < no->chave) {
        no->esquerda = removerAux(no->esquerda, chave);
    } else if (chave > no->chave) {
        no->direita = removerAux(no->direita, chave);
    } else {
        if (no->esquerda == nullptr) {
            NoTreap* temp = no->direita;
            delete no;
            return temp;
        } else if (no->direita == nullptr) {
            NoTreap* temp = no->esquerda;
            delete no;
            return temp;
        } else {
            if (no->esquerda->prioridade > no->direita->prioridade) {
                no = rotacionarDir(no);
                no->direita = removerAux(no->direita, chave);
            } else {
                no = rotacionarEsq(no);
                no->esquerda = removerAux(no->esquerda, chave);
            }
        }
    }
    return no;
}

void Treap::remover(int chave) { raiz = removerAux(raiz, chave); }

void Treap::exportarDotAux(NoTreap* no, std::ofstream& arquivo) {
    if (no == nullptr) return;
    arquivo << "    node" << no << " [label=\"" << no->chave
            << "\\n(p=" << no->prioridade << ")\"];\n";
    if (no->esquerda != nullptr) {
        arquivo << "    node" << no << " -> node" << no->esquerda << ";\n";
        exportarDotAux(no->esquerda, arquivo);
    }
    if (no->direita != nullptr) {
        arquivo << "    node" << no << " -> node" << no->direita << ";\n";
        exportarDotAux(no->direita, arquivo);
    }
}

void Treap::exportarDot(const std::string& caminhoArquivo) {
    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "ERRO: nao foi possivel criar " << caminhoArquivo
                  << " -- a pasta de destino existe?\n";
        return;
    }
    arquivo << "digraph Treap {\n";
    exportarDotAux(raiz, arquivo);
    arquivo << "}\n";
    arquivo.close();
}

int Treap::contarNosAux(NoTreap* no){
    if (no == nullptr) return 0;
    return 1 + contarNosAux(no->esquerda) + contarNosAux(no->direita);
}

int Treap::contarNos(){
    return contarNosAux(raiz);
}
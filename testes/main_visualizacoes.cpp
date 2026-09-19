#include "Trie.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "KDTree.hpp"
#include <iostream>

int main() {
    // ---------- TRIE ----------
    Trie trie;
    trie.inserir("casa");
    trie.inserir("casal");
    trie.inserir("casamento");
    trie.exportarDot("visualizacoes/trie/trie_estado_inicial.dot");

    trie.remover("casa");
    trie.exportarDot("visualizacoes/trie/trie_pos_remocao_casa.dot");
    trie.remover("casal");
    trie.exportarDot("visualizacoes/trie/trie_pos_remocao_casal.dot");

    // ---------- PATRICIA ----------
    Patricia patricia;
    patricia.inserir("casamento");
    patricia.exportarDot("visualizacoes/patricia/patricia_antes_split.dot");

    patricia.inserir("casarao");
    patricia.exportarDot("visualizacoes/patricia/patricia_apos_split.dot");

    patricia.remover("casarao");
    patricia.exportarDot("visualizacoes/patricia/patricia_pos_remocao_fusao.dot");

    // ---------- SPLAY ----------
    Splay splayZigZig;
    splayZigZig.inserir(10);
    splayZigZig.inserir(5);
    splayZigZig.inserir(2);
    splayZigZig.exportarDot("visualizacoes/splay/splay_zigzig.dot");

    Splay splayZigZag;
    splayZigZag.inserir(10);
    splayZigZag.inserir(2);
    splayZigZag.inserir(5);
    splayZigZag.exportarDot("visualizacoes/splay/splay_zigzag.dot");

    // ---------- TREAP ----------
    Treap treap;
    treap.inserir(50);
    treap.inserir(30);
    treap.inserir(70);
    treap.inserir(20);
    treap.inserir(40);
    treap.exportarDot("visualizacoes/treap/treap_estado_inicial.dot");

    treap.inserir(10);
    treap.inserir(60);
    treap.inserir(80);
    treap.exportarDot("visualizacoes/treap/treap_apos_insercoes.dot");

    // ---------- KD-TREE ----------
    KDTree kdtree;
    kdtree.inserir(30, 40);
    kdtree.inserir(5, 25);
    kdtree.inserir(10, 12);
    kdtree.inserir(70, 70);
    kdtree.inserir(50, 30);
    kdtree.inserir(35, 45);
    kdtree.exportarDot("visualizacoes/kdtree/kdtree_estado.dot");

    std::cout << "Todos os arquivos .dot foram gerados em visualizacoes/\n";
    return 0;
}